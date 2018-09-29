#include "bro-config.h"
#include "AF_XDP.h"

#include <net/if.h>

#ifndef SOL_XDP
#define SOL_XDP 283
#endif

#ifndef AF_XDP
#define AF_XDP 44
#endif

#ifndef PF_XDP
#define PF_XDP AF_XDP
#endif

#define DEFAULT_FILL_RING_SIZE 1024
#define DEFAULT_RX_RING_SIZE 1024
#define NUM_FRAMES 2048
#define FRAME_SIZE 2048

using namespace iosource::pktsrc;

AF_XDPSource::~AF_XDPSource() {
	Close();
}

AF_XDPSource::AF_XDPSource(const std::string& path, bool is_live) {
	if (!is_live)
		Error("AF_XDP source does not support offline input");

	props.path = path;
	props.is_live = is_live;
}

void AF_XDPSource::Open() {
	struct xdp_mmap_offsets offsets;
	socklen_t opt_length;
	int xsks_map;
	int r;

	int fill_ring_size = DEFAULT_FILL_RING_SIZE;
	int rx_ring_size = DEFAULT_RX_RING_SIZE;

	ifindex = if_nametoindex(props.path.c_str());
	if (ifindex == 0) {
		Error(errno ? strerror(errno) : "unable to find interface");
		return;
	}

	if ((fd = socket(AF_XDP, SOCK_RAW, 0)) < 0) {
		Error(errno ? strerror(errno) : "unable to create socket");
		return;
	}

	umem.headroom = 0;
	umem.chunk_size = FRAME_SIZE;
	umem.len = NUM_FRAMES * umem.chunk_size;
	posix_memalign((void**) &umem.addr, getpagesize(), umem.len);

	if (setsockopt(fd, SOL_XDP, XDP_UMEM_REG, &umem, sizeof(umem)) < 0) {
		Error(errno ? strerror(errno) : "unable to set UMEM");
		close(fd);
		return;
	}

	if (setsockopt(fd, SOL_XDP, XDP_UMEM_FILL_RING, &fill_ring_size, sizeof(fill_ring_size)) < 0) {
		Error(errno ? strerror(errno) : "unable to set UMEM fill ring size");
		close(fd);
		return;
	}

	if (setsockopt(fd, SOL_XDP, XDP_RX_RING, &rx_ring_size, sizeof(rx_ring_size)) < 0) {
		Error(errno ? strerror(errno) : "unable to set XDP RX ring size");
		close(fd);
		return;
	}
	
	opt_length = sizeof(offsets);
	if (getsockopt(fd, SOL_XDP, XDP_MMAP_OFFSETS, &offsets, &opt_length) < 0) {
		Error(errno ? strerror(errno) : "unable to get ring offsets");
		close(fd);
		return;
	}

	if (!fill.Init(fd, XDP_UMEM_PGOFF_FILL_RING, offsets.fr, fill_ring_size)) {
		Error(errno ? strerror(errno) : "unable to map UMEM fill ring");
		close(fd);
		return;
	}

	if (!rx.Init(fd, XDP_PGOFF_RX_RING, offsets.rx, rx_ring_size)) {
		Error(errno ? strerror(errno) : "unable to map XDP RX fill ring");
		close(fd);
		return;
	}

	for (int i = 0; i < NUM_FRAMES * FRAME_SIZE; i += FRAME_SIZE)
	    fill.Enqueue(i);

	props.netmask = NETMASK_UNKNOWN;
	props.selectable_fd = fd;
	props.is_live = true;
	props.link_type = DLT_EN10MB;

	struct sockaddr_xdp sa = {
	    .sxdp_family = PF_XDP, 
	    .sxdp_ifindex = ifindex,
	};

	if (bind(fd, (struct sockaddr*) &sa, sizeof(sa)) < 0) {
		Error(errno ? strerror(errno) : "unable to bind to interface");
		close(fd);
		return;
	}


	if (bpf.Load("kern.o", ifindex) < 0) {
		Error(errno ? strerror(errno) : "unable to load eBPF program");
		close(fd);
		return;
	}

	xsks_map = bpf.FindMap("xsks_map");
	if (xsks_map < 0) {
		Error(errno ? strerror(errno) : "unable to find eBPF map");
		close(fd);
		return;
	}

	int key = 0;
	if (bpf.UpdateMap(xsks_map, &key, &fd, 0) < 0) {
		Error(errno ? strerror(errno) : "unable to update eBPF map");
		close(fd);
		bpf.Unload(ifindex);
		return;
	}

	stats = { };

	Opened(props);
}

void AF_XDPSource::Close() {
	if (fd) {
		close(fd);
		fd = 0;
	}

	bpf.Unload(ifindex);

	Closed();
}

bool AF_XDPSource::ExtractNextPacket(Packet* pkt) {
	if (!fd)
	    return false;

	current = rx.Next();
	if (!current)
		return false;

	struct timeval ts;
	pkt->Init(props.link_type, &ts, current->len, current->len, &((const u_char*) umem.addr)[current->addr]);

	stats.received++;
	stats.bytes_received += current->len;

	return true;
}

void AF_XDPSource::DoneWithPacket() {
	fill.Enqueue(current->addr);
}

bool AF_XDPSource::PrecompileFilter(int index, const std::string& filter) {
	return true;
}

bool AF_XDPSource::SetFilter(int index) {
	return true;
}

void AF_XDPSource::Statistics(Stats* s) {
	struct xdp_statistics xdp_stats;
	
	socklen_t opt_length = sizeof(xdp_stats);
	
	if (getsockopt(fd, SOL_XDP, XDP_STATISTICS, &xdp_stats, &opt_length) < 0) {
		Error(errno ? strerror(errno) : "unable to retrieve statistics");
		return;
	}

	stats.dropped = xdp_stats.rx_dropped;
	stats.link = stats.received + stats.dropped;

	*s = stats;
}

iosource::PktSrc* AF_XDPSource::InstantiateAF_XDP(const std::string& path, bool is_live) {
	return new AF_XDPSource(path, is_live);
}

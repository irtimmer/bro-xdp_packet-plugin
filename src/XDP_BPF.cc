#include "XDP_BPF.h"

#include <string.h>

extern "C" {
#include <linux/if_link.h>

#include <bpf/libbpf.h>
#include <bpf/bpf.h>
}

int XDP_BPF::Load(const char* file, unsigned short ifindex) {
	int r;
	struct bpf_prog_load_attr prog_load_attr = {
		.file = file,
		.prog_type = BPF_PROG_TYPE_XDP
	};

	r = bpf_prog_load_xattr(&prog_load_attr, &object, &fd);
	if (r < 0)
	    return r;

	r = bpf_set_link_xdp_fd(ifindex, fd, XDP_FLAGS_SKB_MODE);
	if (r < 0)
	    return r;

	return 0;
}

int XDP_BPF::Unload(unsigned short ifindex) {
	printf("Disable eBPF\n");
	if (fd) {
	    fd = 0;
	    return bpf_set_link_xdp_fd(ifindex, -1, XDP_FLAGS_SKB_MODE);
	}
	return 0;
}

int XDP_BPF::FindMap(const char* key) {
	struct bpf_map *map = bpf_object__find_map_by_name(object, key);
	return bpf_map__fd(map);	
}

int XDP_BPF::UpdateMap(int map, void *key, void *value, __u64 flags) {
	return bpf_map_update_elem(map, key, value, flags);
}

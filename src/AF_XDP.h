/** 
 * Copyright (c) 2018 by Iwan Timmer
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef IOSOURCE_PKTSRC_AF_XDP_SOURCE_H
#define IOSOURCE_PKTSRC_AF_XDP_SOURCE_H

extern "C" {
#include <linux/if_link.h>
#include <linux/if_xdp.h>
#include <linux/if_ether.h>
}

#include "iosource/PktSrc.h"

#include "XDP_Ring.h"
#include "XDP_BPF.h"

namespace iosource {
namespace pktsrc {

class AF_XDPSource : public iosource::PktSrc {
public:
	AF_XDPSource(const std::string& path, bool is_live);

	virtual ~AF_XDPSource();

	static PktSrc* InstantiateAF_XDP(const std::string& path, bool is_live);

protected:
	virtual void Open();
	virtual void Close();
	virtual bool ExtractNextPacket(Packet* pkt);
	virtual void DoneWithPacket();
	virtual bool PrecompileFilter(int index, const std::string& filter);
	virtual bool SetFilter(int index);
	virtual void Statistics(Stats* stats);

private:
	Properties props;
	Stats stats;

	unsigned short ifindex;

	int fd;
	struct xdp_umem_reg umem;
	struct xdp_desc* current;

	XDP_BPF bpf;

	XDP_Ring<__u64> fill;
	XDP_Ring<struct xdp_desc> rx;	
};

}
}

#endif

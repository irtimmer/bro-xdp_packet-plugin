/** 
 * Copyright (c) 2018 by Iwan Timmer
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Plugin.h"
#include "AF_XDP.h"

namespace plugin { namespace itimmer_AF_XDP { Plugin plugin; } }

using namespace plugin::itimmer_AF_XDP;

plugin::Configuration Plugin::Configure() {
	AddComponent(new ::iosource::PktSrcComponent("AF_XDPReader", "af_xdp", ::iosource::PktSrcComponent::LIVE, ::iosource::pktsrc::AF_XDPSource::InstantiateAF_XDP));

	plugin::Configuration config;
	config.name = "itimmer::AF_XDP";
	config.description = "Packet acquisition via AF_XDP";
	config.version.major = 0;
	config.version.minor = 1;
	return config;
}

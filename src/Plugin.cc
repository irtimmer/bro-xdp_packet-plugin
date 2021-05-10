/** 
 * Copyright (c) 2018 by Iwan Timmer
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Plugin.h"
#include "AF_XDP.h"

#include <zeek/iosource/Component.h>

namespace plugin { namespace irtimmer_AF_XDP { Plugin plugin; } }

using namespace plugin::irtimmer_AF_XDP;

zeek::plugin::Configuration Plugin::Configure() {
	AddComponent(new zeek::iosource::PktSrcComponent("AF_XDPReader", "af_xdp", zeek::iosource::PktSrcComponent::LIVE, zeek::iosource::pktsrc::AF_XDPSource::InstantiateAF_XDP));

	zeek::plugin::Configuration config;
	config.name = "irtimmer::AF_XDP";
	config.description = "Packet acquisition via AF_XDP";
	config.version.major = 0;
	config.version.minor = 1;
	return config;
}

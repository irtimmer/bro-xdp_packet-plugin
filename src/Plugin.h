/**
 * Copyright (c) 2018 by Iwan Timmer
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BRO_PLUGIN_ITIMMER_AF_XDP
#define BRO_PLUGIN_ITIMMER_AF_XDP

#include <plugin/Plugin.h>

namespace plugin {
namespace irtimmer_AF_XDP {

class Plugin : public zeek::plugin::Plugin {
protected:
	// Overridden from plugin::Plugin.
	zeek::plugin::Configuration Configure() override;
};

extern Plugin plugin;

}
}

#endif

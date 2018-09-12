
#ifndef BRO_PLUGIN_ITIMMER_AF_XDP
#define BRO_PLUGIN_ITIMMER_AF_XDP

#include <plugin/Plugin.h>

namespace plugin {
namespace itimmer_AF_XDP {

class Plugin : public ::plugin::Plugin {
protected:
	// Overridden from plugin::Plugin.
	plugin::Configuration Configure() override;
};

extern Plugin plugin;

}
}

#endif

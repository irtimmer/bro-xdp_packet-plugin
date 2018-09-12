#include "bro-config.h"
#include "AF_XDP.h"

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
	props.is_live = true;

	Opened(props);
}

void AF_XDPSource::Close() {
	Closed();
}

bool AF_XDPSource::ExtractNextPacket(Packet* pkt) {
	return false;
}

void AF_XDPSource::DoneWithPacket() {
}

bool AF_XDPSource::PrecompileFilter(int index, const std::string& filter) {
	return true;
}

bool AF_XDPSource::SetFilter(int index) {
	return true;
}

void AF_XDPSource::Statistics(Stats* s) {
}

iosource::PktSrc* AF_XDPSource::InstantiateAF_XDP(const std::string& path, bool is_live) {
	return new AF_XDPSource(path, is_live);
}

#ifndef XDP_BPF_H
#define XDP_BPF_H

extern "C" {
#include <linux/types.h>
}

class XDP_BPF {
public:
	int Load(const char* file, unsigned short ifindex);
	int Unload(unsigned short ifindex);
	int FindMap(const char* key);
	int UpdateMap(int map, void *key, void *value, __u64 flags);

protected:
	int fd;
	unsigned short ifindex;

	struct bpf_object *object;

};

#endif

#ifndef XDP_RING_H
#define XDP_RING_H

extern "C" {
#include <linux/if_xdp.h>
#include <sys/mman.h>
#include <unistd.h>
}

#define barrier() __asm__ __volatile__("": : :"memory")

template<class T>
class XDP_Ring {
public:
	bool Init(int fd, off_t off, xdp_ring_offset offsets, size_t s) {
		map = mmap(NULL, offsets.desc + s * sizeof(T), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE, fd, off);
		if (map == MAP_FAILED)
		    return false;

		size = s;
		ring = (T*) ((uintptr_t) map + offsets.desc);
		consumer = (__u64*) ((uintptr_t) map + offsets.consumer);
		producer = (__u64*) ((uintptr_t) map + offsets.producer);

		return true;
	}

	T* Peek() {
		if (*producer - *consumer == 0)
			return NULL;

		barrier();

		return &ring[*consumer & (size - 1)];
	}

	T* Next() {
		T* entry = Peek();

		if (entry)
			(*consumer)++;

		return entry;
	}

	int Enqueue(T entry) {
		if ((size - (*producer - *consumer)) == 0)
		    return -1;

		ring[*producer & (size - 1)] = entry;

		barrier();

		(*producer)++;
		return 0;
	}

private:
	void* map;

	size_t size;

	__u64* producer;
	__u64* consumer;
	T* ring;
};

#endif

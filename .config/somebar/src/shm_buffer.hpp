// somebar - dwl bar
// See LICENSE file for copyright and license details.

#pragma once
#include <array>
#include <sys/mman.h>
#include <wayland-client.h>
#include "common.hpp"

class MemoryMapping {
	void* _ptr {nullptr};
	size_t _size {0};
public:
	MemoryMapping() { }
	explicit MemoryMapping(void* ptr, size_t size) : _ptr(ptr), _size(size) { }
	MemoryMapping(const MemoryMapping&) = delete;
	MemoryMapping(MemoryMapping&& other) { swap(other); }
	MemoryMapping& operator=(const MemoryMapping& other) = delete;
	MemoryMapping& operator=(MemoryMapping&& other) { swap(other); return *this; }
	~MemoryMapping() { if (_ptr) munmap(_ptr, _size); }
	void swap(MemoryMapping &other) {
		using std::swap;
		swap(_ptr, other._ptr);
		swap(_size, other._size);
	}
};

// double buffered shm
// format is must be 32-bit
class ShmBuffer {
	struct Buf {
		uint8_t* data {nullptr};
		wl_unique_ptr<wl_buffer> buffer;
	};
	std::array<Buf, 2> _buffers;
	int _current {0};
	MemoryMapping _mapping;
public:
	const uint32_t width, height, stride;

	explicit ShmBuffer(int width, int height, wl_shm_format format);
	uint8_t* data();
	wl_buffer* buffer();
	void flip();
};

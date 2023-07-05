// somebar - dwl bar
// See LICENSE file for copyright and license details.

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "shm_buffer.hpp"
#include "common.hpp"

static int createAnonShm();
constexpr int n = 2;

ShmBuffer::ShmBuffer(int w, int h, wl_shm_format format)
	: width(w)
	, height(h)
	, stride(w*4)
{
	auto oneSize = stride*size_t(h);
	auto totalSize = oneSize * n;
	auto fd = createAnonShm();
	if (fd < 0) {
		diesys("memfd_create");
	}
	if (ftruncate(fd, totalSize) < 0) {
		diesys("ftruncate");
	}
	auto pool = wl_shm_create_pool(shm, fd, totalSize);
	auto ptr = reinterpret_cast<uint8_t*>(mmap(nullptr, totalSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
	if (!ptr) {
		diesys("mmap");
	}
	_mapping = MemoryMapping {ptr, totalSize};
	close(fd);
	for (auto i=0; i<n; i++) {
		auto offset = oneSize*i;
		_buffers[i] = {
			ptr+offset,
			wl_unique_ptr<wl_buffer> { wl_shm_pool_create_buffer(pool, offset, width, height, stride, format) },
		};
	}
	wl_shm_pool_destroy(pool);
}

uint8_t* ShmBuffer::data()
{
	return _buffers[_current].data;
}

wl_buffer* ShmBuffer::buffer()
{
	return _buffers[_current].buffer.get();
}

void ShmBuffer::flip()
{
	_current = 1-_current;
}

#if defined(__linux__)
int createAnonShm() {
	return memfd_create("wl_shm", MFD_CLOEXEC);
}
#elif defined(__FreeBSD__)
int createAnonShm() {
	auto fd = shm_open(SHM_ANON, O_CREAT | O_RDWR, 0600);
	setCloexec(fd);
	return fd;
}
#elif defined(__OpenBSD__)
int createAnonShm() {
	char name[] = "/wl_shm-XXXXXX";
	auto fd = shm_mkstemp(name);
	if (fd >= 0) {
		auto res = shm_unlink(name);
		if (res < 0) {
			return res;
		}
	}
	setCloexec(fd);
	return fd;
}
#else
#error "your system has no sane method of creating an anonymous shared memory object. no, calling shm_open in a loop is not sane."
#endif

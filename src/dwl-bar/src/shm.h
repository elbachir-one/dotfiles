#ifndef SHM_H_
#define SHM_H_

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <wayland-client-protocol.h>

struct MemoryMapping {
    void *ptr;
    int size;
};

struct Buffer {
    struct wl_buffer *buffer;
    uint8_t *buffer_ptr;
};

struct Shm {
    int width, height, stride, current;
    struct MemoryMapping map;
    struct Buffer buffers[2];
};

struct Shm *shm_create(int width, int height, enum wl_shm_format format);
void shm_destroy(struct Shm *shm);
uint8_t *shm_data(struct Shm *shm);
struct wl_buffer *shm_buffer(struct Shm *shm);
void shm_flip(struct Shm *shm);


#endif // SHM_H_

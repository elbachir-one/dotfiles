#include "shm.h"
#include "main.h"
#include <wayland-client-protocol.h>

#define BUFFERS 2

static int allocate_shm(int size);
static struct Buffer buffer_create(struct MemoryMapping *memmap, struct wl_shm_pool *shm,
        int fd, int width, int height, int offset, enum wl_shm_format format);
static void buffer_destroy(struct Buffer *buf);
static struct MemoryMapping memory_mapping_create(int fd, int pool_size);
static void memory_mapping_destroy(struct MemoryMapping *map);


int allocate_shm(int size) {
    char name[] = "wl_shm";
    int fd;

    if ((fd = shm_open(name, O_CREAT | O_RDWR | O_EXCL, 0600)) < 0)
        panic("shm_open when allocating shm");

    shm_unlink(name);

    if (ftruncate(fd, size) < 0)
        panic("ftruncate when allocating shm");

    return fd;
}

static struct Buffer buffer_create(struct MemoryMapping *map, struct wl_shm_pool *shm,
        int fd, int width, int height, int offset, enum wl_shm_format format) {
    if (!map)
        panic("map is null");

    int stride    = width * 4,
        pool_size = height * stride;
    struct Buffer buffer;

    struct wl_buffer *wl_buffer = wl_shm_pool_create_buffer(shm, offset, width, height, stride, format);
    buffer.buffer = wl_buffer;
    buffer.buffer_ptr = map->ptr+offset;

    return buffer;
}

void buffer_destroy(struct Buffer *buffer) {
    if (!buffer) return;
    wl_buffer_destroy(buffer->buffer);
}

struct MemoryMapping memory_mapping_create(int fd, int pool_size) {
    struct MemoryMapping map;
    void* ptr = mmap(NULL, pool_size, PROT_READ | PROT_WRITE,
                      MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED || !ptr) {
        close(fd);
        panic("MAP_FAILED");
    }

    map.ptr  = ptr;
    map.size = pool_size;

    return map;
}

void memory_mapping_destroy(struct MemoryMapping *map) {
    if (!map) return;
    munmap(map->ptr, map->size);
}

struct Shm *shm_create(int width, int height, enum wl_shm_format format) {
    struct Shm *shared_mem = calloc(1, sizeof(*shared_mem));
    int i, offset,
        stride = width * 4,
        size = stride * height,
        total = size * BUFFERS;
    int fd = allocate_shm(total);

    struct MemoryMapping memory = memory_mapping_create(fd, total);
    struct wl_shm_pool* pool = wl_shm_create_pool(shm, fd, total);
    for (i = 0; i < BUFFERS; i++) {
        offset = size*i;
        shared_mem->buffers[i] = buffer_create(&memory, pool, fd, width, height, offset, format);
    }
    close(fd);
    wl_shm_pool_destroy(pool);

    shared_mem->map = memory;
    shared_mem->current = 0;

    shared_mem->height = height;
    shared_mem->width = width;
    shared_mem->stride = stride;

    return shared_mem;
}

void shm_destroy(struct Shm *shm) {
    if (!shm) return;

    memory_mapping_destroy(&shm->map);
    for (int i = 0; i < BUFFERS; i++)
        buffer_destroy(&shm->buffers[i]);
    free(shm);
}

uint8_t *shm_data(struct Shm *shm) {
    return shm->buffers[shm->current].buffer_ptr;
}

struct wl_buffer *shm_buffer(struct Shm *shm) {
    return shm->buffers[shm->current].buffer;
}

void shm_flip(struct Shm *shm) {
    shm->current = 1-shm->current;
}

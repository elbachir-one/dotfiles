#include "util.h"
#include "log.h"
#include "main.h"
#include <alloca.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void list_resize(struct List *list);

void *ecalloc(size_t amnt, size_t size) {
    void *p = calloc(amnt, size);

    if (!p)
        panic("Out of memory");

    return p;
}

void *list_add(struct List *list, void *data) {
    if (!list)
        return NULL;

    list_resize(list);
    list->data[list->length++] = data;
    return list->data[list->length-1];
}

void list_copy(struct List *dest, struct List *src) {
    if (!dest || !src)
        return;

    for (int i = 0; i < src->length; i++)
        list_add(dest, src->data[i]);
}

struct List *list_create(size_t initial_size) {
    struct List *list = ecalloc(1, sizeof(*list));

    list->allocated = initial_size > 10 ? initial_size : 10;
    list->length = 0;
    list->data = ecalloc(list->allocated, sizeof(void*));

    return list;
}

int list_cmp_find(struct List* list, const void *data, int compare(const void *left, const void *right)) {
    if (!list)
        return -1;

    for (int i = 0; i < list->length; i++)
        if (compare(data, list->data[i]) == 0)
            return i;

    return -1;
}

void list_destroy(struct List *list) {
    if (!list)
        return;

    free(list->data);
    free(list);
}

void list_elements_destroy(struct List *list, void (*destroy)(void *data)) {
    if (!list)
        return;

    for (int i = 0; i < list->length; i++)
        destroy(list->data[i]);
    free(list->data);
    free(list);
}

int list_find(struct List* list, const void *data) {
    if (!list)
        return -1;

    for (int i = 0; i < list->length; i++)
        if (data == list->data[i])
            return i;

    return -1;
}

void *list_remove(struct List *list, unsigned int index) {
    if (!list || index > list->length-1)
        return NULL;

    void *tmp = list->data[index];

    list->length--;
    memmove(&list->data[index], &list->data[index + 1], sizeof(void*) * (list->length - index));

    return tmp;
}

void list_resize(struct List *list) {
    if (list->length < list->allocated)
        return;

    list->allocated *= 2;
    list->data = realloc(list->data, sizeof(void*) * list->allocated);
}

char *string_create(const char *fmt, ...) {
    va_list ap, aq;
    va_start(ap, fmt);
    va_copy(aq, ap);

    char *str;
    size_t len = vsnprintf(NULL, 0, fmt, ap) + 1;
    str = ecalloc(1, len);
    vsnprintf(str, len, fmt, aq);

    va_end(ap);
    va_end(aq);

    return str;
}

char *to_delimiter(const char *string, unsigned long *start_end, char delimiter) {
    if (!string || !start_end)
        return NULL;

    char *output;
    const char *read;
    unsigned long i = 0;

    for (read = string + *start_end; *read != '\0' && *read != delimiter; read++)
        i++;

    output = strncpy(ecalloc(i+1, sizeof(*output)),
            string + *start_end, i);
    output[i++] = '\0';
    *start_end += i;

    return output;
}

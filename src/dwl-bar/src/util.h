#ifndef UTIL_H_
#define UTIL_H_
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <wayland-util.h>

#define STRING_EQUAL(string1, string2) strcmp(string1, string2) == 0
#define STRINGN_EQUAL(string1, string2, n) strncmp(string1, string2, n) == 0
#define LENGTH(X) (sizeof X / sizeof X[0] )

struct List {
    void **data;
    size_t allocated, length;
};

void *ecalloc(size_t amnt, size_t size);
void *list_add(struct List *list, void *data);
void list_copy(struct List *dest, struct List *src);
struct List *list_create(size_t initial_size);
int list_cmp_find(struct List *list, const void *data, int compare(const void *left, const void *right));
void list_destroy(struct List *list);
void list_elements_destroy(struct List *list, void (*destroy)(void *data));
int list_find(struct List* list, const void *data);
void *list_remove(struct List *list, unsigned int index);
char *string_create(const char* fmt, ...);
char *to_delimiter(const char* string, ulong *start_end, char delimiter);

#endif // UTIL_H_

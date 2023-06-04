/* See LICENSE file for copyright and license details. */
#include <stdint.h>
#include <stdio.h>

#define LEN(x) (sizeof (x) / sizeof *(x))

extern char *argv0;

void warn(const char *, ...);
void die(const char *, ...);

void ff_read_header(uint32_t *width, uint32_t *height);
void ff_write_header(uint32_t width, uint32_t height);

int parse_mask(const char *, uint16_t mask[3]);

int fshut(FILE *, const char *);

void efread(void *, size_t, size_t, FILE *);
void efwrite(const void *, size_t, size_t, FILE *);

#undef reallocarray
void *reallocarray(void *, size_t, size_t);
void *ereallocarray(void *optr, size_t nmemb, size_t size);

#undef strtonum
long long strtonum(const char *, long long, long long, const char **);
long long estrtonum(const char *, long long, long long);

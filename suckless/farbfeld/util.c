/* See LICENSE file for copyright and license details. */
#include <arpa/inet.h>

#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "util.h"

char *argv0;

static void
verr(const char *fmt, va_list ap)
{
	if (argv0 && strncmp(fmt, "usage", sizeof("usage") - 1)) {
		fprintf(stderr, "%s: ", argv0);
	}

	vfprintf(stderr, fmt, ap);

	if (fmt[0] && fmt[strlen(fmt) - 1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}
}

void
warn(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	verr(fmt, ap);
	va_end(ap);
}

void
die(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	verr(fmt, ap);
	va_end(ap);

	exit(1);
}

void
ff_read_header(uint32_t *width, uint32_t *height)
{
	uint32_t hdr[4];

	efread(hdr, sizeof(*hdr), LEN(hdr), stdin);

	if (memcmp("farbfeld", hdr, sizeof("farbfeld") - 1)) {
		die("Invalid magic value");
	}

	*width = ntohl(hdr[2]);
	*height = ntohl(hdr[3]);
}

void
ff_write_header(uint32_t width, uint32_t height)
{
	uint32_t tmp;

	fputs("farbfeld", stdout);

	tmp = htonl(width);
	efwrite(&tmp, sizeof(tmp), 1, stdout);

	tmp = htonl(height);
	efwrite(&tmp, sizeof(tmp), 1, stdout);
}

int
parse_mask(const char *s, uint16_t mask[3])
{
	size_t slen, i;
	unsigned int col[3], colfac;
	char fmt[] = "%#x%#x%#x";

	slen = strlen(s);
	if (slen != 3 && slen != 6 && slen != 12) {
		return 1;
	}

	fmt[1] = fmt[4] = fmt[7] = ((slen / 3) + '0');
	if (sscanf(s, fmt, col, col + 1, col + 2) != 3) {
		return 1;
	}

	colfac = (slen == 3) ? UINT16_MAX / 0xf :
	         (slen == 6) ? UINT16_MAX / 0xff :
	                       UINT16_MAX / 0xffff;

	for (i = 0; i < 3; i++) {
		mask[i] = col[i] * colfac;
	}

	return 0;
}

int
fshut(FILE *fp, const char *fname)
{
	int ret = 0;

	/* fflush() is undefined for input streams by ISO C,
	 * but not POSIX 2008 if you ignore ISO C overrides.
	 * Leave it unchecked and rely on the following
	 * functions to detect errors.
	 */
	fflush(fp);

	if (ferror(fp) && !ret) {
		warn("ferror '%s':", fname);
		ret = 1;
	}

	if (fclose(fp) && !ret) {
		warn("fclose '%s':", fname);
		ret = 1;
	}

	return ret;
}

void
efread(void *p, size_t s, size_t n, FILE *f)
{
	if (fread(p, s, n, f) != n) {
		if (ferror(f)) {
			die("fread:");
		} else {
			die("fread: Unexpected end of file");
		}
	}
}

void
efwrite(const void *p, size_t s, size_t n, FILE *f)
{
	if (fwrite(p, s, n, f) != n) {
		die("fwrite:");
	}
}

void *
ereallocarray(void *optr, size_t nmemb, size_t size)
{
	void *p;

	if (!(p = reallocarray(optr, nmemb, size))) {
		die("reallocarray: Out of memory");
	}

	return p;
}

long long
estrtonum(const char *numstr, long long minval, long long maxval)
{
	const char *errstr;
	long long ll;

	ll = strtonum(numstr, minval, maxval, &errstr);
	if (errstr) {
		die("strtonum '%s': %s", numstr, errstr);
	}

	return ll;
}

/*
 * This is sqrt(SIZE_MAX+1), as s1*s2 <= SIZE_MAX
 * if both s1 < MUL_NO_OVERFLOW and s2 < MUL_NO_OVERFLOW
 */
#define MUL_NO_OVERFLOW	(1UL << (sizeof(size_t) * 4))

void *
reallocarray(void *optr, size_t nmemb, size_t size)
{
	if ((nmemb >= MUL_NO_OVERFLOW || size >= MUL_NO_OVERFLOW) &&
	    nmemb > 0 && SIZE_MAX / nmemb < size) {
		errno = ENOMEM;
		return NULL;
	}
	return realloc(optr, size * nmemb);
}

#define	INVALID		1
#define	TOOSMALL	2
#define	TOOLARGE	3

long long
strtonum(const char *numstr, long long minval, long long maxval,
         const char **errstrp)
{
	long long ll = 0;
	int error = 0;
	char *ep;
	struct errval {
		const char *errstr;
		int err;
	} ev[4] = {
		{ NULL,		0 },
		{ "invalid",	EINVAL },
		{ "too small",	ERANGE },
		{ "too large",	ERANGE },
	};

	ev[0].err = errno;
	errno = 0;
	if (minval > maxval) {
		error = INVALID;
	} else {
		ll = strtoll(numstr, &ep, 10);
		if (numstr == ep || *ep != '\0')
			error = INVALID;
		else if ((ll == LLONG_MIN && errno == ERANGE) || ll < minval)
			error = TOOSMALL;
		else if ((ll == LLONG_MAX && errno == ERANGE) || ll > maxval)
			error = TOOLARGE;
	}
	if (errstrp != NULL)
		*errstrp = ev[error].errstr;
	errno = ev[error].err;
	if (error)
		ll = 0;

	return (ll);
}

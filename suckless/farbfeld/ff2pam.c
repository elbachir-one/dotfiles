/* See LICENSE file for copyright and license details. */
#include <arpa/inet.h>

#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	die("usage: %s", argv0);
}

int
main(int argc, char *argv[])
{
	size_t rowlen;
	uint32_t width, height, i;
	uint16_t *row;

	/* arguments */
	argv0 = argv[0], argc--, argv++;

	if (argc) {
		usage();
	}

	/* prepare */
	ff_read_header(&width, &height);
	row = ereallocarray(NULL, width, (sizeof("RGBA") - 1) * sizeof(uint16_t));
	rowlen = width * (sizeof("RGBA") - 1);

	/* write data */
	printf("P7\n"
	       "WIDTH %" PRIu32 "\n"
	       "HEIGHT %" PRIu32 "\n"
	       "DEPTH 4\n" /* number of channels */
	       "MAXVAL 65535\n"
	       "TUPLTYPE RGB_ALPHA\n"
	       "ENDHDR\n",
	       width, height);

	for (i = 0; i < height; i++) {
		efread(row, sizeof(uint16_t), rowlen, stdin);
		efwrite(row, sizeof(uint16_t), rowlen, stdout);
	}

	return fshut(stdout, "<stdout>");
}

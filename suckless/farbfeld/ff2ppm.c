/* See LICENSE file for copyright and license details. */
#include <arpa/inet.h>

#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg.h"
#include "util.h"

static void
usage(void)
{
	die("usage: %s [-b colour]", argv0);
}

int
main(int argc, char *argv[])
{
	size_t rowlen, rowoutlen;
	uint64_t a;
	uint32_t width, height, i, j, k, l;
	uint16_t *row, mask[3] = { 0xffff, 0xffff, 0xffff };
	uint8_t *rowout;

	/* arguments */
	ARGBEGIN {
	case 'b':
		if (parse_mask(EARGF(usage()), mask)) {
			usage();
		}
		break;
	default:
		usage();
	} ARGEND

	if (argc) {
		usage();
	}

	/* prepare */
	ff_read_header(&width, &height);
	row = ereallocarray(NULL, width, (sizeof("RGBA") - 1) * sizeof(uint16_t));
	rowout = ereallocarray(NULL, width, (sizeof("RGB") - 1) * sizeof(uint8_t));
	rowlen = width * (sizeof("RGBA") - 1);
	rowoutlen = width * (sizeof("RGB") - 1);

	/* write data */
	printf("P6\n%" PRIu32 " %" PRIu32 "\n255\n", width, height);

	for (i = 0; i < height; ++i) {
		efread(row, sizeof(uint16_t), rowlen, stdin);

		for (j = 0, k = 0; j < rowlen; j += 4, k += 3) {
			a = ntohs(row[j + 3]);
			for (l = 0; l < 3; l++) {
				/* alpha blending and 8-bit-reduction */
				rowout[k + l] = (a * ntohs(row[j + l]) +
				                 (UINT16_MAX - a) * mask[l]) /
				                (UINT16_MAX *
						 (UINT16_MAX / UINT8_MAX));
			}
		}

		efwrite(rowout, sizeof(uint8_t), rowoutlen, stdout);
	}

	return fshut(stdout, "<stdout>");
}

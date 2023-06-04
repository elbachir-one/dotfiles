/* See LICENSE file for copyright and license details. */
#include <arpa/inet.h>

#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jpeglib.h>

#include "arg.h"
#include "util.h"

static void
jpeg_error(j_common_ptr js)
{
	fprintf(stderr, "%s: libjpeg: ", argv0);
	(*js->err->output_message)(js);
	exit(1);
}

static void
jpeg_setup_writer(struct jpeg_compress_struct *s, struct jpeg_error_mgr *e,
                  uint32_t w, uint32_t h, int quality, int opt)
{
	jpeg_create_compress(s);
	e->error_exit = jpeg_error;
	s->err = jpeg_std_error(e);

	jpeg_stdio_dest(s, stdout);
	s->image_width = w;
	s->image_height = h;
	s->input_components = 3;     /* color components per pixel */
	s->in_color_space = JCS_RGB; /* output color space */
	jpeg_set_defaults(s);

	if (opt) {
		s->optimize_coding = 1;
	}
	jpeg_set_quality(s, quality, 1);

	jpeg_start_compress(s, 1);
}

static void
usage(void)
{
	die("usage: %s [-b colour] [-o] [-q quality]", argv0);
}

int
main(int argc, char *argv[])
{
	struct jpeg_compress_struct jcomp;
	struct jpeg_error_mgr jerr;
	size_t rowlen;
	uint64_t a;
	uint32_t width, height, i, j, k, l;
	uint16_t *row, mask[3] = { 0xffff, 0xffff, 0xffff };
	uint8_t *rowout;
	int optimize = 0, quality = 85;

	/* arguments */
	ARGBEGIN {
	case 'b':
		if (parse_mask(EARGF(usage()), mask)) {
			usage();
		}
		break;
	case 'o':
		optimize = 1;
		break;
	case 'q':
		quality = estrtonum(EARGF(usage()), 0, 100);
		break;
	default:
		usage();
	} ARGEND

	if (argc) {
		usage();
	}

	/* prepare */
	ff_read_header(&width, &height);
	jpeg_setup_writer(&jcomp, &jerr, width, height, quality, optimize);
	row = ereallocarray(NULL, width, (sizeof("RGBA") - 1) * sizeof(uint16_t));
	rowlen = width * (sizeof("RGBA") - 1);
	rowout = ereallocarray(NULL, width, (sizeof("RGB") - 1) * sizeof(uint8_t));

	/* write data */
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
		jpeg_write_scanlines(&jcomp, &rowout, 1);
	}

	/* clean up */
	jpeg_finish_compress(&jcomp);
	jpeg_destroy_compress(&jcomp);

	return fshut(stdout, "<stdout>");
}

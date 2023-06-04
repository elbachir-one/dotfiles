/* See LICENSE file for copyright and license details. */
#include <arpa/inet.h>

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jpeglib.h>

#include "util.h"

static void
jpeg_error(j_common_ptr js)
{
	fprintf(stderr, "%s: libjpeg: ", argv0);
	(*js->err->output_message)(js);
	exit(1);
}

static void
jpeg_setup_reader(struct jpeg_decompress_struct *s, struct jpeg_error_mgr *e,
                  uint32_t *w, uint32_t *h)
{
	jpeg_create_decompress(s);
	e->error_exit = jpeg_error;
	s->err = jpeg_std_error(e);

	jpeg_stdio_src(s, stdin);
	jpeg_read_header(s, 1);
	*w = s->image_width;
	*h = s->image_height;
	s->output_components = 3;     /* color components per pixel */
	s->out_color_space = JCS_RGB; /* input color space */

	jpeg_start_decompress(s);
}

static void
usage(void)
{
	die("usage: %s", argv0);
}

int
main(int argc, char *argv[])
{
	struct jpeg_decompress_struct js;
	struct jpeg_error_mgr jerr;
	uint32_t width, height;
	uint16_t *row;
	uint8_t *rowin;
	size_t rowlen, i;

	/* arguments */
	argv0 = argv[0], argc--, argv++;

	if (argc) {
		usage();
	}

	/* prepare */
	jpeg_setup_reader(&js, &jerr, &width, &height);
	row = ereallocarray(NULL, width, (sizeof("RGBA") - 1) * sizeof(uint16_t));
	rowlen = width * (sizeof("RGBA") - 1);
	rowin = ereallocarray(NULL, width, (sizeof("RGB") - 1) * sizeof(uint8_t));

	/* write data */
	ff_write_header(width, height);

	while (js.output_scanline < js.output_height) {
		jpeg_read_scanlines(&js, &rowin, 1);

		for (i = 0; i < width; ++i) {
			row[4 * i + 0] = htons(rowin[3 * i + 0] * 257);
			row[4 * i + 1] = htons(rowin[3 * i + 1] * 257);
			row[4 * i + 2] = htons(rowin[3 * i + 2] * 257);
			row[4 * i + 3] = htons(65535);
		}

		efwrite(row, sizeof(uint16_t), rowlen, stdout);
	}

	/* clean up */
	jpeg_finish_decompress(&js);
	jpeg_destroy_decompress(&js);

	return fshut(stdout, "<stdout>");
}

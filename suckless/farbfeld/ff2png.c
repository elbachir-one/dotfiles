/* See LICENSE file for copyright and license details. */
#include <arpa/inet.h>

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <png.h>

#include "util.h"

static void
png_err(png_struct *pngs, const char *msg)
{
	(void)pngs;
	die("libpng: %s", msg);
}

static void
png_setup_writer(png_struct **s, png_info **i, uint32_t w, uint32_t h)
{
	*s = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, png_err, NULL);
	*i = png_create_info_struct(*s);

	if (!*s || !*i) {
		die("Failed to initialize libpng");
	}

	png_init_io(*s, stdout);
	png_set_IHDR(*s, *i, w, h, 16, PNG_COLOR_TYPE_RGB_ALPHA,
	             PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
	             PNG_FILTER_TYPE_BASE);
	png_write_info(*s, *i);
}

static void
usage(void)
{
	die("usage: %s", argv0);
}

int
main(int argc, char *argv[])
{
	png_struct *pngs;
	png_info *pngi;
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
	png_setup_writer(&pngs, &pngi, width, height);
	row = ereallocarray(NULL, width, (sizeof("RGBA") - 1) * sizeof(uint16_t));
	rowlen = width * (sizeof("RGBA") - 1);

	/* write data */
	for (i = 0; i < height; ++i) {
		efread(row, sizeof(uint16_t), rowlen, stdin);
		png_write_row(pngs, (uint8_t *)row);
	}

	/* clean up */
	png_write_end(pngs, NULL);
	png_destroy_write_struct(&pngs, NULL);

	return fshut(stdout, "<stdout>");
}

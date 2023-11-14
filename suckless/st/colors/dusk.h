static const char *colorname[] = {
	/* 8 normal colors */
	"#361f24",
	"#832f01",
	"#fb9756",
	"#ffde6e",
	"#6b789b",
	"#7c6a8e",
	"#94b5ea",
	"#fef9cd",

	/* 8 bright colors */
	"#270e05",
	"#6b290f",
	"#bd4307",
	"#ffba51",
	"#5376ae",
	"#654d86",
	"#97aee4",
	"#ffffff",

	[255] = 0,

	/* more colors can be added after 255 to use with DefaultXX */
	"#ffffff",
	"#ffffff",
	"#fef9cd", /* default foreground colour */
	"#361f24", /* default background colour */
};


/*
 * Default colors (colorname index)
 * foreground, background, cursor, reverse cursor
 */
unsigned int defaultfg = 258;
unsigned int defaultbg = 259;
unsigned int defaultcs = 256;
static unsigned int defaultrcs = 257;

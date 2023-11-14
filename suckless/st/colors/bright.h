/* Terminal colors (16 first used in escape sequence) */

static const char *colorname[] = {
	"#1d2021", /* hard contrast: #1d2021 / soft contrast: #32302f */
	"#f28fad",
	"#abe9b3",
	"#fae3b0",
	"#06cdfb",
	"#f5c2e7",
	"#89dceb",
	"#d9e0ee",
	"#988ba2",
	"#f28fad",
	"#abe9b3",
	"#fae3b0",
	"#96cdfb",
	"#f5c2e7",
	"#89dceb",
	"#d9e0ee",
	[255] = 0,
	/* more colors can be added after 255 to use with DefaultXX */
	"#add8e6", /* 256 -> cursor */
	"#555555", /* 257 -> rev cursor*/
	"#1e1e1e", /* 258 -> bg */
	"#eff1f5", /* 259 -> fg */
};
/*
 * Default colors (colorname index)
 * foreground, background, cursor, reverse cursor
 */
unsigned int defaultfg = 259;
unsigned int defaultbg = 258;
unsigned int defaultcs = 256;
unsigned int defaultrcs = 257;

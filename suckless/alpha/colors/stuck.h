/* Terminal colors (16 first used in escape sequence) */

static const char *colorname[] = {
    /* 8 normal colors */
    "black",
    "red3",
    "green3",
    "yellow3",
    "blue2",
    "magenta3",
    "cyan3",
    "gray90",

    /* 8 bright colors */
    "gray50",
    "red",
    "green",
    "yellow",
    "#5c5cff",
    "magenta",
    "cyan",
    "white",

    [255] = 0,

    /* more colors can be added after 255 to use with DefaultXX */
    "#cccccc",
    "#555555",
    "gray90", /* default foreground colour */
    "black", /* default background colour */
};

/*
 * Default colors (colorname index)
 * foreground, background, cursor, reverse cursor
 */
unsigned int defaultfg = 258;
unsigned int defaultbg = 259;
unsigned int defaultcs = 256;
static unsigned int defaultrcs = 257;

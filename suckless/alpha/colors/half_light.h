 /* Terminal colors (16 first used in escape sequence) */

static const char *colorname[] = {
    /* 8 normal colors */

    "#fafafa",
    "#e45649",
    "#50a14f",
    "#c18401",
    "#0184bc",
    "#a626a4",
    "#0997b3",
    "#383a42",

    "#fafafa",
    "#e45649",
    "#50a14f",
    "#c18401",
    "#0184bc",
    "#a626a4",
    "#0997b3",
    "#383a42",

    [255] = 0,

    "#ffffff",
    "#000000",
    "#cccccc",
    "#555555",
};

/*
 * Default colors (colorname index)
 * foreground, background, cursor, reverse cursor
 */
unsigned int defaultfg = 258;
unsigned int defaultbg = 259;
unsigned int defaultcs = 256;
static unsigned int defaultrcs = 257;

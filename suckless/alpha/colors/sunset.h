/* Terminal colors (16 first used in escape sequence) */

static const char *colorname[] = {

    "#000000",
    "#ffffff",
    "#c990fc",
    "#383e30",
    "#565f4a",
    "#7b866a",
    "#a5b490",
    "#d6e9bb",
    "#c8a0ef",
    "#c697f2",
    "#2fb0d7",
    "#d39758",
    "#c990fc",
    "#f7c4d7",
    "#fba5c8",
    "#e0931e",
    [255] = 0,

    "#add8e6", /* 256 -> cursor */
    "#555555", /* 257 -> rev cursor*/
    "#000000", /* 258 -> bg */
    "#f38813", /* 259 -> fg */
};

/*
 * Default colors (colorname index)
 * foreground, background, cursor, reverse cursor
 */
unsigned int defaultfg = 259;
unsigned int defaultbg = 258;
unsigned int defaultcs = 256;
unsigned int defaultrcs = 257;

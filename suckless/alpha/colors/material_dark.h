 /* Terminal colors (16 first used in escape sequence) */

static const char *colorname[] = {
    /* 8 normal colors */
    "#073642",
    "#eb606b",
    "#c3e88d",
    "#f7eb95",
    "#80cbc4",
    "#ff2490",
    "#aeddff",
    "#ffffff",

    /* 8 bright colors */
    "#ff002b",
    "#eb606b",
    "#c3e88d",
    "#f7eb95",
    "#7dc6bf",
    "#6c71c4",
    "#35434d",
    "#ffffff",

    [255] = 0,

    /* more colors can be added after 255 to use with DefaultXX */
    "#c4c7d1",
    "#263238",
};


unsigned int defaultfg = 256;
unsigned int defaultbg = 257;
unsigned int defaultcs = 256;
static unsigned int defaultrcs = 257;

static const char *colorname[] = {
    "#26233a",
    "#eb6f92",
    "#9ccfd8",
    "#f6c177",
    "#31748f",
    "#c4a7e7",
    "#ebbcba",
    "#e0def4",
    "#6e6a86",
    "#eb6f92",
    "#9ccfd8",
    "#f6c177",
    "#31748f",
    "#c4a7e7",
    "#ebbcba",
    "#e0def4",
    [255] = 0,
    /* more colors can be added after 255 to use with DefaultXX */
    [256] = "#191724",
};


/*
 * Default colors (colorname index)
 * foreground, background, cursor, reverse cursor
 */
unsigned int defaultbg = 256;
unsigned int defaultfg = 7;
unsigned int defaultcs = 7;
unsigned int defaultrcs = 7;

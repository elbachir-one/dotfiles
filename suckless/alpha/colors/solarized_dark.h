/* Terminal colors (16 first used in escape sequence) */

static const char *colorname[] = {
    "#073642",  /*  0: black    */
    "#dc322f",  /*  1: red      */
    "#859900",  /*  2: green    */
    "#b58900",  /*  3: yellow   */
    "#268bd2",  /*  4: blue     */
    "#d33682",  /*  5: magenta  */
    "#2aa198",  /*  6: cyan     */
    "#eee8d5",  /*  7: white    */
    "#002b36",  /*  8: brblack  */
    "#cb4b16",  /*  9: brred    */
    "#586e75",  /* 10: brgreen  */
    "#657b83",  /* 11: bryellow */
    "#839496",  /* 12: brblue   */
    "#6c71c4",  /* 13: brmagenta*/
    "#93a1a1",  /* 14: brcyan   */
    "#fdf6e3",  /* 15: brwhite  */
};

/*
 * Default colors (colorname index)
 * foreground, background, cursor, reverse cursor
 */

unsigned int defaultfg = 12;
unsigned int defaultbg = 8;
unsigned int defaultcs = 14;
static unsigned int defaultrcs = 15;

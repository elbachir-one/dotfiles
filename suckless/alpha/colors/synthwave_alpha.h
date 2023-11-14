static const char *colorname[] = {
    "#241b30", /*  0: black   */
    "#9a0048", /*  1: red     */
    "#00986c", /*  2: green   */
    "#adad3e", /*  3: yellow  */
    "#6e29ad", /*  4: blue    */
    "#b300ad", /*  5: magenta */
    "#00b0b1", /*  6: cyan    */
    "#b9b1bc", /*  7: white   */
    "#7f7094", /*  8: brblack */
    "#e60a70", /*  9: brred   */
    "#0ae4a4", /* 10: brgreen */
    "#f9f972", /* 11: bryellow*/
    "#aa54f9", /* 12: brblue  */
    "#ff00f6", /* 13: brmagenta*/
    "#00fbfd", /* 14: brcyan  */
    "#f2f2e3", /* 15: brwhite */
        [255] = 0,
    /* more colors can be added after 255 to use with DefaultXX */
    "#add8e6", /* 256 -> cursor */
    "#555555", /* 257 -> rev cursor*/
    "#ffffff", /* 259 -> fg */
    "#49226f", /* 258 -> bg */

};

/*
 * Default colors (colorname index)
 * foreground, background, cursor, reverse cursor
 */
unsigned int defaultfg = 258;
unsigned int defaultbg = 259;
unsigned int defaultcs = 256;
static unsigned int defaultrcs = 257;

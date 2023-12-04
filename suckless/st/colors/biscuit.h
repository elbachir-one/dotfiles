static const char *colorname[] = {
    /* 8 normal colors */
    [0] = "#2d2424", /* black   */
    [1] = "#cf2241", /* red     */
    [2] = "#f07942", /* green   */
    [3] = "#e39c45", /* yellow  */
    [4] = "#949f6b", /* blue    */
    [5] = "#716c97", /* magenta */
    [6] = "#ae3f8a", /* cyan    */
    [7] = "#9c8181", /* white   */

    /* 8 bright colors */
    [8]  = "#725a5a",  /* black   */
    [9]  = "#e3556f",  /* red     */
    [10] = "#f49d75", /* green   */
    [11] = "#edbf86", /* yellow  */
    [12] = "#b0b893", /* blue    */
    [13] = "#9894b3", /* magenta */
    [14] = "#ca6bac", /* cyan    */
    [15] = "#ffe9c7", /* white   */

    [255] = 0,

    /* more colors can be added after 255 to use with DefaultXX */
    "#1a1515", /* background */
    "#ffe9c7", /* foreground */
    "#ffe9c7", /* cursor */
    "#555555", /* 257 -> rev cursor*/
};

unsigned int defaultfg = 257;
unsigned int defaultbg = 256;
unsigned int defaultcs = 257;
static unsigned int defaultrcs = 257;

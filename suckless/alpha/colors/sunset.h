/* Terminal colors (16 first used in escape sequence) */

static const char *colorname[] = {

    "#000000", /* black   */
    "#0f110b", /* red     */
    "#21241b", /* green   */
    "#383e30", /* yellow  */
    "#565f4a", /* blue    */
    "#7b866a", /* magenta */
    "#a5b490", /* cyan    */
    "#d6e9bb", /* white   */
    "#c8a0ef", /* black   */
    "#c697f2", /* red     */
    "#2fb0d7", /* green   */
    "#d39758", /* yellow  */
    "#c990fc", /* blue    */
    "#f7c4d7", /* magenta */
    "#fba5c8", /* cyan    */
    "#e0931e", /* white   */
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

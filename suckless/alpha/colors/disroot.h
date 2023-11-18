const char *colorname[] = {

  /* 8 normal colors */
  [0] = "#0C020C", /* black   */
  [1] = "#635650", /* red     */
  [2] = "#2B3D42", /* blue    */
  [3] = "#8E6A2D", /* yellow  */
  [4] = "#35534B", /* blue    */
  [5] = "#B9723C", /* magenta */
  [6] = "#C19341", /* cyan    */
  [7] = "#ffffff", /* white   */
	

  /* 8 bright colors */
  [8] = "#ffffff", /* black   */
  [9] = "#635650", /* red     */
  [10] = "#35534B", /* green   */
  [11] = "#8E6A2D", /* yellow  */
  [12] = "#35534B", /* blue    */
	[13] = "#B9723C", /* magenta */
  [14] = "#C19341", /* cyan    */
  [15] = "#ffffff", /* white   */

  /* special colors */
  [256] = "#0C020C", /* background */
  [257] = "#ffffff", /* foreground */
  [258] = "#ffffff",     /* cursor */
};

/* Default colors (colorname index)
 * foreground, background, cursor */
 unsigned int defaultbg = 0;
 unsigned int defaultfg = 257;
 unsigned int defaultcs = 258;
 unsigned int defaultrcs= 258;

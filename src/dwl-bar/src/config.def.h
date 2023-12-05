#ifndef CONFIG_H_
#define  CONFIG_H_

#include "user.h"
#include <stdlib.h>
#include <linux/input-event-codes.h>

static const int show_bar = 1;
static const int bar_top = 1;          /* Boolean value, non-zero is true. If not top then bottom */
static const int status_on_active = 1; /* Display the status on active monitor only. If not then on all. */
static const char *font = "Monospace 10";
static const char *terminal[] = { "sakura", NULL };

/*
 * Colors:
 * Colors are in rgba format.
 * The color scheming format is the same as dwm.
 * We use an enum as a index for our scheme types.
 *
 * cyan  - used in an active background
 * grey3 - used in active text and urgent background
 * grey1 - used in an inactive background
 * grey2 - used in inactive text
 */
static const int cyan[4]  = { 0,   85,  119, 255 };
static const int grey1[4] = { 34,  34,  34,  255 };
static const int grey2[4] = { 187, 187, 187, 255 };
static const int grey3[4] = { 238, 238, 238, 255 };

static const int *schemes[3][2] = {
    /* Scheme Type       fg,    bg */
    [InActive_Scheme] = {grey2, grey1},
    [Active_Scheme]   = {grey3, cyan},
    [Urgent_Scheme]   = {grey1, grey3},
};

/*
 * Tags
 * Must not exceed 31 tags and amount must match dwl's tagcount.
 */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/*
 * Buttons
 * See user.h for details on relevant structures.
 */
static const Binding bindings[] = {
    /* Click Location,   button,        callback,     bypass,    arguments */
    { Click_Layout,      BTN_LEFT,      layout,       1,         {.ui = 0} },
    { Click_Layout,      BTN_RIGHT,     layout,       1,         {.ui = 1} },
    { Click_Status,      BTN_MIDDLE,    spawn,        0,         {.v = terminal } },
    { Click_Tag,         BTN_MIDDLE,    tag,          0,         {0} },
    { Click_Tag,         BTN_RIGHT,     toggle_view,  0,         {0} },
    { Click_Tag,         BTN_LEFT,      view,         0,         {0} },
};

#endif // CONFIG_H_

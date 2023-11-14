/* Terminal colors (16 first used in escape sequence) */

static const char *colorname[] = {
    /* 8 normal colors */
    "#212121",
    "#b7141f",
    "#457b24",
    "#f6981e",
    "#134eb2",
    "#560088",
    "#0e717c",
    "#efefef",

    /* 8 bright colors */
    "#424242",
    "#e83b3f",
    "#7aba3a",
    "#ffea2e",
    "#54a4f3",
    "#aa4dbc",
    "#26bbd1",
    "#d9d9d9",

    [255] = 0,

    /* more colors can be added after 255 to use with DefaultXX */
    "#e5e5e5",
    "#232322",
};

unsigned int defaultfg = 256;
unsigned int defaultbg = 257;
unsigned int defaultcs = 256;
static unsigned int defaultrcs = 257;

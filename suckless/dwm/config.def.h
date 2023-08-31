/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx     = 0;        /* border pixel of windows */
static const int startwithgaps[]       = { 1 };	/* 1 means gaps are used by default, this can be customized for each tag */
static const unsigned int gappx[]      = { 2 };   /* default gap between windows in pixels, this can be customized for each tag */
static const unsigned int snap         = 8;       /* snap pixel */
static const int swallowfloating       = 0;        /* 1 means swallow floating windows by default */
static const int showbar               = 1;        /* 0 means no bar */
static const int topbar                = 1;        /* 0 means bottom bar */
static const int user_bh               = 0;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int vertpad               = 2;       /* vertical padding of bar */
static const int sidepad               = 4;       /* horizontal padding of bar */
static const char *fonts[]             = { "terminus:style=Bold:size=14" };
static const char dmenufont[]          = "monospace:size=10";
static const char col_gray1[]          = "#000000";
static const char col_gray2[]          = "#444444";
static const char col_gray3[]          = "#dddfff";
static const char col_gray4[]          = "#ffffff";
static const char col_cyan[]           = "#202020";
static const unsigned int baralpha     = 0xd0;
static const unsigned int borderalpha  = OPAQUE;
static const char *colors[][3]         = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static const unsigned int alphas[][3]  = {
    /*               fg      bg        border*/
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
    [SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x28", NULL };
const char *spcmd2[] = {"st", "-n", "spfm", "-g", "124x30", "-e", "ranger", NULL };
const char *spcmd3[] = {"st", "-n", "spncm", "-g", "120x30", "-e", "ncmpcpp", NULL };
const char *spcmd4[] = {"st", "-n", "spclock", "-g", "80x20", "-e", "tty-clock", "-cC", "6", NULL };
const char *spcmd5[] = {"st", "-n", "spcava", "-g", "120x30", "-e", "cava", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
	{"spncm",       spcmd3},
	{"spclock",     spcmd4},
	{"spcava",      spcmd5},
};

static const char *const autostart[] = {
	"st", NULL,
	"/home/sh/.fehbg", NULL,
	"picom", NULL,
	"slstatus", NULL,
	"mpd", NULL,
	"dunst", NULL,
	"clipmenud", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = {"", "", "", "", "", "", "", "", ""};
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
    /* class              instance          title                 tags mask       isfloating     isterminal   noswallow    monitor */

    {"discord",             NULL,           NULL, 	          1 << 3, 	    False, 	     0,         -1,       -1},
    {"Surf", 	            NULL,           NULL, 	          1 << 3, 	    False, 	     0,          0,       -1},
    {"firefox",             NULL,           NULL, 	          1 << 3, 	    False, 	     0,         -1,       -1},
    {"obs",                 NULL,           NULL, 	          1 << 7, 	    False, 	     0,         -1,       -1},
    {"Surf",                NULL,           NULL, 	          1 << 3, 	    False, 	     0,         -1,       -1},
    {"Virt-manager", 	    NULL,           NULL, 	          1 << 8, 	    False, 	     0,         -1,       -1},
    {"Chromium", 	    NULL,           NULL, 	          1 << 1, 	    False, 	     0,         -1,       -1},
    {"Transmission-gtk",    NULL,           NULL, 	          1 << 5, 	    False, 	     0,         -1,       -1},
    {"Gimp", 	            NULL,           NULL, 	          1 << 5, 	    False, 	     0,         -1,       -1},
    {"kdenlive", 	    NULL,           NULL, 	          1 << 3, 	    False, 	     0,         -1,       -1},
    {"st",                  NULL,           "ranger",             1 << 6, 	    False,           0,         -1,       -1},
    {"qutebrowser", 	    NULL,           NULL, 	          1 << 3, 	    False,           0,         -1,       -1},
    {"st",                  NULL,           NULL, 	          0,                0,               1,         1,        -1},
    {"Alacritty", 	    NULL,           NULL, 	          0,                0,               1,         1,        -1},
    {"Sakura", 	            NULL,           NULL, 	          0,                0,               1,         1,        -1},
    { NULL, 	            NULL,           "Event Tester",       0,                0,               0,         1,        -1},
    { NULL,	            "spterm",	    NULL,	          SPTAG(0),         1,		     0,         0,        -1 },
    { NULL,	            "spfm",         NULL,	          SPTAG(1),         1,		     0,         0,        -1 },
    { NULL,	            "spncm",        NULL,	          SPTAG(2),         1,		     0,         0,        -1 },
    { NULL,	            "spclock",      NULL,	          SPTAG(3),         1,		     0,         0,        -1 },
    { NULL,	            "spcava",       NULL,	          SPTAG(4),         1,		     0,         0,        -1 },
};

/* layout(s) */
static const float mfact        = 0.60; /* factor of master area size [0.05..0.95] */
static const int nmaster        = 1;    /* number of clients in master area */
static const int resizehints    = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1;    /* 1 will force focus on the fullscreen window */

static const Layout layouts[]   = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */

static char dmenumon[2]	            = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]       = { "dmenu_run", NULL };
static const char *termcmd[]        = { "st", NULL };
static const char *clipcmd[]        = { "clipmenu", NULL };
static const char *rancmd[]         = { "st", "-e", "ranger", NULL };
static const char *lockcmd[]        = { "slock", NULL };
static const char *screcmd[]        = { "scrot", "/home/sh/Images/ScreenShots/shot-%Y-%m-%d-%H-%M-%S.jpg", NULL };
static const char *virtcmd[]        = { "virt-manager", NULL };
static const char *chrocmd[]        = { "chromium", NULL };
static const char *boatcmd[]        = { "st", "-e", "newsboat", NULL };
static const char *ytcmd[]          = { "ytfzf", "-D", NULL };
static const char *ytdcmd[]         = { "ytfzf", "-d", "-D", NULL };
static const char *flamcmd[]        = { "flameshot", "gui", NULL };
static const char *up[]             = { "amixer", "set", "Master", "10%+", NULL };
static const char *mut[]            = { "amixer", "set", "Master", "toggle", NULL };
static const char *down[]           = { "amixer", "set", "Master", "10%-", NULL };
static const char *toggle[]         = { "mpc", "toggle", NULL };
static const char *next[]           = { "mpc", "next", NULL };
static const char *prev[]           = { "mpc", "prev", NULL };
static const char *searchcmd[]      = { "search", NULL };
static const char *dowcmd[]         = { "down", NULL };
static const Key keys[] = {

	/* modifier                  key                      function                 argument */

	{MODKEY,                    XK_s,  	                togglescratch,          {.ui = 0 } },
	{MODKEY|ShiftMask,          XK_r,	                togglescratch,          {.ui = 1 } },
	{MODKEY,                    XK_e,	                togglescratch,          {.ui = 2 } },
	{MODKEY,                    XK_a,	                togglescratch,          {.ui = 3 } },
	{MODKEY,                    XK_c,	                togglescratch,          {.ui = 4 } },
	{MODKEY,                    XK_agrave,                  view,                   {.ui = ~0 } },
	{MODKEY|ShiftMask,          XK_agrave,                  tag,                    {.ui = ~0 } },
	{MODKEY,                    XK_semicolon,               focusmon,               {.i = +1 } },
	{MODKEY|ShiftMask,          XK_semicolon,               tagmon,                 {.i = +1 } },
	{MODKEY,                    XK_x, 		        spawn, 		        {.v = dowcmd}},
	{MODKEY,                    XK_y, 		        spawn, 		        {.v = ytcmd}},
	{MODKEY,                    XK_d, 		        spawn, 		        {.v = ytdcmd}},
	{MODKEY,                    XK_f, 		        spawn, 		        {.v = searchcmd}},
	{MODKEY | ShiftMask,        XK_f, 		        spawn, 		        {.v = flamcmd}},
	{MODKEY | ControlMask,      XK_f,                       togglefullscr,          {0} },
	{MODKEY | ShiftMask,        XK_n, 		        spawn, 		        {.v = boatcmd}},
	{MODKEY | ShiftMask,        XK_l, 		        spawn, 		        {.v = lockcmd}},
	{MODKEY | ShiftMask,        XK_s, 		        spawn, 		        {.v = screcmd}},
	{MODKEY, 	            XK_w, 		        spawn, 		        {.v = chrocmd}},
	{MODKEY,   	            XK_v, 		        spawn, 		        {.v = virtcmd}},
	{MODKEY, 	            XK_r, 		        spawn, 		        {.v = rancmd}},
	{MODKEY, 	            XK_p, 		        spawn, 		        {.v = dmenucmd}},
	{MODKEY | ShiftMask, 	    XK_Return,                  spawn, 		        {.v = termcmd}},
	{MODKEY,		    XK_n, 		        spawn, 		        {.v = clipcmd}},
	{MODKEY,		    XK_b, 		        togglebar, 	        {0}},
	{MODKEY,		    XK_k, 		        focusstack, 	        {.i = +1}},
	{MODKEY,		    XK_l, 		        focusstack,             {.i = -1}},
	{MODKEY,		    XK_i, 		        incnmaster, 	        {.i = +1}},
	{MODKEY,		    XK_o, 		        incnmaster,             {.i = -1}},
	{MODKEY,		    XK_j, 		        setmfact, 	        {.f = -0.03}},
	{MODKEY,		    XK_m, 		        setmfact, 	        {.f = +0.03}},
	{MODKEY,		    XK_Return, 	    	        zoom, 		        {0}},
	{MODKEY,		    XK_Tab, 	      	        view, 		        {0}},
	{MODKEY,		    XK_space,     		setlayout, 	        {0}},
	{MODKEY | ShiftMask, 	    XK_space,                   togglefloating,         {0}},
	{MODKEY | ShiftMask, 	    XK_x, 		        view, 		        {.ui = ~0}},
	{MODKEY | ShiftMask,        XK_z, 		        tag, 		        {.ui = ~0}},
	{MODKEY,                    XK_g,                       setgaps,                {.i = -5 } },
	{MODKEY,                    XK_h,                       setgaps,                {.i = +5 } },
	{MODKEY|ShiftMask,          XK_g,                       setgaps,                {.i = GAP_RESET } },
	{MODKEY|ShiftMask,          XK_h,                       setgaps,                {.i = GAP_TOGGLE} },
	{MODKEY | ShiftMask, 	    XK_c, 		        killclient, 	        {0}},
	{0, 		            XK_F3, 		        spawn, 		        {.v = up}},
	{0, 		            XK_F2, 		        spawn, 		        {.v = down}},
	{0, 		            XK_F4, 		        spawn, 		        {.v = mut}},
	{0, 		            XK_F6, 		        spawn, 		        {.v = toggle}},
	{0, 		            XK_F8, 		        spawn, 		        {.v = next}},
	{0, 		            XK_F7, 		        spawn, 		        {.v = prev}},
	{MODKEY|ShiftMask,          XK_q,                       quit,                   {0} },
	TAGKEYS(                    XK_ampersand,                       0)
	TAGKEYS(                    XK_eacute,                          1)
	TAGKEYS(                    XK_quotedbl,                        2)
	TAGKEYS(                    XK_apostrophe,                      3)
	TAGKEYS(                    XK_parenleft,                       4)
	TAGKEYS(                    XK_minus,                           5)
	TAGKEYS(                    XK_egrave,                          6)
	TAGKEYS(                    XK_underscore,                      7)
	TAGKEYS(                    XK_ccedilla,                        8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

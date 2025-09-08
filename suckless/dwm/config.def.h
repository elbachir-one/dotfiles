
/* appearance */
static const unsigned int borderpx     = 0;       /* border pixel of windows */
static const int startwithgaps[]       = { 1 };   /* 1 means gaps are used by default, this can be customized for each tag */
static const unsigned int gappx[]      = { 2 };   /* default gap between windows in pixels, this can be customized for each tag */
static const unsigned int snap         = 8;       /* snap pixel */
static const int swallowfloating       = 0;       /* 1 means swallow floating windows by default */
static const unsigned int scph         = 6;       // Change the height of the scratchpads
static const int showbar               = 1;       /* 0 means no bar */
static const int topbar                = 1;       /* 0 means bottom bar */
static const int user_bh               = 0;       /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int vertpad               = 2;       /* vertical padding of bar */
static const int sidepad               = 4;       /* horizontal padding of bar */

static const char *fonts[]             = {
	"TerminessTTF Nerd Font:style=Bold:pixelsize=18:antialias=true:autohint=true",
	"JoyPixels:style=Bold:pixelsize=16:antialias=true:autohint=true"
};

static const char dmenufont[]          = "TerminessTTF Nerd Font:style=Bold:pixelsize=18:antialias=true:autohint=true";

static const char col_gray1[]          = "#000000";
static const char col_gray2[]          = "#444444";
static const char col_gray3[]          = "#dddfff";
static const char col_gray4[]          = "#ffffff";
static const char col_gray5[]          = "#805f4e";
static const char col_cyan1[]          = "#000000";
static const char col_cyan2[]          = "#00ffff";

static const unsigned int baralpha     = 0xc0;
static const unsigned int borderalpha  = OPAQUE;

static const char *colors[][4]         = {
	/*                       fg         bg       border     float    */
	[SchemeNorm]      = { col_gray3, col_gray1, col_gray2, col_gray5 },
	[SchemeSel]       = { col_gray4, col_cyan1, col_cyan1, col_cyan2 },
	[SchemeStatus]    = { col_gray3, col_gray1, "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]   = { "#000000", "#0079aa", "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm]  = { col_gray3, col_gray1, "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]   = { col_gray4, col_cyan1, "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm]  = { col_gray3, col_gray1, "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

static const unsigned int alphas[][3]  = {
	/*                      fg      bg        border*/
	[SchemeNorm]      = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]       = { OPAQUE, baralpha, borderalpha },
	[SchemeStatus]    = { OPAQUE, baralpha, borderalpha }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]   = { OPAQUE, baralpha, borderalpha }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm]  = { OPAQUE, baralpha, borderalpha }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]   = { OPAQUE, baralpha, borderalpha }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm]  = { OPAQUE, baralpha, borderalpha }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

static const char *const autostart[] = {
	"st", NULL,
	"picom", NULL,
	"slstatus", NULL,
	"mpd", NULL,
	"dunst", NULL,
	"clipmenud", NULL,
//	"monitor", NULL,
//	"xrdb", ".Xresources", NULL,
//	"barrier", NULL,
	"fehbg", NULL,
	"xhidecursor", NULL,
	NULL
};

/* tagging */
static const char *tags[] = {"", "", "", "", "󰑈", "󰛍", "", "󰣠", "", ""};
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */

	/* class               instance         title            tags mask    isfloating  isterminal  noswallow   monitor   float x,y,w,h   floatborderpx */
	{"Firefox",             NULL,           NULL,             1 << 9,       False,       0,         -1,       -1},
	{"obs",                 NULL,           NULL,             1 << 7,       False,       0,         -1,       -1},
	{"Virt-manager",        NULL,           NULL,             1 << 8,       False,       0,         -1,       -1},
	{"Chromium",            NULL,           NULL,             1 << 1,       False,       0,         -1,       -1},
	{"transmission-gtk",    NULL,           NULL,             1 << 9,       False,       0,         -1,       -1},
	{"krita",               NULL,           NULL,             1 << 9,       False,       0,         -1,       -1},
	{"Gimp",                NULL,           NULL,             1 << 7,       False,       0,         -1,       -1},
	{"Audacity",            NULL,           NULL,             1 << 7,       False,       0,         -1,       -1},
	{"kdenlive",            NULL,           NULL,             1 << 7,       False,       0,         -1,       -1},
	{"st",                  NULL,       "ranger",             1 << 6,       False,       0,         -1,       -1},
	{"st",                  NULL,     "newsboat",             1 << 6,       False,       0,         -1,       -1},
	{"st",                  NULL,      "ncmpcpp",             1 << 9,       False,       0,         -1,       -1},
	{"vlc",                 NULL,           NULL,             1 << 9,       False,       0,         -1,       -1},
	{"qutebrowser",         NULL,           NULL,             1 << 3,       False,       0,         -1,       -1},
	{"Sakura",              NULL,           NULL,             1 << 2,           0,       1,          1,       -1},
	{"st",                  NULL,           NULL,                  0,           0,       1,          1,       -1},
	{"Surf",                NULL,           NULL,                  0,           1,       0,         -1,       -1,         964,38,940,1028,  1},
	{"Zathura",             NULL,           NULL,                  0,           1,       0,         -1,       -1,         202,32,646,712,   1},
	{"Alacritty",           NULL,           NULL,                  0,           1,       1,          1,       -1,         700,32,646,712,   1},
	{"feh",                 NULL,           NULL,                  0,           1,       0,         -1,       -1,         466,217,878,534, 1},
	{ NULL,                 NULL, "Event Tester",                  0,           0,       0,          1,       -1},
	//{"Qemu-system-x86_64",  NULL,           NULL,             1 << 9,           1,       0,         -1,       -1},
};

/* layout(s) */
static const float mfact        = 0.60; /* factor of master area size [0.05..0.95] */
static const int nmaster        = 1;    /* number of clients in master area */
static const int resizehints    = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1;    /* 1 will force focus on the fullscreen window */

static const Layout layouts[]   = {
	/* symbol     arrange function */
	{ " |󱃢 |",      tile },
	{ " |󰨇 |",      monocle },
	{ " |󰉧 |",      NULL },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },


#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2]             = "0";
static const char *dmenucmd[]       = { "dmenu_run", NULL };
static const char *termcmd[]        = { "st", NULL };
static const char *clipcmd[]        = { "clipmenu", NULL };
static const char *rancmd[]         = { "st", "-e", "ranger", NULL };
static const char *ncmcmd[]         = { "st", "-e", "ncmpcpp", NULL };
static const char *lockcmd[]        = { "slock", NULL };
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
static const char *boomercmd[]      = { "boomer", NULL };
static const char *screencmd[]      = { "screenshot", NULL };
static const char *reccmd[]         = { "recording", NULL };
static const char *wallcmd[]        = { "wall", NULL };
static const char *mandcmd[]        = { "mand", NULL };
static const char *mantermcmd[]     = { "manterm", NULL };
static const char *mojocmd[]        = { "mojo", NULL };
static const char *bkmarkscmd[]     = { "bookmarks_store", NULL };
static const char *bkmarkpcmd[]     = { "bookmarks_past", NULL };
static const char scratchpadname[]  = "scratchpad";
static const char *scratchpadcmd[]  = { "st", "-t", scratchpadname, "-g", "106x26", NULL };

#include "exitdwm.c"

static const Key keys[] = {
	/* modifier                      key                function               argument */
	{MODKEY,                         XK_z,            spawn,                  {.v = boomercmd}},
	{MODKEY,                         XK_b,            spawn,                  {.v = bkmarkscmd}},
	{MODKEY,                         XK_Insert,       spawn,                  {.v = bkmarkpcmd}},
	{MODKEY,                         XK_x,            spawn,                  {.v = dowcmd}},
	{MODKEY,                         XK_y,            spawn,                  {.v = ytcmd}},
	{MODKEY|ControlMask,             XK_d,            spawn,                  {.v = ytdcmd}},
	{MODKEY,                         XK_f,            spawn,                  {.v = searchcmd}},
	{MODKEY|ShiftMask,               XK_f,            spawn,                  {.v = flamcmd}},
	{MODKEY|ShiftMask,               XK_p,            spawn,                  {.v = boatcmd}},
	{MODKEY|ShiftMask,               XK_l,            spawn,                  {.v = lockcmd}},
	{MODKEY,                         XK_w,            spawn,                  {.v = chrocmd}},
	{MODKEY,                         XK_v,            spawn,                  {.v = virtcmd}},
	{MODKEY,                         XK_r,            spawn,                  {.v = rancmd}},
	{MODKEY,                         XK_e,            spawn,                  {.v = ncmcmd}},
	{MODKEY,                         XK_p,            spawn,                  {.v = dmenucmd}},
	{MODKEY|ShiftMask,               XK_Return,       spawn,                  {.v = termcmd}},
	{MODKEY,                         XK_n,            spawn,                  {.v = clipcmd}},
	{MODKEY|ShiftMask,               XK_s,            spawn,                  {.v = screencmd}},
	{MODKEY|ControlMask|ShiftMask,   XK_r,            spawn,                  {.v = reccmd}},
	{MODKEY|ShiftMask,               XK_w,            spawn,                  {.v = wallcmd}},
	{MODKEY|ShiftMask,               XK_d,            spawn,                  {.v = mandcmd}},
	{MODKEY,                         XK_d,            spawn,                  {.v = mantermcmd}},
	{MODKEY|ShiftMask,               XK_m,            spawn,                  {.v = mojocmd}},
	{MODKEY|ControlMask|ShiftMask,   XK_x,            spawn,                  {.v = (const char*[]){ "pkill", "-SIGINT", "ffmpeg", NULL }}},
	{MODKEY,                         XK_semicolon,    focusmon,               {.i = +1 }},
	{MODKEY|ShiftMask,               XK_semicolon,    tagmon,                 {.i = +1 }},
	{MODKEY|ControlMask,             XK_f,            togglefullscr,          {0}},
	{MODKEY|ShiftMask,               XK_b,            togglebar,              {0}},
	{MODKEY|ControlMask,             XK_s,            togglesticky,           {0}},
	{MODKEY,                         XK_k,            focusstack,             {.i = +1}},
	{MODKEY,                         XK_l,            focusstack,             {.i = -1}},
	{MODKEY,                         XK_i,            incnmaster,             {.i = +1}},
	{MODKEY,                         XK_o,            incnmaster,             {.i = -1}},
	{MODKEY,                         XK_j,            setmfact,               {.f = -0.03}},
	{MODKEY,                         XK_m,            setmfact,               {.f = +0.03}},
	{MODKEY,                         XK_Return,       zoom,                   {0}},
	{MODKEY,                         XK_Tab,          view,                   {0}},
	{MODKEY,                         XK_space,        setlayout,              {0}},
	{MODKEY|ShiftMask,               XK_space,        togglefloating,         {0}},
	{MODKEY,                         XK_s,            togglescratch,          {.v = scratchpadcmd}},
	{MODKEY|ShiftMask|ControlMask,   XK_z,            view,                   {.ui = ~0}},
	{MODKEY|ShiftMask,               XK_z,            tag,                    {.ui = ~0}},
	{MODKEY,                         XK_g,            setgaps,                {.i = -5 }},
	{MODKEY,                         XK_h,            setgaps,                {.i = +5 }},
	{MODKEY|ShiftMask,               XK_g,            setgaps,                {.i = GAP_RESET }},
	{MODKEY|ShiftMask,               XK_h,            setgaps,                {.i = GAP_TOGGLE}},
	{MODKEY|ShiftMask,               XK_c,            killclient,             {0}},

	{0,                              XK_F3,           spawn,                  {.v = up}},
	{0,                              XK_F2,           spawn,                  {.v = down}},
	{0,                              XK_F4,           spawn,                  {.v = mut}},
	{0,                              XK_F6,           spawn,                  {.v = toggle}},
	{0,                              XK_F8,           spawn,                  {.v = next}},
	{0,                              XK_F7,           spawn,                  {.v = prev}},

	{MODKEY|ShiftMask,               XK_q,            quit,                   {1}},
	{MODKEY|ShiftMask,               XK_e,            exitdwm,                {0}},


	{MODKEY|ControlMask,             XK_t,            setlayout,              {.v = &layouts[0]}},
	{MODKEY|ControlMask,             XK_y,            setlayout,              {.v = &layouts[1]}},
	{MODKEY|ControlMask,             XK_u,            setlayout,              {.v = &layouts[2]}},

	TAGKEYS(                         XK_ampersand,                            0)
	TAGKEYS(                         XK_eacute,                               1)
	TAGKEYS(                         XK_quotedbl,                             2)
	TAGKEYS(                         XK_apostrophe,                           3)
	TAGKEYS(                         XK_parenleft,                            4)
	TAGKEYS(                         XK_minus,                                5)
	TAGKEYS(                         XK_egrave,                               6)
	TAGKEYS(                         XK_underscore,                           7)
	TAGKEYS(                         XK_ccedilla,                             8)
	TAGKEYS(                         XK_agrave,                               9)
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

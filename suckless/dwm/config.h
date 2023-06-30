/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx    = 0;      /* border pixel of windows */
static const unsigned int snap        = 8;     /* snap pixel */
static const unsigned int gappih      = 3;      /* horiz inner gap between windows */
static const unsigned int gappiv      = 3;      /* vert inner gap between windows */
static const unsigned int gappoh      = 3;      /* horiz outer gap between windows and screen edge */
static const unsigned int gappov      = 3;      /* vert outer gap between windows and screen edge */
static const int smartgaps            = 0;      /* 1 means no outer gap when there is only one window */
static const int showbar              = 1;      /* 0 means no bar */
static const int topbar               = 1;      /* 0 means bottom bar */
static const int user_bh              = 1;      /* 2 is the default spacing around the bar's font */
static const int vertpad              = 4;      /* vertical padding of bar */
static const int sidepad              = 10;     /* horizontal padding of bar */
static const char *fonts[]            = {"terminus:style=Bold:size=14"};
static const char dmenufont[]         = "terminus:style=Bold:size=18";
static const char col_gray1[]         = "#000000";
static const char col_gray2[]         = "#444444";
static const char col_gray3[]         = "#dddfff";
static const char col_gray4[]         = "#ffffff";
static const char col_cyan[]          = "#202020";
static const unsigned int baralpha    = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]        = {
    /*                  fg         bg         border   */
    [SchemeNorm]  = {col_gray3, col_gray1,  col_gray2},
    [SchemeSel]   = {col_gray4, col_cyan,   col_cyan},
};
static const unsigned int alphas[][3] = {
	/*                 fg       bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] 	      = {"", "", "", "", "", "", "", "", ""};
static const char *alttags[] 	      = {"", "", "", "", "", "", "", "", ""};

static const unsigned int ulinepad 	= 5; /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke 	= 0; /* thickness / height of the underline */
static const unsigned int ulinevoffset 	= 0; /* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0; /* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[]               = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
   /* class                 instance    title               tagsMask  isfloating    isterminal      noswallow       monitor */
    {"obs",                   NULL,     NULL, 		     1 << 7, 	False, 		0, 	        0, 	      -1},
    {"Surf", 		      NULL,     NULL, 		     1 << 3, 	False, 		0, 	        0, 	      -1},
    {"Virt-manager", 	      NULL,     NULL, 		     1 << 8, 	False, 		0, 	       -1, 	      -1},
    {"Chromium", 	      NULL,     NULL, 		     1 << 1, 	False, 		0, 	       -1, 	      -1},
    {"Transmission-gtk",      NULL,     NULL, 		     1 << 5, 	False, 		0, 	       -1, 	      -1},
    {"Gimp", 		      NULL,     NULL, 		     1 << 3, 	False, 		0, 	       -1,  	      -1},
    {"kdenlive", 	      NULL,     NULL, 		     1 << 3, 	False, 		0, 	       -1,  	      -1},
    {"st",                    NULL,     "ranger", 	     1 << 6, 	False, 	    	0, 	       -1, 	      -1},
    {"st",                    NULL,     "ncmpcpp", 	     1 << 6, 	False, 	    	0, 	       -1, 	      -1},
    {"st",                    NULL,     "htop", 	     1 << 5, 	False, 		0, 	       -1,  	      -1},
    {"qutebrowser", 	      NULL,     NULL, 		     1 << 3, 	False, 		0, 	       -1, 	      -1},
    {"st",                    NULL,     NULL, 		        0,        0, 	        1, 	        1, 	      -1},
    {"Alacritty", 	      NULL,     NULL, 		     1 << 2,      0, 	        1, 	        1, 	      -1},
    {NULL, 	              NULL,     "Event Tester", 	0,        0, 	        0, 	        1, 	      -1}, /* xev */
};

/* layout(s) */
static const float mfact            = 0.60; /* factor of master area size [0.05..0.95] */
static const int nmaster 	    = 1;    /* number of clients in master area */
static const int resizehints 	    = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen     = 1;    /* 1 will force focus on the fullscreen window */

static const Layout layouts[]       = {
    /* symbol     arrange function */
    {"", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
};

/* volume control */
/*
#define XF86AudioLowerVolume	0x1008ff11
#define XF86AudioMute	0x1008ff12
#define XF86AudioRaiseVolume	0x1008ff13
*/

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2]	            = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]       = { "dmenu_run", NULL };
static const char *termcmd[]        = { "st", NULL };
static const char *clipcmd[]        = { "clipmenu", NULL };
static const char *hcmd[]           = { "st", "-e", "htop", "-t", NULL };
static const char *rancmd[]         = { "st", "-e", "ranger", NULL };
static const char *lockcmd[]        = { "slock", NULL };
static const char *screcmd[]        = { "scrot", "/home/sh/Images/shots/shot-%Y-%m-%d-%H-%M-%S.jpg", NULL };
static const char *virtcmd[]        = { "virt-manager", NULL };
static const char *chrocmd[]        = { "chromium", NULL };
static const char *boatcmd[]        = { "st", "-e", "newsboat", NULL };
static const char *ytcmd[]          = { "ytfzf", "-D", NULL };
static const char *ytdcmd[]         = { "ytfzf", "-d", "-D", NULL };
static const char *flamcmd[]        = { "flameshot", "gui", NULL };
static const char *up[]             = { "amixer", "set", "Master", "10%+", NULL };
static const char *mut[]            = { "amixer", "set", "Master", "toggle", NULL };
static const char *down[]           = { "amixer", "set", "Master", "10%-", NULL };
static const char scratchpadname[]  = "scratchpad";
static const char *scratchpadcmd[]  = { "st", "-t", scratchpadname, "-g", "108x30", NULL };
static const char *alcmd[]          = { "alacritty", NULL };
static const char *searchcmd[]      = { "search", NULL };
static const char *dowcmd[]         = { "dow", NULL };

static Key keys[] = {
/* modifier                                               key                          function                  argument */
    {MODKEY,                                              XK_a, 		        spawn, 		        {.v = alcmd}},
    {MODKEY,                                              XK_x, 		        spawn, 		        {.v = dowcmd}},
    {MODKEY,                                              XK_y, 		        spawn, 		        {.v = ytcmd}},
    {MODKEY, 					          XK_d, 		        spawn, 		        {.v = ytdcmd}},
    {MODKEY, 					          XK_f, 		        spawn, 		        {.v = searchcmd}},
    {MODKEY | ShiftMask, 			          XK_f, 		        spawn, 		        {.v = flamcmd}},
    {MODKEY,     		                          XK_s,  		        togglescratch,          {.v = scratchpadcmd}},
    {MODKEY | ShiftMask, 			          XK_n, 		        spawn, 		        {.v = boatcmd}},
    {MODKEY | ShiftMask, 			          XK_l, 		        spawn, 		        {.v = lockcmd}},
    {MODKEY | ShiftMask, 			          XK_s, 		        spawn, 		        {.v = screcmd}},
    {MODKEY, 					          XK_w, 		        spawn, 		        {.v = chrocmd}},
    {MODKEY, 					          XK_v, 		        spawn, 		        {.v = virtcmd}},
    {MODKEY, 					          XK_r, 		        spawn, 		        {.v = rancmd}},
    {MODKEY, 					          XK_e, 		        spawn, 		        {.v = hcmd}},
    {MODKEY, 					          XK_p, 		        spawn, 		        {.v = dmenucmd}},
    {MODKEY | ShiftMask, 			          XK_Return, 	                spawn, 		        {.v = termcmd}},
    {MODKEY, 					          XK_n, 		        spawn, 		        {.v = clipcmd}},
    {MODKEY, 					          XK_b, 		        togglebar, 	        {0}},
    {MODKEY, 					          XK_k, 		        focusstack, 	        {.i = +1}},
    {MODKEY, 					          XK_l, 		        focusstack, 	        {.i = -1}},
    {MODKEY, 					          XK_i, 		        incnmaster, 	        {.i = +1}},
    {MODKEY, 					          XK_o, 		        incnmaster, 	        {.i = -1}},
    {MODKEY, 					          XK_j, 		        setmfact, 	        {.f = -0.03}},
    {MODKEY, 					          XK_m, 		        setmfact, 	        {.f = +0.03}},
    {MODKEY | Mod4Mask, 			          XK_h, 		        incrgaps, 	        {.i = +1}},
    {MODKEY | Mod4Mask, 			          XK_l, 		        incrgaps, 	        {.i = -1}},
    {MODKEY | Mod4Mask | ShiftMask, 		          XK_h, 		        incrogaps, 	        {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, 		          XK_l, 		        incrogaps, 	        {.i = -1}},
    {MODKEY | Mod4Mask | ControlMask, 	                  XK_h, 		        incrigaps, 	        {.i = +1}},
    {MODKEY | Mod4Mask | ControlMask,		          XK_l, 		        incrigaps, 	        {.i = -1}},
    {MODKEY | Mod4Mask, 			          XK_0, 		        togglegaps, 	        {0}},
    {MODKEY | Mod4Mask | ShiftMask, 		          XK_0, 		        defaultgaps, 	        {0}},
    {MODKEY | ShiftMask | ControlMask,	                  XK_y, 		        incrihgaps, 	        {.i = +1}},
    {MODKEY | ControlMask, 		                  XK_o, 		        incrihgaps, 	        {.i = -1}},
    {MODKEY | ControlMask, 			          XK_y, 		        incrivgaps, 	        {.i = +1}},
    {MODKEY | ControlMask, 			          XK_o, 	          	incrivgaps, 	        {.i = -1}},
    {MODKEY | Mod4Mask, 			          XK_y, 		        incrohgaps, 	        {.i = +1}},
    {MODKEY | Mod4Mask, 			          XK_o, 		        incrohgaps, 	        {.i = -1}},
    {MODKEY | ControlMask | ShiftMask,                    XK_y, 		        incrovgaps, 	        {.i = +1}},
    {MODKEY | ShiftMask | ControlMask, 		          XK_o, 		        incrovgaps, 	        {.i = -1}},
    {MODKEY, 					          XK_Return, 	      		zoom, 		        {0}},
    {MODKEY, 					          XK_Tab, 	        	view, 		        {0}},
    {MODKEY | ShiftMask, 			          XK_c, 		        killclient, 	        {0}},
    {MODKEY | ShiftMask, 			          XK_t, 		        setlayout, 	        {.v = &layouts[0]}},
    {MODKEY | ShiftMask, 			          XK_y, 		        setlayout, 	        {.v = &layouts[1]}},
    {MODKEY | ShiftMask, 			          XK_u, 		        setlayout, 	        {.v = &layouts[2]}},
    {MODKEY, 					          XK_space, 	      		setlayout, 	        {0}},
    {MODKEY | ShiftMask, 			          XK_space,         		togglefloating,         {0}},
    {MODKEY | ShiftMask, 			          XK_x, 		        view, 		        {.ui = ~0}},
    {MODKEY | ShiftMask, 			          XK_z, 		        tag, 		        {.ui = ~0}},
    {MODKEY, 					          XK_comma, 	      		focusmon, 	        {.i = -1}},
    {MODKEY, 					          XK_semicolon,     		focusmon, 	        {.i = +1}},
    {MODKEY | ShiftMask, 			          XK_comma, 	      		tagmon, 	        {.i = -1}},
    {MODKEY | ShiftMask, 			          XK_semicolon, 	  	tagmon, 	        {.i = +1}},
    {0, 					          XK_F3, 		        spawn, 		        {.v = up}},
    {0, 					          XK_F2, 		        spawn, 		        {.v = down}},
    {0, 					          XK_F4, 		        spawn, 		        {.v = mut}},
    {MODKEY | ShiftMask, 			          XK_q,             		quit, 		        {0}},
  	TAGKEYS( 			                  XK_ampersand, 		        0)
	TAGKEYS( 			                  XK_eacute, 			        1)
	TAGKEYS( 			                  XK_quotedbl, 			        2)
  	TAGKEYS( 			                  XK_apostrophe, 		        3)
	TAGKEYS( 			                  XK_parenleft, 		        4)
	TAGKEYS( 			                  XK_minus, 		            	5)
  	TAGKEYS( 			                  XK_egrave, 		            	6)
	TAGKEYS( 			                  XK_underscore, 		        7)
  	TAGKEYS( 			                  XK_ccedilla, 			        8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function       argument */
    {ClkLtSymbol,               0,          Button1,        setlayout,        {0}},
    {ClkLtSymbol,               0,          Button3,        setlayout,        {.v = &layouts[2]}},
    {ClkStatusText,             0,          Button2,        spawn,            {.v = termcmd}},
    {ClkClientWin,              MODKEY,     Button1,        movemouse,        {0}},
    {ClkClientWin,              MODKEY,     Button2,        togglefloating,   {0}},
    {ClkClientWin,              MODKEY,     Button3,        resizemouse,      {0}},
    {ClkTagBar,                 0,          Button1,        view,             {0}},
    {ClkTagBar,                 0,          Button3,        toggleview,       {0}},
    {ClkTagBar,                 MODKEY,     Button1,        tag,              {0}},
    {ClkTagBar,                 MODKEY,     Button3,        toggletag,        {0}},
};

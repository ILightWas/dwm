/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char statussep         = ';';      /* separator between status bars */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_red[]         = "#991111";
static const char col_purple[]      = "#46357C"; //gentoo forum purple
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_purple },
	[SchemeSel]  = { col_gray4, col_purple,  col_red  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, OPAQUE, borderalpha },
	[SchemeSel]  = { OPAQUE, OPAQUE, borderalpha },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;

const char *spcmd1[] = {"st", "-n", "spterm", "-g", "144x41", NULL };
const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "ranger", NULL };
const char *spcmd3[] = {"st", "-n", "pulsemixer", "-e", "pulsemixer", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
	{"pulsemixer",  spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    	title       tags mask     iscentered   isfloating   monitor */
	{ "Gimp",     NULL,       	NULL,       0,            	0,           1,          	-1 },
	{ "Firefox",  NULL,       	NULL,       1 << 8,       	0,           0,          	-1 },
	{ "discord",  NULL,      	NULL,       1 << 2,       	0,           0,				-1 },
	{ "Spotify",  NULL,  		NULL,      	1 << 3,       	0,           0,				-1 },
	{ "Steam",    NULL,       	NULL,       1 << 4,       	0,           0,				-1 },
	{ NULL,		  "spterm",		NULL,	  	SPTAG(0), 		1,			 1,			 	-1 },
	{ NULL,		  "spfm",		NULL,	  	SPTAG(1),		1,			 1,			 	-1 },
	{ NULL,		  "pulsemixer",	NULL,	  	SPTAG(2),		1,			 1,			 	-1 }
};

/* Bar rules allow you to configure what is shown where on the bar, as well as
 * introducing your own bar modules.
 *
 *    monitor:
 *      -1  show on all monitors
 *       0  show on monitor 0
 *      'A' show on active monitor (i.e. focused / selected) (or just -1 for active?)
 *    bar - bar index, 0 is default, 1 is extrabar
 *    alignment - how the module is aligned compared to other modules
 *    widthfunc, drawfunc, clickfunc - providing bar module width, draw and click functions
 *    name - does nothing, intended for visual clue and for logging / debugging
 */
static const BarRule barrules[] = {
	/* monitor  bar    alignment         widthfunc              drawfunc              clickfunc           name */
	{ -1,       0,     BAR_ALIGN_LEFT,   width_tags,            draw_tags,            click_tags,         "tags" },
	{ -1,       0,     BAR_ALIGN_LEFT,   width_ltsymbol,        draw_ltsymbol,        click_ltsymbol,     "layout" },
	{ 'A',      0,     BAR_ALIGN_RIGHT,  width_status2d,        draw_status2d,        click_statuscmd,    "status2d" },
	{ -1,       0,     BAR_ALIGN_NONE,   width_awesomebar,      draw_awesomebar,      click_awesomebar,   "awesomebar" },
	//{  0,       1,     BAR_ALIGN_CENTER, width_status2d_es,     draw_status2d_es,     click_statuscmd_es, "status2d_es" },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "patch/vanitygaps.h"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */ 
	{ "[M]",      monocle }, 				// 1
	{ "[@]",      spiral },  				// 2
	{ "[\\]",     dwindle }, 				// 3
	{ "D[]",      deck },    				// 4
	{ "TTT",      bstack },  				// 5
	{ "===",      bstackhoriz },			// 6
	{ "HHH",      grid },					// 7 -
	{ "###",      nrowgrid },				// 8 -
	{ "---",      horizgrid },				// 9 -
	{ ":::",      gaplessgrid },			// 10 -
	{ "|M|",      centeredmaster },			// 11 -
	{ ">M>",      centeredfloatingmaster }, // 12 -
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	{ MOD, XK_x,     ACTION##stack, {.i = -1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_purple, "-sf", col_gray3, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };

#include <X11/XF86keysym.h>
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY|ControlMask,           XK_l,  	   shiftview,      { .i = +1 } },
	{ MODKEY|ControlMask,           XK_h,  	   shiftview,      { .i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },

	// gaps
	{ MODKEY,                       XK_h,		    setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,		    setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,		    setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,		    setcfact,       {.f = -0.25} },
									// keypad del
	{ MODKEY|ShiftMask,             XK_KP_Delete,	setcfact,       {.f =  0.00} },
								    // keypad 1
	{ MODKEY,              			XK_KP_End, 		incrgaps,       {.i = +1 } }, // keypad 1
	{ MODKEY|ShiftMask,			    XK_KP_End, 		incrgaps,       {.i = -1 } },
									// keypad 2
	{ MODKEY,		            	XK_KP_Down,		incrigaps,      {.i = +1 } },
	{ MODKEY|ShiftMask,			    XK_KP_Down,		incrigaps,      {.i = -1 } },
									// keypad 3
	{ MODKEY,              			XK_KP_Next,     incrogaps,      {.i = +1 } },
	{ MODKEY|ShiftMask,    			XK_KP_Next,     incrogaps,      {.i = -1 } },
									// keypad 4	
	{ MODKEY,		            	XK_KP_Left,     incrihgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,			    XK_KP_Left,     incrihgaps,     {.i = -1 } },
									// keypad 5
	{ MODKEY,		             	XK_KP_Begin,    incrivgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,			    XK_KP_Begin,    incrivgaps,     {.i = -1 } },
									// keypad 6
	{ MODKEY,			            XK_KP_Right,    incrohgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,    			XK_KP_Right,    incrohgaps,     {.i = -1 } },
									// keypad 7
	{ MODKEY,			            XK_KP_Home,     incrovgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,  			XK_KP_Home,     incrovgaps,     {.i = -1 } },
									// keypad 0
	{ MODKEY,              			XK_KP_Insert,	togglegaps,     {0} },
	{ MODKEY|ShiftMask,    			XK_KP_Insert,	defaultgaps,    {0} },

	// layouts
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask,           XK_t,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_g,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ControlMask,           XK_g,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,             XK_i,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY|ControlMask,           XK_m,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY,			            XK_o,      setlayout,      {.v = &layouts[8]} },
	{ MODKEY|ShiftMask,	            XK_o,      setlayout,      {.v = &layouts[9]} },
	{ MODKEY|ControlMask,	        XK_o,      setlayout,      {.v = &layouts[10]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[11]} },
	{ MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[12]} },



	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },

	// fullscreen
	{ MODKEY,                       XK_f,      togglefullscreen, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefakefullscreen, {0} },
	
	// monitors
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	// Scratchpads
	{ MODKEY,            			XK_n,  	   togglescratch,  {.ui = 0 } },
	{ MODKEY,            			XK_e,	   togglescratch,  {.ui = 1 } },
	{ MODKEY,            			XK_v,	   togglescratch,  {.ui = 2 } },
	
	// tags
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	{ MODKEY,                       XK_Print,      			 spawn,   SHCMD("printscreen.sh") },
	{ MODKEY,             			XK_F5, 					 spawn,   SHCMD("firefox") 		  },


	#define SPOTIFY_CTL "playerctl -p spotify"

	{ 0,                            XF86XK_AudioPlay, 		 spawn,   SHCMD(SPOTIFY_CTL" play-pause") },
	{ 0,                            XF86XK_AudioNext, 		 spawn,   SHCMD(SPOTIFY_CTL" next") },
	{ 0,                            XF86XK_AudioPrev, 		 spawn,   SHCMD(SPOTIFY_CTL" previous") },
	{ 0,                            XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },
	{ 0,                            XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
	{ 0,                            XF86XK_AudioMute, 		 spawn, {.v = mutevol } },

	{ MODKEY|ShiftMask,             XK_F12, quit,        {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[13]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1 } },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2 } },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3 } },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4 } },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5 } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


// See LICENSE file for copyright and license details.

// XRES VARS
//   normbgcolor
//   normbordercolor
//   normfgcolor
//   selbgcolor
//   selbordercolor
//   selfgcolor
//   borderpx
//   snap
//   gappih
//   gappiv
//   gappoh
//   gappov
//   showbar
//   topbar
//   mfact
//   nmaster
//   resizehints

// appearance
static unsigned int borderpx = 1;        // border pixel of windows
static unsigned int snap     = 32;       // snap pixel
static unsigned int gappih   = 0;       /* horiz inner gap between windows */
static unsigned int gappiv   = 10;       /* vert inner gap between windows */
static unsigned int gappoh   = 25;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov   = 25;       /* vert outer gap between windows and screen edge */
static int showbar           = 0;        // 0 means no bar
static int topbar            = 1;        // 0 means bottom bar

// layouts
static float mfact       = 0.60; // factor of master area size [0.05..0.95]
static int   nmaster     = 1;    // number of clients in master area
static int   resizehints = 1;    // 1 means respect size hints in tiled resizals

static const int smartgaps       = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating = 0;        /* 1 means swallow floating windows by default */
static const char *fonts[]       = { "monospace:size=10" };
static const char dmenufont[]    = "Iosevka 15";

static char normbgcolor[]     = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[]     = "#bbbbbb";
static char selfgcolor[]      = "#eeeeee";
static char selbordercolor[]  = "#005577";
static char selbgcolor[]      = "#005577";
static char *colors[][3]      = {
	/*               fg           bg           border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

static const char *const autostart[] = {
	"dwmautos", NULL,
	NULL // terminate
};

// TAGS
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
//	class      instance    title       tags mask     isfloating   isfakefullscreen monitor
//	{ "Gimp",     NULL,       NULL,       0,            1,           0,               -1 },
//	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           1,               -1 },
// class                  inst  title           tags mask  isfloat isterm noswall isfakefull mon
	{ "Google-chrome-beta", NULL,         NULL,   1,         0,       0,    -1,     1,         -1 },
	{ "Gimp",               NULL,         NULL,   0,         1,       0,     0,     0,         -1 },
	{ "Firefox",            NULL,         NULL,   1,         0,       0,    -1,     1,         -1 },
	{ "st",                 "visualiser", NULL,   2,         0,       1,    -1,     0,         -1 },
	{ "Spotify",            NULL,         NULL,   2,         0,       1,    -1,     0,         -1 },
	{ NULL,                 NULL, "Event Tester", 0,         1,       0,     1,     0,         -1 }, /* xev */
};

static const Layout layouts[] = {
	// symbol     arrange function
	{ "[]=",      tile },    // first entry is default
	{ "|M|",      centeredmaster },
	{ "[M]",      monocle },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    // no layout function means floating behavior
};

// key definitions
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,           {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

// helper for spawning shell commands in the pre dwm-5.0 fashion
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

// commands
static char dmenumon[2] = "0"; // component of dmenucmd, manipulated in spawn()
static const char *dmenucmd[] = { "dmrap",
	"-a lightgrn ", "-l 10 -L mc",
	NULL
	};
static const char *termcmd[]  = { "st", NULL };

#include "movestack.c"
static Key keys[] = {
	// TAGBINDS  key  tag
	TAGKEYS(    XK_1, 0)
	TAGKEYS(    XK_2, 1)
	TAGKEYS(    XK_3, 2)
	TAGKEYS(    XK_4, 3)
	TAGKEYS(    XK_5, 4)
	TAGKEYS(    XK_6, 5)
	TAGKEYS(    XK_7, 6)
	TAGKEYS(    XK_8, 7)
	TAGKEYS(    XK_9, 8)
	// VANITY GAPS CTRL
	{ MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },

	// modifier         key        function        argument
	{ MODKEY,           XK_0,      view,           {.ui = ~0 } },
	{ MODKEY,           XK_Tab,    comboview,           {0} },
	{ MODKEY,           XK_q,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,           XK_w,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,           XK_e,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,           XK_r,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,           XK_y,      incnmaster,     {.i = +1 } },
	{ MODKEY,           XK_u,      incnmaster,     {.i = -1 } },
	{ MODKEY,           XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,           XK_Return, zoom,           {0} },
	{ MODKEY,           XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,           XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,           XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,           XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,           XK_x,      transfer,       {0} },
	{ MODKEY,           XK_b,      togglebar,      {0} },
//	{ MODKEY,           XK_f,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,           XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,           XK_period, focusmon,       {.i = +1 } },
	{ MODKEY,           XK_space,  setlayout,      {0} },

	{ MODKEY|ShiftMask, XK_0,      combotag,            {.ui = ~0 } },
	{ MODKEY|ShiftMask, XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask, XK_f,      togglefullscr,  {0} },
	{ MODKEY|ShiftMask, XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask, XK_k,      movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask, XK_c,      killclient,     {0} },
	{ MODKEY|ShiftMask, XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask, XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask, XK_space,  togglefloating, {0} },

	{ MODKEY|ControlMask, XK_q,     quit,           {0} },

	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
};

// button definitions
// click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin
static Button buttons[] = {
	// click                event mask      button          function        argument
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        comboview,      {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        combotag,       {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

void
setlayoutex(const Arg *arg)
{
	setlayout(&((Arg) { .v = &layouts[arg->i] }));
}

void
viewex(const Arg *arg)
{
	view(&((Arg) { .ui = 1 << arg->ui }));
}

void
viewall(const Arg *arg)
{
	view(&((Arg){.ui = ~0}));
}

void
toggleviewex(const Arg *arg)
{
	toggleview(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagex(const Arg *arg)
{
	tag(&((Arg) { .ui = 1 << arg->ui }));
}

void
toggletagex(const Arg *arg)
{
	toggletag(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagall(const Arg *arg)
{
	tag(&((Arg){.ui = ~0}));
}

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"` */
static Signal signals[] = {
	/* signum           function */
	{ "focusstack",     focusstack },
	{ "setmfact",       setmfact },
	{ "togglebar",      togglebar },
	{ "incnmaster",     incnmaster },
	{ "togglefloating", togglefloating },
	{ "focusmon",       focusmon },
	{ "tagmon",         tagmon },
	{ "zoom",           zoom },
	{ "view",           view },
	{ "viewall",        viewall },
	{ "viewex",         viewex },
	{ "toggleview",     view },
	{ "toggleviewex",   toggleviewex },
	{ "tag",            tag },
	{ "tagall",         tagall },
	{ "tagex",          tagex },
	{ "toggletag",      tag },
	{ "toggletagex",    toggletagex },
	{ "killclient",     killclient },
	{ "quit",           quit },
	{ "setlayout",      setlayout },
	{ "setlayoutex",    setlayoutex },
};

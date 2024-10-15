/*
 Title:		global.h
 Author:	Badre
Created:	23/06/2024
*/

#ifndef _GLOBAL_H
#define _GLOBAL_H


//#define RUNCPM


// Compatible with Unix Curses
/*Horizontal Line: ESC(0 followed by q for a horizontal line.
Vertical Line: ESC(0 followed by x for a vertical line.
Top Left Corner: ESC(0 followed by l.
Top Right Corner: ESC(0 followed by k.
Bottom Left Corner: ESC(0 followed by m.
Bottom Right Corner: ESC(0 followed by j.
*/

//#ifdef RUNCPM
/*
#define ACS_ULCORNER (0x2B)        // upper left corner 
#define ACS_LLCORNER (0x2B)        // lower left corner 
#define ACS_URCORNER (0x2B)        // upper right corner 
#define ACS_LRCORNER (0x2B)        // lower right corner 
#define ACS_HLINE (0x2D)           // horizontal line 
#define ACS_VLINE (0xFC)           // (0x3A) vertical line 
#else*/
#define ACS_ULCORNER (0xDA)        /* upper left corner */
#define ACS_LLCORNER (0xC0)        /* lower left corner */
#define ACS_URCORNER (0xBF)        /* upper right corner */
#define ACS_LRCORNER (0xD9)        /* lower right corner */
#define ACS_HLINE (0xC4)           /* horizontal line */
#define ACS_VLINE (0xB3)           /* vertical line */
//#endif

#define ACS_LTEE (acs_map['t'])    /* tee pointing right */
#define ACS_RTEE (acs_map['u'])    /* tee pointing left */
#define ACS_BTEE (acs_map['v'])    /* tee pointing up */
#define ACS_TTEE (acs_map['w'])    /* tee pointing down */
#define ACS_PLUS (acs_map['n'])    /* large plus or crossover */
#define ACS_S1 (acs_map['o'])      /* scan line 1 */
#define ACS_S9 (acs_map['s'])      /* scan line 9 */
#define ACS_DIAMOND (acs_map['`']) /* diamond */
#define ACS_CKBOARD (acs_map['a']) /* checker board (stipple) */
#define ACS_DEGREE (acs_map['f'])  /* degree symbol */
#define ACS_PLMINUS (acs_map['g']) /* plus/minus */
#define ACS_BULLET (acs_map['~'])  /* bullet */
/* Teletype 5410v1 symbols begin here */
#define ACS_LARROW (acs_map[','])  /* arrow pointing left */
#define ACS_RARROW (acs_map['+'])  /* arrow pointing right */
#define ACS_DARROW (acs_map['.'])  /* arrow pointing down */
#define ACS_UARROW (acs_map['-'])  /* arrow pointing up */
#define ACS_BOARD (acs_map['h'])   /* board of squares */
#define ACS_LANTERN (acs_map['i']) /* lantern symbol */
#define ACS_BLOCK (acs_map['0'])   /* solid square block */

typedef enum color {
  Black = 0,          /**< Equivalent to RGB888(0,0,0) */
  Red,            /**< Equivalent to RGB888(128,0,0) */
  Green,          /**< Equivalent to RGB888(0,128,0) */
  Yellow,         /**< Equivalent to RGB888(128,128,0) */
  Blue,           /**< Equivalent to RGB888(0,0,128) */
  Magenta,        /**< Equivalent to RGB888(128,0,128) */
  Cyan,           /**< Equivalent to RGB888(0,128,128) */
  White,          /**< Equivalent to RGB888(128,128,128) */  
  BrightBlack,    /**< Equivalent to RGB888(64,64,64) */
  BrightRed,      /**< Equivalent to RGB888(255,0,0) */
  BrightGreen,    /**< Equivalent to RGB888(0,255,0) */
  BrightYellow,   /**< Equivalent to RGB888(255,255,0) */
  BrightBlue,     /**< Equivalent to RGB888(0,0,255) */
  BrightMagenta,  /**< Equivalent to RGB888(255,0,255) */
  BrightCyan,     /**< Equivalent to RGB888(0,255,255) */
  BrightWhite,    /**< Equivalent to RGB888(255,255,255) */
  Lightgray,       /**< Equivalent to RGB888(211,211,211) */
  Orange,         /**< Equivalent to RGB888(255,165,0) */
  DarkYellow,       /**< Equivalent to RGB888(204,204,0) */
  DarkBlue,        /**< Equivalent to RGB888(0,0,139) */
  DarkRed,         /**< Equivalent to RGB888(139,0,0) */
  DarkGrey         /**< Equivalent to RGB888(169,169,169) */
} Color;

typedef enum {
  TAB_KEY = 9,
  ENTER_KEY = 13,
  ESC_KEY = 27,
  BACKSPACE = 127,
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  INS_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN,
  CTRL_UP,
  CTRL_DOWN,
  CTRL_LEFT,
  CTRL_RIGHT,
  CTRL_C_KEY,
  F1_KEY,
  F2_KEY,
  F3_KEY,
  F4_KEY,
  F5_KEY,
  F6_KEY,
  F7_KEY,
  F8_KEY,
  F9_KEY,
  F10_KEY,
  F11_KEY,
  F12_KEY  
} editorKey;

/*struct Curs {
   const int RIGHT = 0;
   const int LEFT = 1;
   const int NONE = 2;
};*/

typedef enum {
                RIGHT = 0,
                LEFT = 1
              } Curs;

typedef enum {  BLINK_BLOCK = 0,
                STEADY_BLOCK = 2,
                BLINK_UNDERLINE,
                STEADY_UNDERLINE,
                BLINK_BAR,
                STEADY_BAR
              } CursorType;
					  
#endif

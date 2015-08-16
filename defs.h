#ifndef __defs_h__
#define __defs_h__

//==========================================
//  defs.h
//
//  this file contains constants used in
//  various parts of the program
//==========================================


//===========================
//  PROGRAM DETAILS
//===========================

#define PROGRAM_NAME_STRING                     "smallsprite"
#define PROGRAM_VERSION_STRING                  "v0.3"

//===========================
//  PROGRAM CONSTANTS
//===========================

// size of sprites in pixels
#define SPRITE_W                                16
#define SPRITE_H                                16
#define SPRITE_SIZE                             SPRITE_W*SPRITE_H

#define FRAME_TIME                              16      // desired length of one frame in ms

//===========================
//  MAIN.C CONSTANTS
//===========================

// dimensions of window
#define WINDOW_WIDTH                            832
#define WINDOW_HEIGHT                           800

//===========================
//  SCREEN LAYOUT DETAILS
//===========================

// sprite size in display windows and animation player
#define GUI_SPRITE_W                            64
#define GUI_SPRITE_H                            64

#define GUI_MIN_SEPARATION                      16
#define GUI_WINDOW_BORDER                       32

// sprite editor window
#define GUI_AREA_SPRITE_EDIT_LABEL              "SPRITE EDITOR"
#define GUI_AREA_SPRITE_EDIT_X                  GUI_WINDOW_BORDER
#define GUI_AREA_SPRITE_EDIT_Y                  GUI_WINDOW_BORDER
#define GUI_AREA_SPRITE_EDIT_W                  256
#define GUI_AREA_SPRITE_EDIT_H                  256
#define GUI_AREA_SPRITE_EDIT_PIXEL_W            16
#define GUI_AREA_SPRITE_EDIT_PIXEL_H            16

// sprite display window
#define GUI_AREA_SPRITE_GRID_LABEL              "SPRITE GRID"
#define GUI_AREA_SPRITE_GRID_ROWS               4           // sprite display shows a grid of sprites
#define GUI_AREA_SPRITE_GRID_COLUMNS            11
#define GUI_AREA_SPRITE_NUMBER                  GUI_AREA_SPRITE_GRID_ROWS*GUI_AREA_SPRITE_GRID_COLUMNS

#define GUI_AREA_SPRITE_GRID_X                  GUI_WINDOW_BORDER
#define GUI_AREA_SPRITE_GRID_Y                  320
#define GUI_AREA_SPRITE_GRID_W                  GUI_AREA_SPRITE_GRID_COLUMNS*GUI_SPRITE_W
#define GUI_AREA_SPRITE_GRID_H                  GUI_AREA_SPRITE_GRID_ROWS*GUI_SPRITE_H

// sprite display scroller (when more than sprite_rows*sprite_columns sprites exist)
#define GUI_AREA_SPRITE_GRID_SCROLL_LABEL       "SPRITE GRID SCROLL"
#define GUI_AREA_SPRITE_GRID_SCROLL_X           768
#define GUI_AREA_SPRITE_GRID_SCROLL_Y           GUI_AREA_SPRITE_GRID_Y
#define GUI_AREA_SPRITE_GRID_SCROLL_W           32
#define GUI_AREA_SPRITE_GRID_SCROLL_H           GUI_AREA_SPRITE_GRID_H

// main palette selection
#define GUI_AREA_MAIN_PALETTE_LABEL             "MAIN PALETTE"
#define GUI_AREA_MAIN_PALETTE_X                 304
#define GUI_AREA_MAIN_PALETTE_Y                 GUI_WINDOW_BORDER
#define GUI_AREA_MAIN_PALETTE_W                 496
#define GUI_AREA_MAIN_PALETTE_H                 64

// details for displaying individual colours in the main palette
#define GUI_AREA_MAIN_PALETTE_ROWS              4
#define GUI_AREA_MAIN_PALETTE_COLUMNS           16      // 64 colours in total
#define GUI_AREA_MAIN_PALETTE_COLOR_W           GUI_AREA_MAIN_PALETTE_W/GUI_AREA_MAIN_PALETTE_COLUMNS
#define GUI_AREA_MAIN_PALETTE_COLOR_H           GUI_AREA_MAIN_PALETTE_H/GUI_AREA_MAIN_PALETTE_ROWS

// user selected colours (mini palette)
#define GUI_AREA_USER_PALETTE_LABEL             "USER PALETTE"
#define GUI_AREA_USER_PALETTE_X                 304
#define GUI_AREA_USER_PALETTE_Y                 112
#define GUI_AREA_USER_PALETTE_W                 496
#define GUI_AREA_USER_PALETTE_H                 176

#define GUI_AREA_USER_PALETTE_NUMBER            16
#define GUI_AREA_USER_PALETTE_COLOR_W           16
#define GUI_AREA_USER_PALETTE_COLOR_H           16

// animation editor frame display
#define GUI_AREA_ANIM_EDIT_LABEL                "ANIMATION EDITOR"
#define GUI_AREA_ANIM_EDIT_X                    GUI_WINDOW_BORDER
#define GUI_AREA_ANIM_EDIT_Y                    650
#define GUI_AREA_ANIM_EDIT_W                    GUI_SPRITE_W*10
#define GUI_AREA_ANIM_EDIT_H                    GUI_SPRITE_H

// animation editor controls
#define GUI_AREA_ANIM_CONTROL_LABEL             "ANIMATION CONTROLS"
#define GUI_AREA_ANIM_CONTROL_X                 GUI_WINDOW_BORDER
#define GUI_AREA_ANIM_CONTROL_Y                 724
#define GUI_AREA_ANIM_CONTROL_W                 768
#define GUI_AREA_ANIM_CONTROL_H                 64

#define GUI_AREA_ANIM_FRAMES                    10

// animation previewer
#define GUI_AREA_ANIM_PLAYER_LABEL              "ANIMATION PLAYER"
#define GUI_AREA_ANIM_PLAYER_X                  736
#define GUI_AREA_ANIM_PLAYER_Y                  650
#define GUI_AREA_ANIM_PLAYER_W                  GUI_SPRITE_W
#define GUI_AREA_ANIM_PLAYER_H                  GUI_SPRITE_H

//=====================================
//  PALETTE CONSTANTS
//=====================================

#define PAL_MAIN_SIZE               64
#define PAL_USER_SIZE               16
#define PAL_MAX_USER_PALETTES       1024



#endif // __defs_h__

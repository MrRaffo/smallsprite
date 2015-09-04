//====================================================================
//
//  sprite.h
//
//  used by the smallsprite program to store sprite definitions
//  
//  Paul Rafferty 2015
//
//====================================================================




#ifndef __sprite_h__
#define __sprite_h__

#include "defs.h"

//====================================================================
//  CONSTANTS
//====================================================================


#define MAX_SPRITES             1024


//====================================================================
//  TYPES
//====================================================================

struct sprite_s {   uint8_t         definition[SPRITE_SIZE];            // sprite shape and colour info
                    uint32_t        palette;                            // index of the palette used by the sprite
                };

typedef struct sprite_s sprite_type;


//====================================================================
//  PROTOTYPES
//====================================================================

// initialize copy buffers
void SPR_Init();


// add a sprite to the list
void SPR_Add_Sprite();

// remove last sprite
void SPR_Remove_Sprite();


// set all of a sprites pixels to 0 (transparent)
void SPR_Clear_Sprite( int index );

 
// copy given sprite to the buffer, return 1 on success
int SPR_Copy_Sprite( int index );


// copy sprite buffer to current sprite
int SPR_Paste_Sprite( int index );


// Set a particular pixel in the sprite definition
void SPR_Set_Pixel( int sprite_index, int pixel_index, uint8_t pixel_value );


// retrieve the color index of a given sprite pixel, returns 0 (transparency) on failure
uint8_t SPR_Get_Pixel( int sprite_index, int pixel_index );


// return the current number of sprites
int SPR_Get_Number_Of_Sprites();

// returns the current color index of the sprite
int SPR_Get_Sprite_Palette_Index( int sprite_index );


// set the sprites palette
void SPR_Set_Sprite_Palette_Index( int sprite_index, int palette_index );


// free allocated sprite memory
void SPR_Free();

//==========================
//  SPRITE SHIFT/FLIP
//==========================

void SPR_Shift_Left( int index );

void SPR_Shift_Right( int index );

void SPR_Shift_Up( int index );

void SPR_Shift_Down( int index );

void SPR_Flip_Horizontal( int index );

void SPR_Flip_Vertical( int index );


//=============================
//  FILE I/O
//=============================

// return pointer to the sprite definition
uint8_t         *SPR_Get_Sprite( int index );

// load from file
int             SPR_Load_Sprite( sprite_type *definition );

//===================================
//  TESTING AND DEBUG
//===================================

// test sprite is made
void SPR_DEBUG_Show_Sprite( int index );

#endif // __sprite_h__

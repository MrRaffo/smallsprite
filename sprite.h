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

// add a sprite to the list
void SPR_Add_Sprite();


// Set a particular pixel in the sprite definition
void SPR_Set_Pixel( int sprite_index, int pixel_index, uint8_t pixel_value );


// retrieve the color index of a given sprite pixel, returns 0 (transparency) on failure
uint8_t SPR_Get_Pixel( int sprite_index, int pixel_index );


// free allocated sprite memory
void SPR_Free();


//===================================
//  TESTING AND DEBUG
//===================================

// test sprite is made
void SPR_DEBUG_Show_Sprite( int index );

#endif // __sprite_h__

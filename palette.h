#ifndef __palette_h__
#define __palette_h__

//==============================================
//  palette.h
//
//  this controls all aspects of creating and
//  managing the palettes used by the sprite
//  editor
//
//  Paul Rafferty 2015
//==============================================

//========================================================================
//  INCLUDES
//========================================================================

#include "defs.h"


//========================================================================
//  PRIVATE CONSTANTS
//========================================================================





//========================================================================
//  STRUCTS AND TYPEDEFS
//========================================================================

// structure used for holding each user-made mini palette
struct user_palette_s   {   uint8_t palette[PAL_USER_SIZE];  };
typedef struct user_palette_s       user_palette_type;

//========================================================================
//  FUNCTION PROTOTYPES
//========================================================================

// initialize palette data, sets all pointers to NULL
void            PAL_Init();

// create a 64 colour RGBA palette
void            PAL_Generate_Main_Palette();

// returns requested RGBA value of the palette
uint32_t        PAL_Get_Main_Palette_Color( int index );

// returns RGBA value of user palette color
uint32_t        PAL_Get_User_Palette_Color( int pal_index, int col_index );

// returns main palette index of user palette color, -1 on fail
int             PAL_Get_User_Palette_Index( int pal_index, int col_index );

// sets the color of the user palette to the desired index of the main palette
void            PAL_Set_User_Palette_Index( int pal_index, int col_index, int new_val );

// add another user palette, returns index of new palette on success, -1 on failure
int             PAL_Add_User_Palette();


// set current_palette to the next palette on the list, create one if necessary, returns -1 on fail, current
// palette value on success
int             PAL_Next_Palette();

// set current palette to the previous palette in the list, returns current_palette on success, -1 on fail
int             PAL_Prev_Palette();

// set the current_palette to the value passed, returns current_palette on success, -1 on fail
int             PAL_Set_Palette( int index );

// clean up mallocd memory
void            PAL_Free();

#endif  // __palette_h__

//====================================
//  palette.c
//
//  contains all code related to the
//  palette and user palettes
//
//  Paul Rafferty 2015
//====================================

#include <stdio.h>
#include <stdint.h>

#include "defs.h"
#include "palette.h"

#include "utility.h"
#include "graphics.h"



//========================================================================
//  FILE WIDE VARIABLES
//========================================================================

static uint32_t                 main_palette[PAL_MAIN_SIZE];
static user_palette_type        *user_palette[PAL_MAX_USER_PALETTES];

static int                      no_of_palettes = 0;
static int                      current_palette = 0;

//========================================================================
//  PRIVATE FUNCTIONS
//========================================================================

// creates space for a new user palette and initializes all values to 0 (transparency)
// returns 1 on success
static int Create_User_Palette( int palette_index )
{
    // check pointer has been initialized and is not being used
    if( palette_index < 0 || palette_index > PAL_MAX_USER_PALETTES )
    {
        return 0;
    }

    if( user_palette[palette_index] != NULL )
    {
        UTI_Print_Debug( "Pointer is not null!" );
        return 0;
    }

    printf( "Requesting %d bytes for palette...\n", SPRITE_SIZE );

    // get space for the palette
    user_palette[palette_index] = UTI_EC_Malloc( SPRITE_SIZE );

    // initialize palette values
    int i;
    for( i = 0; i < PAL_USER_SIZE; i++ )
    {
        //TODO change this to 0 instead of i
        user_palette[palette_index]->palette[i] = 0;
    }
    
    return 1;
}

//========================================================================
//  PUBLIC FUNCTION BODIES
//========================================================================

// initialize all pointers to zero
void PAL_Init()
{
    int i = 0;
    for( i = 0; i < PAL_MAX_USER_PALETTES; i++ )
    {
        user_palette[i] = NULL;
    }

    return;
}

// creates a 64 colour RGBA palette
void PAL_Generate_Main_Palette()
{
    int increment = 75;
    int r, g, b, a = 0xff;
    for( r = 0; r < 4; r++ )
    {
        for( g = 0; g < 4; g++ )
        {
            for( b = 0; b < 4; b++ )
            {
                main_palette[r*16+g*4+b] = GRA_Create_Color( r*increment, g*increment, 
                                                             b*increment, a );
            }
        }
    }

    return;
}

// returns the requested RGBA value of the main palette, or 0 if an unreasonable index is given
uint32_t PAL_Get_Main_Palette_Color( int index )
{
    if( index < 0 || index > PAL_MAIN_SIZE )
    {
        return 0x00000000;
    }
    
    return main_palette[index];

}


// returns RGBA value of user palette color, returns 0 (transparency) on fail
uint32_t PAL_Get_User_Palette_Color( int pal_index, int col_index )
{
    if( pal_index < no_of_palettes && pal_index >= 0 )
    {
        if( col_index < PAL_USER_SIZE && col_index >= 0 )
        {
            return PAL_Get_Main_Palette_Color( user_palette[pal_index]->palette[col_index] );
        }
    }

    return 0x00000000;
}

// returns main palette index of user palette color, returns -1 on fail
int PAL_Get_User_Palette_Index( int pal_index, int col_index )
{
    if( pal_index < no_of_palettes && pal_index >= 0 )
    {
        if( col_index < PAL_USER_SIZE && col_index >= 0 )
        {
            return user_palette[pal_index]->palette[col_index];
        }
    }

    return -1;
}


// sets the color of the user palette to the desired index of the main palette
void PAL_Set_User_Palette_Index( int pal_index, int col_index, int new_val )
{
    if( pal_index < no_of_palettes && pal_index >= 0 )
    {
        if( col_index < PAL_USER_SIZE && col_index >= 0 )
        {
            if( new_val < PAL_MAIN_SIZE && new_val >= 0 )
            {
                user_palette[pal_index]->palette[col_index] = new_val;
            }
        }
    }

    return;
}


// add another user palette, returns index of new palette on success, -1 on failure
int PAL_Add_User_Palette()
{
    // check there is room for another palette
    if( no_of_palettes >= PAL_MAX_USER_PALETTES-1 )
    {
        UTI_Print_Debug( "Unable to add another palette, limit reached" );
        return -1;
    }

    // create data for a new palette
    Create_User_Palette( no_of_palettes );

    return no_of_palettes++;
}


// set current_palette to the next palette on the list, create one if necessary, returns -1 on fail, current
// palette value on success
int PAL_Next_Palette()
{
    if( current_palette < no_of_palettes-1 )
    {
        return ++current_palette;
    }

    // create a new palette if possible
    if( current_palette >= no_of_palettes-1 && no_of_palettes < PAL_MAX_USER_PALETTES-1  )
    {
        current_palette = PAL_Add_User_Palette();   
        return current_palette;
    }

    // otherwise, return -1
    return -1;
}

// set current palette to the previous palette in the list, returns current_palette on success, or 0 if there is
// no previous palette
int PAL_Prev_Palette()
{
    if( current_palette > 0 )
    {
        return --current_palette;
    }

    return 0;
}


// set the current_palette to the value passed, returns current_palette on success, -1 on fail
int PAL_Set_Palette( int index )
{
    if( index < no_of_palettes && index >= 0 )
    {
        current_palette = index;
        return current_palette;
    }

    return -1;
}


// clean up mallocd memory
void PAL_Free()
{
    int i;
    for( i = 0; i < no_of_palettes; i++ )
    {
        UTI_EC_Free( user_palette[i] );
    }

    return;
}


//====================================================================
//
//  sprite.c
//
//  Paul Rafferty 2015
//
//====================================================================

#include <stdio.h>
#include <stdint.h>

#include "utility.h"
#include "sprite.h"
#include "defs.h"


//====================================================================
//  CONSTANTS
//====================================================================





//====================================================================
//  FILE VARIABLES
//====================================================================

static sprite_type                  *sprite[MAX_SPRITES];
static int                          no_of_sprites = 0;

//====================================================================
//  PRIVATE FUNCTIONS
//====================================================================






//====================================================================
//  PUBLIC FUNCTION BODIES
//====================================================================

void SPR_Add_Sprite()
{

    if( no_of_sprites < MAX_SPRITES )
    {

        sprite[no_of_sprites] = UTI_EC_Malloc( sizeof( sprite_type ) );
        
        // set all bytes of sprite definition to '0'
        int i;
        for( i = 0; i < SPRITE_SIZE; i++ )
        {
            sprite[no_of_sprites]->definition[i] = 0x00;
        }

        no_of_sprites++;
        return;
    
    }

    UTI_Print_Debug( "Cannot add sprite, limit reached" );
    
    return;
    
}


// Set a particular pixel in the sprite definition
void SPR_Set_Pixel( int sprite_index, int pixel_index, uint8_t pixel_value )
{
    if( sprite_index < 0 || sprite_index >= no_of_sprites )
    {
        printf( "Sprite index = %d\n", sprite_index );
        UTI_Print_Debug( "Invalid sprite index" );
        return;
    }

    if( pixel_index < 0 || pixel_index >= SPRITE_SIZE )
    {
        printf( "Pixel index = %d\n", pixel_index );
        UTI_Print_Debug( "Invalid pixel index" );
        return;
    }

    sprite[sprite_index]->definition[pixel_index] = pixel_value;
    return;
}


// retrieve the color index of a given sprite pixel, returns 0 (transparency) on failure
uint8_t SPR_Get_Pixel( int sprite_index, int pixel_index )
{

    if( sprite_index < 0 || sprite_index >= no_of_sprites )
    {
        UTI_Print_Debug( "Invalid sprite index" );
        return 0;
    }

    if( pixel_index < 0 || pixel_index >= SPRITE_SIZE )
    {
        printf( "Pixel Index = %d\n", pixel_index );
        UTI_Print_Debug( "Invalid pixel index" );
        return 0;
    }

    return sprite[sprite_index]->definition[pixel_index];

}



// clean up function
void SPR_Free()
{
    int i;
    for( i = 0; i < no_of_sprites; i++ )
    {
        UTI_EC_Free( sprite[i] );
    }

    return;
}

//================================
//  TESTING AND DEBUG
//================================


void SPR_DEBUG_Show_Sprite( int sprite_index )
{

    if( sprite_index < 0 || sprite_index >= no_of_sprites )
    {
        UTI_Print_Debug( "Not a valid sprite index" );
        return;
    }
 
    int i;

    for( i = 0; i < SPRITE_SIZE; i++ )
    {
        printf( "%x%s", sprite[sprite_index]->definition[i], ( i % 16 == 15 ) ? "\n" : " " );
    }

    return;
}


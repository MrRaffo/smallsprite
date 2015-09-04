//====================================================================
//
//  sprite.c
//
//  Paul Rafferty 2015
//
//====================================================================

#include <stdio.h>
#include <stdint.h>
#include <string.h>

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


static sprite_type                  spr_buffer;                 // for copy/paste
static uint8_t                      spr_line_1[SPRITE_W];       // for shift/flip
static uint8_t                      spr_line_2[SPRITE_W];

//====================================================================
//  PRIVATE FUNCTIONS
//====================================================================


void Copy_Sprite( sprite_type *spr1, sprite_type *spr2 )
{
    memcpy( spr2, spr1, sizeof( sprite_type ) );
    
    return;
}

void Copy_Line_To_Sprite( sprite_type *spr, uint8_t *line, int line_index )
{
    if( line_index < 0 || line_index >= SPRITE_H )
    {
        return;
    }

    int i, row_start = line_index * SPRITE_W;
    
    for( i = 0; i < SPRITE_W; i++ )
    {
        spr->definition[row_start + i] = line[i];
    }

    return;
}


void Copy_Line_From_Sprite( sprite_type *spr, uint8_t *line, int line_index )
{
    if( line_index < 0 || line_index >= SPRITE_H )
    {
        return;
    }

    int i, row_start = line_index * SPRITE_W;

    for( i = 0; i < SPRITE_W; i++ )
    {
        line[i] = spr->definition[row_start + i];
    }

    return;
}


//====================================================================
//  PUBLIC FUNCTION BODIES
//====================================================================

void SPR_Init()
{
    int i;

    for( i = 0; i < SPRITE_SIZE; i++ )
    {
        spr_buffer.definition[i] = 0;
    }

    spr_buffer.palette = 0;
    
    
    for( i = 0; i < SPRITE_W; i++ )
    {
        spr_line_1[i] = 0;
        spr_line_2[i] = 0;
    }

    return;
}


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

        sprite[no_of_sprites]->palette = 0;

        no_of_sprites++;
        return;
    
    }

    UTI_Print_Debug( "Cannot add sprite, limit reached" );
    
    return;
    
}


// set all sprite pixels to 0 (transparent)
void SPR_Clear_Sprite( int index )
{
    if( index < 0 || index >= no_of_sprites )
    {
        UTI_Print_Error( "Invalid sprite index" );
        return;
    }

    int i;
    for( i = 0; i < SPRITE_SIZE; i++ )
    {
        sprite[index]->definition[i] = 0;
    }

    return;
}


// copy given sprite to the buffer, return 1 on success
int SPR_Copy_Sprite( int index )
{
    if( index < 0 || index >= no_of_sprites )
    {
        UTI_Print_Error( "Invalid sprite index" );
        return 0;
    }

    Copy_Sprite( sprite[index], &spr_buffer );

    return 1;
}

// copy sprite buffer to current sprite
int SPR_Paste_Sprite( int index )
{
    if( index < 0 || index >= no_of_sprites )
    {
        UTI_Print_Error( "Invalid sprite index" );
        return 0;
    }

    Copy_Sprite( &spr_buffer, sprite[index] );

    return 1;
}


// remove the last sprite on the list
void SPR_Remove_Sprite()
{
    // check there are more than 1 sprite, must be at least one sprite to display
    if( no_of_sprites > 1 )
    {
        UTI_EC_Free( sprite[--no_of_sprites] );
    }

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


int SPR_Get_Sprite_Palette_Index( int sprite_index )
{
    return sprite[sprite_index]->palette;
}


void SPR_Set_Sprite_Palette_Index( int sprite_index, int palette_index )
{
    if( sprite_index < 0 || sprite_index >= no_of_sprites )
    {
        return;
    }

    sprite[sprite_index]->palette = palette_index;

    return;
}


// return the current number of sprites
int SPR_Get_Number_Of_Sprites()
{
    return no_of_sprites;
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


//==========================
//  SPRITE SHIFT/FLIP
//==========================

void SPR_Shift_Left( int index )
{
    if( index < 0 || index >= no_of_sprites )
    {
        UTI_Print_Error( "Invalid sprite index" );
        return;
    }
    
    int line, i;
    uint8_t temp;

    for( line = 0; line < SPRITE_H; line++ )
    {
        Copy_Line_From_Sprite( sprite[index], spr_line_1, line );
        temp = spr_line_1[0];
        for( i = 1; i < SPRITE_W; i++ )
        {
            spr_line_1[i-1] = spr_line_1[i];
        }

        spr_line_1[SPRITE_W-1] = temp;

        Copy_Line_To_Sprite( sprite[index], spr_line_1, line );
    }

    return;
}

void SPR_Shift_Right( int index ) 
{
    if( index < 0 || index >= no_of_sprites )
    {
        UTI_Print_Error( "Invalid sprite index" );
        return;
    }
    
    int line, i;
    uint8_t temp;

    for( line = 0; line < SPRITE_H; line++ )
    {
        Copy_Line_From_Sprite( sprite[index], spr_line_1, line );
        temp = spr_line_1[SPRITE_W-1];
        for( i = SPRITE_W-2; i >= 0; i-- )
        {
            spr_line_1[i+1] = spr_line_1[i];
        }

        spr_line_1[0] = temp;

        Copy_Line_To_Sprite( sprite[index], spr_line_1, line );
    }

    return;
}

void SPR_Shift_Up( int index )
{
    if( index < 0 || index >= no_of_sprites )
    {
        return;
    }

    int i;
    
    Copy_Line_From_Sprite( sprite[index], spr_line_2, 0 );

    for( i = 1; i <= SPRITE_H-1; i++ )
    {
        Copy_Line_From_Sprite( sprite[index], spr_line_1, i );
        Copy_Line_To_Sprite( sprite[index], spr_line_1, i-1 );
    }

    Copy_Line_To_Sprite( sprite[index], spr_line_2, SPRITE_H-1 );

    return;
}


void SPR_Shift_Down( int index )
{
    if( index < 0 || index >= no_of_sprites )
    {
        return;
    }

    int i;
    
    Copy_Line_From_Sprite( sprite[index], spr_line_2, SPRITE_H-1 );

    for( i = SPRITE_H-1; i >= 0; i-- )
    {
        Copy_Line_From_Sprite( sprite[index], spr_line_1, i );
        Copy_Line_To_Sprite( sprite[index], spr_line_1, i+1 );
    }

    Copy_Line_To_Sprite( sprite[index], spr_line_2, 0 );

    return;
}



void SPR_Flip_Horizontal( int index )
{
    if( index < 0 || index >= no_of_sprites )
    {
        UTI_Print_Error( "Invalid sprite index" );
        return;
    }

    int i, line;
    uint8_t temp;

    for( line = 0; line < SPRITE_H; line++ )
    {
        Copy_Line_From_Sprite( sprite[index], spr_line_1, line );
        for( i = 0; i < SPRITE_W/2; i++ )
        {
            temp = spr_line_1[i];
            spr_line_1[i] = spr_line_1[(SPRITE_W-1)-i];
            spr_line_1[(SPRITE_W-1)-i] = temp;
        }

        Copy_Line_To_Sprite( sprite[index], spr_line_1, line );
    }

    return;
}



void SPR_Flip_Vertical( int index )
{
    
    if( index < 0 || index >= no_of_sprites )
    {
        UTI_Print_Error( "Invalid sprite index" );
        return;
    }

    int i;

    for( i = 0; i < SPRITE_H/2; i++ )
    {
        Copy_Line_From_Sprite( sprite[index], spr_line_1, i );
        Copy_Line_From_Sprite( sprite[index], spr_line_2, (SPRITE_H-1) - i );
        Copy_Line_To_Sprite( sprite[index], spr_line_2, i );
        Copy_Line_To_Sprite( sprite[index], spr_line_1, (SPRITE_H-1) - i );
    }

    return;
}



//=============================
//  FILE I/O
//=============================

// return pointer to the sprite definition
uint8_t         *SPR_Get_Sprite( int index )
{
    if( index >= 0 && index < no_of_sprites )
    {
        return sprite[index]->definition;
    }

    return NULL;
}

// take pointer from file
int             SPR_Load_Sprite( sprite_type *definition )
{
    if( no_of_sprites >= MAX_SPRITES )
    {
        UTI_Print_Debug( "Cannot add sprite, limit reached" );
        return 0;
    }

    sprite[no_of_sprites++] = definition;

    return 1;
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


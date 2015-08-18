
#include <stdio.h>
#include <stdint.h>

#include "utility.h"
#include "anim.h"
#include "defs.h"



//===================================================================
//  CONSTANTS
//===================================================================



//===================================================================
//  FILE VARIABLES
//===================================================================


static anim_type                *animation[MAX_ANIMATIONS];
static int                      no_of_animations = 0;


//===================================================================
//  PRIVATE FUNCTIONS
//===================================================================


//===================================================================
//  PUBLIC FUNCTION BODIES
//===================================================================

// add a new animation
void     ANI_Add_Animation()
{
    if( no_of_animations < MAX_ANIMATIONS )
    {
        animation[no_of_animations] = UTI_EC_Malloc( sizeof( anim_type ) );

        // set all frames to '-1' (equivalent to '\0' in strings)
        int i;
        for( i = 0; i < MAX_ANIMATION_FRAMES; i++ )
        {
            animation[no_of_animations]->frame_list[i] = -1;
        }

        animation[no_of_animations]->frame_wait = 1;
        animation[no_of_animations]->no_of_frames = 0;

        no_of_animations++;
 
        return;
    }

    UTI_Print_Debug( "Cannot add animation, limit reached" );

    return;
}

// remove last animation
void    ANI_Remove_Animation()
{
    if( no_of_animations > 1 )
    {
        UTI_EC_Free( animation[--no_of_animations] );
        animation[no_of_animations] = NULL;
    }

    return;
}

// add frame to current animation, return 1 on success
int     ANI_Add_Frame( int anim_index, int value )
{
    if( anim_index < 0 || anim_index >= no_of_animations )
    {
        UTI_Print_Debug( "Invalid animation index" );
        return 0;
    }

    anim_type *temp;
    temp = animation[anim_index];


    if( temp->no_of_frames >= MAX_ANIMATION_FRAMES )
    {
        UTI_Print_Debug( "Cannot add frame, limit reached" );
        return 0;
    }

    temp->frame_list[temp->no_of_frames++] = value;

    return 1;
}


// remove last frame from current animation
void    ANI_Remove_Frame( int anim_index )
{
    if( anim_index < 0 || anim_index >= no_of_animations )
    {
        UTI_Print_Error( "Invalid animation index" );
        return;
    }

    anim_type *temp;
    temp = animation[anim_index];

    if( temp->no_of_frames > 0 )
    {
        temp->frame_list[temp->no_of_frames--] = -1;
    }

    return;
}


// return number of frames in an animation
int     ANI_Get_Number_Of_Frames( int anim_index )
{
    if( anim_index >= 0 && anim_index < no_of_animations )
    {
        return animation[anim_index]->no_of_frames;
    }

    return -1;
}

// return the number of animations
int     ANI_Get_Number_Of_Animations()
{
    return no_of_animations;
}



void    ANI_Free()
{
    while( no_of_animations > 0 )
    {
        ANI_Remove_Animation();
    }

    return;
}

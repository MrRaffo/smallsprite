
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

// remove given animation
void    ANI_Remove_Animation( int index )
{

    if( no_of_animations > 1 && index < no_of_animations && index >= 0 )
    {
        UTI_EC_Free( animation[index] );
        animation[index] = NULL;

        for( index += 1; index < no_of_animations; index++ )
        {
            // rearrange animation list
            animation[index-1] = animation[index];
        }

        no_of_animations--;
    }

    return;
}

// TODO - this is a mix of Add_Frame and Set_Frame - fix
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


int     ANI_Get_Frame( int anim_index, int frame_index )
{
    if( anim_index < 0 || anim_index >= no_of_animations )
    {
        UTI_Print_Debug( "Invalid animation index" );
        return -1;
    }

    anim_type *temp;
    temp = animation[anim_index];

    if( frame_index < 0 || frame_index > ( temp->no_of_frames ) )
    {
        UTI_Print_Debug( "Invalid frame index" );
        return -1;
    }

    return temp->frame_list[frame_index];

}


// return the number of animations
int     ANI_Get_Number_Of_Animations()
{
    return no_of_animations;
}



void    ANI_Free()
{
    int counter;

    for( counter = 0; counter < no_of_animations; counter++ )
    {
        UTI_EC_Free( animation[counter] );
        animation[counter] = NULL;
    }

    return;
}


//===================================================================
//  TESTING
//===================================================================

void ANI_Print_Frame_List( int index )
{
    int i;
    for( i = 0; animation[index]->frame_list[i] != -1; i++ )
    {
        printf( "%d\t", animation[index]->frame_list[i] );
    }

    return;
}

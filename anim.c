
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

        ANI_Add_Frame( no_of_animations-1 );

 
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


// add frame to current animation, return 1 on success
int     ANI_Add_Frame( int anim_index )
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

    temp->frame_list[temp->no_of_frames++] = 0;

    return 1;
}


// set the given frame in the animation to the desired sprite definition
void    ANI_Set_Frame( int anim_index, int frame_index, int value )
{
    if( anim_index < 0 || anim_index >= no_of_animations )
    {
        UTI_Print_Debug( "Invalid anim_index" );
        return;
    }

    anim_type *temp;
    temp = animation[anim_index];

    if( frame_index < 0 || frame_index > temp->no_of_frames )
    {
        UTI_Print_Debug( "Invalid frame_index" );
        return;
    }

    if( value < 0 )
    {
        UTI_Print_Debug( "Invalid value" );
        return;
    }

    temp->frame_list[frame_index] = value;

    return;
}


void    ANI_Remove_Frame( int anim_index, int frame_index )
{
    if( anim_index < 0 || anim_index >= no_of_animations )
    {
        UTI_Print_Error( "Invalid animation index" );
        return;
    }

    anim_type *temp;
    temp = animation[anim_index];

    if( frame_index < 1 || frame_index > temp->no_of_frames )
    {
        UTI_Print_Error( "Invalid frame index" );
        return;
    }

    for( ; frame_index <= temp->no_of_frames; frame_index++ )
    {
        temp->frame_list[frame_index] = temp->frame_list[frame_index+1];
    }

    temp->no_of_frames--;

    return;
}

// remove last frame from current animation
void    ANI_Delete_Frame( int anim_index )
{
    if( anim_index < 0 || anim_index >= no_of_animations )
    {
        UTI_Print_Error( "Invalid animation index" );
        return;
    }

    anim_type *temp;
    temp = animation[anim_index];

    if( temp->no_of_frames > 1 )
    {
        temp->frame_list[--(temp->no_of_frames)] = -1;
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


//=============================
//  PLAYER
//=============================

#define MAX_DELAY               1024

anim_type       *current_anim = NULL;

int     current_animation = 0;
int     current_frame = 0;
int     playing = 0;
int     frame_delay = 1;                    // speed of animation cycle, lower is faster
int     frame_timer = 0;
int     loop = 0;

void    ANI_Init_Animation()
{
    ANI_Add_Animation();
    current_anim = animation[0];
}

void    ANI_Play_Animation( int anim_index )
{
    if( anim_index < 0 || anim_index > no_of_animations )
    {
        UTI_Print_Debug( "Invalid animation index" );
        return;
    }

    current_anim = animation[anim_index];
    current_frame = 0;
    frame_timer = frame_delay;

    playing = 1;
}

void    ANI_Stop_Animation()
{
    playing = 0;
}


void    ANI_Update_Animation()
{
    // this is called every frame, handles most of the animation
    if( playing == 0 )
    {
        return;
    }

    // check if its time to update the animation cycle
    --frame_timer;
    if( frame_timer <= 0 )
    {
        current_frame++;
        if( current_anim->frame_list[current_frame] < 0 )
        {
            current_frame = 0;

            // we've reached a negative index, the end of the cycle
            playing = ( loop == 1 ) ? 1 : 0;
        }

        frame_timer = frame_delay;
    }

    return;
}


void    ANI_Loop_Toggle()
{
    loop = ( loop == 0 ) ? 1 : 0;

    return;
}


void    ANI_Speed_Up()
{
    if( frame_delay > 1 )
    {
        frame_delay--;
    }
    else
    {
        frame_delay = 1;
    }

    return;
}


void    ANI_Speed_Down()
{
    if( frame_delay < MAX_DELAY )
    {
        frame_delay++;
    }
    else
    {
        frame_delay = MAX_DELAY;
    }

    return;
}


int     *ANI_Get_Loop_Address()
{
    return &loop;
}


int     ANI_Get_Current_Frame()
{
    return current_anim->frame_list[current_frame];
}


//==============================
//  File I/O
//==============================

// retrieve data pointer for saving to file
anim_type   *ANI_Get_Animation( int index )
{
    if( index >= 0 && index < no_of_animations )
    {
        return animation[index];
    }

    return NULL;
}

// takes a pointer of data loaded from file
int         ANI_Load_Animation( int32_t *frames, int no_of_frames, int speed )
{
    // TODO

    return 1;
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

    putchar( '\n' );

    return;
}

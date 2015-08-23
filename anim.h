//===================================================================
//
//  anim.h
//
//  provides structs and function prototypes for the smallsprite 
//  program's animation functionality
//
//  Paul Rafferty 2015
//
//===================================================================

#ifndef __anim_h__
#define __anim_h__

#include "defs.h"

//===================================================================
//  CONSTANTS
//===================================================================

#define MAX_ANIMATIONS          1024
#define MAX_ANIMATION_FRAMES    1024

//===================================================================
//  TYPES
//===================================================================

// the list of frames is terminated by '-1' this is a signal to go to the beginning (or next anim)
struct anim_s   {   int32_t    frame_list[MAX_ANIMATION_FRAMES];        // a list of sprite indices
                    int32_t    frame_wait;              // number of frames to wait before changing
                    int32_t    no_of_frames;
                };

typedef struct anim_s anim_type;

//===================================================================
//  PROTOTYPES
//===================================================================


// creates an animation and sets current_anim pointer
void    ANI_Init_Animation();

// add a new animation
void    ANI_Add_Animation();

// remove last animation
void    ANI_Remove_Animation( int index );

// add frame to given animation
int     ANI_Add_Frame( int anim_index );


// set a frame to a given sprite definition
void    ANI_Set_Frame( int anim_index, int frame_index, int value );


// remove last frame from given animation
void    ANI_Delete_Frame( int anim_index );

// remove given frame from animation
void    ANI_Remove_Frame( int anim_index, int frame_index );

// return number of frames in an animation
int     ANI_Get_Number_Of_Frames( int anim_index );

// return index of a given frame in a given animation, -1 on fail
int     ANI_Get_Frame( int anim_index, int frame_index );

// return the number of animations
int     ANI_Get_Number_Of_Animations();

// free used memory TODO needs fixed
void    ANI_Free();

//=============================
//  PLAYER
//=============================

void    ANI_Play_Animation( int anim_index );

void    ANI_Stop_Animation();

void    ANI_Update_Animation();

void    ANI_Loop_Toggle();

void    ANI_Speed_Up();

void    ANI_Speed_Down();

int     *ANI_Get_Loop_Address();

int     ANI_Get_Current_Frame();

//==============================
//  File I/O
//==============================

// retrieve data pointer for saving to file
anim_type   *ANI_Get_Animation( int index );

// takes a pointer of data loaded from file
int         ANI_Load_Animation( int32_t *anim, int no_of_frames, int speed );

//===================================================================
//  TESTING AND DEBUGING
//===================================================================

void ANI_Print_Frame_List( int index );


#endif  // __anim_h__

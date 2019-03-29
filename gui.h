#ifndef __gui_h__
#define __gui_h__


//====================================================================
//  DEFINE
//====================================================================



//====================================================================
//  TYPES
//====================================================================



//====================================================================
//  PUBLIC PROTOTYPES
//====================================================================

// initialize colours used by the gui
int GUI_Init();


// draw the user interface
void GUI_Draw_Interface();


// draw current sprite to the edit window
void GUI_Draw_Edit_Sprite();


//====================
//  MOUSE INPUT
//====================

// read user input using the mouse
void GUI_Get_Mouse_Input();


#endif // __gui_h__

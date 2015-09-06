#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "defs.h"

#include "utility.h"
#include "graphics.h"
#include "gui.h"
#include "palette.h"
#include "sprite.h"
#include "anim.h"
#include "file.h"

//====================================================================
//  CONSTANTS
//====================================================================





//====================================================================
//  TYPES
//====================================================================





//====================================================================
//  GLOBALS
//====================================================================





//====================================================================
//  FUNCTION PROTOTYPES
//====================================================================

void Print_Program_Info( char *prog_name )
{
    printf( "\n%s - A Sprite Editing Program\tBy Paul Rafferty 2015\n", prog_name );
    printf( "\n" );
}



//====================================================================
//  MAIN
//====================================================================

int main( int argc, char *argv[] )
{
 
    Print_Program_Info( argv[0] );

    // parse arguments
    FIL_Parse_Arguments( argc, argv );

    // CREATE DISPLAY
    if( GRA_Create_Display( "SmallSprite",  WINDOW_WIDTH, WINDOW_HEIGHT, 
                                            WINDOW_WIDTH, WINDOW_HEIGHT ) == 0 )
    {
        UTI_Fatal_Error( "Unable to create screen" );
    }

    // LOAD FONT
    if( GRA_Load_Font( "data/font" ) == 0 )
    {
        UTI_Fatal_Error( "Unable to load font data" );
    }

    // INITIALIZE PALETTE DATA AND GENERATE MAIN PALETTE
    PAL_Init();
    PAL_Generate_Main_Palette();

    // INITIALIZE GUI
    GUI_Init();

    // INITIALIZE SPRITES (SET UP BUFFER)
    SPR_Init();

    // OPEN OR CREATE DATA
    if(  FIL_Open_File() == 0 )
    {
        // file not found, need to create a 'blank' file
        SPR_Add_Sprite();
        ANI_Add_Animation();
        PAL_Add_User_Palette();
    }

    ANI_Init_Animation();

    int running = 1;            // loop control
    unsigned int start_time;
    unsigned int end_time;
    while( running )
    {
        start_time = GRA_GetTicks();

        // clear screen for next render
        GRA_Clear_Screen();

        ANI_Update_Animation();

        // draw the user interface
        GUI_Draw_Interface();
        GUI_Draw_Edit_Sprite();

        // check buttons
        GRA_Check_User_Input();

        // check mouse use
        GUI_Get_Mouse_Input();

        // update display and swap buffers
        GRA_Refresh_Window();

        // check for user quit
        running = GRA_Check_Quit();


        end_time = GRA_GetTicks() - start_time;
    
        if( end_time < FRAME_TIME )
        {
            GRA_Delay( FRAME_TIME - end_time );
        }
    }

    // SPR_DEBUG_Show_Sprite( 0 );

    // write the file
    if( FIL_Write_File() == 0 )
    {
        printf( "Error writing file\n" );
    }

    // free palette memory
    PAL_Free();

    // free sprite memory
    SPR_Free();

    // free animation data
    ANI_Free();

    // free graphics memory and shut down SDL
    GRA_Close(); 


    return 0;
}




//====================================================================
//  FUNCTION BODIES
//====================================================================



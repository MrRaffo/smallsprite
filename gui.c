//===========================================
//  gui.c
//
//  contains code for drawing and interacting
//  with the screen elements
//
//  Paul Rafferty 2015
//===========================================

#include <stdio.h>
#include <stdint.h>

#include "defs.h"

#include "graphics.h"
#include "utility.h"
#include "palette.h"
#include "sprite.h"

//=====================================================================
//  FILE VARIABLES
//=====================================================================

//=======================
// USER OPTIONS
//=======================

static int                  selected_palette_index  = 0;            // palette user is currently using
static int                  selected_palette_option = 1;            // palette option to change

static int                  sprite_grid_base        = 0;            // index of sprite grid 0 (scroll)
static int                  sprite_grid_index = 0;         // sprite definition to edit (sprite_grid)




//=======================
//  BASIC COLOURS
//=======================

// set in GUI_Init();

static uint32_t             BLACK;
static uint32_t             WHITE;
static uint32_t             RED;
static uint32_t             GREEN;
static uint32_t             BLUE;
static uint32_t             YELLOW;
static uint32_t             CYAN;
static uint32_t             LIGHT_GREY;
static uint32_t             DARK_GREY;
static uint32_t             V_DARK_GREY;
static uint32_t             INVIS;


//=======================
//  SCREEN AREAS
//=======================

// screen area list
enum    area_list   {   AREA_SPRITE_EDIT,
                        AREA_SPRITE_GRID,
                        AREA_SPRITE_GRID_SCROLL,
                        AREA_MAIN_PALETTE,
                        AREA_USER_PALETTE,
                        AREA_ANIM_EDIT,
                        AREA_ANIM_CONTROL,
                        AREA_ANIM_PLAYER,

                        NO_OF_SCREEN_AREAS
                    };

// data needed to draw the main screen areas and capture the mouse

// area x positions
static int area_pos_x[]     = { GUI_AREA_SPRITE_EDIT_X,
                                GUI_AREA_SPRITE_GRID_X,
                                GUI_AREA_SPRITE_GRID_SCROLL_X,
                                GUI_AREA_MAIN_PALETTE_X,
                                GUI_AREA_USER_PALETTE_X,
                                GUI_AREA_ANIM_EDIT_X,
                                GUI_AREA_ANIM_CONTROL_X,
                                GUI_AREA_ANIM_PLAYER_X
                              };

// area y positions
static int area_pos_y[]     = { GUI_AREA_SPRITE_EDIT_Y,
                                GUI_AREA_SPRITE_GRID_Y,
                                GUI_AREA_SPRITE_GRID_SCROLL_Y,
                                GUI_AREA_MAIN_PALETTE_Y,
                                GUI_AREA_USER_PALETTE_Y,
                                GUI_AREA_ANIM_EDIT_Y,
                                GUI_AREA_ANIM_CONTROL_Y,
                                GUI_AREA_ANIM_PLAYER_Y
                              };

// width of each screen area
static int area_w[]         = { GUI_AREA_SPRITE_EDIT_W,
                                GUI_AREA_SPRITE_GRID_W,
                                GUI_AREA_SPRITE_GRID_SCROLL_W,
                                GUI_AREA_MAIN_PALETTE_W,
                                GUI_AREA_USER_PALETTE_W,
                                GUI_AREA_ANIM_EDIT_W,
                                GUI_AREA_ANIM_CONTROL_W,
                                GUI_AREA_ANIM_PLAYER_W
                              };


// height of each screen area
static int area_h[]         = { GUI_AREA_SPRITE_EDIT_H,
                                GUI_AREA_SPRITE_GRID_H,
                                GUI_AREA_SPRITE_GRID_SCROLL_H,
                                GUI_AREA_MAIN_PALETTE_H,
                                GUI_AREA_USER_PALETTE_H,
                                GUI_AREA_ANIM_EDIT_H,
                                GUI_AREA_ANIM_CONTROL_H,
                                GUI_AREA_ANIM_PLAYER_H
                              };

// label/name of each screen area
static char *area_label[]   = { GUI_AREA_SPRITE_EDIT_LABEL,
                                GUI_AREA_SPRITE_GRID_LABEL,
                                GUI_AREA_SPRITE_GRID_SCROLL_LABEL,
                                GUI_AREA_MAIN_PALETTE_LABEL,
                                GUI_AREA_USER_PALETTE_LABEL,
                                GUI_AREA_ANIM_EDIT_LABEL,
                                GUI_AREA_ANIM_CONTROL_LABEL,
                                GUI_AREA_ANIM_PLAYER_LABEL
                              };

// color to draw each area, set in GUI_Init();
static int area_color[NO_OF_SCREEN_AREAS];


//=======================
//  SPRITE CONTROLS
//=======================

// positions of the user palette options
static int user_palette_control_x[] = { GUI_AREA_USER_PALETTE_X + 300,
                                        GUI_AREA_USER_PALETTE_X + 16,
                                        GUI_AREA_USER_PALETTE_X + 150,
                                        GUI_AREA_USER_PALETTE_X + 300,
                                        GUI_AREA_USER_PALETTE_X + 16,
                                        GUI_AREA_USER_PALETTE_X + 150,
                                        GUI_AREA_USER_PALETTE_X + 300,
                                        GUI_AREA_USER_PALETTE_X + 16,
                                        GUI_AREA_USER_PALETTE_X + 150,
                                        GUI_AREA_USER_PALETTE_X + 300,
                                        GUI_AREA_USER_PALETTE_X + 16,
                                        GUI_AREA_USER_PALETTE_X + 150,
                                        GUI_AREA_USER_PALETTE_X + 300,
                                        GUI_AREA_USER_PALETTE_X + 16,
                                        GUI_AREA_USER_PALETTE_X + 150,
                                        GUI_AREA_USER_PALETTE_X + 300
                                       };

static int user_palette_control_y[] = { GUI_AREA_USER_PALETTE_Y,
                                        GUI_AREA_USER_PALETTE_Y + 32,
                                        GUI_AREA_USER_PALETTE_Y + 32,
                                        GUI_AREA_USER_PALETTE_Y + 32,
                                        GUI_AREA_USER_PALETTE_Y + 64,
                                        GUI_AREA_USER_PALETTE_Y + 64,
                                        GUI_AREA_USER_PALETTE_Y + 64,
                                        GUI_AREA_USER_PALETTE_Y + 96,
                                        GUI_AREA_USER_PALETTE_Y + 96,
                                        GUI_AREA_USER_PALETTE_Y + 96,
                                        GUI_AREA_USER_PALETTE_Y + 128,
                                        GUI_AREA_USER_PALETTE_Y + 128,
                                        GUI_AREA_USER_PALETTE_Y + 128,
                                        GUI_AREA_USER_PALETTE_Y + 160,
                                        GUI_AREA_USER_PALETTE_Y + 160,
                                        GUI_AREA_USER_PALETTE_Y + 160
                                      };

static char *user_palette_control_label[] = {   "TRANSPARENT",
                                                "COLOR 1",
                                                "COLOR 2",
                                                "COLOR 3",
                                                "COLOR 4",
                                                "COLOR 5",
                                                "COLOR 6",
                                                "COLOR 7",
                                                "COLOR 8",
                                                "COLOR 9",
                                                "COLOR 10",
                                                "COLOR 11",
                                                "COLOR 12",
                                                "COLOR 13",
                                                "COLOR 14",
                                                "COLOR 15"
                                             };

//=====================================================================
//  PRIVATE FUNCTIONS
//=====================================================================

// for printing to screen
#define MAX_INT_STRING          16
#include <string.h>


static char         palette_index_text[MAX_INT_STRING];

static void reverse_string( char str[] )
{
    int len = strlen( str );
    char *begin, *end, temp;
    int i;

    begin = end = str;

    for( i = 0; i < len - 1; i++ )
    {
        end++;
    }

    for( i = 0; i < len / 2; i++ )
    {
        temp = *begin;
        *begin = *end;
        *end = temp;
    
        begin++;
        end--;
    }

    return;
}

static void Convert_Int_To_String( char str[], int x, int limit )
{

    int c;

    if( x == 0 )
    {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    for( c = 0; x != 0 && c < limit; c++ )
    {
        str[c] = '0' +  x % 10;
        x /= 10;
    }

    
    str[c] = '\0';

    reverse_string( str );

    return;
}

#define PALETTE_INDEX_TEXT_X        GUI_AREA_MAIN_PALETTE_X
#define PALETTE_INDEX_TEXT_Y        8

void Set_Palette_Index_Text()
{
    GRA_Simple_Text( "PALETTE = ", PALETTE_INDEX_TEXT_X, PALETTE_INDEX_TEXT_Y, WHITE, 0, 0 );
    GRA_Simple_Text( palette_index_text, PALETTE_INDEX_TEXT_X+80, PALETTE_INDEX_TEXT_Y, WHITE, 0, 0 );

    return;
}

// drawing a sprite preview (64x64 pixels) for the grid and animation preview areas
static void Draw_Sprite_Preview( int x, int y, int sprite_index )
{
    int i, dx, dy;
    int palette_index = SPR_Get_Sprite_Palette_Index( sprite_index );

    for( i = 0; i < SPRITE_SIZE; i++ )
    {
        dx = i % SPRITE_W;
        dy = i / SPRITE_W;

        int color_index = SPR_Get_Pixel( sprite_index, i );
        int main_palette_index = PAL_Get_User_Palette_Index( palette_index, color_index );
        uint32_t color = PAL_Get_Main_Palette_Color( main_palette_index );

        GRA_Draw_Filled_Rectangle(  x+dx*4,
                                    y+dy*4,
                                    4, 4,
                                    color
                                 );
    }

    return;
}


//========================
//  DRAW FUNCTIONS
//========================

// draw the border for each of the main screen areas
static void Draw_Area_Outlines()
{
    int i;
    for( i = 0; i < NO_OF_SCREEN_AREAS; i++ )
    {
        GRA_Draw_Hollow_Rectangle(  area_pos_x[i]-1,
                                    area_pos_y[i]-1,
                                    area_w[i]+1,
                                    area_h[i]+2,
                                    area_color[i]
                                 );
    }

    return;
}


// draw the controls for the sprite editor
static void Draw_User_Palette_Controls()
{
    int i;
    for( i = 0; i < 16; i++ )
    {
        GRA_Draw_Hollow_Rectangle(  user_palette_control_x[i]-1,
                                    user_palette_control_y[i]-1,
                                    GUI_AREA_USER_PALETTE_COLOR_W+1,
                                    GUI_AREA_USER_PALETTE_COLOR_H+2,
                                    LIGHT_GREY
                                 );

        // draw the option the user has selected, defaults to 0 on program start (transparency option)
        GRA_Draw_Hollow_Rectangle(  user_palette_control_x[selected_palette_option]-1,
                                    user_palette_control_y[selected_palette_option]-1,
                                    GUI_AREA_USER_PALETTE_COLOR_W+1,
                                    GUI_AREA_USER_PALETTE_COLOR_H+2,
                                    CYAN
                                 );

        // make highlight double thickness of other box outlines for clarity
        GRA_Draw_Hollow_Rectangle(  user_palette_control_x[selected_palette_option]-2,
                                    user_palette_control_y[selected_palette_option]-2,
                                    GUI_AREA_USER_PALETTE_COLOR_W+3,
                                    GUI_AREA_USER_PALETTE_COLOR_H+4,
                                    CYAN
                                 );


        //TODO This function should take its color data from the current palette
        GRA_Draw_Filled_Rectangle(  user_palette_control_x[i],
                                    user_palette_control_y[i],
                                    GUI_AREA_USER_PALETTE_COLOR_W,
                                    GUI_AREA_USER_PALETTE_COLOR_H,
                                    PAL_Get_User_Palette_Color( selected_palette_index, i )
                                 );

        GRA_Simple_Text(    user_palette_control_label[i],
                            user_palette_control_x[i]+32,
                            user_palette_control_y[i]+6,
                            LIGHT_GREY, 0, 0 );
    }

    return;
}


static void Draw_Main_Palette()
{
    int row, column;

    for( column = 0; column < GUI_AREA_MAIN_PALETTE_COLUMNS; column++ )
    {
        for( row = 0; row < GUI_AREA_MAIN_PALETTE_ROWS; row++ )
        {
            GRA_Draw_Filled_Rectangle(  column * GUI_AREA_MAIN_PALETTE_COLOR_W + GUI_AREA_MAIN_PALETTE_X,
                                        row    * GUI_AREA_MAIN_PALETTE_COLOR_H + GUI_AREA_MAIN_PALETTE_Y,
                                        GUI_AREA_MAIN_PALETTE_COLOR_W,
                                        GUI_AREA_MAIN_PALETTE_COLOR_H,
                                        PAL_Get_Main_Palette_Color( row*GUI_AREA_MAIN_PALETTE_COLUMNS+column )
                                     );
        }
    }

    return;
}

// draws the sprite grid border and sprite definitions
static void Draw_Sprite_Grid()
{
    
    // this will show which sprite definitions are not yet active
    GRA_Draw_Filled_Rectangle(  GUI_AREA_SPRITE_GRID_X,
                                GUI_AREA_SPRITE_GRID_Y,
                                GUI_AREA_SPRITE_GRID_W,
                                GUI_AREA_SPRITE_GRID_H,
                                V_DARK_GREY
                             );
    
    int no_of_sprites = SPR_Get_Number_Of_Sprites();

    int cur_sprite, cur_x, cur_y;
    for( cur_sprite = sprite_grid_base; cur_sprite < no_of_sprites; cur_sprite++ )
    {
        cur_x = (cur_sprite - sprite_grid_base) % GUI_AREA_SPRITE_GRID_COLUMNS;
        cur_y = (cur_sprite - sprite_grid_base) / GUI_AREA_SPRITE_GRID_COLUMNS;

        Draw_Sprite_Preview (   GUI_AREA_SPRITE_GRID_X + cur_x*GUI_SPRITE_W,
                                GUI_AREA_SPRITE_GRID_Y + cur_y*GUI_SPRITE_H,
                                cur_sprite
                            );
    }


    int r, c;

    // Draw the grid
    for( r = 1; r < GUI_AREA_SPRITE_GRID_ROWS; r++ )
    {
        GRA_Draw_Horizontal_Line(   GUI_AREA_SPRITE_GRID_X,
                                    GUI_AREA_SPRITE_GRID_X + GUI_AREA_SPRITE_GRID_W,
                                    GUI_AREA_SPRITE_GRID_Y + (r*GUI_SPRITE_H),
                                    DARK_GREY
                                );
    }

    for( c = 1; c < GUI_AREA_SPRITE_GRID_COLUMNS; c++ )
    {
        GRA_Draw_Vertical_Line(     GUI_AREA_SPRITE_GRID_X + (c*GUI_SPRITE_W),
                                    GUI_AREA_SPRITE_GRID_Y,
                                    GUI_AREA_SPRITE_GRID_Y + GUI_AREA_SPRITE_GRID_H,
                                    DARK_GREY
                              );
    }
    

    // highlight the selected definition
    int sprite_x = 0, sprite_y = 0, index = sprite_grid_index - sprite_grid_base;
    
    sprite_x = index % GUI_AREA_SPRITE_GRID_COLUMNS;
    sprite_y = index / GUI_AREA_SPRITE_GRID_COLUMNS;


    GRA_Draw_Hollow_Rectangle(  GUI_AREA_SPRITE_GRID_X + sprite_x * GUI_SPRITE_W,
                                GUI_AREA_SPRITE_GRID_Y + sprite_y * GUI_SPRITE_H,
                                GUI_SPRITE_W,
                                GUI_SPRITE_H,
                                CYAN
                             );

    return;
}



//======================
//  BUTTON FUNCTIONS
//======================

// test function for button
void BTN_Next_User_Palette()
{
    printf( "Next Palette\n" );

    if( ( selected_palette_index = PAL_Next_Palette() ) != -1 )
    {  
        Convert_Int_To_String( palette_index_text, selected_palette_index, MAX_INT_STRING );
    }
    else
    {
        strncpy( palette_index_text, "N/A", 4 );
        palette_index_text[3] = '\0';
    }

    // set the selected palette to the current sprite
    SPR_Set_Sprite_Palette_Index( sprite_grid_index, selected_palette_index );

    return;

}

// test function for button
void BTN_Prev_User_Palette()
{
    printf( "Prev Palette\n" );

    if( ( selected_palette_index = PAL_Prev_Palette() ) != -1 )
    {
        Convert_Int_To_String( palette_index_text, selected_palette_index, MAX_INT_STRING );
    }
    else
    {
        strncpy( palette_index_text, "N/A", 4 );
        palette_index_text[3] = '\0';
    }

    // set the selected palette to the current sprite
    SPR_Set_Sprite_Palette_Index( sprite_grid_index, selected_palette_index );

    return;
}

    //== GRID SCROLL ==//

void BTN_Scroll_Grid_Up()
{
    if( sprite_grid_base > 0 )
    {
        sprite_grid_base -= GUI_AREA_SPRITE_GRID_COLUMNS;

        // check that the sprite_grid_base is correctly aligned
        if( (sprite_grid_base % GUI_AREA_SPRITE_GRID_COLUMNS) != 0 )
        {
            UTI_Print_Debug( "sprite_grid_base incorrectly aligned" );
            sprite_grid_base = 0;
        }
    }

    if( sprite_grid_base < 0 )
    {
        sprite_grid_base = 0;
    }

    return;
}

void BTN_Scroll_Grid_Down()
{
    sprite_grid_base += GUI_AREA_SPRITE_GRID_COLUMNS;

    // protect against signed int wrap-around, however unlikely
    if( sprite_grid_base < 0 )
    {
        sprite_grid_base = 0;
    }

    return;
}

    //== SPRITE GRID ==//

void BTN_Add_Sprite()
{
    SPR_Add_Sprite();

    return;
}

void BTN_Remove_Sprite()
{
    SPR_Remove_Sprite();
    
    int no_of_sprites;

    if( sprite_grid_index >= ( no_of_sprites = SPR_Get_Number_Of_Sprites() ) )
    {
        sprite_grid_index = no_of_sprites - 1;
    }

    return;
}

//======================
//  INPUT FUNCTIONS
//======================

// report which area the given coord is in, used for capturing where the mouse is
// returns -1 if the mouse is between elements of the screen (ie nowhere useful)

static int Get_Area( int x, int y )
{
    int i;
    for( i = 0; i < NO_OF_SCREEN_AREAS; i++ )
    {
        if( x > area_pos_x[i] && x <= ( area_pos_x[i] + area_w[i] ) &&
            y > area_pos_y[i] && y <= ( area_pos_y[i] + area_h[i] ) )
        {
            return i;
        }
    }

    return -1;
}

// get the offset of the mouse in its current area
static void Get_Relative_Position( int area_index, int *mouse_x, int *mouse_y )
{
    *mouse_x = *mouse_x - area_pos_x[area_index];
    *mouse_y = *mouse_y - area_pos_y[area_index];

    return;
}


static void Input_Sprite_Edit( int button, int x, int y )
{

    if( button > 0 )
    {

        Get_Relative_Position( AREA_SPRITE_EDIT, &x, &y );
        
        int index = (y / GUI_AREA_SPRITE_EDIT_PIXEL_H)*SPRITE_W + (x / GUI_AREA_SPRITE_EDIT_PIXEL_W );
        
        // set pixel to selected colour (mouse button 1)
        if( button == 1 )
        {
            SPR_Set_Pixel( sprite_grid_index, index, selected_palette_option );
        }
        // set pixel to transparency (mouse button 2, ie erase)
        else if( button == 2 )
        {
            SPR_Set_Pixel( sprite_grid_index, index, 0 );
        }
    }

    return;
}


// checks if user selects a colour option
static void Input_User_Palette( int button, int x, int y )
{
    int i;
    
    if( button == 1 )
    {
        for( i = 0; i < GUI_AREA_USER_PALETTE_NUMBER; i++ )
        {
            if( x > user_palette_control_x[i] && 
                ( x <= user_palette_control_x[i] + GUI_AREA_USER_PALETTE_COLOR_W ) &&
                y > user_palette_control_y[i] &&
                ( y <= user_palette_control_y[i] + GUI_AREA_USER_PALETTE_COLOR_H )
              )
            {
                selected_palette_option = i;
            }
        }
    }


    return;
}


// checks if user has selected a colour from the main palette and sets it to the highlighted
// user palette option
static void Input_Main_Palette( int button, int x, int y )
{
    if( button == 1 )
    {
        // get position of mouse in the main palette
        Get_Relative_Position( AREA_MAIN_PALETTE, &x, &y );

        // get index of the selected colour
        int row, col, index;
        
        // necessary as GUI_AREA_MAIN_PALETTE_COLOR area cpp evaluations assumed to be floats
        int pal_w = GUI_AREA_MAIN_PALETTE_COLOR_W;
        int pal_h = GUI_AREA_MAIN_PALETTE_COLOR_H;

        row = y / pal_h;
        col = x / pal_w;

        index = row * GUI_AREA_MAIN_PALETTE_COLUMNS + col;

        PAL_Set_User_Palette_Index( selected_palette_index, selected_palette_option , index );
    }

    return;
}


static void Input_Sprite_Grid( int button, int x, int y )
{
    if( button == 1 )
    {
        Get_Relative_Position( AREA_SPRITE_GRID, &x, &y );

        int row, col, index;

        row = y / GUI_SPRITE_H;
        col = x / GUI_SPRITE_W;

        printf( "ROW: %d\tCOL: %d\n", row, col );

        index = row * GUI_AREA_SPRITE_GRID_COLUMNS + col;

        index += sprite_grid_base;

        printf( "Attempting to select sprite def %d\n", index );

        // check sprite exists
        if( index >= SPR_Get_Number_Of_Sprites() )
        {
            UTI_Print_Debug( "Invalid sprite index choice (Grid)" );
            return;
        }

        sprite_grid_index = index;

        // change the selected palette to the one associated with the new sprite definition
        selected_palette_index = SPR_Get_Sprite_Palette_Index( sprite_grid_index );
        Convert_Int_To_String( palette_index_text, selected_palette_index, MAX_INT_STRING );
 
    }


    return;
}

//=====================================================================
//  PUBLIC FUNCTIONS
//=====================================================================


// initializes colours for GUI, returns 1 on success
int GUI_Init()    
{

    BLACK       = GRA_Create_Color( 0x00, 0x00, 0x00, 0xff );
    WHITE       = GRA_Create_Color( 0xff, 0xff, 0xff, 0xff );
    RED         = GRA_Create_Color( 0xff, 0x00, 0x00, 0xff );
    GREEN       = GRA_Create_Color( 0x00, 0xff, 0x00, 0xff );
    BLUE        = GRA_Create_Color( 0x00, 0x00, 0xff, 0xff );
    YELLOW      = GRA_Create_Color( 0xff, 0xff, 0x00, 0xff );
    CYAN        = GRA_Create_Color( 0x00, 0xff, 0xff, 0xff );
    LIGHT_GREY  = GRA_Create_Color( 0xb0, 0xb0, 0xb0, 0xff );
    DARK_GREY   = GRA_Create_Color( 0x60, 0x60, 0x60, 0xff );
    V_DARK_GREY = GRA_Create_Color( 0x20, 0x20, 0x20, 0xff );
    INVIS       = GRA_Create_Color( 0x00, 0x00, 0x00, 0x00 );
   
    area_color[AREA_SPRITE_EDIT]                        = LIGHT_GREY;
    area_color[AREA_SPRITE_GRID]                        = DARK_GREY;
    area_color[AREA_SPRITE_GRID_SCROLL]                 = LIGHT_GREY;
    area_color[AREA_MAIN_PALETTE]                       = LIGHT_GREY;
    area_color[AREA_USER_PALETTE]                       = INVIS;
    area_color[AREA_ANIM_EDIT]                          = YELLOW;
    area_color[AREA_ANIM_CONTROL]                       = BLUE;
    area_color[AREA_ANIM_PLAYER]                        = RED;

    //========================
    //  CREATE BUTTONS
    //========================

    //== PALETTE BUTTONS ==//

    GRA_Make_Button (   GUI_AREA_USER_PALETTE_X+15, 
                        GUI_AREA_USER_PALETTE_Y+0, 
                        96, 16, "PREV", 
                        BTN_Prev_User_Palette 
                    );

    GRA_Make_Button (   GUI_AREA_USER_PALETTE_X+149, 
                        GUI_AREA_USER_PALETTE_Y+0, 
                        96, 16, "NEXT", 
                        BTN_Next_User_Palette 
                    );

    Convert_Int_To_String( palette_index_text, selected_palette_index, MAX_INT_STRING );


    //== SCROLL BUTTONS ==//

    GRA_Make_Button (   GUI_AREA_SPRITE_GRID_SCROLL_X,
                        GUI_AREA_SPRITE_GRID_SCROLL_Y,
                        GUI_AREA_SPRITE_GRID_SCROLL_W,
                        GUI_AREA_SPRITE_GRID_SCROLL_W,
                        "^",
                        BTN_Scroll_Grid_Up
                    );

    GRA_Make_Button (   GUI_AREA_SPRITE_GRID_SCROLL_X,
                        GUI_AREA_SPRITE_GRID_SCROLL_Y + (GUI_AREA_SPRITE_GRID_SCROLL_H - GUI_AREA_SPRITE_GRID_SCROLL_W),
                        GUI_AREA_SPRITE_GRID_SCROLL_W,
                        GUI_AREA_SPRITE_GRID_SCROLL_W,
                        "v",
                        BTN_Scroll_Grid_Down
                    );

    //== GRID BUTTONS ==//

    GRA_Make_Button (   GUI_AREA_SPRITE_GRID_X,
                        GUI_AREA_SPRITE_GRID_Y + GUI_AREA_SPRITE_GRID_H + 8,
                        96, 24, "ADD SPR",
                        BTN_Add_Sprite
                    );

    GRA_Make_Button (   GUI_AREA_SPRITE_GRID_X + 128,
                        GUI_AREA_SPRITE_GRID_Y + GUI_AREA_SPRITE_GRID_H + 8,
                        96, 24, "REM SPR",
                        BTN_Remove_Sprite
                    );

    return 1;
};


// draws the gui to the screen
void GUI_Draw_Interface()
{
    Draw_Area_Outlines();
    Draw_Main_Palette();
    Draw_User_Palette_Controls();

    Draw_Sprite_Grid();

    Set_Palette_Index_Text();

    GRA_Draw_Buttons();

    return;
}


void GUI_Draw_Edit_Sprite()
{
    int i, x, y;

    for( i = 0; i < SPRITE_SIZE; i++ )
    {
        x = i % SPRITE_W;
        y = i / SPRITE_W;

        int color_index = SPR_Get_Pixel( sprite_grid_index, i );
        int main_palette_index = PAL_Get_User_Palette_Index( selected_palette_index, color_index );
        uint32_t color  = PAL_Get_Main_Palette_Color( main_palette_index );

        GRA_Draw_Filled_Rectangle(  GUI_AREA_SPRITE_EDIT_X+x*SPRITE_W, GUI_AREA_SPRITE_EDIT_Y+y*SPRITE_H,
                                    GUI_AREA_SPRITE_EDIT_PIXEL_W, GUI_AREA_SPRITE_EDIT_PIXEL_H,
                                    color
                                 );
    }

    return;
}


//==============================
//  MOUSE FUNCTIONS
//==============================

void GUI_Get_Mouse_Input()
{
    char no_output[] = "";           // used when the mouse is not in a main editor area
    char *mouse_area = NULL;

    mouse_area = no_output;

    // current mouse state
    int m_button = 0, mouse_x, mouse_y;
    int current_area;

    // get the mouse position, and state of buttons
    m_button = GRA_Get_Mouse_State( &mouse_x, &mouse_y );

    if( ( current_area =  Get_Area( mouse_x, mouse_y  )) >= 0 )
    {
        mouse_area = area_label[current_area];
    }

    GRA_Simple_Text( mouse_area, 32, 8, WHITE, 0, 0 );
    
    
    // call function that handles user input for the specified area
    // TODO - consider replacing this with an array of function pointers
    switch( current_area )
    {
        case AREA_SPRITE_EDIT:
            Input_Sprite_Edit( m_button, mouse_x, mouse_y );
            break;

        case AREA_USER_PALETTE:
            Input_User_Palette( m_button, mouse_x, mouse_y );
            break;

        case AREA_MAIN_PALETTE:
            Input_Main_Palette( m_button, mouse_x, mouse_y );
            break;
        
        case AREA_SPRITE_GRID:
            Input_Sprite_Grid ( m_button, mouse_x, mouse_y );
            break;

        default:
            break;

    }


    return;
}

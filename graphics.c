/*
    graphics.h
    a software graphics library using 2 banks of screen memory for double buffering.
    all drawing is done to raw memory which are then transferred to a SDL_Surface of the same
    res, which is then upscaled to another SDL_Surface to allow any window size to show any
    lower resolution - ie a 320x200 resolution in a 1024x640 window.

    made for use with my texture and palette definitions to give an old fashioned look
*/

#include <stdio.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "utility.h"
#include "graphics.h"


//===============================================================
//  CONSTANTS AND GLOBALS
//===============================================================

#define PALETTE_SIZE            256

//====================
//  DISPLAY
//====================

static SDL_Window           *scr_window         = NULL;         // display window
static SDL_Surface          *scr_surface        = NULL;         // window surface
static SDL_Surface          *scr_render         = NULL;         // render surface

static SDL_Rect             scr_rect           = { 0, 0, 0, 0 };

static uint32_t             *w_buffer           = NULL;         // buffer to write to
static uint32_t             *r_buffer           = NULL;         // buffer to display (read from)

static int                  scr_width           = 0;            // window dimensions
static int                  scr_height          = 0;            

static int                  res_width           = 0;            // render dimensions
static int                  res_height          = 0;

// these values are used to convert mouse position (scr_width x scr_height) to the relative render
// resolution, so that the mouse pos is always correct with respect to drawn items such as buttons
static float                pixel_ratio_x       = 0.0f;         // scr_width  / res_width (for mouse position)
static float                pixel_ratio_y       = 0.0f;         // scr_height / res_height

// double buffer to write to
static scr_buffer_type      scr_buffer          = { 0, 0, NULL, NULL };

static uint32_t             *palette            = NULL;

//===========================
//  TEXTURE VARIABLES
//===========================


uint32_t            *texture_buffer = NULL;

int                 TEX_SIZE = 0;               // size of each texture in texels
int                 NO_OF_TEXTURES = 0;


// font data is loaded here
static uint8_t             *font_buffer        = NULL;

//===============================================================
//  PRIVATE FUNCTIONS
//===============================================================

// All int returning functions return 1 on success or 0 on failure unless otherwise stated

// draws the w_buffer to the render surface
void Draw_Buffer()
{
    uint32_t *bufp;
    bufp = scr_render->pixels;              // get pointer to surface pixel data
    int x, y;

    for( y = 0; y < scr_render->h; y++ )
    {
        for( x = 0; x < scr_render->w; x++ )
        {
            *bufp = w_buffer[y * scr_render->w + x];
            bufp++;
        }
    }

    return;
}


// swaps the pointers to w_buffer and r_buffer
void Swap_Buffer()
{
    uint32_t *temp;
    temp = w_buffer;
    w_buffer = r_buffer;
    r_buffer = temp;

    return;
}



//===============================================================
//  FUNCTION BODIES
//===============================================================

// All int returning functions return 1 on success or 0 on failure unless otherwise stated

//=======================
//  INITIALIZATION
//=======================

// starts SDL Video and opens a window. Also creates a screen_buffer_type object for 
// writing to and initializes 2 SDL_Surfaces - one w_res x h_res which is stretched onto the
// second which is width x height, which is rendered to the window.
int GRA_Create_Display( char *title, int width, int height, int w_res, int h_res )
{
 
    // initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
    {
        UTI_Print_Error( "Unable to initialize SDL" );
        GRA_Print_SDL_Error();
        return 0;
    }

    // set screen globals
    scr_width = width;
    scr_height = height;

    res_width = w_res;
    res_height = h_res;

    // these values are used to convert mouse position (scr_width x scr_height) to the relative render
    // resolution, so that the mouse pos is always correct with respect to drawn items such as buttons
    pixel_ratio_x = (float)scr_width  / (float)res_width;
    pixel_ratio_y = (float)scr_height / (float)res_height;

    // create display window
    scr_window = SDL_CreateWindow(  title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    width, height, SDL_WINDOW_SHOWN );
    if( scr_window == NULL )
    {
        UTI_Print_Error( "Unable to create display window" );
        GRA_Print_SDL_Error();
        return 0;
    }

    // create display surface
    scr_surface = SDL_GetWindowSurface( scr_window );
    if( scr_surface == NULL )
    {
        UTI_Print_Error( "Unable to get window surface" );
        GRA_Print_SDL_Error();
        return 0;
    }


    // create render surface
    scr_render = SDL_CreateRGBSurface(  SDL_SWSURFACE, w_res, h_res, 32,
                                        R_MASK, G_MASK, B_MASK, A_MASK );
    if( scr_render == NULL )
    {
        UTI_Print_Error( "Unable to create render surface" );
        GRA_Print_SDL_Error();
        return 0;
    }

    // create rect for blitting render to screen
    scr_rect.x = 0;
    scr_rect.y = 0;
    scr_rect.w = width;
    scr_rect.h = height;

    // create double buffer
    scr_buffer.w = w_res;
    scr_buffer.h = h_res;
    scr_buffer.buffer1 = UTI_EC_Malloc( sizeof( uint32_t ) * w_res * h_res );
    scr_buffer.buffer2 = UTI_EC_Malloc( sizeof( uint32_t ) * w_res * h_res );

    // set write and read buffer pointers
    w_buffer = scr_buffer.buffer1;
    r_buffer = scr_buffer.buffer2;


    return 1;
}


// frees the SDL types, such as the window and surfaces and the 
void GRA_Close()
{
    // free SDL_ Structs
    SDL_DestroyWindow( scr_window );
    scr_window = NULL;

    SDL_FreeSurface( scr_render );
    scr_render = NULL;

    // free screen buffers
    UTI_EC_Free( scr_buffer.buffer1 );
    scr_buffer.buffer1 = NULL;

    UTI_EC_Free( scr_buffer.buffer2 );
    scr_buffer.buffer2 = NULL;

    // free font data
    UTI_EC_Free( font_buffer );
    
    // TODO - free texture data
    

    return;
}


//=======================
//  CONTROL
//=======================

// wrapper for SDL_Delay, stalls program for milli milliseconds
void GRA_Delay( int milli )
{
    SDL_Delay( milli );
    return;
}

// check if user quits, by clicking window 'x' or pressed escape
int GRA_Check_Quit()
{
    SDL_Event e;

    while( SDL_PollEvent( &e ) != 0 )
    {
        // check for user closing window
        if( e.type == SDL_QUIT )
        {
            return 0;
        }
        else if( e.type == SDL_KEYDOWN )
        {
            // check for user pressing escape
            switch( e.key.keysym.sym )
            {
                case SDLK_ESCAPE:
                    return 0;
                    break;

                default:
                    break;
            }
        }
    }

    return 1;
}

// wrapper
int GRA_GetTicks()
{
    return SDL_GetTicks();
}



//=======================
//  GRAPHICS
//=======================

// clears the current buffer for writing
void GRA_Clear_Screen()
{
    int i;
    for( i = 0; i < res_width * res_height; i++ )
    {
        w_buffer[i] = 0;        // black
    }

    SDL_FillRect( scr_surface, NULL, 0x00000000 );
    
    return;
}

// fill screen with color
void GRA_Fill_Screen( uint32_t color )
{
    SDL_FillRect( scr_surface, NULL, color );
    return;
}



// writes the current active buffer to the render_surface and displays it, then
// switches buffers for the next write
void GRA_Refresh_Window()
{
    Draw_Buffer();
    Swap_Buffer();

    SDL_BlitScaled( scr_render, NULL, scr_surface, &scr_rect );

    SDL_UpdateWindowSurface( scr_window );

    return;
}



// generates a 256 colour palette
int GRA_Generate_Palette()
{

    palette = UTI_EC_Malloc( sizeof( uint32_t ) * PALETTE_SIZE );

    int r, g, b, a = 0xff;
    for( r = 0; r < 8; r++ )
    {
        for( g = 0; g < 8; g++ )
        {
            for( b = 0; b < 4; b++ )
            {
                palette[r*32+g*4+b] = GRA_Create_Color( r*32, g*32, b*64, a );
            }
        }
    }

    return 1;
}



// loads a 256 colour palette from file
int GRA_Load_Palette( char *filename )
{
    // TODO

    return 1;
}


// free memory
void GRA_Free_Palette()
{

    return;
}



// returns corresponding uint32_t for r g b a colour
uint32_t GRA_Create_Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
{
    return( r*R_ADJUST + g*G_ADJUST + b*B_ADJUST + a*A_ADJUST );
}



// returns color at given palette index
uint32_t GRA_Get_Palette_Color( int index )
{
    if( index < 0 || index > PALETTE_SIZE )
    {
        return 0;
    }

    return palette[index];
}

// draws a pixel at given coordinates, uses color variable as an index for the palette table,
// not as a RGBA value to draw
void GRA_Set_Palette_Pixel( int x, int y, int color )
{ 
    GRA_Set_RGBA_Pixel( x, y, palette[color] );

    return;
}


// draws a pixel at the given coordinates, using color as RGBA value
void GRA_Set_RGBA_Pixel( int x, int y, uint32_t color )
{
    // check if pixel is within screen bounds
    if( x < 0 || x > res_width || y < 0 || y > res_height )
    {
        return;
    }

    w_buffer[y*res_width + x] = color;

    return;
}


// draws a vertical line of given color index to the buffer, uses color as RGBA value
void GRA_Draw_Vertical_Line( int x, int y1, int y2, uint32_t color )
{
    // check line is within screen bounds
    if( x < 0 || x >= res_width )
    {
        return;
    }

    // make sure y2 is larger (lower on screen)
    if( y1 > y2 )
    {
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }

    // make sure line is within vertical screen bounds
    if( y2 < 0 || y1 > res_height )
    {
        return;
    }

    // if line is on screen but extremes are off, set extremes within screen limits
    if( y1 < 0 )            y1 = 0;
    if( y2 > res_height-1)  y2 = res_height-1;

    // draw the line
    for( ; y1 <= y2; y1++ )
    {
        GRA_Set_RGBA_Pixel( x, y1, color );
    }

    return;
}


// draws a textured column of pixels to the screen
void GRA_Draw_Vertical_Texture_Line( float texel_normal, int col_x, int col_start, int col_end, int texture )
{ 
    
    // check column is horizontally on screen
    if( col_x < 0 || col_x > res_width )
    {
        return;
    }

    // check that col_start is less than col_end
    if( col_start > col_end )
    {
        int temp = col_start;
        col_start = col_end;
        col_end = temp;
    }

    // make sure line is vertically on screen
    if( col_start >= res_height || col_end < 0 )
    {
        return;
    }

    int top = 0, bottom = res_height - 1;
    int col_height = col_end - col_start;

    // restrict line to screen limits
    if( col_start < 0 )
    {
        top -= col_start;
        col_start = 0;
    }
    if( col_end > res_height-1 )
    {
        bottom = col_end;
        col_end = res_height - 1;
    }


    // row of texels to draw
    int tex_x = texel_normal * (float)TEX_SIZE;

    // number of texels used to represent 1 pixel on screen:
    float tex_per_pix       = (float)TEX_SIZE / (float) col_height;
    float tex_counter       = top * tex_per_pix;
    float tex_max           = bottom * tex_per_pix;
    int   tex_y             = 0;
    int   color_index       = 0;
    // beginning of the desired texture in the buffer
    int   texture_offset    = texture * TEX_SIZE * TEX_SIZE;       

    for( ; col_start <= col_end && tex_y < tex_max; col_start++ )
    {
        tex_y = tex_counter;
        if( tex_y >= TEX_SIZE )             tex_y = TEX_SIZE-1;
        color_index = texture_buffer[texture_offset + (tex_y * TEX_SIZE + tex_x)];
        GRA_Set_Palette_Pixel( col_x, col_start, color_index );
        tex_counter += tex_per_pix;
    }

    return; 
}        


// draws a horizontal line
void GRA_Draw_Horizontal_Line( int x1, int x2, int y, uint32_t color )
{
    // check line is on screen
    if( y < 0 || y >= res_height )
    {
        return;
    }

    // check x1 is lower number
    if( x1 > x2 )
    {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }

    // check line is within horizontal screen bounds
    if( x2 < 0 || x1 > res_width )
    {
        return;
    }

    // if line extremes are off-screen, adjust them
    if( x1 < 0 )                x1 = 0;
    if( x2 > res_width-1 )      x2 = res_width-1;

    // draw the line
    for( ; x1 <= x2; x1++ )
    {
        GRA_Set_RGBA_Pixel( x1, y, color );
    }

    return;
}


// draws a hollow rectangle to the screen
void GRA_Draw_Hollow_Rectangle( int x, int y, int w, int h, uint32_t color )
{
    GRA_Draw_Vertical_Line( x,      y,      y+h,    color );
    GRA_Draw_Vertical_Line( x+w,    y,      y+h,    color );

    GRA_Draw_Horizontal_Line( x,    x+w,    y,      color );
    GRA_Draw_Horizontal_Line( x,    x+w,    y+h,    color );

    return;
}


// draws a filled rectangle to the screen - TODO - cant use until palette implemented
void GRA_Draw_Filled_Rectangle( int x, int y, int w, int h, uint32_t color )
{
    int i;
    for( i = 0; i < w; i++ )
    {
        GRA_Draw_Vertical_Line( x+i, y, y+h, color );
    }

    return;
}

//==========================
//  TEXTURES
//==========================

#include <string.h>


// loads textures from file TODO - FINISH
int GRA_Load_Textures( char *filename )
{ 
    FILE *file = NULL;
    char check[5];

    // open file
    file = fopen( filename, "r" );
    if( file == NULL )
    {
        UTI_Print_Error( "Unable to open texture file" );
        return 0;
    }
    
    // check for texture signature ("TXTR") at the beginning of the file
    fread( check, 4, 1, file );
    check[4] = '\0';

    if( strcmp( check, "TXTR" ) != 0 )
    {
        UTI_Print_Error( "Not a valid texture file" );
        return 0;
    }

    // the next 8 bytes of the file are the size of a texture and the number of textures
    // respectively
    fread( &TEX_SIZE, sizeof( uint32_t ), 1, file );
    fread( &NO_OF_TEXTURES, sizeof( uint32_t ), 1, file );

    printf( "%d textures read, %dx%d\n", NO_OF_TEXTURES, TEX_SIZE, TEX_SIZE );

    texture_buffer = UTI_EC_Malloc( sizeof( uint32_t ) * TEX_SIZE * TEX_SIZE * NO_OF_TEXTURES );
    fread( texture_buffer, sizeof( uint32_t ) * TEX_SIZE * TEX_SIZE * NO_OF_TEXTURES, 1, file );

    fclose( file );

    return 1;     
}


// free texture memory
void GRA_Free_Textures()
{
    // TODO

    return;
}

//==========================
//  TEXT
//==========================

#include <ctype.h>

// size in bytes of a font file
#define FONT_FILE_SIZE          2048

#define CHAR_SET_SIZE           256     // no of chars in set
#define CHAR_SIZE               8       // size in bytes

#define CHAR_WIDTH              8       // in pixels
#define CHAR_HEIGHT             8

// loads my own custom made font files for use in these functions - TODO
int GRA_Load_Font( char *filename )
{ 
    // open file for reading
    FILE    *file = NULL;
    int     filesize = 0;

    file = fopen( filename, "r" );
    if( file == NULL )
    {
        UTI_Print_Error( "Unable to open font file\n" );
        return 0;
    }

    // check filesize is correct
    fseek( file, 0, SEEK_END );
    filesize = ftell( file );
    rewind( file );
    
    if( filesize != FONT_FILE_SIZE )
    {
        UTI_Print_Error( "Font file is incorrect size" );
        return 0;
    }

    // create temp buffer
    uint8_t *temp_buffer;
    temp_buffer = UTI_EC_Malloc( filesize );

    // load data to buffer
    fread( temp_buffer, filesize, 1, file );
    
    // create font buffer
    font_buffer = UTI_EC_Malloc( filesize * CHAR_SIZE );

    // unpack font data
    int i, j;
    uint8_t c;
    for( i = 0; i < filesize; i++ )
    {
        for( j = 0; j < CHAR_SIZE; j++ )
        {
            c = temp_buffer[i] & (0x80 >> j);
            font_buffer[i*CHAR_SIZE + j] = (c) ? 1 : 0;
        }
    }

    UTI_EC_Free( temp_buffer );
    fclose( file );
    
    return 1; 
}


// places a character at (x, y), draw_bg is a flag used to tell whether or not to draw the 
// background color
void GRA_Place_Char( int letter, int x, int y, int forecolor, int bgcolor, int draw_bg )
{
    int offset = letter * CHAR_WIDTH * CHAR_HEIGHT; // this can be changed for non-ascii sets
    int i, j, pixel;

    for( i = 0; i < CHAR_HEIGHT; i++ )
    {
        for( j = 0; j < CHAR_WIDTH; j++ )
        {
            pixel = font_buffer[i*CHAR_WIDTH+j+offset];
            // check if pixel should be drawn (always if draw_bg is set)
            if( draw_bg == 0 && pixel == 0 )
            {
                continue;
            }
            else
            {
                GRA_Set_RGBA_Pixel( x+j, y+i, (pixel) ? forecolor : bgcolor );
            }
        }
    }

    return;
}


// writes a string of text to the buffer, does not check for boundaries and does not wrap text
void GRA_Simple_Text( char *str, int x, int y, int forecolor, int bgcolor, int draw_bg )
{
    int i, len = strlen( str ) + 1;

    for( i = 0; i < len; i++ )
    {
        GRA_Place_Char( str[i], x, y, forecolor, bgcolor, draw_bg );
        x += CHAR_WIDTH;
    }

    return;
}


//==========================
//  CONTROL
//==========================

// wrapper for SDL_GetMouseState, returns 1 for LMB pressed, 2 for RMB pressed
uint32_t GRA_Get_Mouse_State( int *x, int *y )
{
    uint32_t state = SDL_GetMouseState( x, y );
    
    if( state & SDL_BUTTON( SDL_BUTTON_LEFT ) )
    {
        return 1;
    }
    else if( state & SDL_BUTTON( SDL_BUTTON_RIGHT ) )
    {
        return 2;
    }

    return 0;
}


//==========================
//  GUI
//==========================

#define     GUI_ACTIVE_COLOR            0xffffffff                // WHITE
#define     GUI_HOVER_COLOR             0xffb08000                // CYAN
#define     GUI_DISABLED_COLOR          0xff808080                // GREY
#define     BUTTON_DELAY                8                        // frame delay

// used to check if user is using buttons/gui
int         mouse_x = 0;
int         mouse_y = 0;
int         mouse_b = 0;

static scr_button_type         *buttons[MAX_BUTTONS];
static int                      button_p = 0;

static scr_switch_type         *switches[MAX_SWITCHES];
static int                      switch_p = 0;

// create a button for use on the screen, returns index of the button
int GRA_Make_Button( int x, int y, int w, int h, char *label, void (*function)(void) )
{
    // check there is room for another button on screen
    if( button_p >= MAX_BUTTONS )
    {
        UTI_Print_Error( "Cannot make button, MAX_BUTTONS limit reached" );
        return -1;
    }

    // create space for the button
    buttons[button_p] = UTI_EC_Malloc( sizeof( scr_button_type ) );
    
    // button position and dimensions
    buttons[button_p]->x         = x;
    buttons[button_p]->y         = y;
    buttons[button_p]->width     = w;
    buttons[button_p]->height    = h;

    // text that appears on the button
    buttons[button_p]->label     = label;

    // position the label
    int label_len = strlen( label );
    buttons[button_p]->label_x   = x + ( buttons[button_p]->width  - ( label_len * CHAR_WIDTH  ) ) / 2;      // TODO check text fits
    buttons[button_p]->label_y   = y + ( buttons[button_p]->height - ( CHAR_HEIGHT ) ) / 2 + 1;

    // the function executed when the button is pressed
    buttons[button_p]->function  = function;

    // button is active and visible by default
    buttons[button_p]->active    = 1;
    buttons[button_p]->visible   = 1;

    buttons[button_p]->delay     = 0;

    // set default colors (TODO add functions to change these)
    buttons[button_p]->active_color          = GUI_ACTIVE_COLOR;
    buttons[button_p]->hover_color           = GUI_HOVER_COLOR;
    buttons[button_p]->disabled_color        = GUI_DISABLED_COLOR;
    buttons[button_p]->current_color         = GUI_ACTIVE_COLOR;

    return button_p++;

}


// create a binary switch button
int GRA_Make_Switch( int x, int y, char c, int *value )
{
    //check there is room for a switch
    if( switch_p >= MAX_SWITCHES )
    {
        UTI_Print_Error( "Unable to create switch, MAX_SWITCHES limit reached" );
        return -1;
    }

    // make space for the switch
    switches[switch_p] = UTI_EC_Malloc( sizeof( scr_switch_type ) );

    // set position and dimensions
    switches[switch_p]->x               = x;
    switches[switch_p]->y               = y;
    switches[switch_p]->width           = 10;
    switches[switch_p]->height          = 10;

    // set appearance of true state
    switches[switch_p]->true_char       = c;
    switches[switch_p]->char_x          = x+2;
    switches[switch_p]->char_y          = y+2;

    switches[switch_p]->active          = 1;
    switches[switch_p]->visible         = 1;

    switches[switch_p]->state           = value;

    switches[switch_p]->active_color    = GUI_ACTIVE_COLOR;
    switches[switch_p]->hover_color     = GUI_HOVER_COLOR;
    switches[switch_p]->disabled_color  = GUI_DISABLED_COLOR;
    switches[switch_p]->current_color   = GUI_ACTIVE_COLOR;

    return switch_p++;

}


// draw the button to the screen
void GRA_Draw_Buttons()
{
    int i;

    for( i = 0; i < button_p; i++ )
    {
        if( buttons[i]->visible == 0 )
        {
            continue;
        }
        else
        {
            // draw box
            GRA_Draw_Hollow_Rectangle(  buttons[i]->x, buttons[i]->y, buttons[i]->width, buttons[i]->height,
                                        buttons[i]->current_color );

            // draw text
            GRA_Simple_Text( buttons[i]->label, buttons[i]->label_x, buttons[i]->label_y, 
                             buttons[i]->current_color, 0, 0 );
        }
    }

    return;
}


// draw a switch to the screen
void GRA_Draw_Switches()
{
    int i;

    for( i = 0; i < switch_p; i++ )
    {
        if( buttons[i]->visible == 0 )
        {
            continue;
        }
        else
        {
            // draw the switch
            GRA_Draw_Hollow_Rectangle(  switches[i]->x, switches[i]->y, switches[i]->width, 
                                        switches[i]->height, switches[i]->current_color );

            if( *(switches[i]->state) )
            {
                GRA_Place_Char( switches[i]->true_char, switches[i]->char_x, switches[i]->char_y,
                                switches[i]->current_color, 0, 0 );
            }
        }
    }

    return;
}


// activate a disabled button
void GRA_Enable_Button( int button_index )
{
    buttons[button_index]->active = 1;
    return;
}


// disable an active button
void GRA_Disable_Button( int button_index )
{
    buttons[button_index]->active = 0;
    return;
}


// activate a disabled switch
void GRA_Enable_Switch( int switch_index )
{
    return;       // TODO
}


// disable an active switch
void GRA_Disable_Switch( int switch_index )
{
    return;       // TODO
}


// use the button (runs previously passed function)
void GRA_Press_Button( int index )
{
    if( index >= button_p || index < 0 || index >= MAX_BUTTONS )
    {
        UTI_Print_Error( "Button doesn't exist!" );
        printf( "Button index = %d\n", index );
        return;
    }

    if( buttons[index]->active == 0 )
    {
        return;
    }

    buttons[index]->delay = BUTTON_DELAY;
    buttons[index]->active = 0;

    buttons[index]->function();
    return;
}



// flip switch
void GRA_Press_Switch( int index )
{
    if( index >= switch_p || index < 0 || index >= MAX_SWITCHES )
    {
        UTI_Print_Error( "Switch doesn't exist!" );
        printf( "Switch index = %d\n", index );
        return;
    }

    if( *(switches[index]->state) == 0 )
    {
        *(switches[index]->state) = 1;
    }
    else
    {
        *(switches[index]->state) = 0;
    }

    return;
}


// go through all buttons to see if the user has clicked on one
void GRA_Check_User_Input()
{
    int i, mx, my;
    
    // get mouse state
    mouse_b = SDL_GetMouseState( &mx, &my );
    mouse_x = floor( mx / pixel_ratio_x );
    mouse_y = floor( my / pixel_ratio_y );

    //printf( "Mouse abs (%d, %d) - Mouse rel (%d, %d)\n", mx, my, mouse_x, mouse_y );
    
    // check if mouse is over any button
    for( i = 0; i < button_p; i++ )
    {
        // prepare to reset pressed buttons
        if( buttons[i]->active == 0 )
        {
            buttons[i]->delay--;
            if( buttons[i]->delay < 1 )
            {
                buttons[i]->delay = 0;
                buttons[i]->active = 1;
                buttons[i]->current_color = buttons[i]->disabled_color;
            }
        }

        if( mouse_x > buttons[i]->x && mouse_x < (buttons[i]->x + buttons[i]->width) &&
            mouse_y > buttons[i]->y && mouse_y < (buttons[i]->y + buttons[i]->height) && buttons[i]->active )
        {
            // show button highlight
            buttons[i]->current_color = buttons[i]->hover_color;
    
            // activate code if mouse pressed
            if( mouse_b & SDL_BUTTON(SDL_BUTTON_LEFT) )
            {
                GRA_Press_Button( i );
            }
        }
        else
        {
            // show button is not highlighted
            if( buttons[i]->active )
            {
                buttons[i]->current_color = buttons[i]->active_color;
            }
            else
            {
                buttons[i]->current_color = buttons[i]->disabled_color;
            }

        }
    }

    // check for switches
    for( i = 0; i < switch_p; i++ )
    {
        if( mouse_x > switches[i]->x && mouse_x < (switches[i]->x + switches[i]->width)  &&
            mouse_y > switches[i]->y && mouse_y < (switches[i]->y + switches[i]->height) && 
            switches[i]->active )
        {
            // show switch highlight
            switches[i]->current_color = switches[i]->hover_color;

            // change state if active
            if( mouse_b & SDL_BUTTON(SDL_BUTTON_LEFT) )
            {
                GRA_Press_Switch( i );
            }
        }
        else
        {
            // make sure button is not highlighted
            switches[i]->current_color = switches[i]->active_color;
        }
    }
   
    return;
}


// free memory
void GRA_Free_Buttons()
{
    int i;
    for( i = 0; i < button_p; i++ )
    {
        free( buttons[i] );
    }
    
    return;
}

void GRA_Free_Switches()
{
    int i;
    for( i = 0; i < switch_p; i++ )
    {
        free( switches[i] );
    }

    return;
}

//===========================
//  TESTING
//===========================



#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "defs.h"
#include "utility.h"
#include "anim.h"           // for 'anim_type' definition
#include "sprite.h"
#include "palette.h"
#include "file.h"

//====================================================================
//  CONSTANTS
//====================================================================




//====================================================================
//  GLOBALS
//====================================================================


int             argc = 0;
char**          argv = NULL;

char*           filename;


//====================================================================
//  PRIVATE PROTOTYPES
//====================================================================


void        usage()
{
    printf( "Usage:%s [FILENAME] [OPTIONS]\n", argv[0] );

    // TODO list options
    return;
}


//====================================================================
//  FUNCTION BODIES
//====================================================================


// check user args
void        FIL_Parse_Arguments( int m_argc, char *m_argv[] )
{
    argc = m_argc;
    argv = m_argv;

    if( argc < 2 )
    {
        usage();
        UTI_Quiet_Exit( 1 );
    }

    // check filename is the first argument passed
    if( argv[1][0] == '-' )
    {
        usage();
        UTI_Quiet_Exit( 1 );
    }

    filename = argv[1];

    printf( "Working file is '%s'\n", filename );

    return;
}

// attempt to open a file, name given through FIL_Parse_Arguments, return 1 on success
int         FIL_Open_File()
{
    FILE *file = NULL;
    char file_check[5];

    file = fopen( filename, "rb" );
    if( file == NULL )
    {
        // need to create new data
        UTI_Print_Error( "Unable to open file" );
        return 0;
    }

    // TODO - read entire file to buffer, then parse

    fread( file_check, 4, 1, file );
    file_check[4] = '\0';

    if( strcmp( file_check, SIGNATURE ) != 0 )
    {
        UTI_Print_Error( "Cannot open file, signature check failed" );
        return 0;
    }

    // TODO
    // read all data from the file

    rewind( file );

    file_header_type        *header = NULL;
    header = UTI_EC_Malloc( sizeof( file_header_type ) );

    fread( header, sizeof( file_header_type ), 1, file );

    printf( "File Specs: No of Sprites              = %d\n", header->no_of_sprites );
    printf( "            No of Animations           = %d\n", header->no_of_animations );
    printf( "            No of Palettes             = %d\n", header->no_of_palettes );
    printf( "            Animation Offset           = %d\n", header->animation_offset );
    printf( "            Palette Offset             = %d\n", header->palette_offset );

    int i;      // generic counter

    //======= EXTRACT SPRITE DEFINITION DATA =======//
    sprite_type             *sprite_buffer = NULL;            // temp buffer
    
    for( i = 0; i < header->no_of_sprites; i++ )
    {
        sprite_buffer = UTI_EC_Malloc( sizeof( sprite_type ) );
        fread( sprite_buffer, sizeof( sprite_type ), 1, file );
        SPR_Load_Sprite( sprite_buffer );
    }
    // sprite_buffer malloc'd memory will be freed by SPR code

    printf( "Header offset = %d\tFtell gives: %ld\n", header->animation_offset, ftell( file ) );


    //======= EXTRACT ANIMATION DATA =======//
    int32_t                 *frame_buffer = NULL, no_of_frames = 0, frame_wait = 0;
    frame_buffer = UTI_EC_Malloc( sizeof( int32_t ) * MAX_ANIMATION_FRAMES );     // most ever needed

    for( i = 0; i < header->no_of_animations; i++ )
    {
        fread( &no_of_frames, sizeof( int32_t ), 1, file );
        fread( &frame_wait, sizeof( int32_t ), 1, file );
        fread( frame_buffer, sizeof( int32_t ), no_of_frames, file );
        ANI_Load_Animation( frame_buffer, no_of_frames, frame_wait );
    }

    UTI_EC_Free( frame_buffer );

    //======= EXTRACT PALETTE DATA =======//

    user_palette_type           *palette = NULL;

    for( i = 0; i < header->no_of_palettes; i++ )
    {
        palette = UTI_EC_Malloc( sizeof( user_palette_type ) );
        fread( palette, sizeof( user_palette_type ), 1, file );
        if( palette != NULL )
        {
            PAL_Load_Palette( palette );
        }
    }

    //======= FREE MEMORY =======//

    UTI_EC_Free( header );

    fclose( file );

    return 1;
}

// write data to file, filename given by user cmd line args. return 1 on success
int         FIL_Write_File()
{
    FILE *file = NULL;
    int error = 0;

    file = fopen( filename, "wb" );
    if( file == NULL )
    {
        UTI_Print_Error( "Unable to create file" );
        return 0;
    }

    // generate header
    file_header_type    *header = NULL;
    header = UTI_EC_Malloc( sizeof( file_header_type ) );

    strncpy( header->signature, SIGNATURE, 4 );
    header->no_of_sprites           = SPR_Get_Number_Of_Sprites();
    header->no_of_animations        = ANI_Get_Number_Of_Animations();
    header->no_of_palettes          = PAL_Get_Number_Of_Palettes();

    // write header
    fwrite( header, sizeof( file_header_type ), 1, file );

    // get sprite data
    int i;
    uint8_t *data = NULL;
    for( i = 0; i < header->no_of_sprites; i++ )
    {
        data = SPR_Get_Sprite( i );
        if( data != NULL )
        {
            fwrite( data, sizeof( sprite_type ), 1, file );
        }
        else
        {
            printf( "Null Sprite data pointer: %d\n", i );
            error = 1;
            break;
        }
    }

    printf( "Written data for %d sprite definitions\n", i );

    // get animation data


    header->animation_offset = ftell( file );
    printf( "Header -> anim_offset = %d\n", header->animation_offset );

    anim_type       *anim = NULL;
    for( i = 0; i < header->no_of_animations; i++ )
    {
        anim = ANI_Get_Animation( i );
        if( anim != NULL )
        {
            fwrite( &(anim->no_of_frames), 4, 1, file );
            fwrite( &(anim->frame_wait), 4, 1, file );
            fwrite( anim->frame_list, 4, anim->no_of_frames, file );
        }
        else
        {
            printf( "Null animation data pointer: Index %d\n", i );
            error = 1;
            break;
        }
    }
    
    printf( "Written data for %d animations\n", i );

    // write palette data
    header->palette_offset = ftell( file );

    user_palette_type *palette = NULL;
    for( i = 0; i < header->no_of_palettes; i++ )
    {
        palette = PAL_Get_Palette( i );
        if( palette != NULL )
        {
            fwrite( palette, PAL_USER_SIZE, 1, file );
        }
        else
        {
            printf( "Null palette data pointer: Index %d\n", i );
            error = 1;
            break;
        }
    }

    printf( "Written data for %d palettes\n", i );

    //re-write header, now that it has the palette and animation offsets
    rewind( file );
    fwrite( header, sizeof( header ), 1, file );

    UTI_EC_Free( header );
    fclose( file );

    if( error == 1 )
    {
        printf( "WARNING: Possible errors writing file '%s'\n", filename );
    }
    else
    {
        printf( "Successfully written data to %s\n", filename );
    }

    return 1;
}



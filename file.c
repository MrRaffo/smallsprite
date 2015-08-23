#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "defs.h"
#include "utility.h"
#include "anim.h"           // for 'anim_type' definition
#include "sprite.h"
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

    printf( "TEST 1\n" );
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

    file = fopen( filename, "r" );
    if( file == NULL )
    {
        // need to create new data
        UTI_Print_Error( "Unable to open file" );
        return 0;
    }

    fread( file_check, 4, 1, file );
    file_check[4] = '\0';

    if( strcmp( file_check, SIGNATURE ) != 0 )
    {
        UTI_Print_Error( "Cannot open file, signature check failed" );
        return 0;
    }

    // TODO
    // read all data from the file

    fclose( file );

    return 1;
}

// write data to file, filename given by user cmd line args. return 1 on success
int         FIL_Write_File()
{
    FILE *file = NULL;
    int error = 0;

    file = fopen( filename, "w" );
    if( file == NULL )
    {
        UTI_Print_Error( "Unable to create file" );
        return 0;
    }

    // generate header
    file_header_type    *header = NULL;
    header = UTI_EC_Malloc( sizeof( file_header_type ) );

    strncpy( header->signature, SIGNATURE, 4 );
    header->no_of_sprites = SPR_Get_Number_Of_Sprites();
    header->no_of_animations = ANI_Get_Number_Of_Animations();
    header->animation_offset = ( header->no_of_sprites * SPRITE_SIZE ) + sizeof( file_header_type );

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
            fwrite( data, SPRITE_SIZE, 1, file );
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
            printf( "Null animation data pointer: %d\n", i );
            error = 1;
            break;
        }
    }
    
    printf( "Written data for %d animations\n", i );

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



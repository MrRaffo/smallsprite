/*
    utility.c
    useful functions
*/

#include <stdio.h>
#include <stdlib.h>

#include "utility.h"

// quits without error/reason message
void UTI_Quiet_Exit( int signal )
{
    exit( signal );
}

// prints error message then closes program
void UTI_Fatal_Error( char *msg )
{
    printf( "FATAL ERROR: %s\n", msg );
    exit( 1 );
}


// error checked malloc call
void *UTI_EC_Malloc( size_t size )
{
    void *ptr = malloc( size );
    if( ptr == NULL )
    {
        UTI_Fatal_Error( "<UTI_EC_Malloc>: Unable to allocate memory" );
    }

    return ptr;
}


// error checked free
void UTI_EC_Free( void *ptr )
{
    if( ptr == NULL )
    {
        // if pointer is null, no action needed
        return;
    }

    free( ptr );

    return;
}

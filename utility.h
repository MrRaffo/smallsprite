/*
    utility.h
*/

#ifndef __utility_h__
#define __utility_h__

#define DEBUG       1

// check c version for __func__ or __FUNCTION__ use
#if __STDC_VERSION__ < 199901L
#   if __GNUC__ >= 2
#       define __func__ __FUNCTION__
#   else
#       define __func__ "<unknown>"
#   endif
#endif // __STDC_VERSION__

// prints error message with function name
#define         UTI_Print_Error( A )            printf( "[ERROR] <%s()>: %s\n", __func__, A )

// prints debug message with function name
#ifdef DEBUG
#   define          UTI_Print_Debug( A )         printf( "[DEBUG] <%s()>: %s\n", __func__, A )
#else       
#   define          UTI_Print_Debug( A )         NULL       // do nothing
#endif  // DEBUG

// calls 'exit( signal )' without message
void UTI_Quiet_Exit( int signal );

// prints error message then quits
void UTI_Fatal_Error( char *msg );

// error checked malloc call
void *UTI_EC_Malloc( size_t size );


// free malloc'd memory, ignores null pointers
void UTI_EC_Free( void *ptr );

#endif // __utility_h__

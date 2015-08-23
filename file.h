//===================================================================
//
//  file.h
//
//  handles file loading and saving
//
//  Paul Rafferty 2015
//
//===================================================================

#ifndef __file_h__
#define __file_h__


//===================================================================
//  DEFINES
//===================================================================

#define     SIGNATURE               "SPRT"

//===================================================================
//  TYPES
//===================================================================

struct      file_header_s { char            signature[4];
                            uint32_t        no_of_sprites;
                            uint32_t        no_of_animations;
                            uint32_t        animation_offset;
                          };

typedef     struct file_header_s file_header_type;


//===================================================================
//  PROTOTYPES
//===================================================================

// check user args
void        FIL_Parse_Arguments( int argc, char *argv[] );

// attempt to open a file, name given through FIL_Parse_Arguments, return 1 on success
int         FIL_Open_File();

// write data to file, filename given by user cmd line args. return 1 on success
int         FIL_Write_File();



#endif // __file_h__

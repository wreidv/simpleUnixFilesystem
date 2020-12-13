#include <string.h>
#ifndef UNIX_DATASTRUCTURE
#define UNIX_DATASTRUCTURE

/* TerpConnect ID: wreidv                                                     
   UID: 115943735                                                              
   Discussion section: 0105                            
                   
   I pledge on my honor that I have not given or recieved any unauthorized
   assistance on this examination or assignment.                              
*/

typedef struct file {
  char *name;
  struct file *next;/*next file in directory*/
}File;

typedef struct directory {
  char *name;
  File *head;/*start of files in directory*/
  struct directory *sub;/*any subdirectories */
  struct directory *next;/*next directory in current directory*/
  struct directory *prev;/*parent directory*/
}Directory;

typedef struct unix {
  Directory *root;/*root directory*/
  Directory *curr;/*current directory*/
}Unix;

#endif

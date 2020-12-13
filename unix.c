#include "unix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* TerpConnect ID: wreidv                                                      
   UID: 115943735                                                
   Discussion section: 0105                                                    
                                                                         
   I pledge on my honor that I have not given or recieved any unauthorized     
   assistance on this examination or assignment.                              
*/

/*This program represents a unix system that can make directories and files\
it can add files and directories, change directories, print the working\
directory, list the files and directories in directories, remove files\
and directories, and remove the whole filesystem */

/*This function initializes the filesystem*/
void mkfs(Unix *filesystem) {
  Directory *temp;
  
  if(filesystem != NULL) {
    temp = malloc(sizeof(Directory));
    temp->name = malloc(sizeof(char));

    if(temp != NULL) {/*initialize fields*/
      strcpy(temp->name, "");
      temp->head = NULL;
      temp->next = NULL;
      temp->prev = NULL;
      temp->sub = NULL;
      filesystem->root = temp;
      filesystem->curr = temp;
    }
  }
}

/*This function makes a new file if the name is not the name of another file\
  or directory in the current file. Returns 1 if everything is valid and 0 if\
  not*/
int touch(Unix *filesystem, const char arg[]) {
  int result = 0;
  File *currF = NULL, *prevF = NULL, *alpha, *new_file = NULL;
  Directory *currD;

  /*check for error cases*/
  if(filesystem != NULL && strlen(arg) > 0 && !(strstr(arg, "/") != NULL && \
					       strlen(arg) > 1)) {
    if(strcmp(".", arg) != 0 && strcmp("..", arg) && strcmp("/", arg) != 0) {
      currD = filesystem->curr;
      if(currD != NULL) /* check if current directory is NULL */
	currF = currD->head;

	while(currF != NULL && strcmp(arg, currF->name) != 0) {
	  prevF = currF;
	  currF = currF->next;
	}/*check if arg is a file */

	if(currD == filesystem->root) {/*if curr is home directory*/
	  while(currD != NULL && strcmp(arg, currD->name) != 0)
	    currD = currD->next; /*check if arg is a directory */
	} else {/*if curr is a sub directory*/
	  currD = currD->sub;
	  while(currD != NULL && strcmp(arg, currD->name) != 0) 
	    currD = currD->next;
	}
      
	if(currF == NULL && currD == NULL) {
	  /*allocate space for new file*/
	  new_file = malloc(sizeof(File));
	  new_file->name = malloc(sizeof(char) * strlen(arg) + 1);
	  
	  if(new_file != NULL) 
	    strcpy(new_file->name, arg); /*put arg into new_file's name*/
	  
	  if(prevF == NULL) {
	    /*if no files or directories in current directory */
	    new_file->next = NULL;
	    filesystem->curr->head = new_file;
	  } else {
	    /*if file not found in current directory and arg is valid */
	    alpha = filesystem->curr->head;
	    prevF = NULL;
	    while(alpha != NULL && strcmp(alpha->name, new_file->name) < 0) {
	      prevF = alpha;
	      alpha = alpha->next; /*alphabetize the files*/
	    }

	    if(prevF == NULL) 
	      filesystem->curr->head = new_file;/*change first file*/
	    else
	      prevF->next = new_file;/*insert file in list*/
	  
	    new_file->next = alpha;
	  }
	}
      
    }
    result = 1;
  }
  return result;
}

/*This function makes a new directory if the files and directories in the\
  current directory do not have the same name. Returns 1 if everyting is valid\
  0 otherwise*/
int mkdir(Unix *filesystem, const char arg[]) {
  int result = 0;
  File *currF, *prevF = NULL;
  Directory *currD, *prevD = NULL, *alpha, *new_dir;

  /*check for error cases*/
  if(filesystem != NULL && strlen(arg) > 0 && !(strstr(arg, "/") != NULL &&\
					       strlen(arg) > 1)) {
    if(strcmp(".", arg) != 0 && strcmp("..", arg) && strcmp("/", arg) != 0) {
      currD = filesystem->curr;
      if(currD != NULL)
	currF = currD->head;

	while(currF != NULL && strcmp(arg, currF->name) != 0) {
	  prevF = currF;
	  currF = currF->next;
	}/*check if arg is a file */

	if(currD == filesystem->root) {/*if curr is home directory*/
	  while(currD != NULL && strcmp(arg, currD->name) != 0) {
	    prevD = currD;
	    currD = currD->next;
	  }/*check if arg is a directory */
	} else {/*if curr is a sub directory*/
	  currD = currD->sub;
	  while(currD != NULL && strcmp(arg, currD->name) != 0) {
	    prevD = currD;
	    currD = currD->next;
	  }
	}

	if(currF == NULL && currD == NULL) {
	  /*allocate space for directory*/
	  new_dir = malloc(sizeof(Directory));
	  new_dir->name = malloc(sizeof(char) * strlen(arg) + 1);

	  if(new_dir != NULL) {
	    strcpy(new_dir->name, arg);
	  
	  if(prevF == NULL && prevD == NULL) {
	    if(filesystem->curr == filesystem->root) {
	      /*no files or directories in home directory*/
	      filesystem->root->next = new_dir;
	      filesystem->curr->next = new_dir;
	    } else/*make new directory in a subdirectory*/
	      filesystem->curr->sub = new_dir;
	    
	    new_dir->next = NULL;
	  } else {
	    /*if file not found in current directory and arg is valid */
	    alpha = filesystem->curr;
	    prevD = NULL;

	    if(alpha == filesystem->root) { /*in home directory*/
	      while(alpha != NULL && strcmp(alpha->name, new_dir->name) < 0) {
		prevD = alpha;
		alpha = alpha->next; /*alphabetize the directories*/
	      }

	      if(prevD == NULL) {/*nothing in current directory*/
		filesystem->curr->next = new_dir;
		filesystem->root->next = new_dir;
	      } else
		prevD->next = new_dir;
	    
	    } else {/*we are in a subdirectory*/
	      alpha = alpha->sub;
	      while(alpha != NULL && strcmp(alpha->name, new_dir->name) < 0) {
		prevD = alpha;
		alpha = alpha->next; /*alphabetize the directories*/
	      }

	      if(prevD == NULL)/*nothing in subdirectory of curr*/
		filesystem->curr->sub = new_dir;
	      else
		prevD->next = new_dir;
	    }

	    new_dir->next = alpha;
	  }
	  new_dir->sub = NULL;
	  new_dir->prev = filesystem->curr;
	  new_dir->head = NULL;
	  }
	}
	result = 1;
      
    }
  }
  return result;
}

/*This function changes the current directory to the specified directory\
  .. goes back one directory and / goes to home directory. Returns 1 if\
  everything is valid and 0 otherwise.*/
int cd(Unix *filesystem, const char arg[]) {
  int result = 0;
  Directory *curr;
  
  if(filesystem != NULL && !(arg[0] == '/' && strlen(arg) > 1)) {
    if(strcmp(arg, ".") != 0 || strlen(arg) > 0) {/*check arg*/
      if(strcmp(arg, "..") == 0) /*go to previous directory*/
	filesystem->curr = filesystem->curr->prev;
      else if(strcmp(arg, "/") == 0) /*go to home directory*/
	filesystem->curr = filesystem->root;
      else {
	curr = filesystem->curr;

	if(curr != filesystem->root)/*if in a subdirectory*/
	  curr = curr->sub;
	
	while(curr != NULL && strcmp(curr->name, arg) != 0) {
	  curr = curr->next;
	}/*search for directory name*/

	if(curr != NULL) { /*arg was found*/
	  curr->prev = filesystem->curr;/*change previous directory*/
	  filesystem->curr = curr;/*change current directory*/
	} else
	  return 0;/*arg was not found*/
      }
    }

    result = 1;
  }

  return result;
}

/*This function prints out the contents of the specified directory. Files are\
  printed as their name and directories have a / at the end of its name. \
  Returns 1 if everything is valid, 0 otherwise*/
int ls(Unix *filesystem, const char arg[]) {
  int result = 0;
  File *currF;
  Directory *currD;

  if(filesystem != NULL && !(arg[0] == '/' && strlen(arg) > 1)) {
    currD = filesystem->curr;

    if(currD->next == NULL && currD->head == NULL && currD->sub == NULL &&\
       currD->prev == NULL)
      /*if currD is empty directory*/
      currD = NULL;
    
    if(currD != NULL) {
      currF = currD->head;

      if(strcmp(arg, ".") == 0 || strcmp(arg, "") == 0) {
	if(currD == filesystem->root)/*print current directory*/
	  currD = currD->next;
	else
	  currD = currD->sub;
      } else if(strcmp(arg, "..") == 0) {/*print one directory back*/
	if(currD != filesystem->root && currD->prev != filesystem->root)
	  currD = currD->prev->sub;/*in sub directory*/
	else
	  currD = currD->prev->next;

	currF = currD->prev->head;/*put currF to start of files*/
      } else if(strcmp(arg, "/") == 0) {/*print to home directory*/
	currD = filesystem->root->next;
	currF = filesystem->root->head;
      } else {/*search for specified directory*/
	if(strcmp(currD->name, filesystem->root->name) != 0)
	  currD = currD->sub;
	
	while(currD != NULL && strcmp(currD->name, arg) != 0)
	  currD = currD->next;/*search for directory*/

	if(currD == NULL)/*if directory name not found*/
	  while(currF != NULL && strcmp(currF->name, arg) != 0)
	    currF = currF->next;/*search for file*/

	if(currF != NULL && currD == NULL)/*if file found & dir not found*/
	  printf("%s\n", currF->name);
	else if(currF == NULL && currD == NULL)
	  return 0; /*if arg not found in files or directories*/
	else if(currD != filesystem->root) {/*change to specified directory*/
	  currF = currD->head;
	  currD = currD->sub;
	}
      }

      /*print the files and directories*/
      while(currD != NULL || currF != NULL) {
	if(currD != NULL && currF != NULL) {
	  if(strcmp(currD->name, currF->name) < 0) { /*compare names*/
	    printf("%s/\n", currD->name);
	    currD = currD->next;
	  } else {
	    printf("%s\n", currF->name);
	    currF = currF->next;
	  }
	} else if(currD != NULL) { /*if no more files to print*/
	  printf("%s/\n", currD->name);
	  currD = currD->next;
	} else { /*if no more directories to print*/
	  printf("%s\n", currF->name);
	  currF = currF->next;
	}
      }
    }
    result = 1; 
  }

  return result;
}

/*This function prints out the current directory and it's predecssors. If\
 current is the home directory then it prints out / */
void pwd(Unix *filesystem) {
  Directory *temp, *list;
  
  if(filesystem != NULL) {
    temp = filesystem->curr;
    list = filesystem->root;
    printf("/");/*start with root directory*/
    while(strcmp(filesystem->curr->name, list->name) != 0) {
      while(strcmp(temp->prev->name, list->name) != 0)
	temp = temp->prev; /*get the parent directory and reverse them*/
    
    printf("%s", temp->name);
    if(strcmp(temp->name,filesystem->curr->name) != 0)
      printf("/");
    
    list = temp;
    temp = filesystem->curr;
    }
    printf("\n");
  }
}

/*removes the whole filesystem and all the directories and files in it. \
it frees the memory of those directories and files also*/
void rmfs(Unix *filesystem) {
  File *tempf, *nextf;
  Directory *tempd, *nextd, *prevd;

  if(filesystem != NULL) {
    tempd = filesystem->root;

    while(tempd != NULL) {
      
      while(tempd->sub != NULL) {/*go to furthest sub directory*/
	tempd = tempd->sub;
	nextd = tempd->next;
      }


      tempf = tempd->head;

      while(tempf != NULL) {/*go thru files and free them*/
	nextf = tempf->next;
	free(tempf->name);
	free(tempf);
	tempf = nextf;
	if(nextf != NULL)
	  nextf = nextf->next;
      }
      tempd->head = NULL;/*set start of files to null*/

      nextd = tempd->next;
      prevd = tempd->prev;
      
      if(strcmp(tempd->name, "") == 0) {/*set beginning to NULL*/
	filesystem->curr = NULL;
	filesystem->root = NULL;
	while(nextd != NULL) {
	  nextd->prev = NULL;/*make all beginning directoies have no parent*/
	  nextd = nextd->next;
	}
	nextd = tempd->next;
      }
	
      free(tempd->name);
      free(tempd);
      
      if(nextd != NULL)
	tempd = nextd;/*switch the directory to be freed*/
      else {
	tempd = prevd;
	if(tempd != NULL)
	  tempd->sub = NULL;
      }
    }
  }
}

/*this function will remove a specific file or directory. if arg is a \
  directory then it will remove all the contents of that directory. returns\
1 on success and 0 on failure */
int rm(Unix *filesystem, const char arg[]) {
  int result = 0;
  Directory *tempd, *prevd = NULL, *par, *travd, *nextd;
  File *tempf, *prevf = NULL, *nextf;

  if(filesystem != NULL && strlen(arg) > 0 && strstr(arg, "/") == NULL\
     && strcmp(arg,".") != 0 && strcmp(arg,"..") != 0) {
    
    /*check if arg is a file in current directory*/
    tempf = filesystem->curr->head;
    while(tempf != NULL && strcmp(tempf->name, arg) != 0) {
      prevf = tempf;
      tempf = tempf->next;
    }

    if(tempf != NULL) {/*arg is a file*/
      if(prevf == NULL) /*arg is first file in list*/
	filesystem->curr->head = tempf->next;
      else 
	prevf->next = tempf->next;

      free(tempf->name);
      free(tempf);
      tempf = NULL;
      result = 1;
    } else { /*arg is not a file*/
      if(strcmp(filesystem->curr->name, filesystem->root->name) == 0)
	tempd = filesystem->curr->next;
      else
	tempd = filesystem->curr->sub;

      /*search for arg as a directory*/
      while(tempd != NULL && strcmp(tempd->name, arg) != 0) {
	prevd = tempd;
	tempd = tempd->next;
      }

      if(tempd != NULL) {/*arg is a directory*/

	travd = tempd->sub;
	while(travd != NULL && strcmp(travd->name, tempd->name) != 0) {
	  while(travd->sub != NULL)/*go to furthest subdirectory*/
	    travd = travd->sub;
	  
	  nextd = travd->next;
	  par = travd->prev;
	  
	  tempf = travd->head;/*free the files in the directory*/
	  while(tempf != NULL) {
	    nextf = tempf->next;
	    free(tempf);
	    tempf = nextf;
	    if(nextf != NULL)
	      nextf = nextf->next;
	  }
	  tempd->head = NULL;

	  travd->next = NULL;
	  free(travd);
	  if(nextd != NULL)
	    travd = nextd;/*move to next part of directory to be freed*/
	  else {
	    travd = par;
	    travd->sub = NULL;
	  }
	}

	/*change what the other directories point to, avoiding removed one*/
	if(prevd == NULL) {
	  if(strcmp(filesystem->curr->name, filesystem->root->name) == 0)
	    filesystem->curr->next = tempd->next;
	  else
	    filesystem->curr->sub = tempd->next;
	} else
	  prevd->next = tempd->next;

	free(tempd->name);
	free(tempd);
	tempd = NULL;
	result = 1;
      }
    }
    
  }
  
  return result;
}

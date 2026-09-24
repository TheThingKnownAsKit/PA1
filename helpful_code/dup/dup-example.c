/****************************************************************
 *
 * Author: Hongzhi Guo
 * Title: dup-example.c
 * Date: February  2, 2024
 * Description: Example for dup()
 *
 * Command line parameters: name of file
 *
 * What are the main file descriptors?
 * 0 = stdin (STDIN_FILENO)
 * 1 = stdout (STDOUT_FILENO)
 * 2 = stderr (STDERR_FILENO)
 *
 ****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	// The function open() returns a file descriptor file_des
    int file_des = open("dup.txt", O_WRONLY | O_APPEND | O_CREAT);
      
    if(file_des < 0)
        printf("There is an error opening the file\n");
      
    // dup() will create the copy of file_des as the copy_des then both can be used interchangeably.
  
    int copy_des = dup(file_des);
          
    // The function write() will write the given string into the file referred by the file descriptors
  
    write(copy_des,"The output is written to the file named dup.txt \n", 49);
          
    write(file_des,"The output is also written to the file named dup.txt \n", 55);
      
    return 0;
}

/****************************************************************
 *
 * Author: Hongzhi Guo
 * Title: dup2-example.c
 * Date: February 2, 2024
 * Description: Example for dup2()
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
    int file_des = open("dup2.txt",O_WRONLY | O_APPEND | O_CREAT);
      
    // here the newfd is the file descriptor of stdout (i.e. 1)
    dup2(file_des, 1) ; 
          
    // All the printf statements will be written in the file
    // "dup2.txt"
    printf("I will be printed in the file dup2.txt \n");
      
    return 0;
}

/****************************************************************
 *
 * Author: Justin Bradley
 * Title: dup2fork.c
 * Date: Tuesday, February  5, 2019
 * Description: Example with fork and dup2
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
#include <sys/wait.h>

// function prototype
void runcmd(int fd, char **cmd);

int main(int argc, char **argv)
{
		int pid, status;
		int fd;	// file descriptor
		// artificial command
		char *cmd[] = { "ls", "-al", NULL }; // don't forget the NULL

		if (argc != 2) {
				fprintf(stderr, "usage: %s output_file\n", argv[0]);
				exit(1);
		}

		// open the file given on the command line
		if ((fd = open(argv[1], O_CREAT|O_TRUNC|O_WRONLY, 0644)) < 0) {
				perror(argv[1]);	/* open failed */
				exit(1);
		}
		// printf still actually goes to stdout
		printf("writing output of the command %s to \"%s\"\n", cmd[0], argv[1]);

		runcmd(fd, cmd);	/* run the command, sending the std output to fd */

		printf("all done!\n");
		exit(0);
}

/*
  runcmd(fd, cmd): fork a child process and run the command cmd,
  sending the standard output to the file descriptor fd.
  The standard input is closed. The parent waits for the child
  to terminate.
*/
void runcmd(int fd, char **cmd)
{
		int status; 

		switch (fork()) {
		case 0:	/* child */
				dup2(fd, STDOUT_FILENO); // map stdout to fd
				status = execvp(cmd[0], cmd);
				perror(cmd[0]);	   // bad execvp
				exit(1);

		default: /* parent */
		        printf("wait status %d\n", wait(&status));
				while (wait(&status) != -1) ; // reaping my zombies
				printf("wait status %d\n", wait(&status));
				printf("wait status %d\n", WEXITSTATUS(status));
				break;

		case -1: // bad fork, bad!
				perror("fork");
		}
		return;
}

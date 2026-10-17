#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

static void
pipeline(char ***cmd)
{
	int fd[2];
	pid_t pid;
	int fdd = 0;				

	while (*cmd != NULL) {
		pipe(fd);				
		if ((pid = fork()) == -1) {
			perror("fork");
			exit(1);
		}
		else if (pid == 0) {
			dup2(fdd, 0);
        
            // current pipe
            close(fd[0]);
			if (*(cmd + 1) != NULL) {
				dup2(fd[1], 1);
			}
			execvp((*cmd)[0], *cmd);

			close(fd[1]);
            
            exit(1);
		}
		else {
			wait(NULL); 		/* Collect children */
			close(fd[1]);
            // **********************
            // Save file descriptor
            // **********************
			fdd = fd[0]; // do not close this read end
			cmd++;
		}
	}
}


int main(int argc, char *argv[])
{
	char *ls[] = {"ls", "-l", NULL};
	char *cat[] = {"cat", NULL};
	char *grep[] = {"grep", "pipes", NULL};
	char **cmd[] = {ls, cat, grep, NULL};

	pipeline(cmd);
	return (0);
}
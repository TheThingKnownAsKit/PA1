#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

static void pipeline(char ***cmd)
{

	pid_t pid;
			

	while (*cmd != NULL) {
		
			

		if ((pid = fork()) == -1) {
			perror("fork");
			exit(1);
		}
		else if (pid == 0) {
		
        
            // current pipe
            
            exit(1);
		}
		else {
			wait(NULL); 		/* Collect children */
			
    
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
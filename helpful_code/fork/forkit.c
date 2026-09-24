/* Test file */

#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

int x = 10;

int main(void)
{
		int pid1,pid2;
		int x = 10;

		/* PID of child process returned to parent
		   0 returned as PID to child */
		pid1 = fork(); // spawn a new process
		/* pid2 = fork(); */
		printf("Hello, I am %d\n", getpid());
		printf(" (1)x = %d, from %d\n", x, getpid());
		if(pid1 == 0) // child process
		{
				/* setpriority(PRIO_PROCESS, 0, -20); // set to high priority */
				printf(" (2)x = %d, from %d\n",x, getpid());
				sleep(1);
				x = x + 5;
				printf(" (3)x = %d, from %d\n",x,getpid());			
		}
		else // parent process
		{
				/* setpriority(PRIO_PROCESS, 0, 1000000); // set to low priority */
				sleep(1);
				pid2 = fork();
				x = x - 1;
		}
		printf(" (4)x = %d, from %d\n",x,getpid());

}

//****************************
// Hongzhi Guo
// Redesigned this example for 
// fork() demo
// 2/5/2023
//****************************

#include <stdio.h>
#include <unistd.h> 

int x = 10; 

int main(void)
{

	int pid; 

	pid = fork(); /* Spawn a new process */
	printf("(1)  x=%d, pid = %d\n", x, pid); 
	if(pid == 0) { /* Child process */
		printf("(2) x=%d, pid = %d\n", x, pid); 
		x=x+5; 
		printf("(3) x=%d, pid = %d\n", x, pid); 
	}
	printf("(4) x=%d, pid = %d\n", x, pid); 
}

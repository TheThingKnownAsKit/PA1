#include <algorithm>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string>

using namespace std;

int main() {

pid_t cpid;
int status;

std::vector<std::string> vector = {"my","PA1","is","working"};
const char *command = "echo";

const char **argv = new const char* [vector.size() + 2];
argv[0] = command;


for (int i = 0;  i < (signed) vector.size() + 1;  i++){

	argv[i + 1] = vector[i].c_str();

}
	

argv[vector.size() + 1] = NULL;

cpid = fork();

if (cpid == 0){//child process
   execvp(argv[0], (char**) argv);
   exit(1);
}
else{//parent process
wait(&status);    
printf("child process done!\n");
}
}
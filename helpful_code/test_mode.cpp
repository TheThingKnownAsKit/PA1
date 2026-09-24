#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
 
#include "command.hpp"
#include "parser.hpp"

#define MAX_ALLOWED_LINES 25

using namespace std;

int main(int argc, char* argv[]) {

	 std::string input_line;
     std::vector<shell_command> shell_commands;

	 if ((argc > 1) && (argv[1] == std::string("-t"))) {

		  int command_lines = 0;
		  while (std::getline(std::cin, input_line) && (command_lines < MAX_ALLOWED_LINES)) {

			   command_lines++;

			   if (input_line == "exit")
					exit(0);
			   else {

					std::cout << input_line << std::endl; 
                    shell_commands = parse_command_string(input_line);

                     for (auto cmd : shell_commands) {

                         std::cout << cmd.cmd << std::endl;
                         for (const auto& arg : cmd.args) {
                             std::cout << "arg: " << arg << "\n";
                         } 

                         std::cout << "cin_file: " << cmd.cin_file << "\n";
                     }
                     
                     std::cout << "-------------------------------" << std::endl;
			   }
		  }
	 }
}

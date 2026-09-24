#include <iostream>
#include <string>
#include <vector>

#include "command.hpp"
#include "parser.hpp"

#define MAX_ALLOWED_LINES 25

int main()
{
    std::string input_line;

    for (int i=0;i<MAX_ALLOWED_LINES;i++) { // Limits the shell to MAX_ALLOWED_LINES
        // Print the prompt.
        std::cout << "osh> " << std::flush;

        // Read a single line.
        if (!std::getline(std::cin, input_line) || input_line == "exit") {
            break;
        }

        try {
            // Parse the input line.
            std::vector<shell_command> shell_commands
                    = parse_command_string(input_line);

            // Print the list of commands.
            
            // *********************************************************************
            // This for loop prints multiple commands
            // *********************************************************************
            std::cout << "-------------------------\n";
            for (const auto& cmd : shell_commands) {
                std::cout  << "cmd: " << cmd.cmd << std::endl;
                
                for (int i=0; i< (signed)cmd.args.size();i++){
                  std::cout << "arg: " << cmd.args[i] << std::endl;
                }

                std::cout << "cin_file: " << cmd.cin_file << std::endl;
                std::cout << "cin_mode: " << cmd.cin_mode << std::endl;

                std::cout << "-------------------------\n";
            }
            // *********************************************************************
            //  You can define functions and use them here
            //  You can write your codes here
            //  You can follow this structure and create multiple section depending
            //  on the conditions. 
            // *********************************************************************



        }
        catch (const std::runtime_error& e) {
            std::cout << "osh: " << e.what() << "\n";
        }
    }

    std::cout << std::endl;
}

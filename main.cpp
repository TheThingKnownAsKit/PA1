#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <fcntl.h>

#include "command.hpp"
#include "parser.hpp"

#define MAX_ALLOWED_LINES 25

int execute_command(const shell_command& cmd)
{
    // itemized_cmd here represents a vector of arguments to execute
    // each is just gotten from the cmd parameter.
    // The whole point is to be able to iterate over the whole vector easier since it's
    // structured like [command, arguments...arguments, nullptr] because execvp() needs a char* const argv[] list.
    std::vector<char*> itemized_cmd;
    itemized_cmd.push_back(const_cast<char*>(cmd.cmd.c_str()));
    for (const auto& arg : cmd.args) {
        itemized_cmd.push_back(const_cast<char*>(arg.c_str()));
    }
    itemized_cmd.push_back(nullptr);

    // this is where we fork the process
    pid_t child_pid = fork();

    if (child_pid < 0) {
        // this means fork failed, so exit
        std::cerr << "fork failed\n";
        return -1;
    } else if (child_pid == 0) {
        // child forked successfully so we set up redirection as needed and execute

        // redirection INTO a file
        if (cmd.cin_mode == istream_mode::file) {
            int file = open(cmd.cin_file.c_str(), O_RDONLY);
            if (file < 0) {
                std::cerr << "No such file or directory\n";
                exit(1);
            }
            dup2(file, STDIN_FILENO); // this overwrites stdin to this file
            close(file);
        }

        // redirection OUT OF a file
        // also worth noting that if this isn't appending to a file, there's no point opening
        // it for writing, which is why this also checks if it is both OUT and APPEND
        if (cmd.cout_mode == ostream_mode::file || cmd.cout_mode == ostream_mode::append) {
            int flags = O_WRONLY | O_CREAT;
            flags |= (cmd.cout_mode == ostream_mode::append) ? O_APPEND : O_TRUNC;

            int file = open(cmd.cout_file.c_str(), flags, 0644);
            if (file < 0) {
                std::cerr << "Cannot open file to write to it.";
                exit(1);
            }
            dup2(file, STDOUT_FILENO); // overwrite stdout to write to this file
            close(file);
        }

        // execvp will execute commands HERE
        execvp(itemized_cmd[0], itemized_cmd.data());

        // the above function returns if failed, so fail out here
        std::cerr << cmd.cmd << ": command not found\n";
        exit(1);
    }
    else {
        int status;
        waitpid(child_pid, &status, 0); // wait for the child

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        return -1; // child didn't exit normally
    }
}

int main(int argc, char* argv[])
{
    bool quiet_mode = false;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-t") {
            quiet_mode = true;
        }
    }

    std::string input_line;

    for (int i=0;i<MAX_ALLOWED_LINES;i++) { // Limits the shell to MAX_ALLOWED_LINES
        // Print the prompt.
        if (!quiet_mode) {
            std::cout << "osh> " << std::flush;
        }

        // Read a single line.
        if (!std::getline(std::cin, input_line) || input_line == "exit") {
            break;
        }

        try {
            // Parse the input line.
            std::vector<shell_command> shell_commands
                    = parse_command_string(input_line);

            int previous_status = 0;
            next_command_mode pending_mode = next_command_mode::always;

            // Print the list of commands.
            for (size_t i = 0; i < shell_commands.size(); i++) {
                const auto& cmd = shell_commands[i];

                bool can_run = true;
                if (i > 0) {
                    if (pending_mode == next_command_mode::on_success) {
                        can_run = (previous_status == 0);
                    }
                    else if (pending_mode == next_command_mode::on_fail) {
                        can_run = (previous_status != 0);
                    }
                    // dont need an if for always run since it should always run
                }

                if (can_run) {
                    previous_status = execute_command(cmd);
                }
                else {
                    previous_status = previous_status;
                }

                pending_mode = cmd.next_mode;
            }
        }
        catch (const std::runtime_error& e) {
            std::cout << "osh: " << e.what() << "\n";
        }
    }

    if (!quiet_mode) {
        std::cout << std::endl;
    }
}

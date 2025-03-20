#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>
#include <errno.h>

// Function declaration
std::vector<std::string> parseInput(const std::string &input);
void executeCommand(const std::vector<std::string> &args);
void handleCd(const std::vector<std::string> &args);
void printError(const std::string &message);

bool DEBUG = true;

void debugPrint(const std::string &message){
    if(DEBUG){
        std::cerr <<"{DEBUG}"<< message << std::endl;
    }
}

int main() {
    std::string input;
    const size_t MAX_INPUT_SIZE = 1000;

    while (true) {
        // Print the prompt
        std::cerr << "$";
        
        // Read input
        if (!std::getline(std::cin, input)) {
            break; 
        }

        // Check input size
        if (input.size() > MAX_INPUT_SIZE) {
            printError("Input exceeds maximum allowed length of 1000 characters");
            continue;
        }

        // Parse the input into arguments
        std::vector<std::string> args = parseInput(input);

        // Skip empty input
        if (args.empty()) {
            debugPrint("No args");
            continue;
        }

        // Handle "exit" command
        if (args[0] == "exit") {
            return 0;
        }

        // Handle "cd" command
        if (args[0] == "cd") {
            handleCd(args);
            continue;
        }

        // Execute the command
        executeCommand(args);
    }

    return 0;
}

// Function parseInput
std::vector<std::string> parseInput(const std::string &input) {
    std::vector<std::string> args;
    bool singleQuote = false;
    bool doubleQuote = false;
    std::string currentArg;
    
    for (size_t i = 0; i < input.size(); ++i)
    {
        char ch = input[i];

        if (ch == '\'') {
            if (!doubleQuote) {
                singleQuote = !singleQuote;
                if(!singleQuote){
                    if(currentArg.empty()){
                        args.push_back("");
                    }
                }
                continue;
            }
        } else if (ch == '"') {
            if (!singleQuote) {
                doubleQuote = !doubleQuote;
                if(!doubleQuote){
                    if(currentArg.empty()){
                        args.push_back("");
                    }
                }
                continue;
            }
        } else if (std::isspace(ch) && !singleQuote && !doubleQuote) {
            if (!currentArg.empty()) {
                args.push_back(currentArg);
                currentArg.clear();
            }
            continue;
        }
        currentArg += ch;
    }

    if (!currentArg.empty()) {
        args.push_back(currentArg);
    }

    if (singleQuote || doubleQuote) {
        printError("mismatched quotes");
        return {};
    }

    if(args.empty()){
        debugPrint("No valid Arguments");
    }

    return args;
}

// Function executeCommand
void executeCommand(const std::vector<std::string> &args) {
    pid_t pid = fork();

    if (pid < 0) {
        printError("Fork failed");
        return;
    }

    if (pid == 0) {
        // Child process
        std::vector<char *> c_args;
        for (const auto &arg : args) {
            c_args.push_back(const_cast<char *>(arg.c_str()));
        }
        c_args.push_back(nullptr);

        // Execute the command
        execvp(c_args[0], c_args.data());

        // If execvp fails
        printError("Command execution failed");
        exit(errno);
    } else {
        // Parent process: wait for the child to complete
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            std::cerr << "error: command exited with code " << WEXITSTATUS(status) << std::endl;
        }
    }
}

// Function to handle the "cd" 
void handleCd(const std::vector<std::string> &args) {
    if (args.size() != 2) {
        printError("cd requires exactly one argument");
        return;
    }

    const std::string &path = args[1];
    if (chdir(path.c_str()) != 0) {
        printError("cd failed: " + std::string(strerror(errno)));
    }
}

// Function to print an error message
void printError(const std::string &message) {
    std::cerr << "error: " << message << std::endl;
}

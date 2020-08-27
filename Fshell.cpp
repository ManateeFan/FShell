#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unistd.h>

constexpr int MAX_CMD_LINE = 80;

int main()
{
    int argc = 0;
    bool run = true;
    char *args[MAX_CMD_LINE / 2 + 1];

    std::vector<std::string> enteredInput;
    std::vector<std::string> cmdHistory;

    while (run)
    {
        int status = 1;

        // user input value
        std::string inputLine;

        std::cout << "FShell>";

        // get entire user input line
        std::getline(std::cin, inputLine);

        // istringstring to get tokens
        std::istringstream iss(inputLine);

        // store each token
        std::string input;
        while (getline(iss, input, ' '))
        {
            // input exit to stop FShell
            if (input == "exit")
            {
                std::cout << "Exiting the FShell..." << std::endl;
                run = false;
            }

            //
            if (!input.empty() && (input != "history"))
            {
                if (inputLine[0] == '!')
                {
                    continue;
                }
                else
                {
                    // this token is not history or start with !
                    // it is a command
                    enteredInput.push_back(input);
                    cmdHistory.push_back(input);
                    argc++;
                }
            }
        }

        // store args
        std::string comArr[argc];
        std::copy(std::begin(cmdHistory), std::end(cmdHistory), comArr);

        // user input history to get command history
        if (inputLine == "history")
        {
            // if no commands in the history
            if (cmdHistory.empty())
            {
                std::cout << "No commands in history..." << std::endl;
            }
            for (int i = argc; i > 0; --i) 
            {
                std::cout << i << ": " << comArr[i-1] << std::endl;
            }
        }
        if (inputLine != "history")
        {
            // use history command !1 / !2...
            if (inputLine[0] == '!' && inputLine[1] != '!')
            {
                int num = 0;

                // get the string after !
                std::string hold = &inputLine[1];
                num = std::stoi(hold.c_str());
                num--;
                args[0] = const_cast<char *>(enteredInput.begin()[num].c_str());
            }
            else if (inputLine == "!!")
            {
                // the most recent command is to be run
                if (cmdHistory.empty())
                {
                    std::cout << "No commands in history..." << std::endl;
                }
                else
                {
                    args[0] = const_cast<char *>(enteredInput.end()[-1].c_str());
                }
            }
            else
            {
                args[0] = const_cast<char *>(enteredInput.back().c_str());
            }

            if (status == 1 && run)
            {
                args[argc] = nullptr;

                pid_t pid = fork();

                if (pid < 0)
                {
                    std::cout << "Error" << std::endl;
                    return 1;
                }
                else if (pid == 0)
                {
                    execvp(args[0], args);
                    return 0;
                }
                else
                {
                    int childStatus;
                    waitpid(pid, &childStatus, 0);
                    run = true;
                }
            }
        }
    }

    return 0;
}
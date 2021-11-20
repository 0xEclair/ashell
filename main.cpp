#include <cstdio>
#include <string>
#include <vector>
#include <regex>
#include <iostream>

#include <unistd.h>
#include <sys/wait.h>

namespace as {
    auto read_line = []() {
        auto buffer = std::string()/*= std::string(buf_size, '\0')*/;
        getline(std::cin, buffer);
        return buffer;
    };

    auto split = [](auto&& input) {
        std::regex rgx(R"(\s+)");
        std::sregex_token_iterator iter(input.begin(),
                                        input.end(),
                                        rgx,
                                        -1);
        std::sregex_token_iterator end;

        std::vector<std::string> res;
        for ( ; iter != end; ++iter) {
            res.push_back(std::move(*iter));
        }
        return res;
    };

    auto launch = [](auto&& args){
        pid_t pid;
        if(pid == 0) {
            std::vector<char*> arg2(nullptr, args.size());
            for(auto i = 0; i<args.size(); ++i) {
                arg2[i] = args[i].data();
            }
            if(execvp(args[0],&arg2[0]) == -1) {
                perror("lsh");
            }
            exit(EXIT_FAILURE);
        }
        else {
            int status;
            do{
                waitpid(pid, &status, WUNTRACED);
            } while(!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        return 1;
    };

    auto execute = [](auto&& params) {
        return 0;
    };
    auto loop() {
        std::vector<std::string> args;
        int status;

        do {
            printf("> ");
            auto line = read_line();
            args = split(line);
            status = execute(args);
        } while(status);

    }
}

auto main(int arc, char* argv[]) -> int {
    using namespace as;
    loop();
    return 1;
}
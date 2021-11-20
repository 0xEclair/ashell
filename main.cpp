#include <cstdio>
#include <string>
#include <vector>
#include <regex>
#include <iostream>

#include <unistd.h>
#include <sys/wait.h>

#include "command.hpp"

namespace as {
    auto read_line = []() {
        auto buffer = std::string()/*= std::string(buf_size, '\0')*/;
        getline(std::cin, buffer);
        return buffer;
    };

    // TODO: std::string_view version
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
        pid_t pid = fork();
        if(pid == 0) {
            if(execvp(args[0],&args[0]) == -1) {
                perror("as");
                exit(EXIT_FAILURE);
            }
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
        // TODO
        std::vector<char*> args(params.size(), nullptr);
        for(auto i = 0; i < params.size(); ++i) {
            args[i] = params[i].data();
        }
        if(params[0].empty()) {
            return 1;
        }
        // TODO: Perhaps i won't to do "as::cmd::help".
        for(; params[0] == "cd";) {
            return cmd::cd(args);
        }
        return launch(args);
    };
    auto loop() {
        while(printf("> "),execute(split(read_line())));
    }
}

auto main() -> int {
    using namespace as;
    loop();
    return 1;
}
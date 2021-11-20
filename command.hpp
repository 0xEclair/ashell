#pragma once
#include <unistd.h>

// built-in commands
namespace as::cmd {
    auto cd = [](auto&& args){
        if(args[1] == nullptr) {
            printf("as:cd no target\n");
        }
        else {
            if(chdir(args[1]) != 0) {
                perror("as");
            }
        }
        return 1;
    };
    auto help = [](auto&& args){
        //TODO
    };
    auto exit = [](auto&& args){
        return 0;
    };
}
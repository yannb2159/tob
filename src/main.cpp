#include <iostream>
#include <cstdlib>
#include "interpreter.hpp"

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cerr << "[Error] No program provided !\n";
        std::cerr << "[Error] Usage : ./tob <my_program.bf>\n";
        return EXIT_FAILURE;
    }

    std::string program_path = std::string(argv[1]);

    Interpreter my_interpreter;
    
    my_interpreter.load_program_from_file(program_path);

    my_interpreter.interpret();
}
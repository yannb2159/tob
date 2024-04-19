#include "interpreter.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

Interpreter::Interpreter() {
    m_program.resize(0x0);

    m_ptr = 0;
    m_pc = 0;
    m_program_size = 0;
}

void Interpreter::load_program_from_file(const std::string& program_path) {
    std::ifstream program_file(program_path);

    if (!program_file.is_open())
        throw std::runtime_error("[Error] Unable to open the brainf*ck file !");
    
    char c;
    while (program_file.good()) {
        program_file.get(c);

        switch (c) {
            case '>': m_program.push_back(Ops::OP_INCR_PTR); break;
            case '<': m_program.push_back(Ops::OP_DECR_PTR); break;
            case '+': m_program.push_back(Ops::OP_INCR_BYTE); break;
            case '-': m_program.push_back(Ops::OP_DECR_BYTE); break;
            case '.': m_program.push_back(Ops::OP_BYTE_OUT); break;
            case ',': m_program.push_back(Ops::OP_BYTE_IN); break;
            case '[': m_program.push_back(Ops::OP_JMP_IF_ZERO); break;
            case ']': m_program.push_back(Ops::OP_JMP_IF_NOT_ZERO); break;
            default: break; // Other characters are treated as comments, ignore them.
        }
    }

    program_file.close();

    m_program_size = m_program.size();
}

void Interpreter::precompute_jumps() {
    std::vector<uint64_t> stack;

    uint64_t pc = 0;
    uint64_t target;

    while (pc < m_program_size) {
        Ops op = m_program[pc];

        switch (op) {
            case Ops::OP_JMP_IF_ZERO: stack.push_back(pc); break;
            case Ops::OP_JMP_IF_NOT_ZERO:
                target = stack.back();
                stack.pop_back();
                m_jump_map[pc] = target;
                m_jump_map[target] = pc;
                break;
            default: break; // Ignore all other ops as they aren't jmp related...
        }

        pc++;
    }
}

void Interpreter::interpret() {
    precompute_jumps();

    while (m_pc < m_program_size) {
        Ops op = m_program[m_pc];

        uint8_t* mem_cell = nullptr;

        if (m_memory.count(m_ptr) < 0)
            m_memory[m_ptr] = 0;

        mem_cell = &m_memory[m_ptr];

        switch (op) {
            case Ops::OP_INCR_PTR: m_ptr++; break;
            case Ops::OP_DECR_PTR: m_ptr--; break;
            case Ops::OP_INCR_BYTE: *mem_cell += 1; break;
            case Ops::OP_DECR_BYTE: *mem_cell -= 1; break;
            case Ops::OP_BYTE_OUT:
                {
                    char byte_as_char = static_cast<char>(*mem_cell);
                    std::cout << byte_as_char;
                    std::cout.flush();
                }
                break;
            case Ops::OP_BYTE_IN:
                {
                    char byte_input_as_char;
                    std::cin >> byte_input_as_char;
                    std::cin.clear();
                    *mem_cell = static_cast<uint8_t>(byte_input_as_char);
                }
                break;
            case Ops::OP_JMP_IF_ZERO:
                if (*mem_cell == 0)
                    m_pc = m_jump_map[m_pc];
                break;
            case Ops::OP_JMP_IF_NOT_ZERO:
                if (*mem_cell != 0)
                    m_pc = m_jump_map[m_pc];
                break;
        }

        m_pc++;
    }
}
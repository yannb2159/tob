#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <string>
#include <vector>
#include <map>
#include <cstdint>

enum class Ops {
    OP_INCR_PTR,
    OP_DECR_PTR,
    OP_INCR_BYTE,
    OP_DECR_BYTE,
    OP_BYTE_OUT,
    OP_BYTE_IN,
    OP_JMP_IF_ZERO,
    OP_JMP_IF_NOT_ZERO
};

class Interpreter {
    public:
        Interpreter();

        void load_program_from_file(const std::string& program_path);
        void interpret();
    private:
        void precompute_jumps();

        std::map<uint64_t, uint8_t> m_memory; // using std::map to simulate infinite memory.
        std::map<uint64_t, uint64_t> m_jump_map;
        std::vector<Ops> m_program;

        uint64_t m_ptr;
        uint64_t m_pc;
        std::size_t m_program_size;
};

#endif
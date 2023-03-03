#ifndef OOP_DOMACI2_MACHINE_H
#define OOP_DOMACI2_MACHINE_H

#include "Exception.h"
#include "Instruction.h"

#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

class Machine {

public:
    Machine();
    ~Machine();

    void loadProgram(const std::string& filepath);
    void execute(const std::string& filepath);
    void fileOutput(const std::string& filepath);
private:
    std::vector<Instruction*> program;
    std::map <char, int> variables;

    std::stack<Instruction*> stack;

    bool compiled= false;

    static std::map<std::string, InstructionType> mapInstructionTypes;
    static void InitializeInstMap();
};


#endif //OOP_DOMACI2_MACHINE_H

#include "Machine.h"

std::map<std::string, InstructionType> Machine::mapInstructionTypes;

Machine::Machine() {
	InitializeInstMap();
}

void Machine::InitializeInstMap() {
	mapInstructionTypes["SET"] = SET;
	mapInstructionTypes["ADD"] = ADD;
	mapInstructionTypes["SUB"] = SUB;
	mapInstructionTypes["MUL"] = MUL;
	mapInstructionTypes["DIV"] = DIV;
	mapInstructionTypes["GOTO"] = GOTO;
	mapInstructionTypes["IFGR"] = IFGR;
	mapInstructionTypes["IFEQ"] = IFEQ;
	mapInstructionTypes["ELSE"] = ELSE;
	mapInstructionTypes["ENDIF"] = ENDIF;
	mapInstructionTypes["LOOP"] = LOOP;
	mapInstructionTypes["ENDLOOP"] = ENDLOOP;
}

void Machine::loadProgram(const std::string& filepath) {
	std::ifstream input(filepath);

	std::string instruction;

	try {
		if (input.is_open()) {
			while (!input.eof()) {
				//string instruction;
				input >> instruction;
				if (!mapInstructionTypes.count(instruction)) {
					throw UnknownInstructionError();
				}

				Instruction* tmp;


				std::string s1;
				std::string s2;
				std::string s3;


				switch (mapInstructionTypes[instruction]) {
				case SET:
					input >> s1 >> s2;
					if (s1.length() != 1) throw ParssingError();
					tmp = new Set(s1[0], s2);
					program.push_back(tmp);
					break;
				case ADD:
					input >> s1 >> s2 >> s3;
					if (s1.length() != 1) throw ParssingError();
					tmp = new Arithmetic(s1[0], s2, s3, ADD);
					program.push_back(tmp);
					break;
				case SUB:
					input >> s1 >> s2 >> s3;
					if (s1.length() != 1) throw ParssingError();
					tmp = new Arithmetic(s1[0], s2, s3, SUB);
					program.push_back(tmp);
					break;
				case MUL:
					input >> s1 >> s2 >> s3;
					if (s1.length() != 1) throw ParssingError();
					tmp = new Arithmetic(s1[0], s2, s3, MUL);
					program.push_back(tmp);
					break;
				case DIV:
					input >> s1 >> s2 >> s3;
					if (s1.length() != 1) throw ParssingError();
					tmp = new Arithmetic(s1[0], s2, s3, DIV);
					program.push_back(tmp);
					break;
				case GOTO:
					input >> s1;
					tmp = new GoTo(s1);
					program.push_back(tmp);
					break;
				case IFEQ:
					input >> s1 >> s2;
					tmp = new If(s1, s2, EQ);
					program.push_back(tmp);
					stack.push(tmp);

					break;
				case IFGR:
					input >> s1 >> s2;
					tmp = new If(s1, s2, GR);
					program.push_back(tmp);
					stack.push(tmp);

					break;
				case ELSE:
					tmp = new Else();
					program.push_back(tmp);
					if ((stack.top())->type != IFEQ && (stack.top())->type != IFGR) throw LoopError();
					((If*)stack.top())->setElse(tmp->getAdress());
					break;
				case ENDIF:
					tmp = new EndIf();
					program.push_back(tmp);

					if ((stack.top())->type != IFEQ && (stack.top())->type != IFGR) throw LoopError();
					if (((If*)stack.top())->getElseAdress() == -1) throw IfMatchingError();
					((Else*)(program[((If*)stack.top())->getElseAdress()]))->setEndIf(tmp->getAdress());
					stack.pop();
					break;
				case LOOP:
					if (input.peek() == '\n') {
						tmp = new Loop();
						program.push_back(tmp);
						stack.push(tmp);
					}
					else {
						input >> s1;
						tmp = new Loop(stoi(s1));
						program.push_back(tmp);
						stack.push(tmp);
					}
					break;
				case ENDLOOP:
					tmp = new EndLoop();
					program.push_back(tmp);

					if ((stack.top())->type != LOOP) throw BranchingError();
					((Loop*)stack.top())->setEndLoop(tmp->getAdress());
					stack.pop();
					break;
				}
			}

			if (!stack.empty())throw MatchingError();
			input.close();

		}
		else throw FileNotFound('I');
		compiled = true;
	}

	catch (UnknownInstructionError& e) {
		std::cout << e.msg(program.size()) << std::endl;
		return;
	}
	catch (FileNotFound& e) {
		std::cout << e.msg() << std::endl;
		return;
	}
	catch (MatchingError& e) {
		std::cout << e.msg(stack.size()) << std::endl;
		return;
	}
	catch (ParssingError& e) {
		std::cout << e.msg(program.size() + 1) << std::endl;
		return;
	}
	catch (LoopError& e) {
		std::cout << e.msg(program.size(), instruction, (stack.top())->getAdress() + 1) << std::endl;
		return;
	}
	catch (IfMatchingError& e) {
		std::cout << e.msg(stack.top()->getAdress() + 1) << std::endl;
		return;
	}
	catch (BranchingError& e) {
		std::cout << e.msg(program.size(), stack.top()->getAdress() + 1) << std::endl;
		return;
	}
	catch (GoToZero& e) {
		std::cout << e.msg(program.size()) << std::endl;
		return;
	}
}

void Machine::execute(const std::string& filepath) {
	if (!compiled) return;

	int i = 0;
	try {
		program[0]->clearMemory();
		while (i < program.size()) {
			i = program[i]->execute();
			variables = program[0]->printVariables();
			if (i < 0) throw IndexOutOfBounds(i);
			//if (i > program.size()) throw IndexOutOfBounds(i);
		}

	}
	catch (InvalidArgument& e) {
		std::cout << e.msg(i + 1) << std::endl;
		return;
	}
	catch (Error& e) {
		std::cout << e.msg() << std::endl;
		return;
	}

	fileOutput(filepath);
}

Machine::~Machine() {
	for (auto& elem : program)
	{
		delete elem;
	}
	while (!stack.empty()) {
		delete stack.top();
		stack.pop();
	}
}

void Machine::fileOutput(const std::string& filepath) {
	try {
		std::ofstream output(filepath);

		if (output.is_open()) {
			for (const auto& elem : variables)
			{
				output << elem.first << "=" << elem.second;
				if (elem.first != prev(variables.end())->first) output << "\n";
			}
			output.close();
		}
		else {
			throw FileNotFound('O');
		}
	}
	catch (Error& e) {
		std::cout << e.msg() << std::endl;
	}

}

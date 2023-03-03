#ifndef OOP_DOMACI2_INSTRUCTION_H
#define OOP_DOMACI2_INSTRUCTION_H


#include "Exception.h"

#include <map>
#include <stack>
#include <string>
#include <fstream>

enum Type {
	NAME, VALUE
};

enum InstructionType {
	SET, ADD, SUB, MUL, DIV, GOTO, IFGR, IFEQ, ELSE, ENDIF, LOOP, ENDLOOP
};


class Instruction {
public:
	~Instruction();

	static std::map <char, int> printVariables();
	virtual int execute();

	int getAdress() const;
	static void clearMemory();
	InstructionType type;

protected:
	static std::stack<Instruction*> loop_stack;
	static std::map<char, int> variables;

	static Type checkValueType(std::string val);


private:
	static int last_adress;
	int adress = last_adress++;

	static bool isNumber(const std::string& s);
};

class Set : public Instruction {
public:
	Set() = default;
	Set(char var, std::string val);
	int execute() override;

	void setVariable(char c);
	void setValue(std::string s);

	// friend std::istream& operator>>(std::istream& is, Set& i);


protected:
	char variable;
	std::string value;
private:
};


class Arithmetic : public Instruction {
public:
	Arithmetic() = default;
	Arithmetic(char var, std::string val1, std::string val2, InstructionType op);
	int execute() override;
protected:
	InstructionType operation;
	char variable;
	std::string value1;
	std::string value2;
private:
};

class Loop : public Instruction {
public:
	Loop();
	Loop(int count);

	void setEndLoop(int adress);
	int execute() override;
	int getCounter() const;
	int getEndLoop() const;
	void decrementCounter();
	void resetCounter();
protected:
private:
	int count = 0;
	int counter;
	int endloop;
};

class EndLoop : public Instruction {
public:
	int execute() override;
protected:
private:
};

class GoTo : public Instruction {
public:
	GoTo() = default;
	explicit GoTo(const std::string& val);
	int execute() override;
protected:
private:
	int move;
};

enum Operator {
	GR, EQ
};

class If : public Instruction {
public:
	If() = default;
	If(std::string val1, std::string val2, Operator o);

	void setElse(int adress);
	int getElseAdress() const;
	int execute() override;
protected:
private:
	bool condition;
	std::string value1;
	std::string value2;

	Operator op;
	int else_adress;
	void setCondition();
};

class Else : public Instruction {
public:
	Else() = default;

	void setEndIf(int adress);
	int execute() override;
protected:
private:
	int endif_adress;
};

class EndIf : public Instruction {
public:
	EndIf() = default;
	int execute() override;
protected:
private:
};
#endif //OOP_DOMACI2_INSTRUCTION_H

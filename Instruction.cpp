#include "Instruction.h"


std::map<char, int> Instruction::variables;
std::stack<Instruction*> Instruction::loop_stack;

int Instruction::last_adress = 0;

int Instruction::getAdress() const {
	return this->adress;
}

void Loop::decrementCounter() {
	this->counter--;
}

int Loop::execute() {
	loop_stack.push(this);

	return (this->getAdress() + 1);
}

int Loop::getCounter() const {
	return counter;
}


Loop::Loop(int count) {
	this->count = count;
	this->counter = count;
	this->type = LOOP;
}

void Loop::setEndLoop(int adress) {
	this->endloop = adress;
}

int Loop::getEndLoop() const {
	return this->endloop;
}

void Loop::resetCounter() {
	this->counter = this->count;
}

Loop::Loop() {
	this->type = LOOP;
}


int Set::execute() {

	switch (checkValueType(this->value)) {
	case NAME:
		if (variables.count(this->value[0])) variables[this->variable] = variables[this->value[0]];
		else throw UnknownVariable(this->getAdress() + 1);
		break;
	case VALUE:
		variables[this->variable] = stoi(this->value);
		break;
	}
	return (this->getAdress() + 1);
}

/*std::istream &operator>>(istream & is, Set& i) {
	//pls pls pls realizuj ovo svega ti
	printf("TU SAM");
	char c;
	string s;

	is >> c;
	is >> s;

	i.value=s;
	i.variable=c;

	return is;
}*/

void Set::setVariable(char c) {
	this->variable = c;
}

void Set::setValue(std::string s) {
	this->value = std::move(s);
}

Set::Set(char var, std::string val) {
	this->variable = var;
	if (!isupper(var)) throw ParssingError();
	this->value = std::move(val);
}


Type Instruction::checkValueType(std::string val) {
	if (val[0] >= 'A' && val[0] <= 'Z' && val.length() == 1) return NAME;
	else if (isNumber(val)) return VALUE;
	else throw InvalidArgument();
}

bool Instruction::isNumber(const std::string& s) {
	if (!(s[0] == '-' || (s[0] >= '0' && s[0] <= '9'))) return false;
	for (int i = 1; i < s.length(); i++) {
		if (s[i] < '0' || s[i] > '9') return false;
	}
	return true;
}

std::map<char, int> Instruction::printVariables() {
	return variables;
}

void Instruction::clearMemory() {
	variables.clear();
	last_adress = 0;
	while (!loop_stack.empty()) { loop_stack.pop(); }
}

int Arithmetic::execute() {
	int first, second;

	switch (checkValueType(value1)) {
	case NAME:
		if (variables.count(this->value1[0])) first = variables[value1[0]];
		else throw UnknownVariable(this->getAdress() + 1);
		break;
	case VALUE:
		first = stoi(value1);
		break;
	}

	switch (checkValueType(value2)) {
	case NAME:
		if (variables.count(this->value2[0])) second = variables[value2[0]];
		else throw UnknownVariable(this->getAdress() + 1);
		break;
	case VALUE:
		second = stoi(value2);
		break;
	}

	switch (this->operation) {
	case ADD:
		variables[variable] = first + second;
		break;
	case SUB:
		variables[variable] = first - second;
		break;
	case MUL:
		variables[variable] = first * second;
		break;
	case DIV:
		if (second != 0) variables[variable] = first / second;
		else throw DividingByZero(this->getAdress());
		break;
	}

	return (this->getAdress() + 1);
}

Arithmetic::Arithmetic(char var, std::string val1, std::string val2, InstructionType op) {
	this->variable = var;
	this->value1 = std::move(val1);
	this->value2 = std::move(val2);
	this->operation = op;

	this->type = op;
}

int EndLoop::execute() {
	if (((Loop*)loop_stack.top())->getCounter() == 0) {

		return loop_stack.top()->getAdress();
	}
	if (((Loop*)loop_stack.top())->getCounter() == 1) {
		((Loop*)loop_stack.top())->resetCounter();
		loop_stack.pop();
		return this->getAdress() + 1;
	}
	else {
		((Loop*)loop_stack.top())->decrementCounter();
		return loop_stack.top()->getAdress();
	}
}

int GoTo::execute() {
	//if it escapes a loop :
	if (!loop_stack.empty()) {
		if (this->getAdress() + this->move > ((Loop*)loop_stack.top())->getEndLoop()) loop_stack.pop();
	}
	return this->getAdress() + move;
}

GoTo::GoTo(const std::string& val) {
	this->move = stoi(val);
	if (this->move == 0) throw GoToZero();
}

int If::execute() {
	setCondition();

	if (condition)return this->getAdress() + 1;
	else return this->else_adress + 1;
}

int Else::execute() {
	return endif_adress;
}

int EndIf::execute() {
	return this->getAdress() + 1;
}

void If::setCondition() {
	int first, second;

	switch (checkValueType(value1)) {
	case NAME:
		if (variables.count(this->value1[0])) first = variables[value1[0]];
		else throw UnknownVariable(this->getAdress() + 1);
		break;
	case VALUE:
		first = stoi(value1);
		break;
	}

	switch (checkValueType(value2)) {
	case NAME:
		if (variables.count(this->value2[0])) second = variables[value2[0]];
		else throw UnknownVariable(this->getAdress() + 1);
		break;
	case VALUE:
		second = stoi(value2);
		break;
	}

	switch (op) {
	case GR:
		this->condition = first > second;
		break;
	case EQ:
		this->condition = first == second;
		break;
	}
}

void If::setElse(int adress) {
	this->else_adress = adress;
}

If::If(std::string val1, std::string val2, Operator o) {
	this->value1 = std::move(val1);
	this->value2 = std::move(val2);
	this->op = o;

	if (this->op == GR) type = IFGR;
	else type = IFEQ;

	this->else_adress = -1;
}

int If::getElseAdress() const {
	return else_adress;
}

void Else::setEndIf(int adress) {
	this->endif_adress = adress;
}

Instruction::~Instruction() {
	variables.clear();
	/*
	while (!loop_stack.empty()) {
		delete loop_stack.top();
		//loop_stack.pop();
	}
	*/
	last_adress = 0;

}

int Instruction::execute() {
	return 0;
}

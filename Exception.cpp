#include "Exception.h"

std::string UnknownInstructionError::msg() {
	return "? : Instruction unknown.";
}

std::string UnknownInstructionError::msg(int line) {
	return "(Broke down after instruction " + std::to_string(line) + ") : Instruction unknown.";
}

std::string UnknownVariable::msg() {
	return "(Line " + std::to_string(line) + ") : Unknown variable.";
}

std::string DividingByZero::msg() {
	return "(Line " + std::to_string(line + 1) + ") : Dividing by zero.";
}


std::string InvalidArgument::msg(int line) {
	return "(Line " + std::to_string(line) + ") : Invalid arguments.";
}

std::string InvalidArgument::msg() {
	return "Arguments invalid.";
}

std::string IndexOutOfBounds::msg() {
	return "(Attempted jump to " + std::to_string(line) + ") : Program goes out of bounds.";
}

std::string GoToZero::msg() {
	return "GOTO Argument cannot be 0.";
}

std::string GoToZero::msg(int line) {
	return "(Line " + std::to_string(line + 1) + ") : GOTO Argument cannot be 0.";
}

std::string ParssingError::msg() {
	return "Parsing error.";
}

std::string ParssingError::msg(int line) {
	return "(Line " + std::to_string(line) + ") : Parsing error.";
}

std::string FileNotFound::msg() {
	switch (io) {
	case 'I':
		return "(File not found) : Could not open input file.";
	case 'O':
		return "(File not found) : Could not open output file.";
	default:
		return "(File not found) : Could not open file.";
	}
}

std::string BranchingError::msg() {
	return "Branching must be closed before ENDLOOP.";
}

std::string BranchingError::msg(int line, int unmatched) {
	return "(Line " + std::to_string(line) + ") : Branching at line " + std::to_string(unmatched) + " must be closed before ENDLOOP.";
}

std::string LoopError::msg() {
	return "Unclosed loop.";
}

std::string LoopError::msg(int line, const std::string& instruction, int unmatched) {
	return "(Line " + std::to_string(line) + ") : Loop at line " + std::to_string(unmatched) + " must be closed before " + instruction + ".";
}

std::string IfMatchingError::msg() {
	return "IF must have ELSE before ENDIF.";
}

std::string IfMatchingError::msg(int unmatched) {
	return "IF at line " + std::to_string(unmatched) + " must have ELSE before ENDIF.";
}

std::string MatchingError::msg() {
	return "Matching error.";
}

std::string MatchingError::msg(int stack_size) {
	return "Matching error: At least " + std::to_string(stack_size) + " branches unmatched.";
}


#ifndef OOP_DOMACI2_EXCEPTION_H
#define OOP_DOMACI2_EXCEPTION_H

#include <exception>
#include <string>


class Error : public std::exception
{
public:
    Error() : std::exception() {}
    virtual std::string msg() = 0;
};

class FileNotFound : public Error
{
public:
    explicit FileNotFound(char io_) : Error() , io(io_){}
    std::string msg() override;

private:
    char io;
};

//while reading from file :

class UnknownInstructionError : public Error
{
public:
    UnknownInstructionError() : Error() {}
    std::string msg() override;
    static std::string msg(int line);
};

class ParssingError : public Error
{
public:
    explicit ParssingError () : Error() {}
    std::string msg() override;
    static std::string msg(int line);
};

class GoToZero : public Error
{
public:
    explicit GoToZero() : Error() {}
    std::string msg() override;
    static std::string msg(int line);
};

class BranchingError : public Error
{
public:
    explicit BranchingError() : Error() {}
    std::string msg() override;
    static std::string msg(int line, int unmatched);
};

class LoopError : public Error
{
public:
    explicit LoopError() : Error() {}
    std::string msg() override;
    static std::string msg(int line, const std::string& instruction, int unmatched);
};

class IfMatchingError : public Error
{
public:
    explicit IfMatchingError() : Error() {}
    std::string msg() override;
    static std::string msg(int unmatched);
};

class MatchingError : public Error
{
public:
    explicit MatchingError() : Error() {}
    std::string msg() override;
    static std::string msg(int stack_size);
};

//while executing :

class UnknownVariable : public Error
{
public:
    explicit UnknownVariable(int line_) : Error(), line(line_) {}
    std::string msg() override;
private:
    int line;
};

class DividingByZero : public Error
{
public:
    explicit DividingByZero (int line_) : Error(), line(line_) {}
    std::string msg() override;
private:
    int line;
};

class InvalidArgument : public Error
{
public:
    explicit InvalidArgument () : Error() {}
    std::string msg() override;
    static std::string msg(int line);
};

class IndexOutOfBounds : public Error
{
public:
    explicit IndexOutOfBounds (int line_) : Error(), line(line_) {}
    std::string msg() override;
private:
    int line;
};












#endif //OOP_DOMACI2_EXCEPTION_H

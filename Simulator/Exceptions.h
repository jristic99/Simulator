#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
using namespace std;

class WrongInput : public exception {
public:
	WrongInput(const char* msg) : exception(msg) {}
};

class NegativeInput : public exception {
public:
	NegativeInput(const char* msg) : exception(msg) {}
};

class WrongIdentifier : public exception {
public:
	WrongIdentifier(const char* msg) : exception(msg) {}
};

class WrongElementType : public exception {
public:
	WrongElementType(const char* msg) : exception(msg) {}
};

class WrongPin : public exception {
public:
	WrongPin(const char* msg) : exception(msg) {}
};

#endif
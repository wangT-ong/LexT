#pragma once
#include<string>
#include<fstream>
#include"SmartPointer.h"
#include"Buffer.h"
enum Tag {
	NAME=264,ACTION=265,FUNCTION=266,NUM=270,DIGITE=271,LETTER=272,SPACE=32,TAB=9,RETURN=10,DOT=46,
	LE=276,NE=277,GE=278,EMPTY=279
};


class Token
{
public:
	Token(int t) :tag(t) {};
	int tag;

	virtual std::string toString() 
	{
		std::string str;
		str += tag;
		return str;
	};
};
using TokenPtr = SmartPointer<Token>;
class Num :public Token
{
public:
	int value;
	Num(int v) :Token(NUM), value(v) {};
	virtual std::string toString() 
	{
		std::string str = std::to_string(value);
		return str;
	};
};
using NumPtr = SmartPointer<Num>;

class Name :public Token
{
public:
	std::string lexeme;
	Name(std::string& s, int tag) :Token(tag), lexeme(s) {};
	virtual std::string toString() { return lexeme; };
};
using NamePtr = SmartPointer<Name>;


class Digit :public Token
{
public:
	char digit;
	Digit(char d) :Token(DIGITE), digit(d) {}
	virtual std::string toString()
	{
		std::string str;
		str += digit - '0';
		return str;
	};
};
using DigitPtr = SmartPointer<Digit>;


class Letter :public Token
{
public:
	char letter;
	Letter(char l) :Token(LETTER), letter(l) {}
	virtual std::string toString()
	{
		std::string str;
		str += letter;
		return str;
	};
};
using LetterPtr = SmartPointer<Letter>;

class Action :public Token
{
public:
	std::string action;
	Action(std::string& str) :Token(ACTION), action(str) {}
	virtual std::string toString()
	{
		std::string str;
		str += action;
		return str;
	};
};
using ActionPtr = SmartPointer<Action>;

class Function :public Token
{
public:
	std::string signature;
	ActionPtr actionPtr;
	Function(std::string& str,ActionPtr actPtr) :Token(FUNCTION), signature(str) ,actionPtr(actPtr){}
	virtual std::string toString()
	{
		std::string str;
		if (!actionPtr.isValidate())
			return str;
		str += signature;
		str += actionPtr->toString();
		return str;
	};
};
using FunctionPtr = SmartPointer<Function>;


class Lexer
{
public:
	static int line;
	
	Lexer(std::wstring fileName);
	TokenPtr scan();
	TokenPtr peek();
	void clearWhiteSpace();
	void setClearSpace() { m_isClearSpace = true; }
	void setNonClearSpace() { m_isClearSpace = false; }
	void setReadSingleChar() { m_isReadSingleChar = true; }
	void setReadMultiChar() { m_isReadSingleChar = false; }
	void setReadAction(bool isRead) { m_isReadAction = isRead; }
	void setReadFunction(bool isRead) { m_isReadFunction = isRead; }

private:
	bool m_isClearSpace = true;
	bool m_isReadSingleChar = true;
	bool m_isReadAction = false;
	bool m_isReadFunction = false;
	Buffer m_buffer;
	char readch();
	TokenPtr _peek(int& length);
	void clearWhiteSpace(int& length);
	NamePtr readName(int& length);
	LetterPtr readLetter(int& length);
	NumPtr readNumber(int& length);
	TokenPtr readDigit(int& length);
	TokenPtr readSymbol(int& length);
	ActionPtr readAction(int& length);
	FunctionPtr readFunction(int& length);
	bool isLetter(char c);
	bool isDigit(char c);
	bool isLetterOrDigit(char c);
	std::string trim(std::string& str);
};


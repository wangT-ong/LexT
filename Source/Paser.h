#pragma once
#include"Lexer.h"
#include<sstream>
#include<map>
#include<set>
#include<utility>
#include"NFS.h"
#include"RegularNode.h"



class Paser
{
public:
	Paser(std::wstring fileName);
	std::vector<RegularActionPair>getRegularActionList() {
		return regularActionList;
	}
	std::map<std::string, int>getConstTable() {
		return constTable;
	}
	std::vector<FunctionPtr>getFunctionList() {
		return functionList;
	}
private:

	Lexer lex;
	TokenPtr look;
	std::map<std::string, int>constTable;
	std::map<std::string, RegualrNodePtr>regularDefTable;
	std::vector<RegularActionPair>regularActionList;
	std::vector<FunctionPtr>functionList;
	void error(std::string s) 
	{
		std::ostringstream os;
		os<< "near line" <<lex.line << ": " << s;
		throw new std::runtime_error(os.str());
	}
	void move() { look = lex.scan(); }
	void match(int t) {
		move();
		_ASSERT(look.isValidate());
		if (look->tag != t)
			error("syntax error");		
	}
	TokenPtr peek();
	void clearWhiteSpace() { lex.clearWhiteSpace(); }
	void program();
	void decls();
	void rules();
	void rule();
	void constants();
	void constant();
	void regularDefs();
	void regularDef();
	void functions();
	RegualrNodePtr regular();
	RegualrNodePtr connection();
	RegualrNodePtr unitary();
	RegualrNodePtr factor();
	RegualrNodePtr R1();
	RegualrNodePtr R2();
	RegualrNodePtr R3();
	RegualrNodePtr regularRef();
	RegualrNodePtr characterClass();
	ActionPtr action();
	
	void setParseDecl();
	void setParseRegular();
	void setParseRegularDef();
	void setParseAction();
	void setParseFunction();
}; 
#pragma once
#include"Paser.h"

class Lex
{
public:
	Lex(std::wstring fileName) :m_paser(fileName) {};
	bool outPutResult(std::wstring outputPath);
private:
	Paser m_paser;
	bool outPutTransitionTable(std::wstring outPutPath);
	bool outPutLexer(std::wstring outPutPath);
	bool outPutLexerHeader(std::wstring outPutPath);
};
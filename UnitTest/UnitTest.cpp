//#define private public

#include "pch.h"
#include "CppUnitTest.h"
#include"Lexer.h"
#include"Paser.h"
#include"NFS.h"
#include"TransitionData.h"
#include"Lex.h"
#include<sstream>
#include<Windows.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;



class LexCompilerDummy
{
public:
	Lexer lex;
	TokenPtr look;
	std::map<std::string, int>constTable;
	std::map<std::string, RegualrNodePtr>regularDefTable;
	std::vector<RegularActionPair>regularActionList;
	std::vector<FunctionPtr>functionList;
};

class TransitionDataDummy
{
public:
	using Transitions = std::map<int, std::set<int>>;
	std::vector<Transitions> m_transitionTable;
	std::vector<int>m_finalStats;
	std::map<int, std::string>m_actions;
	int m_startState;
};

void printString(RegualrNodePtr nodePtr, std::ostringstream& os)
{
	if (!nodePtr.isValidate())
		return;
	TokenPtr tokenPtr = nodePtr->getToken();
	if (tokenPtr.isValidate())
	{
		switch (tokenPtr->tag)
		{
		case LETTER:
			os << LetterPtr(tokenPtr)->letter;
			break;
		case DIGITE:
			os << DigitPtr(tokenPtr)->digit;
			break;
		case NAME:
			os << NamePtr(tokenPtr)->lexeme;
			break;
		case NUM:
			os << NumPtr(tokenPtr)->value;
			break;
		case  TAB:
			os << "\t";
			break;
		case RETURN:
			os << "\n";
			break;
		case DOT:
			os << ".";
			break;
		case GE:
			os << ">=";
			break;
		case LE:
			os << "<=";
			break;
		case NE:
			os << "<>";
			break;
		default:
			os << char(tokenPtr->tag);
			break;
		}
	}
	else
	{
		std::vector<RegualrNodePtr> childs = nodePtr->getChilds();
		for (RegualrNodePtr& childPtr : childs) {
			printString(childPtr, os);
		}
	}
}


std::string printString(RegualrNodePtr nodePtr)
{
	std::ostringstream os;
	printString(nodePtr, os);
	return os.str();
}

namespace UnitTest
{
	TEST_CLASS(LexerTest)
	{
		TEST_METHOD(lexerTest)
		{
			Lexer lexer(L"D:\\学习\\编译原理\\UnitTest\\测试数据\\scanTest.txt");
			Assert::AreEqual(lexer.line, 1);
			lexer.setReadMultiChar();
			lexer.setClearSpace();
			NamePtr namePtr = lexer.peek();
			Assert::IsTrue(namePtr.isValidate());
			Assert::AreEqual(namePtr->lexeme, std::string("delim"));
			namePtr = lexer.scan();
			Assert::IsTrue(namePtr.isValidate());
			Assert::AreEqual(namePtr->lexeme ,std::string( "delim"));
			
			TokenPtr tokePtr= lexer.scan();
			Assert::IsTrue(tokePtr.isValidate());
			Assert::AreEqual(tokePtr->tag,int( '['));
			lexer.setNonClearSpace();
			tokePtr = lexer.scan();
			lexer.setReadSingleChar();
			lexer.setNonClearSpace();
			LetterPtr letterPtr;
			tokePtr = lexer.scan();
			Assert::AreEqual(tokePtr->tag, int(' '));
			tokePtr = lexer.scan();
			Assert::AreEqual(tokePtr->tag, int(TAB));
			tokePtr = lexer.scan();
			Assert::AreEqual(tokePtr->tag, int(RETURN));
			tokePtr = lexer.scan();
			Assert::AreEqual(tokePtr->tag, int(']'));
			lexer.setClearSpace();
			lexer.setReadMultiChar();
			namePtr = lexer.scan();
			Assert::AreEqual(namePtr->lexeme, std::string("w13"));
			Assert::AreEqual(lexer.line, 2);
			lexer.setReadMultiChar();
			NumPtr numPtr = lexer.scan();
			Assert::AreEqual(numPtr->value, 123);
			lexer.setReadSingleChar();
			DigitPtr digitPtr = lexer.scan();
			Assert::AreEqual(digitPtr->digit,char('0'+5));
			tokePtr = lexer.scan();
			Assert::AreEqual(tokePtr->tag, int('%'));
			tokePtr = lexer.scan();
			Assert::AreEqual(lexer.line, 3);
			Assert::AreEqual(tokePtr->tag, int('{'));
			tokePtr = lexer.scan();
			Assert::AreEqual(tokePtr->tag, int(GE));
			Assert::AreEqual(lexer.line, 4);	
			lexer.setReadAction(true);
			ActionPtr actionPtr = lexer.scan();
			Assert::AreEqual(actionPtr->tag, int(ACTION));
			Assert::AreEqual(actionPtr->action.c_str(), "{yylval=(int) installID();return(ID);}");
			lexer.setReadAction(false);
			lexer.setReadFunction(true);
			FunctionPtr function= lexer.scan();
			Assert::AreEqual(lexer.line, 10);
			Assert::AreEqual(function->tag, int(FUNCTION));
			Assert::AreEqual(function->toString().c_str(), "int installID(){\n\t\tint a=0;\n\t\ta++;\n\t\treturn a;\n\t}");
		}

		
	};

	TEST_CLASS(PaserTest)
	{
		TEST_METHOD(parseConstantsTest)
		{
			Paser paser(L"D:\\学习\\编译原理\\UnitTest\\测试数据\\parseConstanTest.txt");
			std::map<std::string, int> constTable = paser.getConstTable();
			Assert::IsTrue(constTable.find("LT") != constTable.end());
			Assert::AreEqual(constTable["LT"], 1);
			Assert::IsTrue(constTable.find("LE") != constTable.end());
			Assert::AreEqual(constTable["LE"], 2);
			Assert::IsTrue(constTable.find("EQ") != constTable.end());
			Assert::AreEqual(constTable["EQ"], 3);
			Assert::IsTrue(constTable.find("NE") != constTable.end());
			Assert::AreEqual(constTable["NE"], 4);
			Assert::IsTrue(constTable.find("GT") != constTable.end());
			Assert::AreEqual(constTable["GT"], 5);
			Assert::IsTrue(constTable.find("GE") != constTable.end());
			Assert::AreEqual(constTable["GE"], 6);
			Assert::IsTrue(constTable.find("LF") != constTable.end());
			Assert::AreEqual(constTable["LF"], 7);
			Assert::IsTrue(constTable.find("THEN") != constTable.end());
			Assert::AreEqual(constTable["THEN"], 8);
			Assert::IsTrue(constTable.find("ELSE") != constTable.end());
			Assert::AreEqual(constTable["ELSE"], 9);
			Assert::IsTrue(constTable.find("ID") != constTable.end());
			Assert::AreEqual(constTable["ID"], 10);
			Assert::IsTrue(constTable.find("NUMBER") != constTable.end());
			Assert::AreEqual(constTable["NUMBER"], 11);
			Assert::IsTrue(constTable.find("RELOP") != constTable.end());
			Assert::AreEqual(constTable["RELOP"], 12);
		}

		TEST_METHOD(parseRegularDefTest)
		{
			Paser paser(L"D:\\学习\\编译原理\\UnitTest\\测试数据\\parseRegularDefTest.txt");
			LexCompilerDummy* dummyCompiler = reinterpret_cast<LexCompilerDummy*>(&paser);
			Assert::IsTrue(dummyCompiler->regularDefTable.find("delim") != dummyCompiler->regularDefTable.end());
			RegualrNodePtr nodePtr = dummyCompiler->regularDefTable["delim"];
			std::string str = printString(nodePtr);
			Assert::AreEqual(str.c_str(), "[ \t\n]");
			Assert::IsTrue(dummyCompiler->regularDefTable.find("letter") != dummyCompiler->regularDefTable.end());
			nodePtr = dummyCompiler->regularDefTable["letter"];
			str = printString(nodePtr);
			Assert::AreEqual(str.c_str(), "[A-Za-z]");
			Assert::IsTrue(dummyCompiler->regularDefTable.find("digit") != dummyCompiler->regularDefTable.end());
			nodePtr = dummyCompiler->regularDefTable["digit"];
			str = printString(nodePtr);
			Assert::AreEqual(str.c_str(), "[0-9]");

			Assert::IsTrue(dummyCompiler->regularDefTable.find("id") != dummyCompiler->regularDefTable.end());
			nodePtr = dummyCompiler->regularDefTable["id"];
			str = printString(nodePtr);
			Assert::AreEqual(str.c_str(), "[A-Za-z]([A-Za-z]|[0-9])*");

			Assert::IsTrue(dummyCompiler->regularDefTable.find("number") != dummyCompiler->regularDefTable.end());
			nodePtr = dummyCompiler->regularDefTable["number"];
			str = printString(nodePtr);
			Assert::AreEqual(str.c_str(), "[0-9]+(.[0-9]+)?(E[+-]?[0-9]+)?");
		}

		TEST_METHOD(parseRuleTest)
		{
			Paser paser(L"D:\\学习\\编译原理\\UnitTest\\测试数据\\parseRuleTest.txt");
			std::vector<RegularActionPair> regularActionList = paser.getRegularActionList();
			RegularActionPair& pair = regularActionList[0];
			std::string str;
			str = printString(pair.first);
			str += pair.second->toString();
			Assert::AreEqual("[ \t\n]+{/*no action and no return*/}", str.c_str());

			pair = regularActionList[1];

			str = printString(pair.first);
			str += pair.second->toString();
			Assert::AreEqual("if{return(IF);}", str.c_str());

			pair = regularActionList[2];

			str = printString(pair.first);
			str += pair.second->toString();
			Assert::AreEqual("then{return(THEN);}", str.c_str());

			pair = regularActionList[3];

			str = printString(pair.first);
			str += pair.second->toString();
			Assert::AreEqual("else{return(ELSE);}", str.c_str());

			pair = regularActionList[4];
			str = printString(pair.first);
			str += pair.second->toString();
			Assert::AreEqual("[A-Za-z]([A-Za-z]|[0-9])*{yylval= (int) installID(); return(ID);}", str.c_str());

			pair = regularActionList[5];
			str = printString(pair.first);
			str += pair.second->toString();
			Assert::AreEqual("[0-9]+(.[0-9]+)?(E[+-]?[0-9]+)?{yylval= (int) installNum(); return(NUMBER);}", str.c_str());

			pair = regularActionList[6];
			str = printString(pair.first);
			str += pair.second->toString();
			Assert::AreEqual("<{yylval= LT; return(RELOP);}", str.c_str());

			pair = regularActionList[7];
			str = printString(pair.first);
			str += pair.second->toString();
			Assert::AreEqual("<={yylval= LE; return(RELOP);}", str.c_str());

			pair = regularActionList[8];
			str = printString(pair.first);
			str += pair.second->toString();
			Assert::AreEqual("={yylval= EQ; return(RELOP);}", str.c_str());

			pair = regularActionList[9];
			str = printString(pair.first);
			str += pair.second->toString();
			Assert::AreEqual("<>{yylval= NE; return(RELOP);}", str.c_str());

			pair = regularActionList[10];
			str = printString(pair.first);
			str += pair.second->toString();
			Assert::AreEqual(">{yylval= GT; return(RELOP);}", str.c_str());

			pair = regularActionList[11];
			str = printString(pair.first);
			str += pair.second->toString();
			Assert::AreEqual(">={yylval= GE; return(RELOP);}", str.c_str());
		}

		TEST_METHOD(parseFunctionTest)
		{
			Paser paser(L"D:\\学习\\编译原理\\UnitTest\\测试数据\\parseFunctionTest.txt");
			std::vector<FunctionPtr> functionList = paser.getFunctionList();
			FunctionPtr functionPtr = functionList[0];
			Assert::AreEqual(functionPtr->tag, int(FUNCTION));
			Assert::AreEqual("int installID(){\n\t\t\tint a=10;\n\t\t\ta++;\n\t\t\tint b=5;\n\t\t\tint c=a+b;\n\t\t\treturn c;\n\t\t}", functionPtr->toString().c_str());

			functionPtr = functionList[1];
			Assert::AreEqual(functionPtr->tag, int(FUNCTION));
			Assert::AreEqual("int installNum(){\n\t\t\tstd::vector<int> list;\n\t\t\tlist.push(1);\n\t\t\tlist.push(2);\n\t\t\treturn list;\n\t\t}", functionPtr->toString().c_str());

		}

		TEST_METHOD(transitionTableGenerateTest)
		{
			Paser paser(L"D:\\学习\\编译原理\\UnitTest\\测试数据\\TransitionTableGenerateTest.txt");
			TransitionData data (paser.getRegularActionList(), paser.getConstTable(),paser.getFunctionList());
			TransitionDataDummy* dummydata= reinterpret_cast<TransitionDataDummy*>(&data);
			//验证状态点2

			TransitionDataDummy::Transitions trans = dummydata->m_transitionTable[2];
			Assert::AreEqual(1, (int)trans.size());
			Assert::AreEqual(10, (int)trans[EMPTY].size());
			std::vector<int>states;
			std::copy(trans[EMPTY].begin(), trans[EMPTY].end(),std::back_inserter( states));
			for (int i=0;i<10;++i)
			{
				Assert::AreEqual(i + 3,states[i]);
			}

			//验证状态点24
			trans = dummydata->m_transitionTable[24];
			Assert::AreEqual(1, (int)trans.size());
			Assert::AreEqual(2, (int)trans[EMPTY].size());
			Assert::IsTrue(trans[EMPTY].find(25)!= trans[EMPTY].end());
			Assert::IsTrue(trans[EMPTY].find(26) != trans[EMPTY].end());

			//验证状态点26
			trans = dummydata->m_transitionTable[26];
			Assert::AreEqual(1, (int)trans.size());
			Assert::AreEqual(2, (int)trans[EMPTY].size());
			Assert::IsTrue(trans[EMPTY].find(28) != trans[EMPTY].end());
			Assert::IsTrue(trans[EMPTY].find(29) != trans[EMPTY].end());

			//验证状态点55
			trans = dummydata->m_transitionTable[55];
			Assert::AreEqual(1, (int)trans.size());
			Assert::AreEqual(1, (int)trans['-'].size());
			Assert::IsTrue(trans['-'].find(59) != trans['-'].end());

			//验证状态点31
			trans = dummydata->m_transitionTable[31];
			Assert::AreEqual(1, (int)trans.size());
			Assert::AreEqual(2, (int)trans[EMPTY].size());
			Assert::IsTrue(trans[EMPTY].find(42) != trans[EMPTY].end());
			Assert::IsTrue(trans[EMPTY].find(43) != trans[EMPTY].end());

			//验证状态点82
			trans = dummydata->m_transitionTable[82];
			Assert::AreEqual(1, (int)trans.size());
			Assert::AreEqual(2, (int)trans[EMPTY].size());
			Assert::IsTrue(trans[EMPTY].find(56) != trans[EMPTY].end());
			Assert::IsTrue(trans[EMPTY].find(83) != trans[EMPTY].end());
		}

	};

	TEST_CLASS(TransitionDataTest)
	{
		TEST_METHOD(OutPutHeaderTest)
		{
			std::ifstream aimfile("D:\\学习\\编译原理\\UnitTest\\测试数据\\outPutHeaderTest\\aimTransitionTable.h");
			Assert::IsTrue(aimfile.is_open());
			std::wstring outputPath = L"D:\\学习\\编译原理\\UnitTest\\测试数据\\outPutHeaderTest\\outputHeaderTest";
			Lex lex(L"D:\\学习\\编译原理\\UnitTest\\测试数据\\outPutHeaderTest\\outputHeaderTest.txt");
			lex.outPutResult(outputPath);
			std::ifstream outfile(L"D:\\学习\\编译原理\\UnitTest\\测试数据\\outPutHeaderTest\\outPutHeaderTest\\TransitionTable.h");
			Assert::IsTrue(outfile.is_open());

			while (aimfile && outfile)
			{
				std::string aimWord;
				aimfile >> aimWord;
				std::string word;
				outfile >> word;
				Assert::AreEqual(aimWord, word);
			}
			Assert::IsTrue(aimfile.eof()&&outfile.eof());
			outfile.close();
			if (GetFileAttributes(outputPath.c_str()) == FILE_ATTRIBUTE_DIRECTORY) {
				DeleteFile((outputPath + L"\\TransitionTable.cpp").c_str());
				DeleteFile((outputPath + L"\\TransitionTable.h").c_str());
				RemoveDirectory(outputPath.c_str());

			}
		}

		TEST_METHOD(OutPutSourceTest)
		{
			std::wstring outputPath = L"D:\\学习\\编译原理\\UnitTest\\测试数据\\outPutSourceTest\\outputSourceTest";
			Lex lex(L"D:\\学习\\编译原理\\UnitTest\\测试数据\\outPutSourceTest\\outputSourceTest.txt");
			lex.outPutResult(outputPath);
			std::ifstream aimfile(L"D:\\学习\\编译原理\\UnitTest\\测试数据\\outPutSourceTest\\outputSourceAimFile.cpp");
			Assert::IsTrue(aimfile.is_open());
			std::ifstream outfile(L"D:\\学习\\编译原理\\UnitTest\\测试数据\\outPutSourceTest\\outputSourceTest\\TransitionTable.cpp");
			Assert::IsTrue(aimfile.is_open());
			while (aimfile && outfile)
			{
				std::string aimWord;
				aimfile >> aimWord;
				std::string word;
				outfile >> word;
				Assert::AreEqual(aimWord, word);
			}
			Assert::IsTrue(aimfile.eof() && outfile.eof());
			outfile.close();
			if (GetFileAttributes(outputPath.c_str()) == FILE_ATTRIBUTE_DIRECTORY) {
				DeleteFile((outputPath + L"\\TransitionTable.cpp").c_str());
				DeleteFile((outputPath + L"\\TransitionTable.h").c_str());
				RemoveDirectory(outputPath.c_str());
	
			}
		}
	};
}

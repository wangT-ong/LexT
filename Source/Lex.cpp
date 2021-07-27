#include"Lex.h"
#include"TransitionData.h"
#include<Windows.h>
bool Lex::outPutResult(std::wstring outputPath)
{
    if (outputPath.back() != '\\'&& outputPath.back() != '/')
    {
        outputPath += '\\';
    }
    if (GetFileAttributes(outputPath.c_str()) != FILE_ATTRIBUTE_DIRECTORY) {
        bool flag = CreateDirectory(outputPath.c_str(), NULL);
        // flag 为 true 说明创建成功
    }

    if (!outPutTransitionTable(outputPath))
        return false;
	return true;
}


bool Lex::outPutTransitionTable(std::wstring outPutPath)
{
    TransitionData data(m_paser.getRegularActionList(), m_paser.getConstTable(), m_paser.getFunctionList());
    return data.outPut(outPutPath);
}

bool Lex::outPutLexer(std::wstring outputPath)
{
    return true;
}

bool Lex::outPutLexerHeader(std::wstring outputPath)
{
    outputPath += L"Lexer.h";
    std::ofstream file(outputPath);
    
    return true;
}
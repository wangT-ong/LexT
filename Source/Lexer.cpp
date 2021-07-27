#include"Lexer.h"
int Lexer::line = 1;

Lexer::Lexer(std::wstring fileName):m_buffer(fileName)
{

}

char Lexer::readch()
{
	return m_buffer.readch();
}

TokenPtr Lexer::peek()
{
	int length = 0;
	TokenPtr token = _peek(length);
	m_buffer.restore();
	return token;
}

TokenPtr Lexer::_peek(int& length)
{
	TokenPtr tokenPtr = nullptr;
	length = 0;
	if (m_isClearSpace)
		clearWhiteSpace(length);
	if(m_isReadFunction)
		return  readFunction(length);

	if (m_isReadAction)
		return readAction(length);
	if (tokenPtr.isValidate())
		return tokenPtr;
	if (m_isReadSingleChar)
	{
		tokenPtr = readLetter(length);
	}
	else
	{
		tokenPtr = readName(length);
	}
	if (tokenPtr.isValidate())
		return tokenPtr;
	if (m_isReadSingleChar)
	{
		tokenPtr = readDigit(length);
	}
	else
	{
		tokenPtr = readNumber(length);
	}

	if (tokenPtr.isValidate())
		return tokenPtr;

	return readSymbol(length);
}

TokenPtr Lexer::scan()
{ 
	int length = 0;
	TokenPtr tokenPtr = _peek(length);
	m_buffer.moveForward();
	return tokenPtr;
}

void Lexer::clearWhiteSpace()
{
	int length = 0;
	clearWhiteSpace(length);
	m_buffer.moveForward();
}

void Lexer::clearWhiteSpace(int& length)
{
	int numOfreadChar = 0;
	char c=readch();
	++numOfreadChar;
	while (c == '\t' || c == '\n' || c == ' ')
	{
		if (c == '\n')
			++line;
		c = readch();
		++numOfreadChar;
	}

	length += numOfreadChar - 1;
	m_buffer.rollBack();
}

NamePtr Lexer::readName(int& length)
{
	int numOfreadChar = 0;
	char c = readch();
	++numOfreadChar;
	if (isLetter(c))
	{
		std::string name;
		do {
			name += c;
			c = readch();
			++numOfreadChar;
		} while (isLetterOrDigit(c));
		length += numOfreadChar - 1;
		m_buffer.rollBack();
		return new Name(name, NAME);
	}
	else {
		m_buffer.rollBack();
		return nullptr;
	}
	
}

LetterPtr Lexer::readLetter(int& length)
{
	char c = readch();
	
	if (isLetter(c))
	{
		length += 1;
		return new Letter(c);
	}
	else {
		m_buffer.rollBack();
		return nullptr;
	}
}

NumPtr Lexer::readNumber(int& length)
{

	int numOfreadChar = 0;
	char c = readch();
	++numOfreadChar;
	if (isdigit(c))
	{
		int num = 0;
		do {
			num *= 10;
			num += c - '0';
			c = readch();
			++numOfreadChar;
		} while (isDigit(c));
		length+=numOfreadChar - 1;
		m_buffer.rollBack();
		return new Num(num);
	}
	else {
		m_buffer.rollBack();
		return nullptr;
	}
}

TokenPtr Lexer::readDigit(int& length)
{
	char c = readch();
	if (isdigit(c))
	{
		length += 1;
		return new Digit(c);
	}
	else {
		m_buffer.rollBack();
		return nullptr;
	}
}

TokenPtr Lexer::readSymbol(int& length)
{
	char c = readch();
	if (c == '\\')
	{
		c = readch();
		if (c == 't') {

			length += 2;
			return new Token(TAB);
		}
		else if (c == 'n') {
			length += 2;
			return  new Token(RETURN);
		}
		else if (c == '.') {
			length += 2;
			return  new Token(DOT);
		}
		else {
			length += 1;
			m_buffer.rollBack(1);
			return  new Token('\\');
		}

	}
	else if (c == '<')
	{
		c = readch();
		if (c == '=') {
			length += 2;
			return new Token(LE);
		}
		else if(c == '>')
		{
			length += 2;
			return new Token(NE);
		}
		else {
			length += 1;
			m_buffer.rollBack(1);
			return  new Token('<');
		}
	}
	else if (c == '>')
	{
		c = readch();
		if (c == '=') {

			length += 2;
			return new Token(GE);
		}
		else {
			length += 1;
			m_buffer.rollBack(1);
			return  new Token('>');
		}
			
	}

	length += 1;
	return new Token(c);
}

ActionPtr Lexer::readAction(int& length)
{
	int len = 0;
	char c = readch();
	if (c != '{') {
		m_buffer.rollBack();
		return nullptr;
	}
	std::string str;
	do {
		str += c;
		++len;
		c = readch();
		if (c == '\n')
			++line;
	} while (c != '}');
	str += c;
	++len;
	length += len;
	return new Action(str);
}

FunctionPtr Lexer::readFunction(int& length)
{
	int len = 0;
	char c = readch();
	if (!isLetter(c)) {
		m_buffer.rollBack();
		return nullptr;
	}
	std::string sig;
	do {
		sig += c;
		++len;
		c = readch();
	} while (c != '{');
	
	m_buffer.rollBack();
	bool oldState = m_isReadAction;
	m_isReadAction = true;
	ActionPtr actionPtr = readAction(len);
	length += len;
	m_isReadAction = oldState;
	sig = trim(sig);
	return new Function(sig,actionPtr);
	
}

bool Lexer::isLetter(char c)
{
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

bool Lexer::isDigit(char c)
{
	return c >= '0' && c <= '9';
}

bool Lexer::isLetterOrDigit(char c)
{
	return isDigit(c) || isLetter(c);
}


std::string Lexer::trim(std::string& str)
{
	int i = i = str.size()-1;
	while (i>=0&&(str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
	{
		--i;
	}
	return str.substr(0, i+1);
}

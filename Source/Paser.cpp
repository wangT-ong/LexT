#include"Paser.h"
#include<deque>
Paser::Paser(std::wstring fileName):lex(fileName)
{
	program();
}

void Paser::program()
{
	decls();
	match('%');
	match('%');
	rules();
	match('%');
	match('%');
	functions();
}

void Paser::decls()
{
	setParseDecl();
	match('%');
	match('{');
	constants();
	match('%');
	match('}');
	regularDefs();
}

void Paser::constants()
{

	TokenPtr tokenPtr = nullptr;
	while ((tokenPtr=peek()).isValidate()&&tokenPtr->tag == NAME) {
		
		constant();
		if ((tokenPtr = peek()).isValidate() && tokenPtr->tag != ',')
			break;
		match(',');
	}
}

void Paser::constant()
{
	match(NAME);
	NamePtr name = look;
	match('=');
	match(NUM);
	NumPtr number = look;
	constTable[name->lexeme] = number->value;
}

void Paser::regularDefs()
{
	TokenPtr tokenPtr = nullptr;
	while ((tokenPtr = lex.peek()).isValidate() && tokenPtr->tag == NAME)
	{
		regularDef();
		setParseRegularDef();
	}
}

RegualrNodePtr Paser::unitary()
{
	RegualrNodePtr regularyPtr = new RegualrNode();
	regularyPtr->addNode(factor());
	regularyPtr->addNode(R1());
	regularyPtr->setType(UNITARY);
	return regularyPtr;
}

RegualrNodePtr Paser::factor()
{
	RegualrNodePtr regularyPtr;
	TokenPtr tokenPtr = lex.peek();
	if(!tokenPtr.isValidate())
		error("syntax error");
	switch (tokenPtr->tag)
	{
	case LETTER:
	case DIGITE:
	case ' ':
	case DOT:
	case '<':
	case '>':
	case '=':
	case NE:
	case LE:
	case GE:
		match(tokenPtr->tag);
		regularyPtr = RegualrNodePtr(new RegualrNode(look));
		break;
	case '(':
		regularyPtr = RegualrNodePtr(new RegualrNode());
		match('(');
		regularyPtr->addNode(RegualrNodePtr(new RegualrNode(look)));
		regularyPtr->addNode(regular());
		match(')');
		regularyPtr->addNode(RegualrNodePtr(new RegualrNode(look)));
		regularyPtr->setType(BRACKET);
		break;
	case '[':
		regularyPtr = characterClass();
		break;
	case '{':
		match('{');
		regularyPtr = regularRef();
		match('}');
		break;
	case TAB:
		match(TAB);
		regularyPtr = RegualrNodePtr(new RegualrNode(look));
		break;
	case RETURN:
		match(RETURN);
		regularyPtr = RegualrNodePtr(new RegualrNode(look));
		break;
	
	default:
		error("syntax error");
	}
	return regularyPtr;
}

RegualrNodePtr Paser::regularRef()
{
	lex.setReadMultiChar();
	match(NAME);
	NamePtr namePtr = look;
	_ASSERT(namePtr.isValidate());
	if(regularDefTable.find(namePtr->lexeme)==regularDefTable.end())
		error("syntax error");
	return regularDefTable[namePtr->lexeme];
}



RegualrNodePtr Paser::R1()
{
	RegualrNodePtr regularyPtr = new RegualrNode();
	RegualrNodePtr currentNodePtr = regularyPtr;
	TokenPtr tokenPtr = nullptr;
	while ((tokenPtr = lex.peek()).isValidate() && (tokenPtr->tag == '*' || tokenPtr->tag == '?' ||
		tokenPtr->tag == '+'))
	{
		match(tokenPtr->tag);
		currentNodePtr->addNode(RegualrNodePtr(new RegualrNode(look)));
		RegualrNodePtr nodePtr = new RegualrNode();
		currentNodePtr->setType(UNITARY);
		currentNodePtr->addNode(nodePtr);
		currentNodePtr = nodePtr;
	}
	currentNodePtr->setType(UNITARY);
	currentNodePtr->addNode(nullptr);
	return regularyPtr;
}

RegualrNodePtr Paser::R2()
{
	RegualrNodePtr regularyPtr = new RegualrNode();
	RegualrNodePtr currentNodePtr = regularyPtr;
	TokenPtr tokenPtr = nullptr;
	while ((tokenPtr = lex.peek()).isValidate()&&(tokenPtr->tag==LETTER|| tokenPtr->tag == DIGITE||
			tokenPtr->tag == '('|| tokenPtr->tag == '['|| tokenPtr->tag == '{'))
	{
		currentNodePtr->addNode(unitary());
		RegualrNodePtr nodePtr = new RegualrNode();
		currentNodePtr->addNode(nodePtr);
		currentNodePtr->setType(CONNECTION);
		currentNodePtr = nodePtr;
	}
	currentNodePtr->setType(CONNECTION);
	currentNodePtr->addNode(nullptr);
	return regularyPtr;
}

RegualrNodePtr Paser::R3()
{
	RegualrNodePtr regularyPtr = new RegualrNode();
	RegualrNodePtr currentNodePtr = regularyPtr;
	TokenPtr tokenPtr = nullptr;
	while ((tokenPtr = lex.peek()).isValidate() && tokenPtr->tag == '|') {
		match('|');
		currentNodePtr->addNode(RegualrNodePtr(new RegualrNode(look)));
		currentNodePtr->addNode(connection());
		RegualrNodePtr nodePtr = new RegualrNode();
		currentNodePtr->addNode(nodePtr);
		currentNodePtr->setType(UNION);
		currentNodePtr = nodePtr;
	}
	currentNodePtr->setType(UNION);
	currentNodePtr->addNode(nullptr);

	return regularyPtr;
}

RegualrNodePtr Paser::regular()
{
	clearWhiteSpace();
	setParseRegular();
	RegualrNodePtr regularyPtr = new RegualrNode();
	regularyPtr->addNode(connection());
	regularyPtr->addNode(R3());
	regularyPtr->setType(UNION);
	return regularyPtr;
}

RegualrNodePtr Paser::connection()
{
	RegualrNodePtr regularyPtr = new RegualrNode();
	regularyPtr->addNode(unitary());
	regularyPtr->addNode(R2());
	regularyPtr->setType(CONNECTION);
	return regularyPtr;
}


void Paser::regularDef()
{
	match(NAME);
	NamePtr namePtr = look;
	_ASSERT(namePtr.isValidate());
	regularDefTable[namePtr->lexeme]= regular();
}

RegualrNodePtr Paser::characterClass()
{
	RegualrNodePtr regularyPtr= new RegualrNode();
	match('[');
	regularyPtr->addNode(RegualrNodePtr(new RegualrNode(look)));
	TokenPtr tokenPtr = nullptr;
	while (((tokenPtr = lex.peek()).isValidate()) && tokenPtr->tag !=']')
	{
		switch (tokenPtr->tag)
		{
		case DIGITE:
			match(DIGITE);
			regularyPtr->addNode(RegualrNodePtr(new RegualrNode(look)));
			tokenPtr = lex.peek();
			if (tokenPtr->tag == '-') {
				match('-');
				regularyPtr->addNode(RegualrNodePtr(new RegualrNode(look)));
				match(DIGITE);
				regularyPtr->addNode(RegualrNodePtr(new RegualrNode(look)));
			}
			break;
		case LETTER:
			match(LETTER);
			regularyPtr->addNode(RegualrNodePtr(new RegualrNode(look)));
			tokenPtr = lex.peek();
			if (tokenPtr->tag == '-') {
				match('-');
				regularyPtr->addNode(RegualrNodePtr(new RegualrNode(look)));
				match(LETTER);
				regularyPtr->addNode(RegualrNodePtr(new RegualrNode(look)));
			}
			break;
		default:
			match(tokenPtr->tag);
			regularyPtr->addNode(RegualrNodePtr(new RegualrNode(look)));
		}
	}
	match(']');
	regularyPtr->addNode(RegualrNodePtr(new RegualrNode(look)));
	regularyPtr->setType(CHARACTORCLASS);
	return regularyPtr;
}

void Paser::rules()
{
	RegualrNodePtr regularyPtr = new RegualrNode();
	RegualrNodePtr currentNodePtr = regularyPtr;
	TokenPtr tokenPtr = nullptr; 
	
	while ((tokenPtr = lex.peek()).isValidate() && (tokenPtr->tag == '{'|| tokenPtr->tag == '('|| tokenPtr->tag =='\"'||
		tokenPtr->tag == LETTER|| tokenPtr->tag==DIGITE)) {
		rule();
	}
}

void Paser::rule()
{
	TokenPtr tokenPtr =  lex.peek();
	bool hasQuotation = false;
	_ASSERT(tokenPtr.isValidate());
	RegualrNodePtr nodePtr;

	switch (tokenPtr->tag)
	{
	case '{':
		match('{');
		nodePtr = regularRef();
		match('}');
		lex.setNonClearSpace();
		tokenPtr= peek();
		if(!tokenPtr.isValidate())
			error("syntax error");
		if (tokenPtr->tag == SPACE) {
			match(SPACE);
		}
		else if(tokenPtr->tag == TAB)
			match(TAB);
		else
			error("syntax error");

		lex.setClearSpace();
		break;
	case '\"':
		match('\"');
		nodePtr = regular();
		match('\"');
		break;
	default:
		nodePtr = regular();
	}
	ActionPtr actionPtr = action();
	regularActionList.push_back(std::make_pair(nodePtr, actionPtr));
	lex.setClearSpace();
	lex.setReadSingleChar();
	lex.setReadAction(false);
}

ActionPtr Paser::action()
{
	setParseAction();
	match(ACTION);
	return look;
}

void Paser::functions()
{
	setParseFunction();
	TokenPtr tokenPtr = nullptr;
	while ((tokenPtr = lex.peek()).isValidate() && (tokenPtr->tag == FUNCTION))
	{
		match(FUNCTION);
		FunctionPtr functionPtr = look;
		functionList.push_back(functionPtr);
	}
}


TokenPtr Paser::peek()
{
	return lex.peek();
}

void Paser::setParseDecl()
{
	lex.setClearSpace();
	lex.setReadMultiChar();
	lex.setReadAction(false);
	lex.setReadFunction(false);
}

void Paser::setParseRegular()
{
	lex.setNonClearSpace();
	lex.setReadSingleChar();
	lex.setReadAction(false);
	lex.setReadFunction(false);
}

void Paser::setParseRegularDef()
{
	lex.setClearSpace();
	lex.setReadMultiChar();
	lex.setReadAction(false);
	lex.setReadFunction(false);
}

void Paser::setParseAction()
{
	lex.setClearSpace();
	lex.setReadAction(true);
	lex.setReadFunction(false);
}

void Paser::setParseFunction()
{
	lex.setClearSpace();
	lex.setReadAction(false);
	lex.setReadFunction(true);
}





#pragma once 
#include<vector>
#include"SmartPointer.h"
#include"Lexer.h"
class RegualrNode;
using RegualrNodePtr = SmartPointer<RegualrNode>;
using RegularActionPair = std::pair<RegualrNodePtr, ActionPtr>;
enum NodeType{UNION,CONNECTION,UNITARY,CHARACTORCLASS,BRACKET,BASIC};
class RegualrNode
{
public:
	RegualrNode(TokenPtr token = nullptr, NodeType t = BASIC) :tokenPtr(token), type(t) {};
	void addNode(RegualrNodePtr nodePtr) {
		childs.push_back(nodePtr);
	}
	TokenPtr getToken()
	{
		return tokenPtr;
	}
	std::vector<RegualrNodePtr> getChilds() { return childs; }
	NodeType getType() { return type; }
	void setType(NodeType t) { type=t; }
private:
	std::vector<RegualrNodePtr>childs;
	TokenPtr tokenPtr;
	NodeType type;
};

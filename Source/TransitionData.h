#pragma once
#include<map>
#include<set>
#include<vector>
#include"NFS.h"
class TransitionData
{
public:
	TransitionData(std::vector<RegularActionPair> regularActionPair, std::map<std::string, int>constTable, std::vector<FunctionPtr>functionList);
	TransitionData() {};
	bool outPut(std::wstring outPutPath);
private:
	using Transitions = std::map<int, std::set<int>>;
	bool outPutHeader(std::wstring outPutPath);
	bool outPutSource(std::wstring outPutPath);
	void outPutInitialFun(std::ofstream& file);
	void outPutInitialTransTableFun(std::ofstream& file);
	void outPutInitialActionsFun(std::ofstream& file);
	void outPutInitialFinalStates(std::ofstream& file);
	void outPutFunctions(std::ofstream& file);
	void outPutMoveFun(std::ofstream& file);
	void outPutGetActionFun(std::ofstream& file);
	void outPutIsFinalStateFun(std::ofstream& file);
	void outPutConstructor(std::ofstream& file);
	bool outPutTransitionTable(std::ofstream& file);
	bool outPutTranstions(std::ofstream& file,Transitions& transitions);
	void outPutTransiton(std::ofstream& file,int c,std::set<int> states);
	void outPutActions(std::ofstream& file);

	void outPutFunctionDef(std::ofstream& file);

	void outPutConstDef(std::ofstream& file);
	std::map<std::string, int>m_constTable;
	std::vector<FunctionPtr> m_functionList;
	std::vector<Transitions> m_transitionTable;
	std::vector<int>m_finalStats;
	std::map<int, std::string>m_actions;
	int m_startState;
};



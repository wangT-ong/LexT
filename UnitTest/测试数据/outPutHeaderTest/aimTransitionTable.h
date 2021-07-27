#pragma once
#include<map>
#include<set>
#include<vector>
#include<string>
#include<functional>
#include<stdexcept>
#include<sstream>
#include<algorithm>
extern int yylval;
class TransitionData
{
public:
	enum{ELSE=9,EQ=3,GE=6,GT=5,ID=10,LE=2,LF=7,LT=1,NE=4,NUMBER=11,RELOP=12,THEN=8,EMPTY=279};
	using Action = std::function<int(void)>;
	TransitionData();
	std::set<int> move(std::set<int>&states,int c);
	bool isFinalState(int state);
	Action getAction(int state);
	int getStartState(){return m_startState;};
private:
	using Transitions = std::map<int, std::set<int>>;
	std::vector<Transitions> m_transitionTable;
	std::vector<int>m_finalStates;
	std::map<int,Action>m_actions;
	int m_startState;
	void initial();
	void initialTransTable();
	void initialActions();
	void initialFinalStates();
	void checkState(int state)
	{
		if (state >= m_transitionTable.size()) {
			std::ostringstream os;
			os << state << "不是有效的状态值！" << std::endl;
			throw std::invalid_argument(os.str());
		}
	}
	int installID();
	int installNum();
};

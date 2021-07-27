#include"TransitionData.h"
#include<sstream>

TransitionData::TransitionData(std::vector<RegularActionPair> regularActionPair, std::map<std::string, int>constTable, std::vector<FunctionPtr>functionList):
m_functionList(functionList),m_constTable(constTable)
{
	NFS nfs = NFSGenerator::generateNFS(regularActionPair);
	if (nfs.isEmpty())
		return;
	m_transitionTable.resize(nfs.size());
	std::deque<VertexNodePtr> queue;
	VertexNodePtr startState = nfs.getStartState();
	std::set<int>visited;
	queue.push_back(startState);
	while (!queue.empty())
	{
		VertexNodePtr state = queue.front();
		_ASSERT(state.isValidate());
		queue.pop_front();
		if (visited.find(state->id) != visited.end())
			continue;
		visited.insert(state->id);
		std::vector<Edge>edges = state->edges;
		for (Edge& edge : edges)
		{
			VertexNodePtr adjState = edge.first;
			_ASSERT(adjState.isValidate());
			m_transitionTable[state->index][edge.second].insert(adjState->index);
			queue.push_back(edge.first);
		}
	}

	std::vector<VertexNodePtr>finalStates = nfs.getFinalStates();

	for (int i=0;i<finalStates.size();++i)
	{
		VertexNodePtr finalState = finalStates[i];
		_ASSERT(finalState.isValidate());
		m_finalStats.push_back(finalState ->index);
		m_actions.insert(std::make_pair(finalState->index, regularActionPair[i].second->action));
	}
	startState = nfs.getStartState();
	_ASSERT(startState.isValidate());
	m_startState = startState->index;
}


bool TransitionData::outPut(std::wstring outPutPath)
{
	outPutHeader(outPutPath);
	outPutSource(outPutPath);
	return true;
}

bool TransitionData::outPutHeader(std::wstring outPutPath)
{
	outPutPath += L"TransitionTable.h";
	std::ofstream file(outPutPath);
	if (!file)
		return false;
	file << "#pragma once" << std::endl;
	file << "#include<map>" << std::endl;
	file << "#include<set>" << std::endl;
	file << "#include<vector>" << std::endl;
	file << "#include<string>" << std::endl;
	file << "#include<functional>" << std::endl;
	file<< "#include<stdexcept>" << std::endl;
	file << "#include<sstream>" << std::endl;
	file<< "#include<algorithm>" << std::endl;
	file << "extern int yylval;" << std::endl;
	file << "class TransitionData" << std::endl;
	file<<"{"<< std::endl;
	file<<"public:" << std::endl;
	outPutConstDef(file);
	file << "	using Action = std::function<int(void)>;" << std::endl;
	file << "	TransitionData();" << std::endl;
	file << "	std::set<int> move(std::set<int>&states,int c);" << std::endl;
	file << "	bool isFinalState(int state);" << std::endl;
	file << "	Action getAction(int state);" << std::endl;
	file << "	int getStartState(){return m_startState;};" << std::endl;
	file << "private:" << std::endl;
	file << "	using Transitions = std::map<int, std::set<int>>;" << std::endl;
	file << "	std::vector<Transitions> m_transitionTable;" << std::endl;
	file << "	std::vector<int>m_finalStates;" << std::endl;
	file << "	std::map<int,Action>m_actions;" << std::endl;
	file << "	int m_startState;" << std::endl;
	file << "	void initial();" << std::endl;
	file << "	void initialTransTable();" << std::endl;
	file << "	void initialActions();" << std::endl;
	file << "	void initialFinalStates();" << std::endl;
	file << "	void checkState(int state)" << std::endl;
	file << "	{" << std::endl;
	file << "		if (state >= m_transitionTable.size()) {" << std::endl;
	file << "			std::ostringstream os;" << std::endl;
	file << "			os << state << \"不是有效的状态值！\" << std::endl;" << std::endl;
	file << "			throw std::invalid_argument(os.str());" << std::endl;
	file << "		}" << std::endl;
	file << "	}" << std::endl;
	outPutFunctionDef(file);
	file << "};" << std::endl;
	return true;
}

void TransitionData::outPutFunctionDef(std::ofstream& file)
{
	for (FunctionPtr funPtr : m_functionList)
	{
		_ASSERT(funPtr.isValidate());
		file<<"	"<< funPtr->signature;
		file << ";" << std::endl;
	}	
}

void TransitionData::outPutConstDef(std::ofstream& file)
{
	if (m_constTable.empty())
		return;
	file << "	enum{";
	auto itor = m_constTable.begin();
	while (itor != m_constTable.end())
	{
		file << itor->first << "=" << itor->second;
		file << ",";
		++itor;
	}
	file << "EMPTY="<<Tag::EMPTY;
	file << "};" << std::endl;
}

bool TransitionData::outPutSource(std::wstring outPutPath)
{
	std::string headerFileName =  "TransitionTable.h";
	outPutPath += L"TransitionTable.cpp";
	std::ofstream file(outPutPath);
	if (!file)
		return false;
	file << "#include \"" << headerFileName.c_str()<<"\"" <<std::endl;
	file << "int yylval=-1;" << std::endl;
	outPutConstructor(file);
	outPutInitialFun(file);
	outPutInitialTransTableFun(file);
	outPutInitialActionsFun(file);
	outPutInitialFinalStates(file);
	outPutMoveFun(file);
	outPutGetActionFun(file);
	outPutIsFinalStateFun(file);
	outPutFunctions(file);
	return true;
}

bool TransitionData::outPutTransitionTable(std::ofstream& file)
{
	file << "	Transitions transitions;" << std::endl;
	for (int i=0;i<m_transitionTable.size();++i)
	{
		file << "	transitions=";
		outPutTranstions(file, m_transitionTable[i]);
		file <<";"<< std::endl;
		file << "	m_transitionTable.push_back(transitions);" << std::endl;
	}
	return true;
}

void TransitionData::outPutTransiton(std::ofstream& file, int c, std::set<int> states)
{
	file << "{" << c << ",{";
	auto itor = states.begin();
	if (!states.empty())
	{
		do {
			file << *itor;
			if (++itor != states.end())
				file << ",";
		} while (itor != states.end());
	}
	file << "}}" ;
}

bool TransitionData::outPutTranstions(std::ofstream& file, Transitions& transitions)
{

	auto itor = transitions.begin();
	file << "{";
	if (!transitions.empty()) {
		do {
			outPutTransiton(file, itor->first, itor->second);
			if (++itor != transitions.end())
				file << "," << std::endl;
		} while (itor != transitions.end());
	}
	file << "}" ;
	return true;
}


void TransitionData::outPutInitialFun(std::ofstream& file)
{
	file << "void TransitionData::initial(){" << std::endl;
	file<<"	initialTransTable();"<< std::endl;
	file << "	initialActions();" << std::endl;
	file << "	initialFinalStates();" << std::endl;
	file << "}" << std::endl;

}

void TransitionData::outPutInitialTransTableFun(std::ofstream& file)
{
	file << "void TransitionData::initialTransTable(){" << std::endl;
	outPutTransitionTable(file);
	file << std::endl;
	file << "}" << std::endl;
}

void TransitionData::outPutInitialActionsFun(std::ofstream& file)
{
	file << "void TransitionData::initialActions(){" << std::endl;
	file << "	m_actions=";
	outPutActions(file);
	file <<";"<< std::endl;
	file << "}" << std::endl;
}

void TransitionData::outPutActions(std::ofstream& file)
{
	file << "{"<<std::endl;
	for (int i = 0; i < m_finalStats.size(); ++i)
	{
		int finalState = m_finalStats[i];
		file << "		{" << finalState << ",[this]" << m_actions[finalState] << "}";
		if (i != m_finalStats.size() - 1)
			file << "," << std::endl;
	}
	file <<std::endl<< "	}";
}

void TransitionData::outPutFunctions(std::ofstream& file)
{
	for (FunctionPtr funPtr : m_functionList)
	{
		_ASSERT(funPtr.isValidate());
		int pos = funPtr->signature.find_first_of(" ");
		if(pos==std::string::npos)
			pos= funPtr->signature.find_first_of("	");
		if (pos == std::string::npos)
			throw std::runtime_error("函数签名有误！");
		file << funPtr->signature.substr(0,pos);
		file << " TransitionData::";
		file << funPtr->signature.substr(pos + 1);
		file << funPtr->actionPtr->action;
		file <<  std::endl;
	}
}


void TransitionData::outPutConstructor(std::ofstream& file)
{
	file << "TransitionData::TransitionData():m_startState("<<m_startState<<"){" << std::endl;
	file << "	initial();" << std::endl;
	file << "}" << std::endl;
}

void TransitionData::outPutMoveFun(std::ofstream& file)
{
	file << "std::set<int> TransitionData::move(std::set<int>&states,int c){" << std::endl;
	file << "	std::set<int>newStates;" << std::endl;
	file << "	for (int state : states)" << std::endl;
	file << "	{" << std::endl;
	file << "		checkState(state);" << std::endl;
	file << "		Transitions trans = m_transitionTable[state];" << std::endl;
	file << "		if (trans.find(c) == trans.end())" << std::endl;
	file << "			continue;" << std::endl;
	file << "		std::set<int>& tempStates = trans[c];" << std::endl;
	file << "		for (int s : tempStates)" << std::endl;
	file << "		{" << std::endl;
	file << "			newStates.insert(s);" << std::endl;
	file << "		}" << std::endl;
	file << "	}" << std::endl;
	file << "	return newStates;" << std::endl;
	file << "}" << std::endl;
}

void TransitionData::outPutGetActionFun(std::ofstream& file)
{
	file << "TransitionData::Action TransitionData::getAction(int state)" << std::endl;
	file << "{" << std::endl;
	file << "	checkState(state);" << std::endl;
	file << "	if (m_actions.find(state) == m_actions.end())" << std::endl;
	file << "	{" << std::endl;
	file << "		std::ostringstream os;" << std::endl;
	file << "		os << state << \"不是终止状态\" << std::endl;" << std::endl;
	file << "		throw std::invalid_argument(os.str());" << std::endl;
	file << "	}" << std::endl;
	file << "	return m_actions[state];" << std::endl;
	file << "}" << std::endl;
}

void TransitionData::outPutIsFinalStateFun(std::ofstream& file)
{
	file << "bool TransitionData::isFinalState(int state)" << std::endl;
	file << "{" << std::endl;
	file << "	checkState(state);" << std::endl;
	file << "	return std::find(m_finalStates.begin(),m_finalStates.end(),state)!=m_finalStates.end();" << std::endl;
	file << "}" << std::endl;
}

void TransitionData::outPutInitialFinalStates(std::ofstream& file)
{
	file << "void TransitionData::initialFinalStates()" << std::endl;
	file << "{" << std::endl;
	file << "	m_finalStates={";
	for (int i=0;i<m_finalStats.size();++i)
	{
		file <<m_finalStats[i];
		if (i != m_finalStats.size() - 1)
			file << ",";
	}
	file << "};"<<std::endl;
	file << "}" << std::endl;
}


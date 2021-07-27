#include"NFS.h"
#include<fstream>
#include<sstream>
#include<iostream>
#include<set>
#include<deque>
#include"Lexer.h"
using namespace std;

int VertexNode::numberOfVertex = 0;

NFS::NFS()
{

}

NFS::NFS(int tag)
{
	VertexNodePtr startState = new VertexNode();
	VertexNodePtr finalState = new VertexNode();

	addEdge(startState, finalState, tag);
	setStartState(startState);
	std::vector<VertexNodePtr>finalStates;
	finalStates.push_back(finalState);
	setFinalStates(finalStates);
	m_size = 2;
}

NFS::NFS(int firstTag, int lastTag)
{
	VertexNodePtr startState = new VertexNode();
	VertexNodePtr finalState = new VertexNode();
	setStartState(startState);
	std::vector<VertexNodePtr>finalStates;
	finalStates.push_back(finalState);
	setFinalStates(finalStates);
	_ASSERT(firstTag <= lastTag);
	m_size = 2;
	for (int c = firstTag; c <= lastTag; ++c)
	{
		NFS nfs(c);
		mergeNFS(nfs);
	}
}

void NFS::addEdge(VertexNodePtr v, VertexNodePtr w, int weight)
{
	if (!v.isValidate() || !w.isValidate())
		throw std::invalid_argument("点为空！");
	v->edges.push_back(std::make_pair(w, weight));
}




void NFS::unionNFS(NFS& nfs)
{
	if (size() == 0 || nfs.size() == 0)
		return;
	VertexNodePtr newStartState = new VertexNode();
	VertexNodePtr startState = getStartState();
	addEdge(newStartState, startState, EMPTY);
	startState = nfs.getStartState();
	addEdge(newStartState, startState, EMPTY);
	vector<VertexNodePtr>finalStates = getFinalStates();
	VertexNodePtr newFinalState = new VertexNode();
	for (VertexNodePtr state : finalStates)
	{
		addEdge(state, newFinalState, EMPTY);
	}
	finalStates = nfs.getFinalStates();
	for (VertexNodePtr state : finalStates)
	{
		addEdge(state, newFinalState, EMPTY);
	}
	setStartState(newStartState);
	finalStates.clear();
	finalStates.push_back(newFinalState);
	setFinalStates(finalStates);
	m_size += nfs.m_size + 2;
}

void NFS::connectNFS(NFS& nfs)
{
	if (size() == 0 || nfs.size() == 0)
		return;
	VertexNodePtr startState = nfs.getStartState();
	if (!startState.isValidate())
		return;
	*finalStates[0] = *startState;
	std::vector<VertexNodePtr>finalStates= nfs.getFinalStates();
	setFinalStates(finalStates);
	m_size += nfs.m_size -1;
}

void NFS::makeClosure()
{
	if (size() == 0)
		return;
	VertexNodePtr newStartState = new VertexNode();
	VertexNodePtr startState = getStartState();
	vector<VertexNodePtr>finalStates = getFinalStates();
	VertexNodePtr newFinalState = new VertexNode();
	_ASSERT(finalStates.size() == 1);
	addEdge(finalStates[0], startState, EMPTY);
	addEdge(newStartState, startState, EMPTY);
	addEdge(newStartState, newFinalState, EMPTY);
	addEdge(finalStates[0], newFinalState, EMPTY);
	setStartState(newStartState);
	finalStates.clear();
	finalStates.push_back(newFinalState);
	setFinalStates(finalStates);
	m_size += 2;
}


void NFS::makePlusMark()
{
	if (size() == 0)
		return;
	VertexNodePtr newStartState = new VertexNode();
	VertexNodePtr startState = getStartState();
	vector<VertexNodePtr>finalStates = getFinalStates();
	VertexNodePtr newFinalState = new VertexNode();
	_ASSERT(finalStates.size() == 1);
	addEdge(finalStates[0], startState, EMPTY);
	addEdge(newStartState, startState, EMPTY);
	addEdge(finalStates[0], newFinalState, EMPTY);
	setStartState(newStartState);
	finalStates.clear();
	finalStates.push_back(newFinalState);
	setFinalStates(finalStates);
	m_size += 2;
}


void NFS::makeQuestionMark()
{
	if (size() == 0)
		return;
	VertexNodePtr newStartState = new VertexNode();
	VertexNodePtr startState = getStartState();
	vector<VertexNodePtr>finalStates = getFinalStates();
	VertexNodePtr newFinalState = new VertexNode();
	_ASSERT(finalStates.size() == 1);
	addEdge(newStartState, startState, EMPTY);
	addEdge(newStartState, newFinalState, EMPTY);
	addEdge(finalStates[0], newFinalState, EMPTY);
	setStartState(newStartState);
	finalStates.clear();
	finalStates.push_back(newFinalState);
	setFinalStates(finalStates);
	m_size += 2;
}


void NFS::mergeNFS(NFS& nfs, bool isClosed)
{
	if (nfs.size() == 0)
		return;
	if (m_size == 0) {
		*this = nfs;
		return;
	}
	VertexNodePtr state_start = nfs.getStartState();
	vector<VertexNodePtr>states_final = nfs.getFinalStates();
	_ASSERT(states_final.size() == 1);
	addEdge(startState, state_start, EMPTY);
	if (isClosed) {
		_ASSERT(finalStates.size() == 1);
		addEdge(states_final[0], finalStates[0], EMPTY);
	}
	else {
		finalStates.push_back(states_final[0]);
	}
	m_size += nfs.m_size;
}





bool  NFSGenerator::generateNFS(RegualrNodePtr regularNode, NFS& nfs)
{
	if (!regularNode.isValidate())
		return false;
	switch (regularNode->getType())
	{
	case UNION:
		return buildUnionNFS(regularNode, nfs);
	case CONNECTION:
		return buildConnectionNFS(regularNode, nfs);
	case UNITARY:
		return  buildUnitaryNFS(regularNode, nfs);
	case BRACKET:
		return  buildBracketNFS(regularNode, nfs);
	case BASIC:
		return buildBasicNFS(regularNode, nfs);
	case CHARACTORCLASS:
		return buildCharactorClassNFS(regularNode, nfs);
	default:
		throw std::invalid_argument("节点标签值有误！");
	}
}


bool NFSGenerator::buildUnionNFS(RegualrNodePtr regularNode, NFS& nfs)
{
	_ASSERT(regularNode.isValidate());
	std::vector<RegualrNodePtr>childs = regularNode->getChilds();
	_ASSERT(childs.size() == 1 || childs.size() == 2|| childs.size() == 3);
	RegualrNodePtr leftNode = childs[0];
	if (!leftNode.isValidate())
		return true;

	TokenPtr tokenPtr = leftNode->getToken();
	int connectionNodeIndex = 0;
	if (tokenPtr.isValidate()&&tokenPtr->tag == '|')
		++connectionNodeIndex;
	if (!generateNFS(childs[connectionNodeIndex], nfs))
		return false;
	RegualrNodePtr rightChild = childs[connectionNodeIndex+1];
	if (!rightChild.isValidate())
		return true;

	NFS right;
	if (!generateNFS(rightChild, right))
		return false;
	if(right.size()>0)
		nfs.unionNFS(right);
	return true;
}

bool NFSGenerator::buildConnectionNFS(RegualrNodePtr regularNode, NFS& nfs)
{
	_ASSERT(regularNode.isValidate());
	std::vector<RegualrNodePtr>childs = regularNode->getChilds();
	_ASSERT(childs.size() == 1||childs.size() == 2);
	RegualrNodePtr leftNode= childs[0];
	if (!leftNode.isValidate())
		return true;
	if (!generateNFS(childs[0], nfs))
		return false;
	RegualrNodePtr rightChild = childs[1];
	if (!rightChild.isValidate())
		return true;
	NFS right;
	if (!generateNFS(rightChild, right))
		return false;
	if(right.size()>=0)
		nfs.connectNFS(right);
	return true;
}


bool NFSGenerator::buildUnitaryNFS(RegualrNodePtr regularNode, NFS& nfs)
{
	_ASSERT(regularNode.isValidate());
	std::vector<RegualrNodePtr>childs = regularNode->getChilds();
	_ASSERT(childs.size() == 1||childs.size() == 2);
	if (!generateNFS(childs[0], nfs))
		return false;

	RegualrNodePtr rightChild = childs[1];
	while (rightChild.isValidate()) {
		childs = rightChild->getChilds();
		RegualrNodePtr leftChild = childs[0];
		if (!leftChild.isValidate())
			break;
		TokenPtr tokenPtr = leftChild->getToken();
		_ASSERT(tokenPtr.isValidate());
		int tag = tokenPtr->tag;

		_ASSERT(tag == '*' || tag == '?' || tag == '+');
		switch (tag)
		{
		case '*':
			nfs.makeClosure();
			break;
		case '?':
			nfs.makeQuestionMark();
			break;
		case '+':
			nfs.makePlusMark();
			break;
		}
		rightChild = childs[1];
	}
	return true;
}

bool NFSGenerator::buildBracketNFS(RegualrNodePtr regularNode, NFS& nfs)
{
	_ASSERT(regularNode.isValidate());
	std::vector<RegualrNodePtr>childs = regularNode->getChilds();
	_ASSERT(childs.size() == 3);
	_ASSERT(childs[0]->getToken()->tag == '(' && childs[2]->getToken()->tag == ')');
	return generateNFS(childs[1],nfs);
}

bool NFSGenerator::buildBasicNFS(RegualrNodePtr regularNode, NFS& nfs)
{
	_ASSERT(regularNode.isValidate());
	
	TokenPtr token = regularNode->getToken();
	
	int tag = getTag(token);
	nfs=NFS(tag);
	return true;
}


bool NFSGenerator::buildCharactorClassNFS(RegualrNodePtr regularNode, NFS& nfs)
{
	_ASSERT(regularNode.isValidate());
	std::vector<RegualrNodePtr>childs = regularNode->getChilds();
	_ASSERT(childs.size() > 2);
	_ASSERT(childs[0]->getToken()->tag == '[' && childs.back()->getToken()->tag == ']');
	VertexNodePtr startState = new VertexNode;
	std::vector<VertexNodePtr>finalStates;
	finalStates.push_back(VertexNodePtr(new VertexNode));
	nfs = NFS();
	nfs.setStartState(startState);
	nfs.setFinalStates(finalStates);
	int i = 1;
	while (i <int(childs.size() - 1))
	{
		TokenPtr token = childs[i]->getToken();
		int tag=getTag(token);
		
		if (++i > int(childs.size() - 1))
			break;
		TokenPtr newToken = childs[i]->getToken();
		_ASSERT(newToken.isValidate());
		if (newToken->tag != '-') {
			NFS subNFS(tag);
			nfs.mergeNFS(subNFS);
			continue;
		}
		_ASSERT(i <int(childs.size() - 1));
		newToken = childs[i+1]->getToken();

		int lastTag = getTag(newToken);
		if (lastTag == ']') {
			NFS subNFS_1(tag);
			nfs.mergeNFS(subNFS_1);
			NFS subNFS_2('-');
			nfs.mergeNFS(subNFS_2);
			break;
		}
			
		
		for (int c = tag; c <= lastTag; ++c) {
			NFS subNFS(c);
			nfs.mergeNFS(subNFS);
		}
		i+=2;
	}
	return true;
}

void NFSGenerator::updateStatesIndex(NFS& nfs)
{
	std::set<int>visited;
	VertexNodePtr node = nfs.getStartState();
	std::deque<VertexNodePtr>queue;
	int currInd = 0;
	queue.push_back(node);
	while (!queue.empty())
	{
		node = queue.front();
		queue.pop_front();
		_ASSERT(node.isValidate());
		if (visited.find(node->id) != visited.end())
			continue;
		visited.insert(node->id);
		node->index = currInd++;
		std::vector<Edge> edges = node->edges;
		for (Edge edge : edges)
		{
			queue.push_back(edge.first);
		}
	}
}


NFS NFSGenerator::generateNFS(std::vector<RegularActionPair>& regularActionList)
{
	VertexNodePtr newStartState = new VertexNode();
	NFS newNFS;
	newNFS.setStartState(newStartState);
	for (RegularActionPair& regularAction : regularActionList)
	{
		NFS nfs;
		if (!generateNFS(regularAction.first, nfs))
			continue;
		newNFS.mergeNFS(nfs,false);
	}

	updateStatesIndex(newNFS);
	return newNFS;
}


int NFSGenerator::getTag(TokenPtr token)
{
	_ASSERT(token.isValidate());
	LetterPtr letter = token;
	DigitPtr digit = token;
	if (letter.isValidate())
		return letter->letter;
	else if (digit.isValidate())
		return digit->digit;
	else
		return token->tag;
}
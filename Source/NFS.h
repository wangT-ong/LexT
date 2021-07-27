#pragma once
#include<string>
#include<vector>
#include<deque>
#include<map>
#include<utility>
#include"SmartPointer.h"
#include"RegularNode.h"
#include"Paser.h"
struct VertexNode;
using VertexNodePtr = SmartPointer<VertexNode>;
using Edge = std::pair<VertexNodePtr, int>;

struct VertexNode
{
	VertexNode(int t=EMPTY) :  index(0) {
		id = numberOfVertex++;
	}
	int index;
	int id;
	std::vector<Edge> edges;
private:
	static int numberOfVertex;
};






class NFS
{
public:
	
	NFS();
	NFS(int tag);
	NFS(int firstTag,int lastTag);
	~NFS() {};
	void addEdge(VertexNodePtr v, VertexNodePtr w, int weight);
	void unionNFS(NFS& nfs);
	void connectNFS(NFS& nfs);
	void makeClosure();
	void makePlusMark();
	void makeQuestionMark();
	void mergeNFS(NFS& nfs,bool isClosed=true);
	VertexNodePtr getStartState() { return startState; }
	void setStartState(VertexNodePtr state) {
		if (!startState.isValidate())
			++m_size; 
		startState = state; 
	}
	std::vector<VertexNodePtr>getFinalStates() { return finalStates; }
	void setFinalStates(std::vector<VertexNodePtr>& newFinalStates)
	{
		int oldSize = finalStates.size();
		finalStates = newFinalStates; 
		m_size += newFinalStates.size() - oldSize;
	};
	int size() { return m_size; }
	bool isEmpty() {return m_size<=0;}
private:
	std::vector<VertexNodePtr>finalStates;
	VertexNodePtr startState;
	int m_size=0;
};

class NFSGenerator
{
public:
	static NFS generateNFS(std::vector<RegularActionPair>& regularActionList);
private:
	static bool generateNFS(RegualrNodePtr regularNode, NFS&nfs);
	static bool buildUnionNFS(RegualrNodePtr regularNode, NFS& nfs);
	static bool buildConnectionNFS(RegualrNodePtr regularNode, NFS& nfs);
	static bool buildUnitaryNFS(RegualrNodePtr regularNode, NFS& nfs);
	static bool buildBracketNFS(RegualrNodePtr regularNode, NFS& nfs);
	static bool buildBasicNFS(RegualrNodePtr regularNode, NFS& nfs);
	static bool buildCharactorClassNFS(RegualrNodePtr regularNode, NFS& nfs);
	static void updateStatesIndex(NFS& nfs);
	static int getTag(TokenPtr token);
};




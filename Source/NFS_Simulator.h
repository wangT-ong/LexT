#pragma once
#include"Paser.h"
#include"Buffer.h"
class NFS_Simulator
{
public:
	NFS_Simulator();
private:
	TransitionTable m_translationTable;
	Buffer m_buffer;
	int m_startState;
	std::set<int>m_finalStates;
};
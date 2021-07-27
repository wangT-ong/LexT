#pragma once
#include<fstream>
#include<memory>
class Buffer
{
public:
	Buffer(std::wstring fileName);
	char readch();
	void moveForward();
	void restore();
	void rollBack(int length=1);
private:
	static constexpr const int BufferLength = 1024;
	char m_buff_1[BufferLength];
	char m_buff_2[BufferLength];
	char* m_lexemeBegin;
	char* m_forward;
	std::ifstream m_file;
	bool m_isReadFirstBuffRecent;
	bool isReachBufferEnd();
	void loadBuffer();
};
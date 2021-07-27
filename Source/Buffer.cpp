#include"Buffer.h"

const int Buffer::BufferLength;

Buffer::Buffer(std::wstring fileName)
{
	m_file.open(fileName);
	if (!m_file)
		throw std::invalid_argument("打开文件失败");
	m_isReadFirstBuffRecent = false;
	m_buff_1[BufferLength - 1] = EOF;
	m_buff_2[BufferLength - 1] = EOF;
	loadBuffer();
	m_lexemeBegin = m_buff_1;
}

char Buffer::readch()
{
	if (*m_forward == EOF && isReachBufferEnd())
		loadBuffer();
	char c = *m_forward;
	++m_forward;
	return c;
}

bool Buffer::isReachBufferEnd()
{
	if (m_isReadFirstBuffRecent)
		return m_forward == &m_buff_1[BufferLength - 1];
	else
		return m_forward == &m_buff_2[BufferLength - 1];
}

void Buffer::loadBuffer()
{
	if (!m_file)
		throw std::invalid_argument("文件指针为空！");
	if (m_isReadFirstBuffRecent) {
		if (!m_file.read(m_buff_2, BufferLength - 1) && !m_file.eof())
			throw std::runtime_error("读取文件失败");
		m_forward = m_buff_2;
	}
	else {
		if (!m_file.read(m_buff_1, BufferLength - 1)&& !m_file.eof())
			throw std::runtime_error("读取文件失败");
		m_forward = m_buff_1;
	}
	m_isReadFirstBuffRecent = !m_isReadFirstBuffRecent;
}

void Buffer::moveForward()
{
	m_lexemeBegin = m_forward;
}
void Buffer::restore()
{
	m_forward= m_lexemeBegin;
}

void Buffer::rollBack(int length)
{
	for (int i = 0; i < length; ++i)
	{
		if (m_forward == m_lexemeBegin)
			break;
		if (m_forward == m_buff_1)
			m_forward = &m_buff_2[BufferLength-1];
		else if (m_forward == m_buff_2)
			m_forward = &m_buff_1[BufferLength - 1];
		--m_forward;
	}
}
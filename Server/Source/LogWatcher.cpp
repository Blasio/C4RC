#include "stdafx.h"
#include "LogWatcher.h"

using namespace LogWatching;

LogWatcher::LogWatcher(wstring fName)
{
	// Open Log file
	m_LogFile.open(fName.c_str());

	// Throw a fit if file is not open
	if (!m_LogFile.is_open()) throw std::runtime_error("LogWatcher::LogWatcher(): Could not open log file");

	// Seek to end of file.
	// That will be our starting point.
	m_LogFile.seekg(0, ios::end);
	m_LogCursor = m_LogFile.tellg();
}

LogWatcher::~LogWatcher()
{
	if (m_LogFile.is_open())
		m_LogFile.close();
}

int LogWatcher::Update()
{
	int iNewBytes;
	static string strBuffer;

	// Count number of new bytes in log file
	m_LogFile.seekg(0, ios::end);
	iNewBytes = static_cast<int>(m_LogFile.tellg()) - m_LogCursor;

	// If there are no new bytes, just bail out.
	if (iNewBytes == 0) return(0);

	// Read in new characters and append to our buffer string
	char * tBuf = new char[iNewBytes];
	m_LogFile.read(tBuf, iNewBytes);
	strBuffer += tBuf;
	delete [] tBuf;

	// TODO: This should be number of new events, not number of new bytes
	return (iNewBytes);
}
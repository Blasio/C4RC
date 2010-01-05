#include "stdafx.h"
#include "LogWatcher.h"

using namespace LogWatching;

LogWatcher::LogWatcher(wstring fName)
{
	// Open Log file
	m_LogFile.open(fName.c_str(), ios::binary);

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

	// Update our cursor and restore stream position
	m_LogCursor += iNewBytes;
	m_LogFile.seekg(-iNewBytes, ios::end);

	// Read in new characters and append to our buffer string
	char * tBuf = new char[iNewBytes];
	m_LogFile.read(tBuf, iNewBytes);
	strBuffer += string(tBuf,iNewBytes);
	delete [] tBuf;

	// Break new bytes read into lines and parse each line
	// into an event object (or try to at least)
	int iNewEvents = 0;
	while(true)
	{
		// Is there a CRLF in the log?  If yes then
		// parse this line.
		int iNextCRLF = strBuffer.find("\015\012");
		if (iNextCRLF == string::npos) break;
		string strLine = strBuffer.substr(0, iNextCRLF);

		// Crop used part of the buffer
		strBuffer = strBuffer.substr(iNextCRLF+2);
		
		// Try parsing this event.  If successful, add it to the list
		// and count, otherwise do nothing.
		try
		{
			Event * tEv = new Event(strLine);
			m_Events.push_back(tEv);
			iNewEvents++;
		}
		catch (std::runtime_error & e)
		{
		
		};
		
	}

	return (iNewEvents);
}

Event::Event(std::string LogLine)
{
	
}
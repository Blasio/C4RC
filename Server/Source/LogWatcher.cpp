#include "stdafx.h"

#include <vector>
#include <stdlib.h>

using namespace std;

#include "LogWatcher.h"
#include "Support.h"

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
	
	// Flush any unhandled events left
	for (list<Event*>::iterator it=m_Events.begin(); it!=m_Events.end(); ++it)
	{
			delete *it;
	}

	m_Events.clear();

	// Close log file if it's open
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
		Event * tEv = new Event(strLine);
		if (tEv->get_EventType() != etUnsupported)
		{
			m_Events.push_back(tEv);
			iNewEvents++;
		}
		else
			delete tEv;
	}

	return (iNewEvents);
}


Event::Event(std::string LogLine): m_Type(etUnsupported), m_iClientID(-1)
{
	// Log Entry formats are
	//
	// Join:
	// [ T]TT\+:SS J;32CharGUID;ClientID;Name
	//
	// Leave:
	// [ T]TT\+:SS Q;32CharGUID;ClientID;Name
	//
	// Say:
	// [ T]TT\+:SS say;32CharGUID;ClientID;Name;^UText Message Here
	
	// Split line into Timestamp and entry portions. Start looking for
	// ' ' from position 1 since we can have a leading space in the line
	int iSplitPoint = LogLine.find(' ', 1);
	
	if ((iSplitPoint == string::npos) || (iSplitPoint < 1))
		throw std::runtime_error("Event::Event(): Malformed Log Line");

	string strTimeStamp = LogLine.substr(0, iSplitPoint);
	string strEntry = LogLine.substr(iSplitPoint+1);

	// Split Entry part of line along ';'
	vector<string>vsTokens;
	Tokenize(strEntry, vsTokens, ";");

	if (vsTokens[0]=="J")
	{
		// Join Token found. Copy the rest of the tokens as required
		this->m_Type = etJoin;
		this->m_iClientID = strtol(vsTokens[2].c_str(), NULL, 10);
		this->m_strPlayerName = vsTokens[3];
	}
	else if (vsTokens[0]=="Q")
	{
		// Leave Token found. Copy the rest of the tokens as required
		this->m_Type = etLeave;
		this->m_iClientID = strtol(vsTokens[2].c_str(), NULL, 10);
		this->m_strPlayerName = vsTokens[3];
	}
	else if (vsTokens[0]=="say")
	{
		// Say Token found.  This is either normal speech or an attempt
		// at triggering a command.  Look for ! as 2nd character in string.
		// NOTE: 1st character is either ^U or a colour code in all say entries.
		this->m_strArgString = vsTokens[4];
		if (this->m_strArgString.substr(1,1)=="!")
			this->m_Type = etCMD;
		else
			this->m_Type = etSay;

		this->m_iClientID = strtol(vsTokens[2].c_str(), NULL, 10);
		this->m_strPlayerName = vsTokens[3];
	}
}

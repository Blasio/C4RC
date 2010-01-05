#include <string>
#include <iostream>
#include <fstream>

namespace LogWatching {

	using namespace std;

	enum EventTypes {etJoin, etLeave, etSay, etCMD, etMIN = etJoin, etMAX=etCMD};
	
	class Event {
	public:
		Event(EventTypes EventType, int ClientID, wstring PlayerName, wstring ArgString);
		Event(wstring LogLine);
	private:
		EventTypes m_Type;
		int m_iClientID;
		wstring m_strPlayerName;
		wstring m_strArgString;
	};



	class LogWatcher
	{
	public:
		LogWatcher(wstring fName);
		~LogWatcher();

		int Update();
		bool MoveToNextEvent();
		const Event * CurrentEvent();

	private:

		wstring m_strFileName;
		ifstream m_LogFile;
		int m_LogCursor;
		
	};

}
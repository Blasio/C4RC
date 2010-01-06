#include <string>
#include <iostream>
#include <fstream>
#include <list>

namespace LogWatching {

	using namespace std;

	enum EventTypes {etUnsupported, etJoin, etLeave, etSay, etCMD, etMIN = etUnsupported, etMAX=etCMD};
	
	class Event {
	public:
		Event(EventTypes EventType, int ClientID, wstring PlayerName, wstring ArgString);
		Event(string LogLine);

		// Getter methods
		EventTypes get_EventType(){return m_Type;};
		int get_ClientID(){if (etUnsupported!=m_Type) return m_iClientID;throw std::runtime_error("Event::get_ClientID(): Unknown Event Type");};
		string get_PlayerName(){if (etUnsupported!=m_Type) return m_strPlayerName;throw std::runtime_error("Event::get_PlayerName(): Unknown Event Type");};
		string get_StringArg(){if (etUnsupported!=m_Type) return m_strArgString;throw std::runtime_error("Event::get_StringArg(): Unknown Event Type");};

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
		list<Event *> m_Events;
	};

}
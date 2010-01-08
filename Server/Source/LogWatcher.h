#include <string>
#include <iostream>
#include <fstream>
#include <list>

namespace LogWatching {

	using namespace std;

	enum EventTypes {etUnsupported, etJoin, etLeave, etSay, etCMD, etMIN = etUnsupported, etMAX=etCMD};
	
	class Event {
	public:
		// Constructors
		Event(string LogLine);

		// Getter methods
		EventTypes get_EventType(){return m_Type;};
		int get_ClientID(){if (etUnsupported!=m_Type) return m_iClientID;throw std::runtime_error("Event::get_ClientID(): Unknown Event Type");};
		string get_PlayerName(){if (etUnsupported!=m_Type) return m_strPlayerName;throw std::runtime_error("Event::get_PlayerName(): Unknown Event Type");};
		string get_StringArg(){if (etUnsupported!=m_Type) return m_strArgString;throw std::runtime_error("Event::get_StringArg(): Unknown Event Type");};

	private:
		EventTypes m_Type;
		int m_iClientID;
		string m_strPlayerName;
		string m_strArgString;
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
		// Copy constructor & assignment operators here
		// so that class will not be copyable.
		LogWatcher(const LogWatcher& x);
		LogWatcher& operator=(const LogWatcher& x);

		wstring m_strFileName;
		ifstream m_LogFile;
		int m_LogCursor;
		list<Event *> m_Events;
	};

}
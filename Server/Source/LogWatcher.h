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
		// Standard Constructor & Destructor
		LogWatcher(wstring fName);
		~LogWatcher();

		// Check log file for changes, return number of new events detected
		int Update();

		// Move cursor to next event in list (delete old head of list).  Return true if cursor is pointing to an event
		bool MoveToNextEvent() { if (m_Events.empty()) return false; delete m_Events.front(); m_Events.pop_front(); return (!m_Events.empty());};

		// Get event pointed to by cursor (or NULL if no events in list).
		Event * CurrentEvent() { if (!m_Events.empty()) return m_Events.front();return NULL; };

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
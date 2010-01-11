// C4RC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LogWatcher.h"
#include <list>

using namespace LogWatching;


int _tmain(int argc, _TCHAR* argv[])
{
	// List of Test Log Entries
	list<string>TestEntries;

	// Fill up the list.  Some Joins here.
	TestEntries.push_back(string(" 99:34 J;0123456789abcdef0123456789abcdef;0;Player Name Here"));
	TestEntries.push_back(string("199:23 J;0123456789abcdef0123456789abcdef;1;Unknown Soldier"));
	TestEntries.push_back(string("2689:04 J;0123456789abcdef0123456789abcdef;2;A Noob"));

	// Some Quit/Leave events here.
	TestEntries.push_back(string("335:23 Q;0123456789abcdef0123456789abcdef;0;Player Name Here"));
	TestEntries.push_back(string("244:21 Q;0123456789abcdef0123456789abcdef;1;Unknown Soldier"));
	TestEntries.push_back(string("2989:43 Q;0123456789abcdef0123456789abcdef;2;A Noob"));

	// Some Insults here.
	TestEntries.push_back(string("112:00 say;0123456789abcdef0123456789abcdef;0;Player Name Here;\026U mur l'hemm noob!"));
	TestEntries.push_back(string("115:21 say;0123456789abcdef0123456789abcdef;1;Unknown Soldier;\026Noob int mela jIEn"));
	TestEntries.push_back(string("122:14 say;0123456789abcdef0123456789abcdef;5;|<rb jkage;^1I killed the Turnip and all I got was this lousy T-Shirt."));

	// A couple of commands.
	TestEntries.push_back(string("115:21 say;0123456789abcdef0123456789abcdef;1;Unknown Soldier;\026!NEXTMAP"));
	TestEntries.push_back(string("122:14 say;0123456789abcdef0123456789abcdef;5;|<rb jkage;\026!NOMINATE mp_Haus TDM"));


	// Convert each event and print it out here
	for (list<string>::iterator it = TestEntries.begin(); it != TestEntries.end(); ++it)
	{
		Event * Ev = new Event(*it);

		cout << "Event Type: " << Ev->get_EventType() << ", Client ID: " << Ev->get_ClientID();
		cout << ", Name: " << Ev->get_PlayerName() << ", String=" << Ev->get_StringArg() << endl;

		delete Ev;
	}


	LogWatcher m_LogWatcher(L"C:\\Program Files (x86)\\Activision\\Call of Duty 4 - Modern Warfare\\Mods\\ModWarfare\\games_mp.log");

	Sleep(1000);

	for (int i=0; i<10000; i++)
	{
		if (m_LogWatcher.Update() > 0)
		{
			do
			{
				Event * Ev = m_LogWatcher.CurrentEvent();
				cout << Ev->get_PlayerName() << ": " << Ev->get_EventType() << " " << Ev->get_StringArg() << endl;
			} while (m_LogWatcher.MoveToNextEvent());
		}
		Sleep(100);
	}

	return 0;
}


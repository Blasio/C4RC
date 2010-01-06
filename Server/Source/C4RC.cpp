// C4RC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LogWatcher.h"

using namespace LogWatching;


int _tmain(int argc, _TCHAR* argv[])
{
	string LogEntry1(" 99:34 J;0123456789abcdef0123456789abcdef;0;Player Name Here");
	string LogEntry2("199:23 J;0123456789abcdef0123456789abcdef;1;Unknown Soldier");
	string LogEntry3("2689:04 J;0123456789abcdef0123456789abcdef;2;A Noob");

	Event * Ev1 = new Event(LogEntry1);
	Event * Ev2 = new Event(LogEntry2);
	Event * Ev3 = new Event(LogEntry3);

	cout << "Event Type: " << Ev1->get_EventType() << ", Client ID: " << Ev1->get_ClientID();
	cout << ", Name: " << Ev1->get_PlayerName() << ", String=" << Ev1->get_StringArg() << endl;
	cout << "Event Type: " << Ev2->get_EventType() << ", Client ID: " << Ev2->get_ClientID();
	cout << ", Name: " << Ev2->get_PlayerName() << ", String=" << Ev2->get_StringArg() << endl;
	cout << "Event Type: " << Ev3->get_EventType() << ", Client ID: " << Ev3->get_ClientID();
	cout << ", Name: " << Ev3->get_PlayerName() << ", String=" << Ev3->get_StringArg() << endl;

	//LogWatcher m_LogWatcher(L"C:\\Program Files (x86)\\Activision\\Call of Duty 4 - Modern Warfare\\Mods\\ModWarfare\\games_mp.log");

	//Sleep(1000);

	//for (int i=0; i<100; i++)
	//{
	//	if (m_LogWatcher.Update() > 0)
	//		wcout << L"More Bytes" << endl;
	//	Sleep(100);
	//}

	return 0;
}


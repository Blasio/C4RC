// C4RC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LogWatcher.h"

using namespace LogWatching;

int _tmain(int argc, _TCHAR* argv[])
{
	LogWatcher m_LogWatcher(L"C:\\Program Files (x86)\\Activision\\Call of Duty 4 - Modern Warfare\\Mods\\ModWarfare\\games_mp.log");

	Sleep(1000);

	for (int i=0; i<100; i++)
	{
		if (m_LogWatcher.Update() > 0)
			wcout << L"More Bytes" << endl;
		Sleep(100);
	}

	return 0;
}


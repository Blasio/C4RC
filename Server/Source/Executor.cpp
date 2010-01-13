
#include "stdafx.h"
#include "Executor.h"
#include <sstream>

using namespace Execution;

Executor::Executor(string strServerName, int iServerPort, string strRCONPassword): m_LocalSocket(INVALID_SOCKET)
{
	// Do some basic checks, Server Name required
	if (strServerName == "")
		throw std::runtime_error("Executor::Executor(): Server Name needs to be specified");

	// Password Required
	if (strRCONPassword == "")
		throw std::runtime_error("Executor::Executor(): RCON Password needs to be specified");

	// Initialize Winsock
	if (0 != WSAStartup(MAKEWORD(2,2), &m_WSAData))
		throw std::runtime_error("Executor::Executor(): Failed to initialize winsock library");

	// Look up the specified server name / ip address
	hostent * heServer =  gethostbyname(strServerName.c_str());

	if (heServer == NULL)
		throw std::runtime_error("Executor::Executor(): Server Name is invalid");
	
	if (heServer->h_addrtype != AF_INET)
		throw std::runtime_error("Executor::Executor(): Don't know how to handle this address type");

	// Copy over the address returned
	ZeroMemory(&m_Server, sizeof(m_Server));
	m_Server.sin_family = AF_INET;
	m_Server.sin_port = htons(iServerPort);
	m_Server.sin_addr.s_addr = *((ULONG *)heServer->h_addr);

	// Prepare the first part of the string to be sent to RCON
	// First 4 bytes are 0xFF, then " RCON ", then the RCON password
	// IN PLAIN TEXT (WTF?). Commands will be appended after this.
	m_strCMDPreamble = string("     RCON ") + strRCONPassword + string(" ");;
	m_strCMDPreamble[0] = ~0;
	m_strCMDPreamble[1] = ~0;
	m_strCMDPreamble[2] = ~0;
	m_strCMDPreamble[3] = ~0;

	// Set up a local socket for us
	m_LocalSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (INVALID_SOCKET == m_LocalSocket)
		throw std::runtime_error("Executor::Executor(): Failed to create local socket");
}

Executor::~Executor()
{
	if (INVALID_SOCKET != m_LocalSocket)
		closesocket(m_LocalSocket);

	WSACleanup();
}


void Executor::Say(string strMessage)
{
	// Build Say command string & send it to server
	string strTempBuf = m_strCMDPreamble + string("say ") + strMessage;
	int sentbytes = sendto(m_LocalSocket, strTempBuf.c_str(), strTempBuf.length(), 0, (sockaddr *)&m_Server, sizeof(m_Server));
}

void Executor::Tell(int iClientID, string strMessage)
{
	// Build tell command string & send it to server
	ostringstream ssID;
	ssID << iClientID << " " << strMessage;
	string strTempBuf = m_strCMDPreamble + string("tell ") + ssID.str();
	int sentbytes = sendto(m_LocalSocket, strTempBuf.c_str(), strTempBuf.length(), 0, (sockaddr *)&m_Server, sizeof(m_Server));
}

void Executor::Kick(string strPlayer, bool bTempBan)
{
	// Build kick command string & send it to server
	string strTempBuf = m_strCMDPreamble + (bTempBan ? string("kick ") : string("onlykick ")) + strPlayer;
	int sentbytes = sendto(m_LocalSocket, strTempBuf.c_str(), strTempBuf.length(), 0, (sockaddr *)&m_Server, sizeof(m_Server));
}

void Executor::Kick(int iClientID)
{
	// Build kick command string & send it to server
	ostringstream ssID;
	ssID << iClientID;
	string strTempBuf = m_strCMDPreamble + string("clientkick ") + ssID.str();
	int sentbytes = sendto(m_LocalSocket, strTempBuf.c_str(), strTempBuf.length(), 0, (sockaddr *)&m_Server, sizeof(m_Server));
}

void Executor::ChangeMap(string strMapName)
{
	// Build command string to change map & send it to server
	string strTempBuf = m_strCMDPreamble + string("map ") + strMapName;
	int sentbytes = sendto(m_LocalSocket, strTempBuf.c_str(), strTempBuf.length(), 0, (sockaddr *)&m_Server, sizeof(m_Server));
}

void Executor::ChangeGameType(string strGameType)
{
	// Build command string to change gametype & send it to server
	string strTempBuf = m_strCMDPreamble + string("g_gametype ") + strGameType;
	int sentbytes = sendto(m_LocalSocket, strTempBuf.c_str(), strTempBuf.length(), 0, (sockaddr *)&m_Server, sizeof(m_Server));
}

void Executor::RestartRound(bool bFastRestart)
{
	// Build command string to restart round & send it to server
	string strTempBuf = m_strCMDPreamble + (bFastRestart ? string("fast_restart ") : string("map_restart"));
	int sentbytes = sendto(m_LocalSocket, strTempBuf.c_str(), strTempBuf.length(), 0, (sockaddr *)&m_Server, sizeof(m_Server));
}

void Executor::MapRotate(void)
{
	// Build command string to rotate map & send it to server
	string strTempBuf = m_strCMDPreamble + string("map_rotate");
	int sentbytes = sendto(m_LocalSocket, strTempBuf.c_str(), strTempBuf.length(), 0, (sockaddr *)&m_Server, sizeof(m_Server));
}
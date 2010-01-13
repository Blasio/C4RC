
#include "stdafx.h"
#include "Executor.h"

using namespace Execution;

Executor::Executor(string strServerName, int iServerPort, string strRCONPassword)
{
	if (strServerName == "")
		throw std::runtime_error("Executor::Executor(): Server Name needs to be specified");

	if (strRCONPassword == "")
		throw std::runtime_error("Executor::Executor(): RCON Password needs to be specified");

	if (0 != WSAStartup(MAKEWORD(2,2), &m_WSAData))
		throw std::runtime_error("Executor::Executor(): Failed to initialize winsock library");

	m_ServerHostEnt =  gethostbyname(strServerName.c_str());

	if (m_ServerHostEnt == NULL)
		throw std::runtime_error("Executor::Executor(): Server Name is invalid");
	
	m_strServer = strServerName;
	m_strPassword = strRCONPassword;
	m_iPort = iServerPort;

}

Executor::~Executor()
{
	WSACleanup();
}


void Executor::Say(string strMessage)
{

	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in Server;

	Server.sin_family = AF_INET;
	Server.sin_port = htons(m_iPort);
	Server.sin_addr.s_addr = *((ULONG *)m_ServerHostEnt->h_addr_list[0]);

	string strTempBuf = string("     RCON ") + m_strPassword + string(" say ") + strMessage;
	strTempBuf[0] = ~0;
	strTempBuf[1] = ~0;
	strTempBuf[2] = ~0;
	strTempBuf[3] = ~0;
	int sentbytes = sendto(s, strTempBuf.c_str(), strTempBuf.length(), 0, (sockaddr *)&Server, sizeof(Server));

	closesocket(s);
}
#include <winsock2.h>
#include <string>

namespace Execution {

	using namespace std;

	class Executor {

	public:
		Executor(string strServerName, int iServerPort, string strRCONPassword);
		~Executor();

		// Action methods.  Each of these methods executes one or more
		// RCON commands on the game server
		void Say(string strMessage);
		void Tell(int iClientID, string strMessage);

		void Kick(string strPlayer){ Kick(strPlayer, false); };
		void Kick(string strPlayer, bool bTempBan);
		void Kick(int iClientID);

		void ChangeMap(string strMapName);
		void ChangeGameType(string strGameType);
		void RestartRound(bool bFastRestart);
		void MapRotate(void);

	private:
		// Copy constructor & assignment operators here
		// so that class will not be copyable.
		Executor(const Executor & x);
		Executor& operator=(const Executor& x);

		// private send method used by the public methods to
		// actually send the commands they build
		int m_send(string strCommand);

		// Winsock data structure (needed for init)
		WSAData m_WSAData;

		// Server Socket information (IP & Port)
		sockaddr_in m_Server;

		// Local socket from which we'll send RCON commands
		SOCKET m_LocalSocket;

		// Pre-made string used when sending commands
		string m_strCMDPreamble;
	};

}
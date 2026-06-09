#include "packet_versionmanager.h"
#include "packetmanager.h"
#include "usermanager.h"
#include "serverconsole.h"

Packet_VersionManager packet_VersionManager;

void Packet_VersionManager::ParsePacket_Version(TCPConnection::Packet::pointer packet) {
	if (packet == NULL) {
		return;
	}

	auto& connection = packet->GetConnection();
	if (connection == NULL) {
		return;
	}

	if (connection->IsVersionReceived()) {
		serverConsole.Print(PrefixType::Warn, format("[ Packet_VersionManager ] Client ({}) has sent Packet_Version, but it already sent Packet_Version!\n", connection->GetLogEndpoint()));
		return;
	}

	User* user = userManager.GetUserByConnection(connection);
	if (userManager.IsUserLoggedIn(user)) {
		serverConsole.Print(PrefixType::Warn, format("[ Packet_VersionManager ] User ({}) has sent Packet_Version, but it's already logged in!\n", user->GetUserLogName()));
		return;
	}

	serverConsole.Print(PrefixType::Info, format("[ Packet_VersionManager ] Parsing Packet_Version from client ({})\n", connection->GetLogEndpoint()));

	unsigned char launcherVersion = packet->ReadUInt8();
	unsigned short clientVersion = packet->ReadUInt16_LE();
	unsigned long clientBuildTimestamp = packet->ReadUInt32_LE();
	unsigned long clientNARChecksum = packet->ReadUInt32_LE();

	struct tm date;
	time_t t = clientBuildTimestamp;
	localtime_s(&date, &t);
	char dateStr[9];
	strftime(dateStr, sizeof(dateStr), "%d.%m.%y", &date);

	serverConsole.Print(PrefixType::Info, format("[ Packet_VersionManager ] Client ({}) has sent Packet_Version - launcherVersion: {}, clientVersion: {}, clientBuildTimestamp: {}, clientNARChecksum: {}\n", connection->GetLogEndpoint(), launcherVersion, clientVersion, dateStr, clientNARChecksum));

	if (launcherVersion != LAUNCHER_VERSION) {
		packetManager.SendPacket_Reply(connection, Packet_ReplyType::INVALID_CLIENT_VERSION);
		return;
	}

	if (clientVersion != CLIENT_VERSION) {
		packetManager.SendPacket_Reply(connection, Packet_ReplyType::INVALID_CLIENT_VERSION);
		return;
	}

	if (strcmp(dateStr, CLIENT_BUILD_TIMESTAMP) != 0) {
		packetManager.SendPacket_Reply(connection, Packet_ReplyType::INVALID_CLIENT_VERSION);
		return;
	}

	if (clientNARChecksum != CLIENT_NAR_CHECKSUM) {
		packetManager.SendPacket_Reply(connection, Packet_ReplyType::INVALID_CLIENT_VERSION);
		return;
	}

	connection->SetVersionReceived(true);
	sendPacket_Version(connection);
}

void Packet_VersionManager::sendPacket_Version(TCPConnection::pointer connection) {
	if (connection == NULL) {
		return;
	}

	auto packet = TCPConnection::Packet::Create(PacketSource::Server, connection, { (unsigned char)PacketID::Version });
	if (packet == NULL) {
		return;
	}

	packet->Send();
}
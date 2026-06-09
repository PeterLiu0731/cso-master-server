#pragma once
#include "tcp_connection.h"

constexpr auto LAUNCHER_VERSION = 67;
constexpr auto CLIENT_VERSION = 11;
constexpr auto CLIENT_BUILD_TIMESTAMP = "11.06.08";
constexpr auto CLIENT_NAR_CHECKSUM = 1860010506;

class Packet_VersionManager {
public:
	void ParsePacket_Version(TCPConnection::Packet::pointer packet);

private:
	void sendPacket_Version(TCPConnection::pointer connection);
};

extern Packet_VersionManager packet_VersionManager;
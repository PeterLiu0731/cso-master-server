#pragma once
#include "user.h"

enum Packet_UdpType {
	NetworkFailure = 0,
	HolepunchFail = 1,
	ModifySocketInfo = 1,
	SocketInfo = 2
};

enum Packet_Udp_SocketInfoType {
	SocketInfoSuccess = 0,
	SocketInfoFail = 1,
	HolepunchSuccess = 2,
	RelaySuccess = 3,
	RelayFail = 4,
	MessageHistory = 5
};

class Packet_UdpManager {
public:
	void ParsePacket_Udp(TCPConnection::Packet::pointer packet);

private:
	void parsePacket_Udp_NetworkFailure(User* user);
	void parsePacket_Udp_HolepunchFail(User* user, TCPConnection::Packet::pointer packet);
	void parsePacket_Udp_SocketInfo(User* user, TCPConnection::Packet::pointer packet);
	void sendPacket_Udp_ModifySocketInfo(TCPConnection::pointer connection, PortType portType, unsigned long userID, unsigned long ip, unsigned short port);
};

extern Packet_UdpManager packet_UdpManager;
#pragma once
#include "user.h"

enum Packet_ItemType {
	SelectClass = 1,
	BuyWeapon = 2
};

class Packet_ItemManager {
public:
	void ParsePacket_Item(TCPConnection::Packet::pointer packet);

private:
	void parsePacket_Item_SelectClass(User* user, TCPConnection::Packet::pointer packet);
	void parsePacket_Item_BuyWeapon(User* user, TCPConnection::Packet::pointer packet);
};

extern Packet_ItemManager packet_ItemManager;
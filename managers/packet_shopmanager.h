#pragma once
#include "user.h"

enum Packet_ShopType {
	RequestShopList = 0,
	ShopList = 0
};

class Packet_ShopManager {
public:
	void ParsePacket_Shop(TCPConnection::Packet::pointer packet);

private:
	void parsePacket_Shop_RequestShopList(User* user);
	void sendPacket_Shop_ShopList(TCPConnection::pointer connection);
};

extern Packet_ShopManager packet_ShopManager;
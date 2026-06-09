#include "packet_itemmanager.h"
#include "usermanager.h"
#include "roommanager.h"
#include "serverconsole.h"

Packet_ItemManager packet_ItemManager;

void Packet_ItemManager::ParsePacket_Item(TCPConnection::Packet::pointer packet) {
	if (packet == NULL) {
		return;
	}

	auto& connection = packet->GetConnection();
	if (connection == NULL) {
		return;
	}

	User* user = userManager.GetUserByConnection(connection);
	if (!userManager.IsUserLoggedIn(user)) {
		serverConsole.Print(PrefixType::Warn, format("[ Packet_ItemManager ] Client ({}) has sent Packet_Item, but it's not logged in!\n", connection->GetLogEndpoint()));
		return;
	}

	serverConsole.Print(PrefixType::Info, format("[ Packet_ItemManager ] Parsing Packet_Item from user ({})\n", user->GetUserLogName()));

	unsigned char type = packet->ReadUInt8();

	switch (type) {
		case Packet_ItemType::SelectClass: {
			parsePacket_Item_SelectClass(user, packet);
			break;
		}
		case Packet_ItemType::BuyWeapon: {
			parsePacket_Item_BuyWeapon(user, packet);
			break;
		}
		default: {
			serverConsole.Print(PrefixType::Warn, format("[ Packet_ItemManager ] User ({}) has sent unregistered Packet_Item type {}!\n", user->GetUserLogName(), type));
			break;
		}
	}
}

void Packet_ItemManager::parsePacket_Item_SelectClass(User* user, TCPConnection::Packet::pointer packet) {
	if (user == NULL || packet == NULL) {
		return;
	}

	auto& connection = user->GetConnection();
	if (connection == NULL) {
		return;
	}

	if (user->GetUserStatus() != UserStatus::InGame) {
		serverConsole.Print(PrefixType::Warn, format("[ Packet_ItemManager ] User ({}) has sent Packet_Item SelectClass, but it's not in game!\n", user->GetUserLogName()));
		return;
	}

	Room* room = roomManager.GetRoomByRoomID(user->GetCurrentRoomID());
	if (room == NULL) {
		serverConsole.Print(PrefixType::Warn, format("[ Packet_ItemManager ] User ({}) has sent Packet_Item SelectClass, but its room is NULL!\n", user->GetUserLogName()));
		return;
	}

	GameMatch* gameMatch = room->GetGameMatch();
	if (gameMatch == NULL) {
		serverConsole.Print(PrefixType::Warn, format("[ Packet_ItemManager ] User ({}) has sent Packet_Item SelectClass, but its room doesn't have a gameMatch!\n", user->GetUserLogName()));
		return;
	}

	unsigned char classItemID = packet->ReadUInt8();

	serverConsole.Print(PrefixType::Info, format("[ Packet_ItemManager ] User ({}) has sent Packet_Item SelectClass - classItemID: {}\n", user->GetUserLogName(), classItemID));
}

void Packet_ItemManager::parsePacket_Item_BuyWeapon(User* user, TCPConnection::Packet::pointer packet) {
	if (user == NULL || packet == NULL) {
		return;
	}

	auto& connection = user->GetConnection();
	if (connection == NULL) {
		return;
	}

	if (user->GetUserStatus() != UserStatus::InGame) {
		serverConsole.Print(PrefixType::Warn, format("[ Packet_ItemManager ] User ({}) has sent Packet_Item BuyWeapon, but it's not in game!\n", user->GetUserLogName()));
		return;
	}

	Room* room = roomManager.GetRoomByRoomID(user->GetCurrentRoomID());
	if (room == NULL) {
		serverConsole.Print(PrefixType::Warn, format("[ Packet_ItemManager ] User ({}) has sent Packet_Item BuyWeapon, but its room is NULL!\n", user->GetUserLogName()));
		return;
	}

	GameMatch* gameMatch = room->GetGameMatch();
	if (gameMatch == NULL) {
		serverConsole.Print(PrefixType::Warn, format("[ Packet_ItemManager ] User ({}) has sent Packet_Item BuyWeapon, but its room doesn't have a gameMatch!\n", user->GetUserLogName()));
		return;
	}

	unsigned char primaryItemID = packet->ReadUInt8();
	unsigned char secondaryItemID = packet->ReadUInt8();

	serverConsole.Print(PrefixType::Info, format("[ Packet_ItemManager ] User ({}) has sent Packet_Item BuyWeapon - primaryItemID: {}, secondaryItemID: {}\n", user->GetUserLogName(), primaryItemID, secondaryItemID));
}
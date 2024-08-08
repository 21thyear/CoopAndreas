#include "../project_files/stdafx.h"
unsigned int lastOnFootTickCount = 0;

class CoopAndreas {
public:
    CoopAndreas() {
		Events::shutdownRwEvent += []
			{
				if (CNetwork::m_bConnected)
				{
					enet_peer_disconnect(CNetwork::m_pPeer, 0);
					enet_peer_reset(CNetwork::m_pPeer);
				}
			};
		Events::gameProcessEvent += []
			{
				if (CNetwork::m_bConnected)
				{
					

					CPackets::PlayerOnFoot* packet = CPacketHandler::PlayerOnFoot__Collect();
					int syncRate = 40;

					if (packet->velocity.x == 0 &&
						packet->velocity.y == 0 &&
						packet->velocity.z == 0)
						syncRate = 100;
					if (GetTickCount() > lastOnFootTickCount + syncRate)
					{
						lastOnFootTickCount = GetTickCount();
						CPackets::PlayerOnFoot::m_last = packet;
						CNetwork::SendPacket(CPacketsID::PLAYER_ONFOOT, packet, sizeof * packet, ENET_PACKET_FLAG_UNSEQUENCED);
						delete packet;
					}
				}
			};
		Events::drawBlipsEvent += []
			{
				CNetworkPlayerMapPin::Process();
				CNetworkPlayerWaypoint::Process();
			};
		Events::drawingEvent += []
			{
				CNetworkPlayerNameTag::Process();
				CChat::Draw();
			};
		CCore::Init();
		
	};
} CoopAndreasPlugin;

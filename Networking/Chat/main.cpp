#include <stdio.h>
#include <string.h>
#include <vector>
#include <thread>
#include "raknet\RakPeerInterface.h"
#include "raknet\MessageIdentifiers.h"
#include "raknet\BitStream.h"
#include "raknet\RakNetTypes.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

bool running = true;

enum ChatMessages 
{
	ID_CHAT_MESSAGE = ID_USER_PACKET_ENUM + 1,
	ID_CLIENT_CHAT_MESSAGE = ID_USER_PACKET_ENUM + 2,
	ID_SERVER_RELAY_MESSAGE = ID_USER_PACKET_ENUM + 3
};

struct Client
{
	RakNet::RakPeerInterface* peer;
	std::vector<RakNet::AddressOrGUID> connections;

	Client(bool server = false)
	{
		peer = RakNet::RakPeerInterface::GetInstance();

		if(!server)
		{
			RakNet::SocketDescriptor sd;
			peer->Startup(1, &sd, 1);

			Connect("127.0.0.1");

			printf("Starting client. Enter ~ to close.\n");

			std::thread broadcastThread(&Client::Broadcast, this);
			broadcastThread.detach();
		}
		else
		{
			RakNet::SocketDescriptor sd(SERVER_PORT, 0);
			peer->Startup(MAX_CLIENTS, &sd, 1);

			peer->SetMaximumIncomingConnections(MAX_CLIENTS);

			printf("Starting server. Enter ~ to close.\n");

			//std::thread waitThread(&Client::Wait, this);
			//waitThread.detach();
			std::thread broadcastThread(&Client::Broadcast, this);
			broadcastThread.detach();
		}

		std::thread listenThread(&Client::Listen, this);
		listenThread.join();
	}

	void Connect(char* ip)
	{
		peer->Connect(ip, SERVER_PORT, 0, 0);
	}


	void Listen()
	{
		RakNet::Packet* packet;
		while(running)
		{
			for(packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
			{
				if(packet->data[0] == ID_SERVER_RELAY_MESSAGE)
				{
					// recieved a message; print to screen
					RakNet::RakString rs;
					RakNet::BitStream bsIn(packet->data, packet->length, false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read(rs);
					printf("[MSG] ");
					printf(rs);
					printf("\n");
				}
				else if(packet->data[0] == ID_CLIENT_CHAT_MESSAGE)
				{
					// client sent a message, server needs to broadcast it to everyone.
					RakNet::RakString rs;
					RakNet::BitStream bsIn(packet->data, packet->length, false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read(rs);
					printf("[MSG] ");
					printf(rs);
					printf("\n");

					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID)ID_SERVER_RELAY_MESSAGE);
					bsOut.Write(rs);
					
					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);
				}
				else if(packet->data[0] == ID_CONNECTION_REQUEST_ACCEPTED)
				{
					printf("[SYS] connected to server.\n");
					connections.push_back(RakNet::AddressOrGUID(packet));
				}
				else if(packet->data[0] == ID_NEW_INCOMING_CONNECTION)
				{
					printf("[SYS] client joined server.\n");
					connections.push_back(RakNet::AddressOrGUID(packet));
				}
			}
		}
	}

	void Broadcast()
	{
		while(running)
		{
			char message[512];
			gets(message);

			if(message[0] == '~')
			{
				running = false;
				return;
			}

			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_CLIENT_CHAT_MESSAGE);
			bsOut.Write(message);

			for(int i = 0; i < connections.size(); i++)
			{
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, connections[i], false);
			}
		}
	}

	void Wait()
	{
		while(running)
		{
			char message[512];
			gets(message);

			if(message[0] == '~')
			{
				running = false;
				return;
			}
		}
	}
};

void main()
{
	char str[512];
	printf("[C]lient or [S]erver:\n");
	gets(str);

	Client c(str[0] == 's' || str[0] == 'S');
}
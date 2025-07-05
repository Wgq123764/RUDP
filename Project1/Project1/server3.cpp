#include <iostream>
#include <enet/enet.h>

int main() {
    // ��ʼ�� ENet
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    // ������������ַ
    ENetAddress address;
    address.host = ENET_HOST_ANY; // �������п��ýӿ�
    address.port = 1234;          // ʹ�ö˿�1234

    // ����������
    ENetHost* server = enet_host_create(&address, 32, 2, 0, 0);
    if (server == nullptr) {
        std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Server started on port " << address.port << std::endl;

    // �¼�����ѭ��
    ENetEvent event;
    while (true) {
        // �ȴ��¼�����ʱ��Ϊ1000����
        while (enet_host_service(server, &event, 1000) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                char ip[256];
                enet_address_get_host_ip(&event.peer->address, ip, 256);
                std::cout << "A new client connected from " << ip << ":" << event.peer->address.port << std::endl;

                // ���ͻ�ӭ��Ϣ
                const char* welcomeMsg = "Welcome to the ENet server!";
                ENetPacket* packet = enet_packet_create(welcomeMsg, strlen(welcomeMsg) + 1, ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(event.peer, 0, packet);
                break;
            }

            case ENET_EVENT_TYPE_RECEIVE: {
                std::cout << "Received packet from client: " << event.packet->data << std::endl;

                // �㲥��Ϣ���������ӵĿͻ���
                ENetPacket* broadcastPacket = enet_packet_create(event.packet->data, event.packet->dataLength, ENET_PACKET_FLAG_RELIABLE);
                enet_host_broadcast(server, 0, broadcastPacket);

                // �������ݰ�
                enet_packet_destroy(event.packet);
                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT: {
                std::cout << "A client disconnected." << std::endl;
                event.peer->data = nullptr;
                break;
            }

            default:
                break;
            }
        }
    }

    // ����
    enet_host_destroy(server);
    return EXIT_SUCCESS;
}
#include <iostream>
#include <enet/enet.h>
#include <thread>
#include <string>
int main() {
    // ============= Ӳ���� IP �Ͷ˿� =============
    const char* serverIp = "127.0.0.1";  // ��Ϊ��ķ�����IP���� "192.168.1.100"��
    const int serverPort = 1234;          // ������������˿�һ��
    // ==========================================

    // ��ʼ�� ENet
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialize ENet!" << std::endl;
        return 1;
    }
    atexit(enet_deinitialize);

    // �����ͻ���
    ENetHost* client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!client) {
        std::cerr << "Failed to create ENet client!" << std::endl;
        return 1;
    }

    // ���÷�������ַ
    ENetAddress address;
    if (enet_address_set_host(&address, serverIp) != 0) {
        std::cerr << "Failed to resolve server IP: " << serverIp << std::endl;
        return 1;
    }
    address.port = serverPort;

    // ���ӷ�����
    ENetPeer* peer = enet_host_connect(client, &address, 2, 0);
    if (!peer) {
        std::cerr << "Failed to initiate connection to server!" << std::endl;
        return 1;
    }

    std::cout << "Connecting to server at " << serverIp << ":" << serverPort << "..." << std::endl;

    // �ȴ����ӳɹ�
    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connected to server successfully!" << std::endl;
    }
    else {
        enet_peer_reset(peer);
        std::cerr << "Connection to server failed!" << std::endl;
        return 1;
    }

    // �û������̣߳�������Ϣ��
    std::thread inputThread([&]() {
        while (true) {
            std::string message;
            std::getline(std::cin, message);

            if (message == "quit") {
                enet_peer_disconnect(peer, 0);
                break;
            }

            // ������Ϣ��������
            ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(peer, 0, packet);
        }
        });

    // ��ѭ�������շ�������Ϣ��
    while (true) {
        while (enet_host_service(client, &event, 1000) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                std::cout << "Server: " << event.packet->data << std::endl;
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnected from server." << std::endl;
                inputThread.detach();
                return 0;

            default:
                break;
            }
        }
    }

    // ����
    inputThread.join();
    enet_host_destroy(client);
    return 0;
}
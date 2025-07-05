//#include <iostream>
//#include <string>
//#include <thread>
//#include <atomic>
//#include <chrono>
//#include <conio.h>  // ������ͷ�ļ�
//#include <enet/enet.h>
//#include <ws2tcpip.h>
//
//#pragma comment(lib, "enet.lib")
//#pragma comment(lib, "Ws2_32.lib")
//
//std::atomic<bool> running(true);
//
//// �����̺߳���
//void heartbeat_thread(ENetPeer* peer) {
//    while (running) {
//        ENetPacket* packet = enet_packet_create("HEARTBEAT", 9, ENET_PACKET_FLAG_UNSEQUENCED);
//        enet_peer_send(peer, 1, packet); // ʹ��ͨ��1��������
//        enet_host_flush(peer->host);
//        std::this_thread::sleep_for(std::chrono::seconds(1)); // ÿ��һ������
//    }
//}
//
//// ����״̬���
//void print_connection_stats(ENetPeer* peer) {
//    std::cout << "\n����״̬:\n"
//        << "  ����ʱ��: " << peer->roundTripTime << "ms\n"
//        << "  ������: " << peer->packetLoss / (float)ENET_PEER_PACKET_LOSS_SCALE * 100 << "%\n"
//        << "  ���: " << peer->lastReceiveTime << "ms ago\n";
//}
//
//// �Զ���������
//bool try_reconnect(ENetHost* client, ENetAddress address, int max_attempts = 3) {
//    for (int i = 0; i < max_attempts; ++i) {
//        std::cout << "������������(" << i + 1 << "/" << max_attempts << ")...\n";
//
//        ENetPeer* newPeer = enet_host_connect(client, &address, 2, 0);
//        if (!newPeer) {
//            std::this_thread::sleep_for(std::chrono::seconds(2));
//            continue;
//        }
//
//        ENetEvent event;
//        if (enet_host_service(client, &event, 5000) > 0 &&
//            event.type == ENET_EVENT_TYPE_CONNECT) {
//            std::cout << "�����ɹ�!\n";
//            return true;
//        }
//
//        enet_peer_reset(newPeer);
//        std::this_thread::sleep_for(std::chrono::seconds(2));
//    }
//    return false;
//}
//
//int main() {
//    if (enet_initialize() != 0) {
//        std::cerr << "ENet��ʼ��ʧ��" << std::endl;
//        return EXIT_FAILURE;
//    }
//    atexit(enet_deinitialize);
//
//    // ���ÿ���̨�رմ���
//    SetConsoleCtrlHandler([](DWORD event) {
//        if (event == CTRL_CLOSE_EVENT) {
//            std::cout << "����̨�ر���...\n";
//            running = false;
//        }
//        return TRUE;
//        }, TRUE);
//
//    ENetHost* client = enet_host_create(NULL, 1, 2, 0, 0);
//    if (!client) {
//        std::cerr << "�޷������ͻ�������" << std::endl;
//        return EXIT_FAILURE;
//    }
//
//    ENetAddress serverAddress;
//    ENetPeer* peer = nullptr;
//
//    // ���÷�������ַ
//    std::string serverIP;
//    std::cout << "�����������IP��ַ(ֱ�ӻس�ʹ��Ĭ��127.0.0.1): ";
//    std::getline(std::cin, serverIP);
//    if (serverIP.empty()) serverIP = "127.0.0.1";
//
//    struct addrinfo hints = { 0 }, * result = nullptr;
//    hints.ai_family = AF_INET;
//    hints.ai_socktype = SOCK_DGRAM;
//
//    if (getaddrinfo(serverIP.c_str(), NULL, &hints, &result) != 0 || !result) {
//        std::cerr << "�޷�������������ַ" << std::endl;
//        enet_host_destroy(client);
//        return EXIT_FAILURE;
//    }
//
//    serverAddress.host = ((struct sockaddr_in*)result->ai_addr)->sin_addr.s_addr;
//    serverAddress.port = 1234;
//    freeaddrinfo(result);
//
//    // ��ʼ����
//    peer = enet_host_connect(client, &serverAddress, 2, 0);
//    if (!peer || enet_host_service(client, nullptr, 5000) <= 0) {
//        std::cerr << "��ʼ����ʧ��" << std::endl;
//        if (!try_reconnect(client, serverAddress)) {
//            enet_host_destroy(client);
//            return EXIT_FAILURE;
//        }
//    }
//
//    std::cout << "�ɹ����ӵ������� " << serverIP << ":1234\n";
//    std::thread heartbeat(heartbeat_thread, peer);
//
//    // ��ѭ��
//    while (running) {
//        ENetEvent event;
//        while (enet_host_service(client, &event, 100) > 0) {
//            switch (event.type) {
//            case ENET_EVENT_TYPE_RECEIVE:
//                std::cout << "�յ���Ϣ: " << event.packet->data << "\n";
//                enet_packet_destroy(event.packet);
//                break;
//
//            case ENET_EVENT_TYPE_DISCONNECT:
//                std::cout << "��������Ͽ�����\n";
//                if (!try_reconnect(client, serverAddress)) {
//                    running = false;
//                }
//                break;
//            }
//        }
//
//        // �û����봦��
//        if (_kbhit()) {
//            std::string message;
//            std::getline(std::cin, message);
//
//            if (message == "quit") {
//                running = false;
//            }
//            else if (message == "stats") {
//                print_connection_stats(peer);
//            }
//            else {
//                ENetPacket* packet = enet_packet_create(
//                    message.c_str(), message.size() + 1,
//                    ENET_PACKET_FLAG_RELIABLE);
//                enet_peer_send(peer, 0, packet);
//                std::cout << "�ѷ���: " << message << "\n";
//            }
//        }
//    }
//
//    // ����
//    running = false;
//    heartbeat.join();
//
//    if (peer) {
//        enet_peer_disconnect(peer, 0);
//        enet_host_flush(client);
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    }
//
//    enet_host_destroy(client);
//    return 0;
//}
//#include <iostream>
//#include <string>
//#include <enet/enet.h>
////#include <enet-master/include/enet/enet.h>
//#include <ws2tcpip.h>
//
//#pragma comment(lib, "enet.lib")
//#pragma comment(lib, "Ws2_32.lib")
//
//int main() {
//    if (enet_initialize() != 0) {
//        std::cerr << "ENet��ʼ��ʧ��" << std::endl;
//        return EXIT_FAILURE;
//    }
//    atexit(enet_deinitialize);
//
//    ENetHost* client = enet_host_create(NULL, 1, 2, 0, 0);
//    if (client == NULL) {
//        std::cerr << "�޷������ͻ�������" << std::endl;
//        return EXIT_FAILURE;
//    }
//
//    ENetAddress serverAddress;
//    ENetEvent event;
//    ENetPeer* peer;
//
//    std::string serverIP;
//    std::cout << "�����������IP��ַ(ֱ�ӻس�ʹ��Ĭ��127.0.0.1): ";
//    std::getline(std::cin, serverIP);
//
//    if (serverIP.empty()) {
//        serverIP = "127.0.0.1";
//    }
//
//    // ʹ�� getaddrinfo ��� enet_address_set_host
//    struct addrinfo hints = { 0 }, * result = NULL;
//    hints.ai_family = AF_INET;
//    hints.ai_socktype = SOCK_DGRAM;
//
//    if (getaddrinfo(serverIP.c_str(), NULL, &hints, &result) != 0) {
//        std::cerr << "�޷�������������ַ" << std::endl;
//        enet_host_destroy(client);
//        return EXIT_FAILURE;
//    }
//
//    if (result != NULL) {
//        serverAddress.host = ((struct sockaddr_in*)result->ai_addr)->sin_addr.s_addr;
//        freeaddrinfo(result);
//    }
//    else {
//        std::cerr << "��Ч�ĵ�ַ��Ϣ" << std::endl;
//        enet_host_destroy(client);
//        return EXIT_FAILURE;
//    }
//
//    serverAddress.port = 1234;
//
//    peer = enet_host_connect(client, &serverAddress, 2, 0);
//    if (peer == NULL) {
//        std::cerr << "�޷������Ե�����" << std::endl;
//        enet_host_destroy(client);
//        return EXIT_FAILURE;
//    }
//
//    // �ȴ����ӳɹ�
//    if (enet_host_service(client, &event, 5000) > 0 &&
//        event.type == ENET_EVENT_TYPE_CONNECT) {
//        std::cout << "�ɹ����ӵ������� " << serverIP << ":1234" << std::endl;
//    }
//    else {
//        enet_peer_reset(peer);
//        std::cerr << "���ӷ�����ʧ��" << std::endl;
//        enet_host_destroy(client);
//        return EXIT_FAILURE;
//    }
//
//    // ������Ϣѭ��
//    std::string message;
//    while (true) {
//        std::cout << "����Ҫ���͵���Ϣ(����quit�˳�): ";
//        std::getline(std::cin, message);
//
//        if (message == "quit") break;
//
//        // �����ɿ������ݰ�
//        ENetPacket* packet = enet_packet_create(
//            message.c_str(),
//            message.length() + 1,
//            ENET_PACKET_FLAG_RELIABLE);
//
//        enet_peer_send(peer, 0, packet);
//        enet_host_flush(client);
//        std::cout << "�ѷ���: " << message << std::endl;
//    }
//
//    // �Ͽ�����
//    enet_peer_disconnect(peer, 0);
//    while (enet_host_service(client, &event, 3000) > 0) {
//        switch (event.type) {
//        case ENET_EVENT_TYPE_RECEIVE:
//            enet_packet_destroy(event.packet);
//            break;
//        case ENET_EVENT_TYPE_DISCONNECT:
//            std::cout << "�ѶϿ��������������" << std::endl;
//            break;
//        }
//    }
//
//    enet_host_destroy(client);
//    return 0;
//}

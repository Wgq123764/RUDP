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
//        std::cerr << "ENet初始化失败" << std::endl;
//        return EXIT_FAILURE;
//    }
//    atexit(enet_deinitialize);
//
//    ENetHost* client = enet_host_create(NULL, 1, 2, 0, 0);
//    if (client == NULL) {
//        std::cerr << "无法创建客户端主机" << std::endl;
//        return EXIT_FAILURE;
//    }
//
//    ENetAddress serverAddress;
//    ENetEvent event;
//    ENetPeer* peer;
//
//    std::string serverIP;
//    std::cout << "请输入服务器IP地址(直接回车使用默认127.0.0.1): ";
//    std::getline(std::cin, serverIP);
//
//    if (serverIP.empty()) {
//        serverIP = "127.0.0.1";
//    }
//
//    // 使用 getaddrinfo 替代 enet_address_set_host
//    struct addrinfo hints = { 0 }, * result = NULL;
//    hints.ai_family = AF_INET;
//    hints.ai_socktype = SOCK_DGRAM;
//
//    if (getaddrinfo(serverIP.c_str(), NULL, &hints, &result) != 0) {
//        std::cerr << "无法解析服务器地址" << std::endl;
//        enet_host_destroy(client);
//        return EXIT_FAILURE;
//    }
//
//    if (result != NULL) {
//        serverAddress.host = ((struct sockaddr_in*)result->ai_addr)->sin_addr.s_addr;
//        freeaddrinfo(result);
//    }
//    else {
//        std::cerr << "无效的地址信息" << std::endl;
//        enet_host_destroy(client);
//        return EXIT_FAILURE;
//    }
//
//    serverAddress.port = 1234;
//
//    peer = enet_host_connect(client, &serverAddress, 2, 0);
//    if (peer == NULL) {
//        std::cerr << "无法创建对等连接" << std::endl;
//        enet_host_destroy(client);
//        return EXIT_FAILURE;
//    }
//
//    // 等待连接成功
//    if (enet_host_service(client, &event, 5000) > 0 &&
//        event.type == ENET_EVENT_TYPE_CONNECT) {
//        std::cout << "成功连接到服务器 " << serverIP << ":1234" << std::endl;
//    }
//    else {
//        enet_peer_reset(peer);
//        std::cerr << "连接服务器失败" << std::endl;
//        enet_host_destroy(client);
//        return EXIT_FAILURE;
//    }
//
//    // 发送消息循环
//    std::string message;
//    while (true) {
//        std::cout << "输入要发送的消息(输入quit退出): ";
//        std::getline(std::cin, message);
//
//        if (message == "quit") break;
//
//        // 创建可靠的数据包
//        ENetPacket* packet = enet_packet_create(
//            message.c_str(),
//            message.length() + 1,
//            ENET_PACKET_FLAG_RELIABLE);
//
//        enet_peer_send(peer, 0, packet);
//        enet_host_flush(client);
//        std::cout << "已发送: " << message << std::endl;
//    }
//
//    // 断开连接
//    enet_peer_disconnect(peer, 0);
//    while (enet_host_service(client, &event, 3000) > 0) {
//        switch (event.type) {
//        case ENET_EVENT_TYPE_RECEIVE:
//            enet_packet_destroy(event.packet);
//            break;
//        case ENET_EVENT_TYPE_DISCONNECT:
//            std::cout << "已断开与服务器的连接" << std::endl;
//            break;
//        }
//    }
//
//    enet_host_destroy(client);
//    return 0;
//}

//#include <iostream>
//#include <string>
//#include <thread>
//#include <atomic>
//#include <chrono>
//#include <conio.h>  // 添加这个头文件
//#include <enet/enet.h>
//#include <ws2tcpip.h>
//
//#pragma comment(lib, "enet.lib")
//#pragma comment(lib, "Ws2_32.lib")
//
//std::atomic<bool> running(true);
//
//// 心跳线程函数
//void heartbeat_thread(ENetPeer* peer) {
//    while (running) {
//        ENetPacket* packet = enet_packet_create("HEARTBEAT", 9, ENET_PACKET_FLAG_UNSEQUENCED);
//        enet_peer_send(peer, 1, packet); // 使用通道1发送心跳
//        enet_host_flush(peer->host);
//        std::this_thread::sleep_for(std::chrono::seconds(1)); // 每秒一次心跳
//    }
//}
//
//// 连接状态监控
//void print_connection_stats(ENetPeer* peer) {
//    std::cout << "\n连接状态:\n"
//        << "  往返时间: " << peer->roundTripTime << "ms\n"
//        << "  丢包率: " << peer->packetLoss / (float)ENET_PEER_PACKET_LOSS_SCALE * 100 << "%\n"
//        << "  最后活动: " << peer->lastReceiveTime << "ms ago\n";
//}
//
//// 自动重连功能
//bool try_reconnect(ENetHost* client, ENetAddress address, int max_attempts = 3) {
//    for (int i = 0; i < max_attempts; ++i) {
//        std::cout << "尝试重新连接(" << i + 1 << "/" << max_attempts << ")...\n";
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
//            std::cout << "重连成功!\n";
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
//        std::cerr << "ENet初始化失败" << std::endl;
//        return EXIT_FAILURE;
//    }
//    atexit(enet_deinitialize);
//
//    // 设置控制台关闭处理
//    SetConsoleCtrlHandler([](DWORD event) {
//        if (event == CTRL_CLOSE_EVENT) {
//            std::cout << "控制台关闭中...\n";
//            running = false;
//        }
//        return TRUE;
//        }, TRUE);
//
//    ENetHost* client = enet_host_create(NULL, 1, 2, 0, 0);
//    if (!client) {
//        std::cerr << "无法创建客户端主机" << std::endl;
//        return EXIT_FAILURE;
//    }
//
//    ENetAddress serverAddress;
//    ENetPeer* peer = nullptr;
//
//    // 配置服务器地址
//    std::string serverIP;
//    std::cout << "请输入服务器IP地址(直接回车使用默认127.0.0.1): ";
//    std::getline(std::cin, serverIP);
//    if (serverIP.empty()) serverIP = "127.0.0.1";
//
//    struct addrinfo hints = { 0 }, * result = nullptr;
//    hints.ai_family = AF_INET;
//    hints.ai_socktype = SOCK_DGRAM;
//
//    if (getaddrinfo(serverIP.c_str(), NULL, &hints, &result) != 0 || !result) {
//        std::cerr << "无法解析服务器地址" << std::endl;
//        enet_host_destroy(client);
//        return EXIT_FAILURE;
//    }
//
//    serverAddress.host = ((struct sockaddr_in*)result->ai_addr)->sin_addr.s_addr;
//    serverAddress.port = 1234;
//    freeaddrinfo(result);
//
//    // 初始连接
//    peer = enet_host_connect(client, &serverAddress, 2, 0);
//    if (!peer || enet_host_service(client, nullptr, 5000) <= 0) {
//        std::cerr << "初始连接失败" << std::endl;
//        if (!try_reconnect(client, serverAddress)) {
//            enet_host_destroy(client);
//            return EXIT_FAILURE;
//        }
//    }
//
//    std::cout << "成功连接到服务器 " << serverIP << ":1234\n";
//    std::thread heartbeat(heartbeat_thread, peer);
//
//    // 主循环
//    while (running) {
//        ENetEvent event;
//        while (enet_host_service(client, &event, 100) > 0) {
//            switch (event.type) {
//            case ENET_EVENT_TYPE_RECEIVE:
//                std::cout << "收到消息: " << event.packet->data << "\n";
//                enet_packet_destroy(event.packet);
//                break;
//
//            case ENET_EVENT_TYPE_DISCONNECT:
//                std::cout << "与服务器断开连接\n";
//                if (!try_reconnect(client, serverAddress)) {
//                    running = false;
//                }
//                break;
//            }
//        }
//
//        // 用户输入处理
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
//                std::cout << "已发送: " << message << "\n";
//            }
//        }
//    }
//
//    // 清理
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
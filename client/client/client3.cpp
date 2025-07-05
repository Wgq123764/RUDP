#include <iostream>
#include <enet/enet.h>
#include <thread>
#include <string>
int main() {
    // ============= 硬编码 IP 和端口 =============
    const char* serverIp = "127.0.0.1";  // 改为你的服务器IP（如 "192.168.1.100"）
    const int serverPort = 1234;          // 必须与服务器端口一致
    // ==========================================

    // 初始化 ENet
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialize ENet!" << std::endl;
        return 1;
    }
    atexit(enet_deinitialize);

    // 创建客户端
    ENetHost* client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!client) {
        std::cerr << "Failed to create ENet client!" << std::endl;
        return 1;
    }

    // 设置服务器地址
    ENetAddress address;
    if (enet_address_set_host(&address, serverIp) != 0) {
        std::cerr << "Failed to resolve server IP: " << serverIp << std::endl;
        return 1;
    }
    address.port = serverPort;

    // 连接服务器
    ENetPeer* peer = enet_host_connect(client, &address, 2, 0);
    if (!peer) {
        std::cerr << "Failed to initiate connection to server!" << std::endl;
        return 1;
    }

    std::cout << "Connecting to server at " << serverIp << ":" << serverPort << "..." << std::endl;

    // 等待连接成功
    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connected to server successfully!" << std::endl;
    }
    else {
        enet_peer_reset(peer);
        std::cerr << "Connection to server failed!" << std::endl;
        return 1;
    }

    // 用户输入线程（发送消息）
    std::thread inputThread([&]() {
        while (true) {
            std::string message;
            std::getline(std::cin, message);

            if (message == "quit") {
                enet_peer_disconnect(peer, 0);
                break;
            }

            // 发送消息到服务器
            ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(peer, 0, packet);
        }
        });

    // 主循环（接收服务器消息）
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

    // 清理
    inputThread.join();
    enet_host_destroy(client);
    return 0;
}
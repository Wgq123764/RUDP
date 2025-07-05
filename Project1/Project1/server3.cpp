#include <iostream>
#include <enet/enet.h>

int main() {
    // 初始化 ENet
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    // 创建服务器地址
    ENetAddress address;
    address.host = ENET_HOST_ANY; // 监听所有可用接口
    address.port = 1234;          // 使用端口1234

    // 创建服务器
    ENetHost* server = enet_host_create(&address, 32, 2, 0, 0);
    if (server == nullptr) {
        std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Server started on port " << address.port << std::endl;

    // 事件处理循环
    ENetEvent event;
    while (true) {
        // 等待事件，超时设为1000毫秒
        while (enet_host_service(server, &event, 1000) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                char ip[256];
                enet_address_get_host_ip(&event.peer->address, ip, 256);
                std::cout << "A new client connected from " << ip << ":" << event.peer->address.port << std::endl;

                // 发送欢迎消息
                const char* welcomeMsg = "Welcome to the ENet server!";
                ENetPacket* packet = enet_packet_create(welcomeMsg, strlen(welcomeMsg) + 1, ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(event.peer, 0, packet);
                break;
            }

            case ENET_EVENT_TYPE_RECEIVE: {
                std::cout << "Received packet from client: " << event.packet->data << std::endl;

                // 广播消息给所有连接的客户端
                ENetPacket* broadcastPacket = enet_packet_create(event.packet->data, event.packet->dataLength, ENET_PACKET_FLAG_RELIABLE);
                enet_host_broadcast(server, 0, broadcastPacket);

                // 清理数据包
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

    // 清理
    enet_host_destroy(server);
    return EXIT_SUCCESS;
}
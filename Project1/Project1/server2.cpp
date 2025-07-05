//#include <iostream>
//#include <enet/enet.h>
//#include <iomanip>
//
//#pragma comment(lib, "enet.lib")
//
//const char* get_disconnect_reason(uint32_t reason) {
//    static const char* reasons[] = {
//        "正常断开 (ENET_PEER_DISCONNECT_NORMAL)",
//        "超时断开 (ENET_PEER_DISCONNECT_TIMEOUT)",
//        "本地关闭 (ENET_PEER_DISCONNECT_LOCAL)",
//        "远程强制断开 (ENET_PEER_DISCONNECT_REMOTE)",
//        "协议错误 (ENET_PEER_DISCONNECT_PROTOCOL)",
//        "版本不匹配 (ENET_PEER_DISCONNECT_VERSION)",
//        "数据溢出 (ENET_PEER_DISCONNECT_OVERFLOW)"
//    };
//    reason &= 0x0F;
//    return reason < 7 ? reasons[reason] : "未知原因";
//}
//
//void print_peer_stats(ENetPeer* peer) {
//    std::cout << "连接统计:\n"
//        << "  往返时间: " << peer->roundTripTime << "ms\n"
//        << "  丢包率: " << std::fixed << std::setprecision(1)
//        << (peer->packetLoss / (float)ENET_PEER_PACKET_LOSS_SCALE * 100) << "%\n"
//        << "  最后活动时间: " << peer->lastReceiveTime << "ms\n";
//}
//
//int main() {
//    if (enet_initialize() != 0) {
//        std::cerr << "ENet初始化失败" << std::endl;
//        return EXIT_FAILURE;
//    }
//    atexit(enet_deinitialize);
//
//    ENetAddress address;
//    address.host = ENET_HOST_ANY;
//    address.port = 1234;
//
//    ENetHost* server = enet_host_create(&address, 32, 2, 0, 0);
//    if (server == NULL) {
//        std::cerr << "无法创建服务器主机" << std::endl;
//        return EXIT_FAILURE;
//    }
//
//    // 设置更宽松的超时阈值
//    for (size_t i = 0; i < server->peerCount; ++i) {
//        server->peers[i].timeoutLimit = 64;
//    }
//
//    std::cout << "服务器已启动 (端口: " << address.port << ")，等待客户端连接...\n";
//
//    ENetEvent event;
//    while (true) {
//        int eventStatus = enet_host_service(server, &event, 1000);
//
//        if (eventStatus > 0) {
//            switch (event.type) {
//            case ENET_EVENT_TYPE_CONNECT: {
//                std::cout << "\n新客户端连接: "
//                    << (event.peer->address.host & 0xFF) << "."
//                    << ((event.peer->address.host >> 8) & 0xFF) << "."
//                    << ((event.peer->address.host >> 16) & 0xFF) << "."
//                    << ((event.peer->address.host >> 24) & 0xFF)
//                    << ":" << event.peer->address.port << "\n";
//                event.peer->data = (void*)(uintptr_t)enet_time_get();
//                break;
//            }
//
//            case ENET_EVENT_TYPE_RECEIVE: {
//                std::cout << "收到数据 [" << event.channelID << "]: "
//                    << event.packet->data << "\n";
//                enet_packet_destroy(event.packet);
//                break;
//            }
//
//            case ENET_EVENT_TYPE_DISCONNECT: {
//                uint32_t connectDuration = enet_time_get() - (uintptr_t)event.peer->data;
//
//                std::cout << "\n客户端断开连接!\n"
//                    << "  原因: " << get_disconnect_reason(event.data) << "\n"
//                    << "  连接持续时间: " << connectDuration << "ms\n";
//
//                print_peer_stats(event.peer);
//
//                event.peer->data = NULL;
//                break;
//            }
//            }
//        }
//    }
//
//    enet_host_destroy(server);
//    return 0;
//}
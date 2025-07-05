//#include <iostream>
//#include <enet/enet.h>
//
//#pragma comment(lib, "enet.lib")
//
//int main() {
//    if (enet_initialize() != 0) {
//        std::cerr << "ENet初始化失败" << std::endl;
//        return EXIT_FAILURE;
//    }
//    atexit(enet_deinitialize);
//
//    ENetAddress address;
//    address.host = ENET_HOST_ANY;  // 监听所有网络接口
//    address.port = 1234;           // 服务器端口
//
//    ENetHost* server = enet_host_create(&address, 32, 2, 0, 0);
//    if (server == NULL) {
//        std::cerr << "无法创建服务器主机" << std::endl;
//        return EXIT_FAILURE;
//    }
//
//    std::cout << "服务器已启动，等待客户端连接..." << std::endl;
//
//    ENetEvent event;
//    while (true) {
//        int eventStatus = enet_host_service(server, &event, 1000);
//
//        if (eventStatus > 0) {
//            switch (event.type) {
//            case ENET_EVENT_TYPE_CONNECT:
//                std::cout << "客户端已连接: "
//                    << event.peer->address.host << ":"
//                    << event.peer->address.port << std::endl;
//                break;
//
//            case ENET_EVENT_TYPE_RECEIVE:
//                std::cout << "收到数据: " << event.packet->data << std::endl;
//                enet_packet_destroy(event.packet);
//                break;
//
//            case ENET_EVENT_TYPE_DISCONNECT:
//                std::cout << "客户端断开连接" << std::endl;
//                event.peer->data = NULL;
//                break;
//            }
//        }
//    }
//
//    enet_host_destroy(server);
//    return 0;
//}
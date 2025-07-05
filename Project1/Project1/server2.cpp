//#include <iostream>
//#include <enet/enet.h>
//#include <iomanip>
//
//#pragma comment(lib, "enet.lib")
//
//const char* get_disconnect_reason(uint32_t reason) {
//    static const char* reasons[] = {
//        "�����Ͽ� (ENET_PEER_DISCONNECT_NORMAL)",
//        "��ʱ�Ͽ� (ENET_PEER_DISCONNECT_TIMEOUT)",
//        "���عر� (ENET_PEER_DISCONNECT_LOCAL)",
//        "Զ��ǿ�ƶϿ� (ENET_PEER_DISCONNECT_REMOTE)",
//        "Э����� (ENET_PEER_DISCONNECT_PROTOCOL)",
//        "�汾��ƥ�� (ENET_PEER_DISCONNECT_VERSION)",
//        "������� (ENET_PEER_DISCONNECT_OVERFLOW)"
//    };
//    reason &= 0x0F;
//    return reason < 7 ? reasons[reason] : "δ֪ԭ��";
//}
//
//void print_peer_stats(ENetPeer* peer) {
//    std::cout << "����ͳ��:\n"
//        << "  ����ʱ��: " << peer->roundTripTime << "ms\n"
//        << "  ������: " << std::fixed << std::setprecision(1)
//        << (peer->packetLoss / (float)ENET_PEER_PACKET_LOSS_SCALE * 100) << "%\n"
//        << "  ���ʱ��: " << peer->lastReceiveTime << "ms\n";
//}
//
//int main() {
//    if (enet_initialize() != 0) {
//        std::cerr << "ENet��ʼ��ʧ��" << std::endl;
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
//        std::cerr << "�޷���������������" << std::endl;
//        return EXIT_FAILURE;
//    }
//
//    // ���ø����ɵĳ�ʱ��ֵ
//    for (size_t i = 0; i < server->peerCount; ++i) {
//        server->peers[i].timeoutLimit = 64;
//    }
//
//    std::cout << "������������ (�˿�: " << address.port << ")���ȴ��ͻ�������...\n";
//
//    ENetEvent event;
//    while (true) {
//        int eventStatus = enet_host_service(server, &event, 1000);
//
//        if (eventStatus > 0) {
//            switch (event.type) {
//            case ENET_EVENT_TYPE_CONNECT: {
//                std::cout << "\n�¿ͻ�������: "
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
//                std::cout << "�յ����� [" << event.channelID << "]: "
//                    << event.packet->data << "\n";
//                enet_packet_destroy(event.packet);
//                break;
//            }
//
//            case ENET_EVENT_TYPE_DISCONNECT: {
//                uint32_t connectDuration = enet_time_get() - (uintptr_t)event.peer->data;
//
//                std::cout << "\n�ͻ��˶Ͽ�����!\n"
//                    << "  ԭ��: " << get_disconnect_reason(event.data) << "\n"
//                    << "  ���ӳ���ʱ��: " << connectDuration << "ms\n";
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
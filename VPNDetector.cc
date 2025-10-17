#include "VPNDetector.h"

Define_Module(VPNDetector);

void VPNDetector::initialize()
{
    packetsReceived = 0;
    packetsBlocked = 0;
    packetsForwarded = 0;

    blockIPsec = par("blockIPsec");
    blockOpenVPN = par("blockOpenVPN");
    blockPPTP = par("blockPPTP");
    detectionThreshold = par("detectionThreshold");

    packetsReceivedSignal = registerSignal("packetsReceived");
    packetsBlockedSignal = registerSignal("packetsBlocked");
    packetsForwardedSignal = registerSignal("packetsForwarded");

    WATCH(packetsReceived);
    WATCH(packetsBlocked);
    WATCH(packetsForwarded);

    logFile.open("vpn_detection_log.csv");
    logFile << "timestamp,packet_id,src_ip,dst_ip,protocol,src_port,dst_port,packet_size,vpn_detected,blocked\n";

    EV << "VPN Detector initialized\n";
}

void VPNDetector::handleMessage(cMessage *msg)
{
    Packet *packet = check_and_cast<Packet *>(msg);
    packetsReceived++;
    emit(packetsReceivedSignal, 1L);

    EV << "Packet received: " << packet->getName() << "\n";

    bool isVPN = detectVPNTraffic(packet);

    if (isVPN) {
        EV << ">>> VPN TRAFFIC DETECTED! Blocking packet\n";
        packetsBlocked++;
        emit(packetsBlockedSignal, 1L);
        logPacket(packet, true);
        delete packet;
        bubble("VPN blocked!");
    }
    else {
        EV << "Legitimate traffic - forwarding\n";
        packetsForwarded++;
        emit(packetsForwardedSignal, 1L);
        logPacket(packet, false);
        send(packet, "out");
    }
}

bool VPNDetector::detectVPNTraffic(Packet *packet)
{
    try {
        auto ipHeader = packet->peekAtFront<Ipv4Header>();

        if (blockIPsec && checkIPsecProtocol(ipHeader)) {
            return true;
        }

        if (checkVPNPorts(packet, ipHeader)) {
            return true;
        }

        return false;
    }
    catch (std::exception& e) {
        EV << "Error analyzing packet: " << e.what() << "\n";
        return false;
    }
}

bool VPNDetector::checkIPsecProtocol(const Ptr<const Ipv4Header>& ipHeader)
{
    int protocol = ipHeader->getProtocolId();

    // IPsec ESP (Encapsulating Security Payload) = 50
    // IPsec AH (Authentication Header) = 51
    if (protocol == 50 || protocol == 51) {
        EV << "  IPsec protocol " << protocol << " detected\n";
        return true;
    }

    return false;
}

bool VPNDetector::checkVPNPorts(Packet *packet, const Ptr<const Ipv4Header>& ipHeader)
{
    int protocol = ipHeader->getProtocolId();

    try {
        // Check if UDP (protocol 17)
        if (protocol == 17) {  // IP_PROT_UDP = 17
            auto udpHeader = packet->peekDataAt<UdpHeader>(ipHeader->getChunkLength());
            int srcPort = udpHeader->getSrcPort();
            int dstPort = udpHeader->getDestPort();

            // IKE (Internet Key Exchange) - UDP 500
            // NAT-T (NAT Traversal) - UDP 4500
            if (blockIPsec && (srcPort == 500 || dstPort == 500 ||
                               srcPort == 4500 || dstPort == 4500)) {
                EV << "  VPN port detected: " << dstPort << "\n";
                return true;
            }

            // OpenVPN - UDP 1194
            if (blockOpenVPN && (srcPort == 1194 || dstPort == 1194)) {
                EV << "  OpenVPN port detected\n";
                return true;
            }
        }
    }
    catch (...) {
        // Not UDP or parsing error
    }

    return false;
}

void VPNDetector::logPacket(Packet *packet, bool blocked)
{
    try {
        auto ipHeader = packet->peekAtFront<Ipv4Header>();

        logFile << simTime().dbl() << ","
                << packet->getId() << ","
                << ipHeader->getSrcAddress() << ","
                << ipHeader->getDestAddress() << ","
                << (int)ipHeader->getProtocolId() << ",";

        int srcPort = 0, dstPort = 0;
        try {
            if (ipHeader->getProtocolId() == 17) {  // IP_PROT_UDP = 17
                auto udpHeader = packet->peekDataAt<UdpHeader>(ipHeader->getChunkLength());
                srcPort = udpHeader->getSrcPort();
                dstPort = udpHeader->getDestPort();
            }
        } catch (...) {}

        logFile << srcPort << "," << dstPort << ","
                << packet->getByteLength() << ","
                << (blocked ? "yes" : "no") << ","
                << (blocked ? "yes" : "no") << "\n";
        logFile.flush();
    }
    catch (...) {}
}

void VPNDetector::finish()
{
    recordScalar("totalPacketsReceived", packetsReceived);
    recordScalar("totalPacketsBlocked", packetsBlocked);
    recordScalar("totalPacketsForwarded", packetsForwarded);

    double blockRate = packetsReceived > 0 ?
        (double)packetsBlocked / packetsReceived * 100.0 : 0;
    recordScalar("blockingRate", blockRate);

    EV << "\n=== VPN Detector Statistics ===\n";
    EV << "Received: " << packetsReceived << "\n";
    EV << "Blocked: " << packetsBlocked << "\n";
    EV << "Forwarded: " << packetsForwarded << "\n";

    logFile.close();
}

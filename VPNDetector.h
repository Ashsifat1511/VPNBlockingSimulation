#ifndef __VPNBLOCKING_VPNDETECTOR_H
#define __VPNBLOCKING_VPNDETECTOR_H

#include <omnetpp.h>
#include "inet/common/INETDefs.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "inet/networklayer/common/IpProtocolId_m.h"
#include "inet/transportlayer/udp/UdpHeader_m.h"
#include <fstream>

using namespace omnetpp;
using namespace inet;

class VPNDetector : public cSimpleModule
{
  private:
    long packetsReceived;
    long packetsBlocked;
    long packetsForwarded;

    bool blockIPsec;
    bool blockOpenVPN;
    bool blockPPTP;
    double detectionThreshold;

    simsignal_t packetsReceivedSignal;
    simsignal_t packetsBlockedSignal;
    simsignal_t packetsForwardedSignal;

    std::ofstream logFile;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    bool detectVPNTraffic(Packet *packet);
    bool checkIPsecProtocol(const Ptr<const Ipv4Header>& ipHeader);
    bool checkVPNPorts(Packet *packet, const Ptr<const Ipv4Header>& ipHeader);
    void logPacket(Packet *packet, bool blocked);
};

#endif

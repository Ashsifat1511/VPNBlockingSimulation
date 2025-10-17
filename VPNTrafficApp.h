#ifndef __VPNBLOCKING_VPNTRAFFICAPP_H
#define __VPNBLOCKING_VPNTRAFFICAPP_H

#include "inet/applications/base/ApplicationBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/common/lifecycle/LifecycleOperation.h"

using namespace omnetpp;
using namespace inet;

class VPNTrafficApp : public ApplicationBase, public UdpSocket::ICallback
{
  protected:
    UdpSocket socket;
    cMessage *selfMsg = nullptr;

    int localPort;
    int destPort;
    L3Address destAddr;
    simtime_t startTime;
    simtime_t stopTime;
    simtime_t sendInterval;
    int packetLength;
    bool simulateIPsec;

    long packetsSent = 0;

  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void finish() override;

    virtual void processSend();

    // Lifecycle
    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    // UdpSocket::ICallback
    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    virtual void socketClosed(UdpSocket *socket) override;
};

#endif

#include "VPNTrafficApp.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/networklayer/common/L3AddressResolver.h"

Define_Module(VPNTrafficApp);

void VPNTrafficApp::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        localPort = par("localPort");
        destPort = par("destPort");
        startTime = par("startTime");
        stopTime = par("stopTime");
        sendInterval = par("sendInterval");
        packetLength = par("packetLength");
        simulateIPsec = par("simulateIPsec");

        selfMsg = new cMessage("sendTimer");
    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {
        const char *destAddrs = par("destAddresses");
        destAddr = L3AddressResolver().resolve(destAddrs);

        socket.setOutputGate(gate("socketOut"));
        socket.bind(localPort);
        socket.setCallback(this);
    }
}

void VPNTrafficApp::handleMessageWhenUp(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        processSend();

        simtime_t d = simTime() + sendInterval;
        if (stopTime < SIMTIME_ZERO || d < stopTime) {
            scheduleAt(d, selfMsg);
        }
    }
    else {
        socket.processMessage(msg);
    }
}

void VPNTrafficApp::processSend()
{
    char msgName[32];
    sprintf(msgName, "vpnData-%ld", packetsSent++);

    Packet *packet = new Packet(msgName);
    const auto& payload = makeShared<ByteCountChunk>(B(packetLength));
    packet->insertAtBack(payload);

    int actualDestPort = simulateIPsec ? 4500 : destPort;

    EV << "Sending VPN packet to " << destAddr << ":" << actualDestPort << "\n";

    socket.sendTo(packet, destAddr, actualDestPort);
}

void VPNTrafficApp::handleStartOperation(LifecycleOperation *operation)
{
    scheduleAt(startTime, selfMsg);
}

void VPNTrafficApp::handleStopOperation(LifecycleOperation *operation)
{
    cancelEvent(selfMsg);
    socket.close();
}

void VPNTrafficApp::handleCrashOperation(LifecycleOperation *operation)
{
    cancelEvent(selfMsg);
    socket.destroy();
}

void VPNTrafficApp::socketDataArrived(UdpSocket *socket, Packet *packet)
{
    delete packet;
}

void VPNTrafficApp::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    delete indication;
}

void VPNTrafficApp::socketClosed(UdpSocket *socket)
{
}

void VPNTrafficApp::finish()
{
    ApplicationBase::finish();
    recordScalar("packetsSent", packetsSent);
}

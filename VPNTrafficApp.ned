package vpnblocking;

import inet.applications.contract.IApp;

simple VPNTrafficApp like IApp
{
    parameters:
        @display("i=block/app");
        int localPort = default(5000);
        int destPort = default(5000);
        string destAddresses;
        double startTime @unit(s) = default(1s);
        double stopTime @unit(s) = default(-1s);
        double sendInterval @unit(s) = default(exponential(1s));
        int packetLength @unit(B) = default(1200B);
        bool simulateIPsec = default(false);

    gates:
        input socketIn;
        output socketOut;
}

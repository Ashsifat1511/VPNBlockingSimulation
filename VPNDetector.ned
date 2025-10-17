package vpnblocking;

simple VPNDetector
{
    parameters:
        @display("i=block/filter");
        @signal[packetsReceived](type=long);
        @signal[packetsBlocked](type=long);
        @signal[packetsForwarded](type=long);
        @statistic[receivedPkts](source=packetsReceived; record=count,sum,vector);
        @statistic[blockedPkts](source=packetsBlocked; record=count,sum,vector);
        @statistic[forwardedPkts](source=packetsForwarded; record=count,sum,vector);

        bool blockIPsec = default(true);
        bool blockOpenVPN = default(true);
        bool blockPPTP = default(true);
        double detectionThreshold = default(0.95);

    gates:
        input in;
        output out;
}

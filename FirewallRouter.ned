package vpnblocking;

import inet.node.inet.Router;

module FirewallRouter extends Router
{
    submodules:
        vpnDetector: VPNDetector {
            @display("p=300,100");
        }

    connections allowunconnected:
        // VPN Detector is connected to monitor and filter traffic
        // Note: This is a simplified connection model
        // In a production system, you'd integrate this into the forwarding path
}

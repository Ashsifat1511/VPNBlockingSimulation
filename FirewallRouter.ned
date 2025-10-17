package vpnblocking;

import inet.node.inet.Router;

module FirewallRouter extends Router
{
    submodules:
        vpnDetector: VPNDetector {
            @display("p=300,100");
        }
}

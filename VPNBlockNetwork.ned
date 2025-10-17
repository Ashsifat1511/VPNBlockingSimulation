package vpnblocking;

import inet.node.inet.StandardHost;
import inet.node.inet.Router;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;

network VPNBlockNetwork
{
    parameters:
        @display("bgb=800,400");

    types:
        channel EthernetLink extends ned.DatarateChannel
        {
            datarate = 100Mbps;
            delay = 0.1us;
        }

    submodules:
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=50,50");
                config = xml("<config>"
                    + "<interface hosts='client' address='192.168.1.10' netmask='255.255.255.0'/>"
                    + "<interface hosts='gateway' names='eth0' address='192.168.1.1' netmask='255.255.255.0'/>"
                    + "<interface hosts='gateway' names='eth1' address='10.0.0.1' netmask='255.255.255.0'/>"
                    + "<interface hosts='blockedServer' address='10.0.0.10' netmask='255.255.255.0'/>"
                    + "<route hosts='client' destination='10.0.0.0' netmask='255.255.255.0' gateway='192.168.1.1'/>"
                    + "<route hosts='blockedServer' destination='192.168.1.0' netmask='255.255.255.0' gateway='10.0.0.1'/>"
                    + "</config>");
        }

        visualizer: IntegratedCanvasVisualizer {
            @display("p=50,150");
        }

        client: StandardHost {
            @display("p=150,200;i=device/laptop");
        }

        gateway: FirewallRouter {
            @display("p=400,200;i=device/router");
        }

        blockedServer: StandardHost {
            @display("p=650,200;i=device/server");
        }

    connections:
        client.ethg++ <--> EthernetLink <--> gateway.ethg++;
        gateway.ethg++ <--> EthernetLink <--> blockedServer.ethg++;
}

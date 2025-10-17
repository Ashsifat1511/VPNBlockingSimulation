# VPN Blocking Simulation - INET Version Configuration

## Project Overview
This project simulates VPN traffic detection and blocking using OMNeT++ 6.0.3 and INET 4.5 framework.

## INET Version Requirements

**Required INET Version: 4.5**

This project has been configured to work with **INET 4.5**, which is the recommended version for OMNeT++ 6.0.3.

## Configuration Changes Made

The following configuration files have been updated to reference INET 4.5:

### 1. `.oppbuildspec`
Updated the build specification to link against INET 4.5:
```xml
<buildspec version="4.0">
    <dir makemake-options="--deep -O out -KINET4_5_PROJ=../inet4.5 -DINET_IMPORT -I$(INET4_5_PROJ)/src -L$(INET4_5_PROJ)/src -lINET$(D)" path="." type="makemake"/>
</buildspec>
```

### 2. `.project`
Updated the project dependency from `inet4.4` to `inet4.5`:
```xml
<projects>
    <project>inet4.5</project>
</projects>
```

## Building the Project

### Using OMNeT++ IDE:
1. Open OMNeT++ IDE
2. Import both projects:
   - `inet4.5` (from `omnetpp-6.0.3/samples/inet4.5`)
   - `VPNBlockingSimulation` (this project)
3. Build `inet4.5` first if not already built
4. Clean and rebuild `VPNBlockingSimulation`

### Using Command Line:
```bash
cd d:/Downloads/omnetpp-6.0.3/samples/VPNBlockingSimulation
# Generate Makefile
opp_makemake -f --deep -O out -KINET4_5_PROJ=../inet4.5 -DINET_IMPORT -I$(INET4_5_PROJ)/src -L$(INET4_5_PROJ)/src -lINET$(D)
# Build
make MODE=release
```

## Running the Simulation

After successful build, run:
```bash
./VPNBlockingSimulation -u Cmdenv -c WithVPN
```

Or use the IDE to run with Qtenv for graphical interface.

## Key Dependencies from INET 4.5

The project uses the following INET components:
- `inet.node.inet.StandardHost` - For client and server nodes
- `inet.node.inet.Router` - Base for FirewallRouter
- `inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator` - Network configuration
- `inet.visualizer.integrated.IntegratedCanvasVisualizer` - Network visualization
- `inet.applications.base.ApplicationBase` - Base for VPNTrafficApp
- `inet.transportlayer.contract.udp.UdpSocket` - UDP communication
- Various header files for packet inspection (Ipv4Header, UdpHeader, etc.)

## Compatibility Notes

- **OMNeT++ Version**: 6.0.3 (required)
- **INET Version**: 4.5 (configured)
- The project will NOT work with INET 4.4 due to API differences
- If you experience issues, ensure INET 4.5 is properly built before building this project

## Troubleshooting

### If you get "cannot find -lINET" error:
1. Ensure INET 4.5 is built: `cd ../inet4.5 && make MODE=release`
2. Verify the INET path in `.oppbuildspec` points to the correct location

### If you get header file not found errors:
1. Check that INET 4.5 is in the correct location: `../inet4.5` relative to this project
2. Verify the include paths in the build configuration

### If you get undefined reference errors:
1. Clean both projects: `make clean`
2. Rebuild INET 4.5 first
3. Then rebuild VPNBlockingSimulation

## Project Structure

- `VPNBlockNetwork.ned` - Network topology definition
- `FirewallRouter.ned` - Router with VPN detection capabilities
- `VPNDetector.ned/.h/.cc` - VPN detection module implementation
- `VPNTrafficApp.ned/.h/.cc` - VPN traffic generator application
- `omnetpp.ini` - Simulation configuration
- `package.ned` - Package declaration

## Simulation Configurations

1. **WithVPN** (default): Tests VPN traffic detection and blocking
2. **Baseline**: Runs without VPN traffic for comparison

## Statistics Collected

- Packets received/blocked/forwarded by VPN detector
- Blocking rate percentage
- Detailed packet logs in `vpn_detection_log.csv`

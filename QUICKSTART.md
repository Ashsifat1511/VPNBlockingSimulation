# VPN Blocking Simulation - Quick Start Guide

## ✅ INET Version Fixed!

The project has been successfully configured to use **INET 4.5**, which is the correct version for OMNeT++ 6.0.3.

## 📝 Summary of Changes

| File | Change Description |
|------|-------------------|
| `.oppbuildspec` | Updated to link with INET 4.5 |
| `.project` | Changed project dependency from `inet4.4` to `inet4.5` |
| `Makefile` | Added INET 4.5 include paths, library paths, and linker flags |
| `README_INET_CONFIG.md` | Created comprehensive configuration guide |
| `INET_VERSION_FIX.md` | Created detailed fix documentation |

## 🚀 Quick Build Instructions

### Using OMNeT++ IDE (Easiest):
1. Open OMNeT++ IDE
2. Import projects (if needed):
   - `File` → `Import` → `Existing Projects into Workspace`
   - Browse to `omnetpp-6.0.3/samples`
   - Select both `inet4.5` and `VPNBlockingSimulation`
3. Build `inet4.5` first (right-click → Build Project)
4. Build `VPNBlockingSimulation` (right-click → Build Project)
5. Run simulation (right-click `omnetpp.ini` → Run As → OMNeT++ Simulation)

### Using Command Line:
```bash
# Open OMNeT++ shell (mingwenv.cmd on Windows)
cd d:/Downloads/omnetpp-6.0.3/samples/VPNBlockingSimulation

# Clean previous builds
rm -rf out

# Build the project
make MODE=release

# Run simulation
./VPNBlockingSimulation -u Cmdenv -c WithVPN
```

## 📊 What This Project Does

This simulation demonstrates:
- **VPN Traffic Detection**: Identifies IPsec, OpenVPN, and PPTP traffic
- **Traffic Blocking**: Blocks detected VPN traffic at the firewall/gateway
- **Statistics Collection**: Records detection rates and blocking statistics

### Network Topology:
```
[Client] ←→ [Gateway with VPN Detector] ←→ [Blocked Server]
```

### Traffic Types:
1. Normal legitimate traffic (HTTP - port 80)
2. VPN/IPsec traffic (port 4500) - **Will be blocked**
3. Other legitimate traffic (port 8080)

## 📂 Key Files

- **Network Definition**: `VPNBlockNetwork.ned`
- **VPN Detector Module**: `VPNDetector.ned/.h/.cc`
- **VPN Traffic Generator**: `VPNTrafficApp.ned/.h/.cc`
- **Configuration**: `omnetpp.ini`

## 🎯 Available Configurations

1. **WithVPN** (default):
   - Generates VPN traffic starting at 5s
   - VPN detector blocks the traffic
   - Shows blocking in action

2. **Baseline**:
   - Disables VPN traffic (starts at 1000s, effectively never)
   - Shows normal operation without VPN

## 📈 Expected Results

After running, you should see:
- ✅ VPN traffic detected and blocked
- ✅ Normal traffic forwarded successfully
- ✅ Statistics showing blocking rate (~100% for VPN traffic)
- ✅ Log file: `vpn_detection_log.csv` with packet details

## 🔍 Output Files

- **Scalar Results**: `.sca` files with statistics
- **Vector Results**: `.vec` files with time-series data
- **Detection Log**: `vpn_detection_log.csv` - detailed packet information

## ⚠️ Prerequisites

Before building, ensure:
- ✅ OMNeT++ 6.0.3 is installed and configured
- ✅ INET 4.5 is present at `../inet4.5`
- ✅ INET 4.5 is built (contains compiled libraries)
- ✅ OMNeT++ bin directory is in your PATH

## 🐛 Troubleshooting

### Problem: "cannot find -lINET"
**Solution**: Build INET 4.5 first
```bash
cd ../inet4.5
make MODE=release
```

### Problem: "undefined reference to inet::"
**Solution**: Ensure INET_IMPORT is defined and library path is correct
- Check the Makefile has `-DINET_IMPORT` in COPTS
- Check the Makefile has `-L$(INET4_5_PROJ)/src -lINET$(D)` in LIBS

### Problem: Header files not found
**Solution**: Verify INET path
- Ensure `../inet4.5/src` exists
- Check `.oppbuildspec` has correct INET path

## 📖 Documentation

For more details, see:
- `README_INET_CONFIG.md` - Configuration details and build instructions
- `INET_VERSION_FIX.md` - Complete fix documentation with rationale

## 🎉 You're Ready!

The project is now correctly configured to use INET 4.5. Simply build and run to see VPN detection in action!

---

**Project**: VPN Blocking Simulation  
**OMNeT++ Version**: 6.0.3  
**INET Version**: 4.5 ✅  
**Status**: Ready to build and run

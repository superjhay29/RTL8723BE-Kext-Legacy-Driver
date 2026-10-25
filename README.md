# RTL8723BE-macOS-WiFi (Backported) 🚀
A Realtek RTL8723BE PCIe Wi-Fi driver backport targeting macOS Big Sur and newer, with OpenCore integration.
📌 Overview

RTL8723BE-macOS-WiFi (Backported) aims to bring Realtek RTL8723BE PCIe wireless adapter support to macOS Big Sur (11.0) and newer releases.

Based on the original Ventura-and-newer project, this fork focuses on extending compatibility to Big Sur and Monterey while aiming to maintain support for Ventura, Sonoma, Sequoia, and subsequent releases.

**Upstream Source:** [AA-EION/RTL8723BE-macOS-WiFi](https://github.com/AA-EION/RTL8723BE-macOS-WiFi)

This backport: superjhay29/RTL8723BE-macOS-WiFi

Credits: Full credit goes to AA-EION and all upstream contributors for the original driver architecture, codebase, hardware research, and reverse-engineering work. This project builds upon their foundational contributions.
## ✨ Features
Big Sur Backport: Targets macOS Big Sur 11.x and newer.
Realtek RTL8723BE: Designed for compatible PCIe Wi-Fi adapters.
OpenCore Integration: Supports installation through the OpenCore Kernel -> Add configuration.
Wireless Networking: Aims to enable network scanning, Wi-Fi connections, and DHCP.
Multi-Version Goals: Targets Monterey, Ventura, Sonoma, and Sequoia alongside Big Sur.
Community Development: Encourages compatibility testing, bug reports, and contributions.

Note: Actual functionality depends on the driver build, adapter revision, and macOS version. WPA3, Bluetooth, and advanced continuity features are not guaranteed.

##📋 System Requirements
OS: macOS Big Sur (11.x), Monterey (12.x), Ventura (13.x), Sonoma (14.x), or Sequoia (15.x), subject to testing.
Hardware: Realtek RTL8723BE PCIe Wi-Fi adapter.
Bootloader: OpenCore.
Architecture: Compatible x86-64 Hackintosh system.
Tools: ProperTree for configuration editing and ocvalidate for OpenCore validation.

Realtek's PCI vendor ID is 10EC. Verify your adapter's actual device ID before assuming compatibility.

## 🚀 Quick Start & Installation
1. Clone the repository
bash
git clone https://github.com/superjhay29/RTL8723BE-macOS-WiFi.git
cd RTL8723BE-macOS-WiFi

2. Install the kext

Back up your EFI folder, then copy the compiled RTL8723BE.kext into:

text
EFI/OC/Kexts/RTL8723BE.kext


If the repository contains source code only, build the kext according to its build instructions first.

3. Configure OpenCore
Open EFI/OC/config.plist using ProperTree.
Add the kext under Kernel -> Add.
Ensure BundlePath, ExecutablePath, and PlistPath match the actual kext bundle.
Run ProperTree's OC Snapshot function.
Validate the configuration with the matching version of ocvalidate.
Warning: Do not replace existing configuration entries or inject additional Apple networking kexts without confirming that the driver requires them.
4. Reboot and test

Restart macOS and check System Settings → Network for a Wi-Fi interface.

If the adapter is not detected, inspect the PCI device ID, OpenCore configuration, and system logs.

## 🛠️ Troubleshooting
Problem	Suggested solution

**Q: Adapter not detected**	

    A: Verify PCI IDs, kext paths, and device matching rules.

**Q: Firmware loading failure**	

    A: Check the required firmware files and compatibility.

**Q: Kernel panic**	

    A: Boot from a known-good EFI backup and disable the kext.

**Q: Connected without internet**

    A: Check DHCP, IP assignment, and router configuration.

**Q: Wi-Fi breaks after updating macOS**

    A:   Verify compatibility with the new macOS build.

When reporting issues, include your macOS version, OpenCore version, adapter device ID, driver revision, and relevant logs. Remove sensitive information before sharing.

## 🤝 Credits & Acknowledgements

This project is directly based on:

AA-EION/RTL8723BE-macOS-WiFi

Special thanks to AA-EION and all contributors for the foundational driver implementation, reverse-engineering, and hardware research that made this backport possible.

Upstream repository: https://github.com/AA-EION/RTL8723BE-macOS-WiFi
Backport repository: https://github.com/superjhay29/RTL8723BE-macOS-WiFi

Contributions, testing, and reproducible bug reports are welcome.

📄 License & Disclaimer

Refer to the repository's LICENSE file and the upstream license terms. Preserve all applicable copyright notices and attribution.

This project is not affiliated with or endorsed by Apple Inc. or Realtek Semiconductor Corp. Third-party kernel extensions may cause instability, boot failures, or data loss.

Use at your own risk. Always keep a working EFI backup.

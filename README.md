RTL8723BE-macOS-WiFi (Backported)

Realtek RTL8723BE PCIe Wi-Fi driver for macOS Big Sur and newer, with OpenCore support.

!macOS !OpenCore !Architecture

Original upstream: AA-EION/RTL8723BE-macOS-WiFi

This backport: superjhay29/RTL8723BE-macOS-WiFi

Overview

This project aims to extend the upstream Realtek RTL8723BE Wi-Fi driver's compatibility to macOS Big Sur (11) and newer, including Monterey, Ventura, Sonoma, and Sequoia.

The backport focuses on older macOS compatibility while preserving the foundational driver architecture and reverse-engineering work of the original project.

Status: Compatibility and individual features depend on the driver build and hardware. Check the repository's releases and issues for verified results.
Credits

This project is directly based on and forked from AA-EION/RTL8723BE-macOS-WiFi.

Full credit goes to AA-EION and all upstream contributors for the original codebase, driver architecture, hardware research, and reverse-engineering efforts. This repository builds upon their foundational work.

Please preserve the upstream copyright notices and applicable license terms.

Compatibility
macOS: Big Sur 11.x, Monterey 12.x, Ventura 13.x, Sonoma 14.x, and Sequoia 15.x, subject to testing.
Hardware: Realtek RTL8723BE PCIe Wi-Fi adapters.
Bootloader: OpenCore.
Architecture: Compatible x86-64 Hackintosh systems.

Realtek's PCI vendor ID is 10EC. Verify your adapter's actual device ID before assuming compatibility.

Features

Intended functionality, subject to validation:

Wi-Fi adapter detection and initialization.
Wireless network scanning.
WPA/WPA2 connectivity where implemented.
Network interface registration and DHCP.
macOS network settings integration.

Limitations: WPA3, Bluetooth, AirDrop, sleep/wake, and support for every macOS release are not guaranteed.

Installation
Back up your working EFI folder.
Clone this backport:
bash
git clone https://github.com/superjhay29/RTL8723BE-macOS-WiFi.git

Copy the compiled RTL8723BE.kext into:
text
EFI/OC/Kexts/RTL8723BE.kext

Open config.plist using ProperTree.
Add the kext under Kernel -> Add, ensuring the bundle and executable paths match the actual build.
Run ProperTree's OC Snapshot function and validate the configuration with the matching version of ocvalidate.
Reboot and check whether Wi-Fi appears in macOS Network Settings.
Warning: Do not inject additional Apple networking kexts or change system security settings unless required by the specific driver build. Keep a recoverable EFI backup.
Troubleshooting

Wi-Fi adapter not detected

Verify the PCI device ID and driver matching rules.
Check kext paths and OpenCore configuration.
Review boot and kernel logs.

Firmware loading errors

Verify that the required firmware files are present and compatible.

Kernel panic

Boot using your backed-up EFI.
Disable the kext and inspect the panic report.
Check compatibility with your macOS version.

Connected but no internet

Check DHCP, IP assignment, router settings, and network interface registration.
Contributing

Bug reports and compatibility tests are welcome. Include your macOS version, OpenCore version, adapter device ID, driver revision, and relevant logs. Remove personal information before sharing logs.

License and Disclaimer

Refer to the repository's LICENSE file and the upstream license terms. Do not assume a license without verifying the original project's requirements.

This project is not affiliated with Apple Inc. or Realtek Semiconductor Corp. Kernel extensions may cause boot failures, instability, or data loss. Use at your own risk.

Links
Original upstream repository: https://github.com/AA-EION/RTL8723BE-macOS-WiFi
This backport: https://github.com/superjhay29/RTL8723BE-macOS-WiFi
OpenCore documentation: https://dortania.github.io/OpenCore-Install-Guide/

Special thanks to AA-EION and all contributors for their foundational work.

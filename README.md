# RTL8723BE macOS kext (work in progress)

Status: **Phase 2 (read-only register access).** Attaches to PCI `10EC:B723`, maps BAR2 and logs a
register dump. It does **not** provide Wi-Fi yet. Untested beyond the
checks below.

Target: Intel Hackintosh (OpenCore), macOS 11 Big Sur and later (developed on 12.7.6 Monterey).

## Build (done by GitHub Actions on a macOS runner)

Push to `main`. The `build-kext` workflow builds the kext and uploads two artifacts:

- `RTL8723BE-kext`: a zip containing `RTL8723BE.kext.zip` (extract twice to get `RTL8723BE.kext`)
- `RTL8723BE-dmg`: the same kext packaged in a `.dmg`

Local build on a Mac (no Xcode GUI, needs Xcode Command Line Tools):

    make
    make dmg

## Install (OpenCore)

1. Copy `RTL8723BE.kext` to `EFI/OC/Kexts/`.
2. In `config.plist`, `Kernel -> Add`, add an entry (ProperTree "OC Snapshot" does this):
   - `Arch` = `Any`, `Enabled` = `true`
   - `BundlePath` = `RTL8723BE.kext`
   - `ExecutablePath` = `Contents/MacOS/RTL8723BE`
   - `PlistPath` = `Contents/Info.plist`
   - `Comment`, `MinKernel`, `MaxKernel` can be empty strings
3. Disable any other Realtek Wi-Fi kext so two drivers do not compete for the card.
4. Reboot.

## Verify

    ioreg -l | grep -i RTL8723BE
    kextstat | grep -i RTL8723BE
    log show --last 10m | grep RTL8723BE

Expected: a log line `RTL8723BE: attached; BAR2 len=...`.

## Test-load without rebooting (SIP disabled)

    sudo cp -R RTL8723BE.kext /tmp/
    sudo chown -R root:wheel /tmp/RTL8723BE.kext
    sudo chmod -R 755 /tmp/RTL8723BE.kext
    sudo kmutil load -p /tmp/RTL8723BE.kext

## Uninstall

Remove the `Kernel -> Add` entry and delete the kext from `EFI/OC/Kexts/`, then reboot.

## Known limitations

- No Wi-Fi function yet: no firmware load, TX/RX, scan or association.
- BAR2 as the register window is taken from the Linux rtlwifi driver and still needs hardware confirmation.
- Bundle ID `com.example.RTL8723BE` is a placeholder.

## Changelog

- 0.0.4: read-only register dump in start() (PCI command reg + 8 chip registers), logged as "RTL8723BE: reg ...". No writes to the chip.
- 0.0.3: add src/kmod_info.cpp (kernel logged "can't find prelinked kext kmod_info address").
- 0.0.2: add `com.apple.iokit.IOPCIFamily` to `OSBundleLibraries` (0.0.1 failed to link:
  could not bind `IOPCIDevice::metaClass` and `extendedConfigRead16`).

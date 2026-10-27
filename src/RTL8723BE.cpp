// 0.0.4: PCI attach + READ-ONLY register dump. No writes to the chip. No Wi-Fi yet.
#include <libkern/OSByteOrder.h>
#include "RTL8723BE.hpp"

#define super IOService
OSDefineMetaClassAndStructors(RTL8723BE, IOService)

static const UInt16 kVendorRealtek = 0x10EC;
static const UInt16 kDevRTL8723BE  = 0xB723;

// Register offsets recalled from Linux rtlwifi (rtl8723be/reg.h).
// NEEDS VERIFICATION against the Linux source before trusting any value.
static const UInt32 kRegSysIsoCtrl = 0x0000; // 16-bit
static const UInt32 kRegSysFuncEn  = 0x0002; // 16-bit
static const UInt32 kRegApsFsmco   = 0x0004; // 32-bit
static const UInt32 kRegSysClkr    = 0x0008; // 32-bit
static const UInt32 kRegMcuFwdl    = 0x0080; // 32-bit (firmware download status)
static const UInt32 kRegSysCfg1    = 0x00F0; // 32-bit (chip version bits)
static const UInt32 kRegSysCfg2    = 0x00FC; // 32-bit (chip id)
static const UInt32 kRegCr         = 0x0100; // 16-bit (command register)

IOService *RTL8723BE::probe(IOService *provider, SInt32 *score) {
    IOPCIDevice *dev = OSDynamicCast(IOPCIDevice, provider);
    if (!dev) return nullptr;
    UInt16 ven = dev->configRead16(kIOPCIConfigVendorID);
    UInt16 id  = dev->configRead16(kIOPCIConfigDeviceID);
    if (ven != kVendorRealtek || id != kDevRTL8723BE) {
        RTL_LOG("probe: unexpected PCI ID %04x:%04x, declining", ven, id);
        return nullptr;
    }
    return super::probe(provider, score);
}

UInt32 RTL8723BE::rd32(UInt32 off) {
    if (!regMap || (UInt64)off + 4 > regMap->getLength()) return 0xFFFFFFFF;
    return OSReadLittleInt32((volatile void *)regMap->getVirtualAddress(), off);
}
UInt16 RTL8723BE::rd16(UInt32 off) {
    if (!regMap || (UInt64)off + 2 > regMap->getLength()) return 0xFFFF;
    return OSReadLittleInt16((volatile void *)regMap->getVirtualAddress(), off);
}
UInt8 RTL8723BE::rd8(UInt32 off) {
    if (!regMap || (UInt64)off + 1 > regMap->getLength()) return 0xFF;
    return *((volatile UInt8 *)regMap->getVirtualAddress() + off);
}

void RTL8723BE::dumpRegisters() {
    // PCI config space first (this path is already known to work).
    UInt16 cmd = pci->configRead16(0x04);
    RTL_LOG("pci: command=%04x (bit1=mem, bit2=busmaster)", cmd);

    UInt16 sysIso  = rd16(kRegSysIsoCtrl);
    UInt16 funcEn  = rd16(kRegSysFuncEn);
    UInt32 fsmco   = rd32(kRegApsFsmco);
    UInt32 clkr    = rd32(kRegSysClkr);
    UInt32 fwdl    = rd32(kRegMcuFwdl);
    UInt32 cfg1    = rd32(kRegSysCfg1);
    UInt32 cfg2    = rd32(kRegSysCfg2);
    UInt16 cr      = rd16(kRegCr);

    RTL_LOG("reg 0x0000 SYS_ISO_CTRL = %04x", sysIso);
    RTL_LOG("reg 0x0002 SYS_FUNC_EN  = %04x", funcEn);
    RTL_LOG("reg 0x0004 APS_FSMCO    = %08x", (unsigned)fsmco);
    RTL_LOG("reg 0x0008 SYS_CLKR     = %08x", (unsigned)clkr);
    RTL_LOG("reg 0x0080 MCUFWDL      = %08x", (unsigned)fwdl);
    RTL_LOG("reg 0x00F0 SYS_CFG1     = %08x", (unsigned)cfg1);
    RTL_LOG("reg 0x00FC SYS_CFG2     = %08x", (unsigned)cfg2);
    RTL_LOG("reg 0x0100 CR           = %04x", cr);

    if (cfg1 == 0xFFFFFFFF && cfg2 == 0xFFFFFFFF && fsmco == 0xFFFFFFFF) {
        RTL_LOG("verdict: ALL-ONES; chip not responding (link down, powered off, or wrong BAR)");
    } else if (cfg1 == 0 && cfg2 == 0 && fsmco == 0 && clkr == 0) {
        RTL_LOG("verdict: ALL-ZEROS; chip not powered or BAR2 is not the register window");
    } else {
        RTL_LOG("verdict: registers return mixed values; compare against Linux values");
    }
}

bool RTL8723BE::start(IOService *provider) {
    if (!super::start(provider)) return false;
    pci = OSDynamicCast(IOPCIDevice, provider);
    if (!pci) { super::stop(provider); return false; }
    pci->retain();
    pci->setBusMasterEnable(true);
    pci->setMemoryEnable(true);

    // Linux rtlwifi maps PCI BAR 2 for registers. NEEDS HARDWARE VERIFICATION.
    regMap = pci->mapDeviceMemoryWithRegister(kIOPCIConfigBaseAddress2);
    if (!regMap) {
        RTL_LOG("failed to map BAR2");
        pci->release(); pci = nullptr;
        super::stop(provider);
        return false;
    }
    RTL_LOG("attached; BAR2 len=%llu", (unsigned long long)regMap->getLength());
    dumpRegisters();
    registerService();
    return true;
}

void RTL8723BE::stop(IOService *provider) {
    RTL_LOG("stop");
    if (regMap) { regMap->release(); regMap = nullptr; }
    if (pci) { pci->setBusMasterEnable(false); pci->release(); pci = nullptr; }
    super::stop(provider);
}

void RTL8723BE::free() { super::free(); }

// Phase 1 skeleton: PCI attach only. No Wi-Fi functionality yet.
#include "RTL8723BE.hpp"

#define super IOService
OSDefineMetaClassAndStructors(RTL8723BE, IOService)

static const UInt16 kVendorRealtek = 0x10EC;
static const UInt16 kDevRTL8723BE  = 0xB723;

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

#pragma once
#include <IOKit/IOService.h>
#include <IOKit/pci/IOPCIDevice.h>

#define RTL_LOG(fmt, ...) IOLog("RTL8723BE: " fmt "\n", ##__VA_ARGS__)

class RTL8723BE : public IOService {
    OSDeclareDefaultStructors(RTL8723BE)
public:
    IOService *probe(IOService *provider, SInt32 *score) override;
    bool start(IOService *provider) override;
    void stop(IOService *provider) override;
    void free() override;
private:
    IOPCIDevice *pci = nullptr;
    IOMemoryMap *regMap = nullptr;   // MMIO registers (BAR2)

    // Read-only MMIO helpers. Return 0xFFFFFFFF if the offset is out of range.
    UInt32 rd32(UInt32 off);
    UInt16 rd16(UInt32 off);
    UInt8  rd8(UInt32 off);
    void   dumpRegisters();          // 0.0.4: read-only diagnostic dump
};

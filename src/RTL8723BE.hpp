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
    IOMemoryMap *regMap = nullptr;   // MMIO registers
};

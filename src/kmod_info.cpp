// kmod_info.cpp
// Xcode normally generates this file. Without it the kext has no kmod_info
// symbol, and the kernel logs: "can't find prelinked kext kmod_info address".
// The version below must match CFBundleVersion in Info.plist.
// NEEDS CI/HARDWARE VERIFICATION: macro usage copied from Xcode's generated *_info.c pattern.
#include <mach/mach_types.h>
#include <libkern/libkern.h>

extern "C" {
extern kern_return_t _start(kmod_info_t *ki, void *data);
extern kern_return_t _stop(kmod_info_t *ki, void *data);

__attribute__((visibility("default")))
KMOD_EXPLICIT_DECL(com.example.RTL8723BE, 0.0.3, _start, _stop)
__private_extern__ kmod_start_func_t *_realmain = 0;
__private_extern__ kmod_stop_func_t  *_antimain = 0;
__private_extern__ int _kext_apple_cc = __APPLE_CC__;
}

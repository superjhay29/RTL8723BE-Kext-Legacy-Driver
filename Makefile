# Command-line kext build (no Xcode GUI). Runs on macOS only.
# Builds x86_64 for macOS 11.0 and later.
KEXT      := RTL8723BE
BUNDLE_ID := com.example.RTL8723BE
SDK       := $(shell xcrun --sdk macosx --show-sdk-path)
KHDR      := $(SDK)/System/Library/Frameworks/Kernel.framework/Headers
ARCH      := x86_64
MINOS     := 11.0
BUILD     := build
OUT       := $(BUILD)/$(KEXT).kext
SRCS      := $(wildcard src/*.cpp)
OBJS      := $(patsubst src/%.cpp,$(BUILD)/%.o,$(SRCS))

CXXFLAGS := -arch $(ARCH) -mmacosx-version-min=$(MINOS) -isysroot $(SDK) \
  -mkernel -fno-builtin -fno-rtti -fno-exceptions -fno-common -fapple-kext \
  -nostdinc -nostdinc++ -std=c++11 -Os -Wall \
  -I$(KHDR) -I$(KHDR)/libkern/c++ \
  -DKERNEL -DKERNEL_PRIVATE -DDRIVER_PRIVATE -DAPPLE -DNeXT \
  -DKEXT_BUNDLE_ID=\"$(BUNDLE_ID)\"

LDFLAGS := -arch $(ARCH) -mmacosx-version-min=$(MINOS) -isysroot $(SDK) \
  -nostdlib -Xlinker -kext -Xlinker -object_path_lto \
  -lkmodc++ -lkmod -lcc_kext

all: $(OUT)

$(BUILD)/%.o: src/%.cpp
	@mkdir -p $(BUILD)
	xcrun clang++ $(CXXFLAGS) -c $< -o $@

$(OUT): $(OBJS) Info.plist
	@mkdir -p $(OUT)/Contents/MacOS
	xcrun clang++ $(LDFLAGS) $(OBJS) -o $(OUT)/Contents/MacOS/$(KEXT)
	cp Info.plist $(OUT)/Contents/Info.plist
	plutil -lint $(OUT)/Contents/Info.plist
	codesign --force --sign - $(OUT) || true

dmg: $(OUT)
	bash scripts/make_dmg.sh $(OUT)

clean:
	rm -rf $(BUILD)

.PHONY: all dmg clean

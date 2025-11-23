# ------------------------------------------------------------
# Detect architecture and choose vcpkg triplet
# ------------------------------------------------------------
ARCH := $(shell uname -m)

ifeq ($(ARCH),x86_64)
    DETECT_MSG := "Detected architecture: x86_64"
else ifeq ($(ARCH),aarch64)
    DETECT_MSG := "Detected architecture: aarch64"
else
    $(error Unsupported architecture: $(ARCH))
endif

# Build directory
BUILD_DIR := build

# Default config (Debug or Release)
CONFIG ?= Release

# ------------------------------------------------------------
# Mark phony targets
# ------------------------------------------------------------
.PHONY: init build debug release rebuild configure clean

# ------------------------------------------------------------
# Initialization (run once)
# ------------------------------------------------------------
init:
	@echo $(DETECT_MSG)
	@echo "Using vcpkg triplet: $(VCPKG_TRIPLET)"
	vcpkg integrate install
	cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake"

# ------------------------------------------------------------
# Build (default: Release)
# ------------------------------------------------------------
build:
	@echo "Building ($(CONFIG))..."
	cmake --build $(BUILD_DIR) --config $(CONFIG) -j $$(nproc)

# Shortcut targets
debug:
	$(MAKE) build CONFIG=Debug

release:
	$(MAKE) build CONFIG=Release

# ------------------------------------------------------------
# Force reconfigure
# ------------------------------------------------------------
configure:
	@echo "Reconfiguring CMake..."
	rm -rf $(BUILD_DIR)
	cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake"

# ------------------------------------------------------------
# Full rebuild
# ------------------------------------------------------------
rebuild: clean init build

# ------------------------------------------------------------
# Clean build files
# ------------------------------------------------------------
clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)
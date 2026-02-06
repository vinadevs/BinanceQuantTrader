# ============================================================
#  BinanceQuantTrader - Makefile
# ============================================================

# ------------------------------------------------------------
# Project Information
# ------------------------------------------------------------
PROJECT_NAME    := BinanceQuantTrader
PROJECT_VERSION := 0.0.1
BUILD_DIR       := Build

# ------------------------------------------------------------
# Git Information
# ------------------------------------------------------------
GIT_BRANCH      := $(shell git rev-parse --abbrev-ref HEAD 2>/dev/null || echo "unknown")
GIT_COMMIT      := $(shell git rev-parse --short HEAD 2>/dev/null || echo "unknown")
GIT_COMMIT_FULL := $(shell git rev-parse HEAD 2>/dev/null || echo "unknown")
GIT_DIRTY       := $(shell git diff --quiet 2>/dev/null && echo "" || echo "-dirty")
GIT_TAG         := $(shell git describe --tags --abbrev=0 2>/dev/null || echo "no-tag")

# ------------------------------------------------------------
# Build Information
# ------------------------------------------------------------
BUILD_DATE      := $(shell date '+%Y-%m-%d')
BUILD_TIME      := $(shell date '+%H:%M:%S')
BUILD_TIMESTAMP := $(shell date '+%Y-%m-%d %H:%M:%S')
BUILD_USER      := $(shell whoami)
BUILD_HOST      := $(shell hostname)

# ------------------------------------------------------------
# Detect architecture and choose vcpkg triplet
# ------------------------------------------------------------
ARCH := $(shell uname -m)
OS   := $(shell uname -s)

ifeq ($(ARCH),x86_64)
    DETECT_MSG := "Detected architecture: x86_64"
else ifeq ($(ARCH),aarch64)
    DETECT_MSG := "Detected architecture: aarch64"
    VCPKG_TRIPLET := arm64-linux-dynamic
else
    $(error Unsupported architecture: $(ARCH))
endif

# Default config (Debug, Release, RelWithDebInfo)
CONFIG ?= Debug-Linux

# Colors for terminal output
CYAN    := \033[36m
GREEN   := \033[32m
YELLOW  := \033[33m
RED     := \033[31m
BOLD    := \033[1m
RESET   := \033[0m

# ------------------------------------------------------------
# Mark phony targets
# ------------------------------------------------------------
.PHONY: help info version init build debug release relwithdebinfo rebuild configure clean

# ------------------------------------------------------------
# Default target: help
# ------------------------------------------------------------
.DEFAULT_GOAL := help

# ------------------------------------------------------------
# Help
# ------------------------------------------------------------
help:
	@echo ""
	@echo "$(BOLD)$(CYAN)╔════════════════════════════════════════════════════════════════╗$(RESET)"
	@echo "$(BOLD)$(CYAN)║          $(PROJECT_NAME) Build System                       ║$(RESET)"
	@echo "$(BOLD)$(CYAN)╚════════════════════════════════════════════════════════════════╝$(RESET)"
	@echo ""
	@echo "$(BOLD)Usage:$(RESET)"
	@echo "  make [target] [CONFIG=Debug|Release|RelWithDebInfo]"
	@echo ""
	@echo "$(BOLD)Build Targets:$(RESET)"
	@echo "  $(GREEN)build$(RESET)          Build the project (default: CONFIG=Debug)"
	@echo "  $(GREEN)debug$(RESET)          Build in Debug mode"
	@echo "  $(GREEN)release$(RESET)        Build in Release mode"
	@echo "  $(GREEN)relwithdebinfo$(RESET) Build in RelWithDebInfo mode"
	@echo "  $(GREEN)rebuild$(RESET)        Clean and rebuild the project"
	@echo ""
	@echo "$(BOLD)Setup Targets:$(RESET)"
	@echo "  $(GREEN)init$(RESET)           Initialize vcpkg integration"
	@echo "  $(GREEN)configure$(RESET)      Force reconfigure CMake"
	@echo ""
	@echo "$(BOLD)Utility Targets:$(RESET)"
	@echo "  $(GREEN)clean$(RESET)          Remove build directory"
	@echo "  $(GREEN)info$(RESET)           Show current build configuration"
	@echo "  $(GREEN)version$(RESET)        Show project version"
	@echo "  $(GREEN)help$(RESET)           Show this help message"
	@echo ""
	@echo "$(BOLD)Run Targets:$(RESET)"
	@echo "  $(GREEN)run$(RESET)            Run the project"
	@echo ""
	@echo "$(BOLD)Examples:$(RESET)"
	@echo "  make build                  # Build with default config (Debug)"
	@echo "  make build CONFIG=Release   # Build in Release mode"
	@echo "  make release                # Shortcut for Release build"
	@echo "  make clean build            # Clean then build"
	@echo "  make run                    # Run the project"
	@echo ""

# ------------------------------------------------------------
# Version
# ------------------------------------------------------------
version:
	@echo "$(PROJECT_NAME) v$(PROJECT_VERSION)"

# ------------------------------------------------------------
# Build Information
# ------------------------------------------------------------
info:
	@echo ""
	@echo "$(BOLD)$(CYAN)╔════════════════════════════════════════════════════════════════╗$(RESET)"
	@echo "$(BOLD)$(CYAN)║                    Build Information                           ║$(RESET)"
	@echo "$(BOLD)$(CYAN)╚════════════════════════════════════════════════════════════════╝$(RESET)"
	@echo ""
	@echo "$(BOLD)Project:$(RESET)"
	@echo "  Name:           $(PROJECT_NAME)"
	@echo "  Version:        $(PROJECT_VERSION)"
	@echo ""
	@echo "$(BOLD)Git:$(RESET)"
	@echo "  Branch:         $(GIT_BRANCH)"
	@echo "  Commit:         $(GIT_COMMIT)$(GIT_DIRTY)"
	@echo "  Full SHA:       $(GIT_COMMIT_FULL)"
	@echo "  Tag:            $(GIT_TAG)"
	@echo ""
	@echo "$(BOLD)Build:$(RESET)"
	@echo "  Configuration:  $(CONFIG)"
	@echo "  Build Dir:      $(BUILD_DIR)/$(CONFIG)"
	@echo "  Date:           $(BUILD_DATE)"
	@echo "  Time:           $(BUILD_TIME)"
	@echo "  User:           $(BUILD_USER)"
	@echo "  Host:           $(BUILD_HOST)"
	@echo ""
	@echo "$(BOLD)System:$(RESET)"
	@echo "  OS:             $(OS)"
	@echo "  Architecture:   $(ARCH)"
	@echo "  CMake:          $(shell cmake --version | head -n1)"
	@echo "  vcpkg:          $(shell vcpkg version 2>/dev/null | head -n1 || echo 'not installed')"
	@echo "  GCC:            $(shell gcc --version | head -n1 2>/dev/null || echo 'not installed')"
	@echo "  Clang:          $(shell clang --version | head -n1 2>/dev/null || echo 'not installed')"
	@echo ""

# ------------------------------------------------------------
# Initialization (run once)
# ------------------------------------------------------------
init:
	@echo "$(BOLD)$(CYAN)[INIT]$(RESET) Initializing project..."
	@echo $(DETECT_MSG)
	@echo "Using vcpkg triplet: $(VCPKG_TRIPLET)"
	vcpkg integrate install
	@echo "$(BOLD)$(GREEN)[DONE]$(RESET) Initialization complete"

# ------------------------------------------------------------
# Build (default: Debug-Linux)
# ------------------------------------------------------------
build:
	@echo ""
	@echo "$(BOLD)$(CYAN)════════════════════════════════════════════════════════════════$(RESET)"
	@echo "$(BOLD)  Building $(PROJECT_NAME) v$(PROJECT_VERSION)$(RESET)"
	@echo "$(BOLD)  Configuration: $(CONFIG) | Branch: $(GIT_BRANCH) | Commit: $(GIT_COMMIT)$(GIT_DIRTY)$(RESET)"
	@echo "$(BOLD)$(CYAN)════════════════════════════════════════════════════════════════$(RESET)"
	@echo ""
	@echo "$(BOLD)$(YELLOW)[CMAKE]$(RESET) Configuring with preset $(CONFIG)..."
	cmake --preset $(CONFIG) -DVCPKG_TARGET_TRIPLET=$(VCPKG_TRIPLET)
	@echo ""
	@echo "$(BOLD)$(YELLOW)[BUILD]$(RESET) Compiling with preset $(CONFIG)..."
	cmake --build --preset $(CONFIG) -DVCPKG_TARGET_TRIPLET=$(VCPKG_TRIPLET)
	@echo ""
	@echo "$(BOLD)$(GREEN)[SUCCESS]$(RESET) Build completed at $(BUILD_TIMESTAMP)"

# Shortcut targets
debug:
	@${MAKE} build CONFIG=Debug-Linux

release:
	@${MAKE} build CONFIG=Release-Linux

relwithdebinfo:
	@${MAKE} build CONFIG=RelWithDebInfo-Linux

# ------------------------------------------------------------
# Force reconfigure
# ------------------------------------------------------------
configure:
	@echo "$(BOLD)$(YELLOW)[CONFIGURE]$(RESET) Reconfiguring CMake..."
	rm -rf $(BUILD_DIR)
	@${MAKE} init
	@${MAKE} build CONFIG=Debug-Linux

# ------------------------------------------------------------
# Full rebuild
# ------------------------------------------------------------
rebuild: clean init build

# ------------------------------------------------------------
# Distclean build files
# ------------------------------------------------------------
distclean:
	@echo "$(BOLD)$(RED)[CLEAN]$(RESET) Removing build directory..."
	rm -rf $(BUILD_DIR)
	@echo "$(BOLD)$(GREEN)[DONE]$(RESET) Clean complete"

# ------------------------------------------------------------
# Clean build files
# ------------------------------------------------------------
clean:
	@echo "$(BOLD)$(RED)[CLEAN]$(RESET) Removing build artifacts..."
	cmake --build --preset $(CONFIG) --target clean
	@echo "$(BOLD)$(GREEN)[DONE]$(RESET) Clean complete"

# ------------------------------------------------------------
# Run
# ------------------------------------------------------------
run-app:
	@echo "$(BOLD)$(YELLOW)[RUN]$(RESET) Running project..."
	./Build/$(CONFIG)/Bin/BinanceQuantTrader --config_binance_quant_trader_path Source/Configurations/BQT/BinanceQuantTraderCfg-US.xml --config_binance_access_key_path Source/Configurations/BQT/BinanceAKCfg.xml
	@echo "$(BOLD)$(GREEN)[DONE]$(RESET) Run complete"

run-simulator:
	@echo "$(BOLD)$(YELLOW)[SIMULATE]$(RESET) Simulating project..."
	./Build/$(CONFIG)/Bin/BackTesting --config_exchange_simulator_path Source/Configurations/Simulator/BinanceExchangeSimulatorCfg.xml
	@echo "$(BOLD)$(GREEN)[DONE]$(RESET) Simulate complete"

run-broker:
	@echo "$(BOLD)$(YELLOW)[MESSAGE_SERVER]$(RESET) Running message server..."
	./Build/$(CONFIG)/Bin/MessageHubServer --config_message_hub_server_path Source/Configurations/MessageServer/MessageBrokerCfg.xml
	@echo "$(BOLD)$(GREEN)[DONE]$(RESET) Message server complete"
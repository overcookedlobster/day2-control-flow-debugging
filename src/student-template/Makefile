# Makefile for Day 2: Control Flow and Debugging Lab
# Automated Register Monitoring System

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic
DEBUG_FLAGS = -g -DDEBUG -O0
RELEASE_FLAGS = -O2 -DNDEBUG
INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = bin
TEST_DIR = tests

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
EXECUTABLES = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%)

# Test files
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_EXECUTABLES = $(TEST_SOURCES:$(TEST_DIR)/%.c=$(BUILD_DIR)/%)

# Default target
.PHONY: all
all: setup $(BUILD_DIR)/register_monitor $(BUILD_DIR)/test_functions $(BUILD_DIR)/debug_practice $(BUILD_DIR)/test_validation $(BUILD_DIR)/multi_chip_monitor $(BUILD_DIR)/error_recovery

# Setup build directory
.PHONY: setup
setup:
	@mkdir -p $(BUILD_DIR)

# Build individual programs
$(BUILD_DIR)/register_monitor: $(SRC_DIR)/register_monitor.c $(SRC_DIR)/monitor_utils.c $(SRC_DIR)/test_functions.c
	@echo "Building $@..."
	$(CC) $(CFLAGS) -DREGISTER_MONITOR_STANDALONE -I$(INCLUDE_DIR) $(SRC_DIR)/register_monitor.c $(SRC_DIR)/monitor_utils.c $(SRC_DIR)/test_functions.c -o $@

$(BUILD_DIR)/test_functions: $(SRC_DIR)/test_functions.c $(SRC_DIR)/monitor_utils.c
	@echo "Building $@..."
	$(CC) $(CFLAGS) -DTEST_FUNCTIONS_STANDALONE -I$(INCLUDE_DIR) $< $(SRC_DIR)/monitor_utils.c -o $@

$(BUILD_DIR)/debug_practice: $(SRC_DIR)/debug_practice.c $(SRC_DIR)/monitor_utils.c
	@echo "Building $@..."
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $< $(SRC_DIR)/monitor_utils.c -o $@

# Build test programs
$(BUILD_DIR)/test_%: $(TEST_DIR)/test_%.c $(SRC_DIR)/monitor_utils.c
	@echo "Building test $@..."
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $< $(SRC_DIR)/monitor_utils.c -o $@

# Build validation test
$(BUILD_DIR)/test_validation: $(TEST_DIR)/test_validation.c $(SRC_DIR)/monitor_utils.c $(SRC_DIR)/register_monitor.c $(SRC_DIR)/test_functions.c
	@echo "Building validation tests..."
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(TEST_DIR)/test_validation.c $(SRC_DIR)/monitor_utils.c $(SRC_DIR)/register_monitor.c $(SRC_DIR)/test_functions.c -o $@ -lm

# Build homework programs
$(BUILD_DIR)/multi_chip_monitor: $(SRC_DIR)/multi_chip_monitor.c $(SRC_DIR)/monitor_utils.c $(SRC_DIR)/register_monitor.c $(SRC_DIR)/test_functions.c
	@echo "Building homework 1..."
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(SRC_DIR)/multi_chip_monitor.c $(SRC_DIR)/monitor_utils.c $(SRC_DIR)/register_monitor.c $(SRC_DIR)/test_functions.c -o $@ -lm

$(BUILD_DIR)/error_recovery: $(SRC_DIR)/error_recovery.c $(SRC_DIR)/monitor_utils.c $(SRC_DIR)/register_monitor.c $(SRC_DIR)/test_functions.c
	@echo "Building homework 2..."
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(SRC_DIR)/error_recovery.c $(SRC_DIR)/monitor_utils.c $(SRC_DIR)/register_monitor.c $(SRC_DIR)/test_functions.c -o $@ -lm

# Debug builds
.PHONY: debug
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Release builds
.PHONY: release
release: CFLAGS += $(RELEASE_FLAGS)
release: clean all

# Build specific programs
.PHONY: register-monitor
register-monitor: $(BUILD_DIR)/register_monitor

.PHONY: test-functions
test-functions: $(BUILD_DIR)/test_functions

.PHONY: debug-practice
debug-practice: $(BUILD_DIR)/debug_practice

# Testing targets
.PHONY: test
test: debug test-conditionals test-loops test-functions test-validation test-debug

.PHONY: test-conditionals
test-conditionals: $(BUILD_DIR)/register_monitor
	@echo "=== Testing Conditional Logic ==="
	@$(BUILD_DIR)/register_monitor

.PHONY: test-loops
test-loops: $(BUILD_DIR)/register_monitor
	@echo "=== Testing Loop Operations ==="
	@$(BUILD_DIR)/register_monitor

.PHONY: test-functions
test-functions: $(BUILD_DIR)/test_functions
	@echo "=== Testing Modular Functions ==="
	@$(BUILD_DIR)/test_functions

.PHONY: test-validation
test-validation: $(BUILD_DIR)/test_validation
	@echo "=== Running Validation Tests ==="
	@$(BUILD_DIR)/test_validation

.PHONY: test-debug
test-debug: $(BUILD_DIR)/debug_practice
	@echo "=== Testing Debug Practice ==="
	@echo "Run manually with GDB: gdb $(BUILD_DIR)/debug_practice"

# GDB debugging session
.PHONY: gdb-session
gdb-session: debug-practice
	@echo "Starting GDB debugging session..."
	@echo "Useful GDB commands:"
	@echo "  break main"
	@echo "  run"
	@echo "  next"
	@echo "  step"
	@echo "  print variable_name"
	@echo "  continue"
	@echo "  backtrace"
	@echo "  quit"
	gdb $(BUILD_DIR)/debug_practice

# Memory checking with Valgrind
.PHONY: valgrind
valgrind: debug
	@echo "Running memory check with Valgrind..."
	@if command -v valgrind >/dev/null 2>&1; then \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(BUILD_DIR)/register_monitor; \
	else \
		echo "Valgrind not installed. Install with: sudo apt-get install valgrind"; \
	fi

# Code style checking
.PHONY: style-check
style-check:
	@echo "Checking code style..."
	@if command -v astyle >/dev/null 2>&1; then \
		astyle --style=linux --indent=spaces=4 --max-code-length=100 $(SRC_DIR)/*.c $(INCLUDE_DIR)/*.h; \
	else \
		echo "astyle not installed. Install with: sudo apt-get install astyle"; \
	fi

# Static analysis
.PHONY: static-analysis
static-analysis:
	@echo "Running static analysis..."
	@if command -v cppcheck >/dev/null 2>&1; then \
		cppcheck --enable=all --std=c11 -I$(INCLUDE_DIR) $(SRC_DIR)/; \
	else \
		echo "cppcheck not installed. Install with: sudo apt-get install cppcheck"; \
	fi

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
	@rm -f $(SRC_DIR)/*.orig $(INCLUDE_DIR)/*.orig  # Remove astyle backup files

# Install development tools
.PHONY: install-tools
install-tools:
	@echo "Installing development tools..."
	sudo apt-get update
	sudo apt-get install -y gcc gdb valgrind astyle cppcheck

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all              - Build all programs"
	@echo "  debug            - Build with debug flags"
	@echo "  release          - Build with optimization"
	@echo "  register-monitor - Build register monitor program"
	@echo "  test-functions   - Build test functions program"
	@echo "  debug-practice   - Build debug practice program"
	@echo "  test             - Run all tests"
	@echo "  test-conditionals- Test conditional logic"
	@echo "  test-loops       - Test loop operations"
	@echo "  test-functions   - Test modular functions"
	@echo "  test-debug       - Instructions for debug testing"
	@echo "  gdb-session      - Start GDB debugging session"
	@echo "  valgrind         - Run memory checking"
	@echo "  style-check      - Check code style"
	@echo "  static-analysis  - Run static code analysis"
	@echo "  clean            - Remove build artifacts"
	@echo "  install-tools    - Install development tools"
	@echo "  help             - Show this help message"

# Prevent make from deleting intermediate files
.PRECIOUS: $(BUILD_DIR)/%.o


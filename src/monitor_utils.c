/**
 * @file monitor_utils.c
 * @brief Utility functions for the monitor system
 *
 * This file provides working implementations of utility functions
 * that students can use as reference or building blocks.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <time.h>
#include "monitor.h"

/**
 * @brief Initialize monitor system with default values
 * @param system Pointer to monitor system structure
 */
void init_monitor_system(monitor_system_t *system) {
    if (system == NULL) {
        return;
    }

    // Initialize system parameters
    system->voltage = NOMINAL_VOLTAGE;
    system->temperature = TEMP_NORMAL;
    system->current = NOMINAL_CURRENT;
    system->status = STATUS_NORMAL;
    system->error_count = 0;
    system->system_active = true;
    system->num_registers = 4; // Initialize with 4 test registers

    // Initialize test registers
    const char* reg_names[] = {"CTRL_REG", "STATUS_REG", "DATA_REG", "CONFIG_REG"};
    uint32_t base_addr = 0x40000000;

    for (int i = 0; i < system->num_registers; i++) {
        system->registers[i].address = base_addr + (i * 4);
        system->registers[i].value = 0x12345678 + i;
        system->registers[i].expected_min = 0x10000000;
        system->registers[i].expected_max = 0x20000000;
        system->registers[i].is_valid = true;
        strncpy(system->registers[i].name, reg_names[i], sizeof(system->registers[i].name) - 1);
        system->registers[i].name[sizeof(system->registers[i].name) - 1] = '\0';
    }
}

/**
 * @brief Cleanup monitor system resources
 * @param system Pointer to monitor system structure
 */
void cleanup_monitor_system(monitor_system_t *system) {
    if (system == NULL) {
        return;
    }

    system->system_active = false;
    printf("Monitor system cleaned up\n");
}

/**
 * @brief Simulate reading from a hardware register
 * @param address Register address
 * @return Simulated register value
 */
uint32_t read_register(uint32_t address) {
    // Simulate register read with some variation
    static uint32_t counter = 0;
    counter++;

    // Base value with some variation
    uint32_t base_value = 0x12345678;
    uint32_t variation = (counter % 16) << 4;

    return base_value + variation;
}

/**
 * @brief Simulate writing to a hardware register
 * @param address Register address
 * @param value Value to write
 * @return true if write successful, false otherwise
 */
bool write_register(uint32_t address, uint32_t value) {
    // Simulate register write
    printf("Writing 0x%08X to register 0x%08X\n", value, address);
    return true;
}

/**
 * @brief Delay for specified milliseconds
 * @param milliseconds Delay time in milliseconds
 */
void delay_ms(int milliseconds) {
    usleep(milliseconds * 1000);
}

/**
 * @brief Print system state for debugging
 * @param system Pointer to monitor system structure
 */
void print_system_state(const monitor_system_t *system) {
    if (system == NULL) {
        printf("ERROR: System pointer is NULL\n");
        return;
    }

    printf("=== System State ===\n");
    printf("Voltage: %.2fV\n", system->voltage);
    printf("Temperature: %.1f°C\n", system->temperature);
    printf("Current: %.3fA\n", system->current);
    printf("Status: %d\n", system->status);
    printf("Error Count: %d\n", system->error_count);
    printf("System Active: %s\n", system->system_active ? "YES" : "NO");
    printf("Registers: %d\n", system->num_registers);

    for (int i = 0; i < system->num_registers; i++) {
        printf("  %s: 0x%08X (%s)\n",
               system->registers[i].name,
               system->registers[i].value,
               system->registers[i].is_valid ? "VALID" : "INVALID");
    }
    printf("==================\n");
}

/**
 * @brief Log error message
 * @param error Error code
 * @param message Error message
 */
void log_error(error_code_t error, const char *message) {
    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0'; // Remove newline

    printf("[%s] ERROR %d: %s\n", time_str, error, message);
}

/**
 * @brief Debug register dump
 * @param system Pointer to monitor system structure
 */
void debug_register_dump(const monitor_system_t *system) {
    if (system == NULL) {
        return;
    }

    printf("=== Register Dump ===\n");
    for (int i = 0; i < system->num_registers; i++) {
        printf("Register %d (%s):\n", i, system->registers[i].name);
        printf("  Address: 0x%08X\n", system->registers[i].address);
        printf("  Value: 0x%08X\n", system->registers[i].value);
        printf("  Range: [0x%08X, 0x%08X]\n",
               system->registers[i].expected_min,
               system->registers[i].expected_max);
        printf("  Valid: %s\n", system->registers[i].is_valid ? "YES" : "NO");
    }
    printf("====================\n");
}

/**
 * @brief Simulate hardware failure for testing
 * @param system Pointer to monitor system structure
 * @param error_type Type of error to simulate
 * @return true if simulation successful, false otherwise
 */
bool simulate_hardware_failure(monitor_system_t *system, error_code_t error_type) {
    if (system == NULL) {
        return false;
    }

    switch (error_type) {
        case ERROR_VOLTAGE_LOW:
            system->voltage = MIN_VOLTAGE - 0.1f;
            break;
        case ERROR_VOLTAGE_HIGH:
            system->voltage = MAX_VOLTAGE + 0.1f;
            break;
        case ERROR_TEMPERATURE_HIGH:
            system->temperature = TEMP_CRITICAL + 5.0f;
            break;
        case ERROR_CURRENT_LOW:
            system->current = MIN_CURRENT - 0.01f;
            break;
        case ERROR_CURRENT_HIGH:
            system->current = MAX_CURRENT + 0.1f;
            break;
        default:
            return false;
    }

    system->error_count++;
    printf("Simulated hardware failure: %d\n", error_type);
    return true;
}

/**
 * @brief Emergency shutdown procedure
 * @param system Pointer to monitor system structure
 */
void emergency_shutdown(monitor_system_t *system) {
    if (system == NULL) {
        return;
    }

    printf("=== EMERGENCY SHUTDOWN ===\n");
    printf("Shutting down system safely...\n");

    system->system_active = false;
    system->status = STATUS_CRITICAL;

    // Simulate shutdown sequence
    for (int i = 0; i < system->num_registers; i++) {
        write_register(system->registers[i].address, 0x00000000);
    }

    printf("Emergency shutdown complete\n");
}


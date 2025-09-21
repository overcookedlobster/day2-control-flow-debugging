/**
 * @file multi_chip_monitor.c
 * @brief Homework 1: Advanced Loop Patterns (60 minutes)
 *
 * This file implements advanced loop patterns for multi-chip monitoring
 * systems. Students will learn nested loops, optimization techniques,
 * and complex iteration patterns.
 *
 * Learning objectives:
 * - Master nested loop structures
 * - Implement loop optimization techniques
 * - Handle complex iteration patterns
 * - Practice early termination strategies
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include "../include/monitor.h"

// Multi-chip system constants
#define MAX_CHIPS 8
#define MAX_REGISTERS_PER_CHIP 16
#define CHIP_SCAN_INTERVAL 100  // milliseconds

/**
 * @brief Multi-chip system structure
 */
typedef struct {
    int chip_id;
    bool is_active;
    monitor_system_t monitor;
    int priority_level;  // 1=high, 2=medium, 3=low
} chip_system_t;

/**
 * @brief Multi-chip monitoring array
 */
static chip_system_t chip_systems[MAX_CHIPS];
static int active_chip_count = 0;

/**
 * @brief Initialize multi-chip monitoring system
 * @param num_chips Number of chips to monitor
 * @return true if initialization successful, false otherwise
 */
bool init_multi_chip_system(int num_chips) {
    if (num_chips <= 0 || num_chips > MAX_CHIPS) {
        printf("ERROR: Invalid number of chips (%d). Must be 1-%d\n", num_chips, MAX_CHIPS);
        return false;
    }

    printf("Initializing multi-chip monitoring system with %d chips...\n", num_chips);

    // Initialize each chip system using nested loops
    for (int chip = 0; chip < num_chips; chip++) {
        chip_systems[chip].chip_id = chip;
        chip_systems[chip].is_active = true;
        chip_systems[chip].priority_level = (chip % 3) + 1; // Distribute priorities

        // Initialize monitor system for each chip
        init_monitor_system(&chip_systems[chip].monitor);

        // Customize each chip's register configuration
        for (int reg = 0; reg < chip_systems[chip].monitor.num_registers; reg++) {
            // Modify register addresses to be chip-specific
            chip_systems[chip].monitor.registers[reg].address += (chip * 0x1000);

            // Set chip-specific names
            snprintf(chip_systems[chip].monitor.registers[reg].name,
                    sizeof(chip_systems[chip].monitor.registers[reg].name),
                    "CHIP%d_REG%d", chip, reg);
        }

        printf("  Chip %d initialized (Priority: %d)\n", chip, chip_systems[chip].priority_level);
    }

    active_chip_count = num_chips;
    printf("Multi-chip system initialization complete\n");
    return true;
}

/**
 * @brief Scan all registers across all chips using nested loops
 * @return Total number of valid registers found
 */
int scan_all_chips_registers(void) {
    printf("=== Multi-Chip Register Scan ===\n");

    int total_valid = 0;
    int total_scanned = 0;

    // Outer loop: iterate through all chips
    for (int chip = 0; chip < active_chip_count; chip++) {
        if (!chip_systems[chip].is_active) {
            printf("Chip %d: INACTIVE - skipping\n", chip);
            continue; // Skip inactive chips
        }

        printf("Scanning Chip %d (Priority %d):\n", chip, chip_systems[chip].priority_level);

        // Inner loop: iterate through registers in current chip
        for (int reg = 0; reg < chip_systems[chip].monitor.num_registers; reg++) {
            total_scanned++;

            // Read register value
            uint32_t value = read_register(chip_systems[chip].monitor.registers[reg].address);
            chip_systems[chip].monitor.registers[reg].value = value;

            // Validate register
            bool is_valid = validate_register(
                chip_systems[chip].monitor.registers[reg].address,
                value,
                chip_systems[chip].monitor.registers[reg].expected_min,
                chip_systems[chip].monitor.registers[reg].expected_max
            );

            chip_systems[chip].monitor.registers[reg].is_valid = is_valid;

            if (is_valid) {
                total_valid++;
                printf("  %s: 0x%08X ✓\n",
                       chip_systems[chip].monitor.registers[reg].name, value);
            } else {
                printf("  %s: 0x%08X ✗\n",
                       chip_systems[chip].monitor.registers[reg].name, value);
                chip_systems[chip].monitor.error_count++;

                // Early termination for critical chip failures
                if (chip_systems[chip].priority_level == 1 &&
                    chip_systems[chip].monitor.error_count >= 3) {
                    printf("  CRITICAL: High-priority chip %d has too many errors, aborting scan\n", chip);
                    return total_valid; // Early exit
                }
            }
        }

        printf("  Chip %d scan complete: %d/%d valid\n",
               chip, count_valid_registers(&chip_systems[chip].monitor),
               chip_systems[chip].monitor.num_registers);
    }

    printf("Multi-chip scan complete: %d/%d registers valid across %d chips\n",
           total_valid, total_scanned, active_chip_count);

    return total_valid;
}

/**
 * @brief Priority-based monitoring with nested loops and optimization
 * @param duration_seconds How long to monitor
 */
void priority_based_monitoring(int duration_seconds) {
    printf("=== Priority-Based Multi-Chip Monitoring ===\n");
    printf("Monitoring for %d seconds with priority optimization...\n", duration_seconds);

    time_t start_time = time(NULL);
    int iteration = 0;

    while (time(NULL) - start_time < duration_seconds) {
        iteration++;
        printf("\n--- Monitoring Iteration %d ---\n", iteration);

        // Priority 1 (High): Monitor every iteration
        for (int chip = 0; chip < active_chip_count; chip++) {
            if (!chip_systems[chip].is_active || chip_systems[chip].priority_level != 1) {
                continue;
            }

            printf("High Priority - Chip %d:\n", chip);
            update_all_registers(&chip_systems[chip].monitor);

            // Check for critical conditions
            if (check_critical_conditions(&chip_systems[chip].monitor)) {
                printf("CRITICAL: High-priority chip %d failure detected!\n", chip);
                // Implement emergency response
                chip_systems[chip].is_active = false;
                active_chip_count--;
            }
        }

        // Priority 2 (Medium): Monitor every 2nd iteration
        if (iteration % 2 == 0) {
            for (int chip = 0; chip < active_chip_count; chip++) {
                if (!chip_systems[chip].is_active || chip_systems[chip].priority_level != 2) {
                    continue;
                }

                printf("Medium Priority - Chip %d:\n", chip);
                update_all_registers(&chip_systems[chip].monitor);
            }
        }

        // Priority 3 (Low): Monitor every 3rd iteration
        if (iteration % 3 == 0) {
            for (int chip = 0; chip < active_chip_count; chip++) {
                if (!chip_systems[chip].is_active || chip_systems[chip].priority_level != 3) {
                    continue;
                }

                printf("Low Priority - Chip %d:\n", chip);
                update_all_registers(&chip_systems[chip].monitor);
            }
        }

        // Check if all chips are inactive
        if (active_chip_count == 0) {
            printf("All chips inactive - terminating monitoring\n");
            break;
        }

        // Sleep between iterations
        delay_ms(CHIP_SCAN_INTERVAL);
    }

    printf("Priority-based monitoring completed after %d iterations\n", iteration);
}

/**
 * @brief Cross-chip correlation analysis using nested loops
 */
void cross_chip_correlation_analysis(void) {
    printf("=== Cross-Chip Correlation Analysis ===\n");

    // Nested loops for chip-to-chip comparison
    for (int chip1 = 0; chip1 < active_chip_count; chip1++) {
        if (!chip_systems[chip1].is_active) continue;

        for (int chip2 = chip1 + 1; chip2 < active_chip_count; chip2++) {
            if (!chip_systems[chip2].is_active) continue;

            printf("Comparing Chip %d vs Chip %d:\n", chip1, chip2);

            // Compare voltage levels
            float voltage_diff = chip_systems[chip1].monitor.voltage -
                               chip_systems[chip2].monitor.voltage;
            printf("  Voltage difference: %.3fV\n", voltage_diff);

            if (fabs(voltage_diff) > 0.2f) {
                printf("  WARNING: Significant voltage difference detected\n");
            }

            // Compare temperature levels
            float temp_diff = chip_systems[chip1].monitor.temperature -
                            chip_systems[chip2].monitor.temperature;
            printf("  Temperature difference: %.1f°C\n", temp_diff);

            if (fabs(temp_diff) > 10.0f) {
                printf("  WARNING: Significant temperature difference detected\n");
            }

            // Compare register patterns
            int matching_registers = 0;
            for (int reg = 0; reg < chip_systems[chip1].monitor.num_registers &&
                              reg < chip_systems[chip2].monitor.num_registers; reg++) {

                if (chip_systems[chip1].monitor.registers[reg].is_valid ==
                    chip_systems[chip2].monitor.registers[reg].is_valid) {
                    matching_registers++;
                }
            }

            float match_percentage = (float)matching_registers /
                                   chip_systems[chip1].monitor.num_registers * 100.0f;
            printf("  Register pattern match: %.1f%%\n", match_percentage);

            if (match_percentage < 80.0f) {
                printf("  WARNING: Low register pattern correlation\n");
            }
        }
    }
}

/**
 * @brief Optimized batch processing with loop unrolling
 */
void optimized_batch_processing(void) {
    printf("=== Optimized Batch Processing ===\n");

    // Process chips in batches of 4 for optimization
    const int BATCH_SIZE = 4;

    for (int batch_start = 0; batch_start < active_chip_count; batch_start += BATCH_SIZE) {
        int batch_end = (batch_start + BATCH_SIZE < active_chip_count) ?
                        batch_start + BATCH_SIZE : active_chip_count;

        printf("Processing batch: chips %d-%d\n", batch_start, batch_end - 1);

        // Unrolled loop for better performance (process 4 chips simultaneously)
        for (int i = batch_start; i < batch_end; i++) {
            if (!chip_systems[i].is_active) continue;

            // Parallel processing simulation
            printf("  Chip %d: Reading registers...\n", i);

            // Process registers in groups of 4 for cache efficiency
            for (int reg = 0; reg < chip_systems[i].monitor.num_registers; reg += 4) {
                int reg_end = (reg + 4 < chip_systems[i].monitor.num_registers) ?
                              reg + 4 : chip_systems[i].monitor.num_registers;

                // Unrolled register processing
                for (int r = reg; r < reg_end; r++) {
                    uint32_t value = read_register(chip_systems[i].monitor.registers[r].address);
                    chip_systems[i].monitor.registers[r].value = value;
                }
            }
        }

        printf("  Batch %d processing complete\n", batch_start / BATCH_SIZE);
    }
}

/**
 * @brief Main function for multi-chip monitoring homework
 */
int multi_chip_monitoring(int num_chips) {
    printf("=== Homework 1: Advanced Loop Patterns ===\n");
    printf("Multi-Chip Monitoring System\n\n");

    // Initialize system
    if (!init_multi_chip_system(num_chips)) {
        return -1;
    }

    // Demonstrate advanced loop patterns
    printf("\n1. Nested Loop Register Scanning:\n");
    int valid_registers = scan_all_chips_registers();
    printf("Total valid registers: %d\n", valid_registers);

    printf("\n2. Priority-Based Monitoring (10 seconds):\n");
    priority_based_monitoring(10);

    printf("\n3. Cross-Chip Correlation Analysis:\n");
    cross_chip_correlation_analysis();

    printf("\n4. Optimized Batch Processing:\n");
    optimized_batch_processing();

    // Performance statistics
    printf("\n=== Performance Statistics ===\n");
    int total_registers = 0;
    int total_valid = 0;
    int total_errors = 0;

    for (int chip = 0; chip < active_chip_count; chip++) {
        total_registers += chip_systems[chip].monitor.num_registers;
        total_valid += count_valid_registers(&chip_systems[chip].monitor);
        total_errors += chip_systems[chip].monitor.error_count;
    }

    printf("Total chips monitored: %d\n", active_chip_count);
    printf("Total registers: %d\n", total_registers);
    printf("Total valid registers: %d\n", total_valid);
    printf("Total errors detected: %d\n", total_errors);
    printf("System reliability: %.1f%%\n", (float)total_valid / total_registers * 100.0f);

    printf("\n=== Homework 1 Complete ===\n");
    printf("Advanced loop patterns successfully demonstrated!\n");

    return total_valid;
}

/**
 * @brief Main function for testing
 */
int main(void) {
    printf("=== Day 2 Homework 1: Advanced Loop Patterns ===\n\n");

    // Test with different chip configurations
    printf("Testing with 4 chips:\n");
    multi_chip_monitoring(4);

    printf("\n==================================================\n");
    printf("Testing with 6 chips:\n");
    multi_chip_monitoring(6);

    return 0;
}


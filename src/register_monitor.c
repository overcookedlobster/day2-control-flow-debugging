/**
 * @file register_monitor.c
 * @brief Day 2 Tasks 1, 2 & 5: Conditional Logic, Loops, and Error Handling
 *
 * This file implements the core register monitoring functionality using
 * conditional statements, loops, and switch-based error handling.
 * Students will learn control flow in the context of hardware validation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "monitor.h"

/**
 * Task 1: Conditional Validation Logic (50 minutes)
 *
 * TODO: Implement complex conditional logic for register validation
 *
 * Learning objectives:
 * - Master if-else chains with logical operators
 * - Implement nested conditionals for decision trees
 * - Practice range checking and boundary validation
 * - Handle multiple validation scenarios
 */

/**
 * @brief Validate voltage is within acceptable range
 * @param voltage Voltage reading in Volts
 * @return true if voltage is valid, false otherwise
 */
bool validate_voltage_range(float voltage) {
    // Check if voltage is below minimum threshold
    if (voltage < MIN_VOLTAGE) {
        printf("ERROR: Voltage too low: %.2fV (min: %.2fV)\n", voltage, MIN_VOLTAGE);
        return false;
    }
    // Check if voltage is above maximum threshold
    else if (voltage > MAX_VOLTAGE) {
        printf("ERROR: Voltage too high: %.2fV (max: %.2fV)\n", voltage, MAX_VOLTAGE);
        return false;
    }
    // Voltage is within acceptable range
    else {
        printf("PASS: Voltage within range: %.2fV\n", voltage);
        return true;
    }
}

/**
 * @brief Validate temperature is within safe operating range
 * @param temperature Temperature reading in Celsius
 * @return true if temperature is safe, false otherwise
 */
bool validate_temperature_range(float temperature) {
    // Check for critical temperature (highest priority)
    if (temperature > TEMP_CRITICAL) {
        printf("ERROR: Temperature critical: %.1f°C (critical: %.1f°C)\n",
               temperature, TEMP_CRITICAL);
        return false;
    }
    // Check for warning temperature
    else if (temperature > TEMP_WARNING) {
        printf("WARNING: Temperature elevated: %.1f°C (warning: %.1f°C)\n",
               temperature, TEMP_WARNING);
        return true; // Still valid but needs attention
    }
    // Normal operating temperature
    else {
        printf("PASS: Temperature normal: %.1f°C\n", temperature);
        return true;
    }
}

/**
 * @brief Validate current consumption is within expected range
 * @param current Current reading in Amperes
 * @return true if current is valid, false otherwise
 */
bool validate_current_range(float current) {
    // Handle edge case: negative current
    if (current < 0.0f) {
        printf("ERROR: Negative current reading: %.3fA\n", current);
        return false;
    }

    // Check minimum current threshold
    if (current < MIN_CURRENT) {
        printf("ERROR: Current too low: %.3fA (min: %.3fA)\n", current, MIN_CURRENT);
        return false;
    }

    // Check maximum current threshold
    if (current > MAX_CURRENT) {
        printf("ERROR: Current too high: %.3fA (max: %.3fA)\n", current, MAX_CURRENT);
        return false;
    }

    // Current is within acceptable range
    printf("PASS: Current within range: %.3fA\n", current);
    return true;
}

/**
 * @brief Determine overall system status based on all parameters
 * @param voltage System voltage
 * @param temperature System temperature
 * @param current System current
 * @return Overall system status
 */
system_status_t determine_system_status(float voltage, float temperature, float current) {
    printf("Determining system status:\n");
    printf("  - Voltage: %.2fV, Temperature: %.1f°C, Current: %.3fA\n",
           voltage, temperature, current);

    // Validate each parameter individually
    bool voltage_ok = (voltage >= MIN_VOLTAGE && voltage <= MAX_VOLTAGE);
    bool current_ok = (current >= MIN_CURRENT && current <= MAX_CURRENT && current >= 0.0f);

    // Priority-based status determination (highest priority first)

    // Critical temperature has highest priority
    if (temperature > TEMP_CRITICAL) {
        printf("Status: CRITICAL (temperature exceeds critical threshold)\n");
        return STATUS_CRITICAL;
    }

    // Voltage errors have second priority
    if (!voltage_ok) {
        if (voltage < MIN_VOLTAGE) {
            printf("Status: VOLTAGE_ERROR (voltage too low)\n");
        } else {
            printf("Status: VOLTAGE_ERROR (voltage too high)\n");
        }
        return STATUS_VOLTAGE_ERROR;
    }

    // Current errors have third priority
    if (!current_ok) {
        if (current < 0.0f) {
            printf("Status: CRITICAL (negative current)\n");
            return STATUS_CRITICAL;
        } else if (current < MIN_CURRENT) {
            printf("Status: WARNING (current too low)\n");
            return STATUS_WARNING;
        } else {
            printf("Status: WARNING (current too high)\n");
            return STATUS_WARNING;
        }
    }

    // Temperature warning has fourth priority
    if (temperature > TEMP_WARNING) {
        printf("Status: WARNING (temperature elevated)\n");
        return STATUS_WARNING;
    }

    // All parameters are normal
    if (voltage_ok && current_ok && temperature <= TEMP_WARNING) {
        printf("Status: NORMAL (all parameters within range)\n");
        return STATUS_NORMAL;
    }

    // Fallback (should not reach here with proper logic)
    printf("Status: CRITICAL (unexpected condition)\n");
    return STATUS_CRITICAL;
}

/**
 * @brief Check for critical system conditions
 * @param system Pointer to monitor system structure
 * @return true if critical conditions detected, false otherwise
 */
bool check_critical_conditions(const monitor_system_t *system) {
    if (system == NULL) {
        printf("ERROR: System pointer is NULL\n");
        return true; // Treat NULL as critical
    }

    // Check multiple critical conditions using logical operators
    bool critical_status = (system->status == STATUS_CRITICAL);
    bool high_error_count = (system->error_count >= MAX_ERRORS);
    bool system_inactive = (!system->system_active);
    bool critical_temperature = (system->temperature > TEMP_CRITICAL);
    bool voltage_out_of_range = (system->voltage < MIN_VOLTAGE || system->voltage > MAX_VOLTAGE);
    bool current_negative = (system->current < 0.0f);

    // Compound conditional statement - any of these conditions is critical
    if (critical_status || high_error_count || system_inactive ||
        critical_temperature || voltage_out_of_range || current_negative) {

        printf("CRITICAL CONDITIONS DETECTED:\n");
        if (critical_status) printf("  - System status is CRITICAL\n");
        if (high_error_count) printf("  - Error count too high: %d >= %d\n", system->error_count, MAX_ERRORS);
        if (system_inactive) printf("  - System is inactive\n");
        if (critical_temperature) printf("  - Temperature critical: %.1f°C > %.1f°C\n", system->temperature, TEMP_CRITICAL);
        if (voltage_out_of_range) printf("  - Voltage out of range: %.2fV\n", system->voltage);
        if (current_negative) printf("  - Negative current detected: %.3fA\n", system->current);

        return true;
    }

    printf("No critical conditions detected\n");
    return false;
}

/**
 * Task 2: Loop-Based Testing (50 minutes)
 *
 * TODO: Implement various loop structures for monitoring
 *
 * Learning objectives:
 * - Master for loops for sequential operations
 * - Implement while loops for continuous monitoring
 * - Practice loop control with break and continue
 * - Handle nested loops for complex scenarios
 */

/**
 * @brief Scan all registers in the system
 * @param system Pointer to monitor system structure
 * @return Number of valid registers found
 */
int scan_all_registers(monitor_system_t *system) {
    if (system == NULL) {
        printf("ERROR: System pointer is NULL\n");
        return -1;
    }

    printf("Scanning %d registers...\n", system->num_registers);
    int valid_count = 0;

    for (int i = 0; i < system->num_registers; i++) {
        printf("Scanning register %d: %s\n", i, system->registers[i].name);

        // Read register value (simulated)
        uint32_t value = read_register(system->registers[i].address);
        system->registers[i].value = value;

        // Validate register
        if (validate_register(system->registers[i].address, value,
                             system->registers[i].expected_min,
                             system->registers[i].expected_max)) {
            system->registers[i].is_valid = true;
            valid_count++;
            printf("  ✓ Register %d PASS: 0x%08X\n", i, value);
        } else {
            system->registers[i].is_valid = false;
            printf("  ✗ Register %d FAIL: 0x%08X\n", i, value);
            system->error_count++;

            // Use continue to skip to next register
            continue;
        }

        // Check for critical register failures
        if (i == 0 && !system->registers[i].is_valid) {
            printf("CRITICAL: Control register failed, aborting scan\n");
            break; // Use break for critical failures
        }
    }

    printf("Register scan complete: %d/%d valid\n", valid_count, system->num_registers);
    return valid_count;
}

/**
 * @brief Continuous monitoring loop with time limit
 * @param system Pointer to monitor system structure
 * @param duration_seconds How long to monitor (seconds)
 */
void continuous_monitoring_loop(monitor_system_t *system, int duration_seconds) {
    if (system == NULL || duration_seconds <= 0) {
        printf("ERROR: Invalid parameters for continuous monitoring\n");
        return;
    }

    printf("Starting continuous monitoring for %d seconds...\n", duration_seconds);

    time_t start_time = time(NULL);
    time_t current_time;
    int iteration = 0;

    while (system->system_active) {
        current_time = time(NULL);

        // Check time limit
        if ((current_time - start_time) >= duration_seconds) {
            printf("Monitoring time limit reached\n");
            break;
        }

        // Update system state
        update_all_registers(system);

        // Check for critical conditions
        if (check_critical_conditions(system)) {
            printf("CRITICAL condition detected, stopping monitoring\n");
            break;
        }

        // Check error count
        if (system->error_count >= MAX_ERRORS) {
            printf("Maximum errors reached, stopping monitoring\n");
            break;
        }

        printf("Monitoring iteration %d: Status = %d\n", ++iteration, system->status);

        // Sleep between iterations
        delay_ms(MONITOR_INTERVAL);
    }

    printf("Continuous monitoring completed after %d iterations\n", iteration);
}

/**
 * @brief Count valid registers using loop
 * @param system Pointer to monitor system structure
 * @return Number of valid registers
 */
int count_valid_registers(const monitor_system_t *system) {
    if (system == NULL) {
        return 0;
    }

    int count = 0;

    // Use for loop to iterate through all registers
    for (int i = 0; i < system->num_registers; i++) {
        if (system->registers[i].is_valid) {
            count++;
        }
    }

    printf("Valid registers: %d/%d\n", count, system->num_registers);
    return count;
}

/**
 * @brief Update all register values
 * @param system Pointer to monitor system structure
 */
void update_all_registers(monitor_system_t *system) {
    if (system == NULL) {
        return;
    }

    printf("Updating all registers...\n");

    // Loop through all registers
    for (int i = 0; i < system->num_registers; i++) {
        // Read current values from hardware (simulated)
        uint32_t new_value = read_register(system->registers[i].address);

        // Update register value
        system->registers[i].value = new_value;

        // Validate the new value
        system->registers[i].is_valid = validate_register(
            system->registers[i].address,
            new_value,
            system->registers[i].expected_min,
            system->registers[i].expected_max
        );

        printf("  Register %s: 0x%08X (%s)\n",
               system->registers[i].name,
               new_value,
               system->registers[i].is_valid ? "VALID" : "INVALID");
    }

    // Update system status based on current readings
    system->status = determine_system_status(system->voltage, system->temperature, system->current);

    printf("Register update complete\n");
}

/**
 * Task 5: Switch-Based Error Handling (30 minutes)
 *
 * TODO: Implement comprehensive error handling using switch statements
 *
 * Learning objectives:
 * - Master switch statement syntax and usage
 * - Implement comprehensive error classification
 * - Practice default case handling
 * - Develop error recovery strategies
 */

/**
 * @brief Handle different types of errors using switch statement
 * @param error_code Type of error to handle
 */
void handle_error(error_code_t error_code) {
    switch (error_code) {
        case ERROR_VOLTAGE_LOW:
            printf("ERROR: Voltage below minimum threshold\n");
            printf("Action: Checking power supply connections\n");
            // Implement recovery action
            log_error(error_code, "Voltage too low - checking power supply");
            break;

        case ERROR_VOLTAGE_HIGH:
            printf("ERROR: Voltage above maximum threshold\n");
            printf("Action: Reducing power supply voltage\n");
            // Implement recovery action
            log_error(error_code, "Voltage too high - reducing supply voltage");
            break;

        case ERROR_TEMPERATURE_HIGH:
            printf("ERROR: Temperature critical - initiating thermal protection\n");
            printf("Action: Activating cooling systems\n");
            // Implement emergency cooling
            log_error(error_code, "Critical temperature - activating thermal protection");
            break;

        case ERROR_CURRENT_LOW:
            printf("ERROR: Current consumption too low\n");
            printf("Action: Checking load connections\n");
            log_error(error_code, "Current too low - checking load connections");
            break;

        case ERROR_CURRENT_HIGH:
            printf("ERROR: Current consumption too high\n");
            printf("Action: Checking for short circuits\n");
            log_error(error_code, "Current too high - checking for short circuits");
            break;

        case ERROR_COMMUNICATION:
            printf("ERROR: Communication failure detected\n");
            printf("Action: Attempting to re-establish communication\n");
            // Implement retry mechanism
            log_error(error_code, "Communication failure - attempting reconnection");
            break;

        case ERROR_TIMEOUT:
            printf("ERROR: Operation timeout\n");
            printf("Action: Resetting communication interface\n");
            log_error(error_code, "Operation timeout - resetting interface");
            break;

        case ERROR_INVALID_DATA:
            printf("ERROR: Invalid data received\n");
            printf("Action: Requesting data retransmission\n");
            log_error(error_code, "Invalid data - requesting retransmission");
            break;

        case ERROR_NONE:
            printf("INFO: No error to handle\n");
            break;

        default:
            printf("ERROR: Unknown error code: %d\n", error_code);
            printf("Action: Logging error for analysis\n");
            // Log unknown error for debugging
            log_error(error_code, "Unknown error code detected");
            break;
    }
}

/**
 * @brief Get human-readable error message
 * @param error_code Error code to translate
 * @return Pointer to error message string
 */
const char* get_error_message(error_code_t error_code) {
    switch (error_code) {
        case ERROR_NONE:
            return "No error";
        case ERROR_VOLTAGE_LOW:
            return "Voltage below minimum threshold";
        case ERROR_VOLTAGE_HIGH:
            return "Voltage above maximum threshold";
        case ERROR_TEMPERATURE_HIGH:
            return "Temperature exceeds critical threshold";
        case ERROR_CURRENT_LOW:
            return "Current consumption too low";
        case ERROR_CURRENT_HIGH:
            return "Current consumption too high";
        case ERROR_COMMUNICATION:
            return "Communication interface failure";
        case ERROR_TIMEOUT:
            return "Operation timeout";
        case ERROR_INVALID_DATA:
            return "Invalid or corrupted data";
        default:
            return "Unknown error";
    }
}

/**
 * @brief Attempt to recover from specific error conditions
 * @param system Pointer to monitor system structure
 * @param error_code Type of error to recover from
 * @return true if recovery successful, false otherwise
 */
bool attempt_error_recovery(monitor_system_t *system, error_code_t error_code) {
    if (system == NULL) {
        printf("ERROR: Cannot attempt recovery - system pointer is NULL\n");
        return false;
    }

    printf("Attempting recovery for error: %s\n", get_error_message(error_code));

    switch (error_code) {
        case ERROR_VOLTAGE_LOW:
            printf("Recovery: Attempting to increase supply voltage\n");
            // Simulate voltage adjustment
            system->voltage = MIN_VOLTAGE + 0.1f;
            if (system->voltage >= MIN_VOLTAGE) {
                printf("Recovery SUCCESS: Voltage restored to %.2fV\n", system->voltage);
                return true;
            }
            break;

        case ERROR_VOLTAGE_HIGH:
            printf("Recovery: Attempting to reduce supply voltage\n");
            // Simulate voltage adjustment
            system->voltage = MAX_VOLTAGE - 0.1f;
            if (system->voltage <= MAX_VOLTAGE) {
                printf("Recovery SUCCESS: Voltage reduced to %.2fV\n", system->voltage);
                return true;
            }
            break;

        case ERROR_TEMPERATURE_HIGH:
            printf("Recovery: Activating thermal management\n");
            // Simulate cooling activation
            system->temperature = TEMP_CRITICAL - 5.0f;
            if (system->temperature <= TEMP_CRITICAL) {
                printf("Recovery SUCCESS: Temperature reduced to %.1f°C\n", system->temperature);
                return true;
            }
            break;

        case ERROR_CURRENT_LOW:
            printf("Recovery: Checking and reconnecting load\n");
            // Simulate load reconnection
            system->current = MIN_CURRENT + 0.05f;
            if (system->current >= MIN_CURRENT) {
                printf("Recovery SUCCESS: Current restored to %.3fA\n", system->current);
                return true;
            }
            break;

        case ERROR_CURRENT_HIGH:
            printf("Recovery: Reducing load to prevent overcurrent\n");
            // Simulate load reduction
            system->current = MAX_CURRENT - 0.1f;
            if (system->current <= MAX_CURRENT) {
                printf("Recovery SUCCESS: Current reduced to %.3fA\n", system->current);
                return true;
            }
            break;

        case ERROR_COMMUNICATION:
            printf("Recovery: Resetting communication interface\n");
            // Simulate interface reset
            delay_ms(100);
            printf("Recovery SUCCESS: Communication interface reset\n");
            return true;

        case ERROR_TIMEOUT:
            printf("Recovery: Increasing timeout values\n");
            // Simulate timeout adjustment
            delay_ms(50);
            printf("Recovery SUCCESS: Timeout values increased\n");
            return true;

        case ERROR_INVALID_DATA:
            printf("Recovery: Requesting data retransmission\n");
            // Simulate data refresh
            update_all_registers(system);
            printf("Recovery SUCCESS: Data refreshed\n");
            return true;

        case ERROR_NONE:
            printf("Recovery: No error to recover from\n");
            return true;

        default:
            printf("Recovery: No recovery strategy available for error %d\n", error_code);
            break;
    }

    printf("Recovery FAILED: Unable to recover from error\n");
    return false;
}

/**
 * @brief Main function demonstrating Tasks 1, 2 & 5 (only when compiled as standalone)
 */
#ifdef REGISTER_MONITOR_STANDALONE
int main(void) {
    printf("=== Day 2: Register Monitor - Tasks 1, 2 & 5 ===\n\n");

    // Initialize monitor system
    monitor_system_t system;
    init_monitor_system(&system);

    // Task 1: Conditional Logic Demonstration
    printf("--- Task 1: Conditional Validation Logic ---\n");

    // Test voltage validation
    printf("1. Testing voltage validation:\n");
    float test_voltages[] = {2.8f, 3.3f, 3.8f};
    for (int i = 0; i < 3; i++) {
        bool result = validate_voltage_range(test_voltages[i]);
        printf("   Voltage %.1fV: %s\n", test_voltages[i], result ? "PASS" : "FAIL");
    }

    // Test temperature validation
    printf("\n2. Testing temperature validation:\n");
    float test_temps[] = {25.0f, 80.0f, 90.0f};
    for (int i = 0; i < 3; i++) {
        bool result = validate_temperature_range(test_temps[i]);
        printf("   Temperature %.1f°C: %s\n", test_temps[i], result ? "PASS" : "FAIL");
    }

    // Test system status determination
    printf("\n3. Testing system status determination:\n");
    system_status_t status = determine_system_status(3.3f, 25.0f, 0.5f);
    printf("   System status: %d\n", status);

    // Task 2: Loop Operations Demonstration
    printf("\n--- Task 2: Loop-Based Testing ---\n");

    // Test register scanning
    printf("1. Testing register scan:\n");
    int valid_regs = scan_all_registers(&system);
    printf("   Valid registers found: %d\n", valid_regs);

    // Test continuous monitoring (short duration for demo)
    printf("\n2. Testing continuous monitoring (5 seconds):\n");
    continuous_monitoring_loop(&system, 5);

    // Task 5: Error Handling Demonstration
    printf("\n--- Task 5: Switch-Based Error Handling ---\n");

    // Test error handling
    printf("1. Testing error handling:\n");
    error_code_t test_errors[] = {ERROR_VOLTAGE_LOW, ERROR_TEMPERATURE_HIGH, ERROR_COMMUNICATION};
    for (int i = 0; i < 3; i++) {
        printf("   Handling error %d:\n", test_errors[i]);
        handle_error(test_errors[i]);
        printf("   Message: %s\n", get_error_message(test_errors[i]));

        bool recovered = attempt_error_recovery(&system, test_errors[i]);
        printf("   Recovery: %s\n\n", recovered ? "SUCCESS" : "FAILED");
    }

    printf("=== Tasks 1, 2 & 5 Complete ===\n");
    printf("Next: Implement test_functions.c for Task 3\n");

    return 0;
}
#endif


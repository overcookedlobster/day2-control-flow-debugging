/**
 * @file error_recovery.c
 * @brief Homework 2: Error Recovery Systems (60 minutes)
 *
 * This file implements automatic retry mechanisms and graceful degradation
 * strategies for robust system operation. Students will learn advanced
 * error handling, recovery patterns, and system resilience techniques.
 *
 * Learning objectives:
 * - Implement automatic retry mechanisms
 * - Design graceful degradation strategies
 * - Create comprehensive error logging
 * - Develop system recovery procedures
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include "../include/monitor.h"

// Recovery system constants
#define MAX_RETRY_ATTEMPTS 3
#define RETRY_DELAY_MS 500
#define ERROR_LOG_SIZE 100
#define RECOVERY_TIMEOUT 30  // seconds

/**
 * @brief Error log entry structure
 */
typedef struct {
    time_t timestamp;
    error_code_t error_code;
    int retry_count;
    bool recovery_successful;
    char description[128];
} error_log_entry_t;

/**
 * @brief Recovery system state
 */
typedef struct {
    monitor_system_t *system;
    error_log_entry_t error_log[ERROR_LOG_SIZE];
    int log_count;
    int total_errors;
    int successful_recoveries;
    bool degraded_mode;
    int degradation_level;  // 0=normal, 1=minor, 2=major, 3=critical
} recovery_system_t;

static recovery_system_t recovery_state;

/**
 * @brief Initialize error recovery system
 * @param system Pointer to monitor system
 * @return true if initialization successful
 */
bool init_recovery_system(monitor_system_t *system) {
    if (system == NULL) {
        printf("ERROR: Cannot initialize recovery system - NULL system pointer\n");
        return false;
    }

    printf("Initializing error recovery system...\n");

    recovery_state.system = system;
    recovery_state.log_count = 0;
    recovery_state.total_errors = 0;
    recovery_state.successful_recoveries = 0;
    recovery_state.degraded_mode = false;
    recovery_state.degradation_level = 0;

    // Clear error log
    memset(recovery_state.error_log, 0, sizeof(recovery_state.error_log));

    printf("Error recovery system initialized successfully\n");
    return true;
}

/**
 * @brief Log error with timestamp and details
 * @param error_code Type of error
 * @param description Error description
 * @param retry_count Number of retry attempts
 * @param recovery_successful Whether recovery was successful
 */
void log_recovery_error(error_code_t error_code, const char *description,
                       int retry_count, bool recovery_successful) {
    if (recovery_state.log_count >= ERROR_LOG_SIZE) {
        // Shift log entries to make room (circular buffer behavior)
        memmove(&recovery_state.error_log[0], &recovery_state.error_log[1],
                sizeof(error_log_entry_t) * (ERROR_LOG_SIZE - 1));
        recovery_state.log_count = ERROR_LOG_SIZE - 1;
    }

    error_log_entry_t *entry = &recovery_state.error_log[recovery_state.log_count];
    entry->timestamp = time(NULL);
    entry->error_code = error_code;
    entry->retry_count = retry_count;
    entry->recovery_successful = recovery_successful;
    strncpy(entry->description, description, sizeof(entry->description) - 1);
    entry->description[sizeof(entry->description) - 1] = '\0';

    recovery_state.log_count++;
    recovery_state.total_errors++;

    if (recovery_successful) {
        recovery_state.successful_recoveries++;
    }

    // Print log entry
    char *time_str = ctime(&entry->timestamp);
    time_str[strlen(time_str) - 1] = '\0'; // Remove newline
    printf("[%s] ERROR %d: %s (Retries: %d, Recovery: %s)\n",
           time_str, error_code, description, retry_count,
           recovery_successful ? "SUCCESS" : "FAILED");
}

/**
 * @brief Implement automatic retry mechanism with exponential backoff
 * @param system Pointer to monitor system
 * @param max_retries Maximum number of retry attempts
 * @return true if operation successful after retries
 */
bool implement_retry_mechanism(monitor_system_t *system, int max_retries) {
    if (system == NULL) {
        printf("ERROR: Cannot implement retry - NULL system pointer\n");
        return false;
    }

    printf("=== Automatic Retry Mechanism ===\n");
    printf("Attempting operation with up to %d retries...\n", max_retries);

    for (int attempt = 0; attempt <= max_retries; attempt++) {
        printf("Attempt %d/%d: ", attempt + 1, max_retries + 1);

        // Simulate operation that might fail
        bool operation_success = true;

        // Check system status
        system_status_t status = determine_system_status(
            system->voltage, system->temperature, system->current);

        if (status == STATUS_CRITICAL || status == STATUS_VOLTAGE_ERROR) {
            operation_success = false;
            printf("FAILED - Critical system condition\n");

            if (attempt < max_retries) {
                // Calculate exponential backoff delay
                int delay = RETRY_DELAY_MS * (1 << attempt); // 500ms, 1s, 2s, 4s...
                printf("  Retrying in %dms...\n", delay);
                delay_ms(delay);

                // Attempt recovery before retry
                error_code_t error = (status == STATUS_CRITICAL) ?
                                   ERROR_TEMPERATURE_HIGH : ERROR_VOLTAGE_LOW;

                bool recovery_success = attempt_error_recovery(system, error);
                log_recovery_error(error, "Retry mechanism recovery attempt",
                                 attempt + 1, recovery_success);

                if (recovery_success) {
                    printf("  Recovery successful, continuing...\n");
                } else {
                    printf("  Recovery failed, will retry anyway...\n");
                }
            }
        } else {
            printf("SUCCESS\n");
            if (attempt > 0) {
                printf("Operation succeeded after %d retries\n", attempt);
                log_recovery_error(ERROR_NONE, "Operation successful after retries",
                                 attempt, true);
            }
            return true;
        }
    }

    printf("Operation FAILED after %d attempts\n", max_retries + 1);
    log_recovery_error(ERROR_TIMEOUT, "Maximum retry attempts exceeded",
                      max_retries, false);
    return false;
}

/**
 * @brief Implement graceful degradation based on system health
 * @param system Pointer to monitor system
 */
void graceful_degradation(monitor_system_t *system) {
    if (system == NULL) {
        printf("ERROR: Cannot implement degradation - NULL system pointer\n");
        return;
    }

    printf("=== Graceful Degradation Analysis ===\n");

    // Assess system health
    int health_score = 100;
    int error_penalty = system->error_count * 10;
    int voltage_penalty = 0;
    int temperature_penalty = 0;
    int current_penalty = 0;

    // Voltage assessment
    if (system->voltage < MIN_VOLTAGE || system->voltage > MAX_VOLTAGE) {
        voltage_penalty = 30;
    } else if (system->voltage < MIN_VOLTAGE * 1.1f || system->voltage > MAX_VOLTAGE * 0.9f) {
        voltage_penalty = 15;
    }

    // Temperature assessment
    if (system->temperature > TEMP_CRITICAL) {
        temperature_penalty = 40;
    } else if (system->temperature > TEMP_WARNING) {
        temperature_penalty = 20;
    }

    // Current assessment
    if (system->current < MIN_CURRENT || system->current > MAX_CURRENT) {
        current_penalty = 25;
    }

    health_score -= (error_penalty + voltage_penalty + temperature_penalty + current_penalty);
    if (health_score < 0) health_score = 0;

    printf("System Health Assessment:\n");
    printf("  Base score: 100\n");
    printf("  Error penalty: -%d (errors: %d)\n", error_penalty, system->error_count);
    printf("  Voltage penalty: -%d\n", voltage_penalty);
    printf("  Temperature penalty: -%d\n", temperature_penalty);
    printf("  Current penalty: -%d\n", current_penalty);
    printf("  Final health score: %d/100\n", health_score);

    // Determine degradation level
    int new_degradation_level;
    if (health_score >= 80) {
        new_degradation_level = 0; // Normal operation
    } else if (health_score >= 60) {
        new_degradation_level = 1; // Minor degradation
    } else if (health_score >= 30) {
        new_degradation_level = 2; // Major degradation
    } else {
        new_degradation_level = 3; // Critical degradation
    }

    // Implement degradation if needed
    if (new_degradation_level != recovery_state.degradation_level) {
        recovery_state.degradation_level = new_degradation_level;
        recovery_state.degraded_mode = (new_degradation_level > 0);

        switch (new_degradation_level) {
            case 0:
                printf("DEGRADATION: Returning to normal operation\n");
                printf("  - All features enabled\n");
                printf("  - Full monitoring active\n");
                log_recovery_error(ERROR_NONE, "System returned to normal operation", 0, true);
                break;

            case 1:
                printf("DEGRADATION: Minor - Reducing non-essential features\n");
                printf("  - Disabling detailed logging\n");
                printf("  - Reducing monitoring frequency\n");
                printf("  - Core functions remain active\n");
                log_recovery_error(ERROR_NONE, "Minor degradation activated", 0, true);
                break;

            case 2:
                printf("DEGRADATION: Major - Essential functions only\n");
                printf("  - Disabling advanced monitoring\n");
                printf("  - Basic safety checks only\n");
                printf("  - Reduced register scanning\n");
                log_recovery_error(ERROR_NONE, "Major degradation activated", 0, true);
                break;

            case 3:
                printf("DEGRADATION: Critical - Minimal operation mode\n");
                printf("  - Emergency monitoring only\n");
                printf("  - Preparing for safe shutdown\n");
                printf("  - Critical alerts active\n");
                log_recovery_error(ERROR_NONE, "Critical degradation activated", 0, true);
                break;
        }
    } else {
        printf("DEGRADATION: Level unchanged (%d)\n", new_degradation_level);
    }
}

/**
 * @brief Comprehensive error recovery procedure
 * @param system Pointer to monitor system
 * @param error_code Type of error to recover from
 * @return true if recovery successful
 */
bool comprehensive_error_recovery(monitor_system_t *system, error_code_t error_code) {
    if (system == NULL) {
        printf("ERROR: Cannot perform recovery - NULL system pointer\n");
        return false;
    }

    printf("=== Comprehensive Error Recovery ===\n");
    printf("Recovering from error: %s\n", get_error_message(error_code));

    bool recovery_successful = false;
    int recovery_attempts = 0;

    // Phase 1: Immediate response
    printf("Phase 1: Immediate Response\n");
    switch (error_code) {
        case ERROR_TEMPERATURE_HIGH:
            printf("  - Activating emergency cooling\n");
            printf("  - Reducing system load\n");
            system->temperature -= 10.0f; // Simulate cooling
            break;

        case ERROR_VOLTAGE_LOW:
        case ERROR_VOLTAGE_HIGH:
            printf("  - Adjusting power supply\n");
            printf("  - Checking power connections\n");
            if (error_code == ERROR_VOLTAGE_LOW) {
                system->voltage = MIN_VOLTAGE + 0.1f;
            } else {
                system->voltage = MAX_VOLTAGE - 0.1f;
            }
            break;

        case ERROR_CURRENT_HIGH:
            printf("  - Reducing load to prevent overcurrent\n");
            system->current = MAX_CURRENT - 0.1f;
            break;

        case ERROR_COMMUNICATION:
            printf("  - Resetting communication interface\n");
            delay_ms(100);
            break;

        default:
            printf("  - Standard error handling\n");
            break;
    }

    // Phase 2: Retry with backoff
    printf("Phase 2: Retry Mechanism\n");
    for (recovery_attempts = 1; recovery_attempts <= MAX_RETRY_ATTEMPTS; recovery_attempts++) {
        printf("  Recovery attempt %d/%d\n", recovery_attempts, MAX_RETRY_ATTEMPTS);

        // Attempt basic recovery
        bool basic_recovery = attempt_error_recovery(system, error_code);

        if (basic_recovery) {
            // Verify recovery by checking system status
            system_status_t status = determine_system_status(
                system->voltage, system->temperature, system->current);

            if (status == STATUS_NORMAL || status == STATUS_WARNING) {
                recovery_successful = true;
                printf("  Recovery SUCCESSFUL after %d attempts\n", recovery_attempts);
                break;
            } else {
                printf("  Recovery attempt failed, system still critical\n");
            }
        }

        if (recovery_attempts < MAX_RETRY_ATTEMPTS) {
            int delay = RETRY_DELAY_MS * recovery_attempts;
            printf("  Waiting %dms before next attempt...\n", delay);
            delay_ms(delay);
        }
    }

    // Phase 3: Graceful degradation if recovery failed
    if (!recovery_successful) {
        printf("Phase 3: Graceful Degradation\n");
        printf("  Recovery failed, implementing degradation strategy\n");
        graceful_degradation(system);

        // Check if degradation allows continued operation
        if (recovery_state.degradation_level < 3) {
            recovery_successful = true; // Degraded but operational
            printf("  System operational in degraded mode\n");
        } else {
            printf("  System requires shutdown\n");
        }
    }

    // Log the recovery attempt
    char description[128];
    snprintf(description, sizeof(description),
             "Comprehensive recovery for %s", get_error_message(error_code));
    log_recovery_error(error_code, description, recovery_attempts, recovery_successful);

    return recovery_successful;
}

/**
 * @brief Print error recovery statistics
 */
void print_recovery_statistics(void) {
    printf("=== Error Recovery Statistics ===\n");
    printf("Total errors encountered: %d\n", recovery_state.total_errors);
    printf("Successful recoveries: %d\n", recovery_state.successful_recoveries);
    printf("Recovery success rate: %.1f%%\n",
           recovery_state.total_errors > 0 ?
           (float)recovery_state.successful_recoveries / recovery_state.total_errors * 100.0f : 0.0f);
    printf("Current degradation level: %d\n", recovery_state.degradation_level);
    printf("Degraded mode active: %s\n", recovery_state.degraded_mode ? "YES" : "NO");

    printf("\nRecent Error Log:\n");
    int start_idx = (recovery_state.log_count > 10) ? recovery_state.log_count - 10 : 0;
    for (int i = start_idx; i < recovery_state.log_count; i++) {
        error_log_entry_t *entry = &recovery_state.error_log[i];
        char *time_str = ctime(&entry->timestamp);
        time_str[strlen(time_str) - 1] = '\0'; // Remove newline

        printf("  [%s] Error %d: %s (Retries: %d, %s)\n",
               time_str, entry->error_code, entry->description,
               entry->retry_count, entry->recovery_successful ? "SUCCESS" : "FAILED");
    }
}

/**
 * @brief Simulate various error scenarios for testing
 * @param system Pointer to monitor system
 */
void simulate_error_scenarios(monitor_system_t *system) {
    printf("=== Error Scenario Simulation ===\n");

    // Scenario 1: Temperature spike
    printf("\nScenario 1: Temperature Spike\n");
    system->temperature = TEMP_CRITICAL + 5.0f;
    comprehensive_error_recovery(system, ERROR_TEMPERATURE_HIGH);

    // Scenario 2: Voltage drop
    printf("\nScenario 2: Voltage Drop\n");
    system->voltage = MIN_VOLTAGE - 0.2f;
    comprehensive_error_recovery(system, ERROR_VOLTAGE_LOW);

    // Scenario 3: Communication failure
    printf("\nScenario 3: Communication Failure\n");
    comprehensive_error_recovery(system, ERROR_COMMUNICATION);

    // Scenario 4: Multiple cascading errors
    printf("\nScenario 4: Cascading Errors\n");
    system->temperature = TEMP_CRITICAL + 2.0f;
    system->current = MAX_CURRENT + 0.3f;
    system->error_count = 8;

    comprehensive_error_recovery(system, ERROR_TEMPERATURE_HIGH);
    comprehensive_error_recovery(system, ERROR_CURRENT_HIGH);
}

/**
 * @brief Main function for error recovery homework
 */
int main(void) {
    printf("=== Day 2 Homework 2: Error Recovery Systems ===\n\n");

    // Initialize system
    monitor_system_t system;
    init_monitor_system(&system);

    if (!init_recovery_system(&system)) {
        return -1;
    }

    // Demonstrate retry mechanism
    printf("1. Testing Retry Mechanism:\n");
    system.voltage = MIN_VOLTAGE - 0.1f; // Force error condition
    bool retry_success = implement_retry_mechanism(&system, MAX_RETRY_ATTEMPTS);
    printf("Retry mechanism result: %s\n\n", retry_success ? "SUCCESS" : "FAILED");

    // Demonstrate graceful degradation
    printf("2. Testing Graceful Degradation:\n");
    system.error_count = 5;
    system.temperature = TEMP_WARNING + 5.0f;
    graceful_degradation(&system);

    // Demonstrate comprehensive recovery
    printf("\n3. Testing Comprehensive Recovery:\n");
    simulate_error_scenarios(&system);

    // Print final statistics
    printf("\n4. Final Recovery Statistics:\n");
    print_recovery_statistics();

    printf("\n=== Homework 2 Complete ===\n");
    printf("Error recovery systems successfully demonstrated!\n");

    return 0;
}


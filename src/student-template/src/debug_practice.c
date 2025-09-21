/**
 * @file debug_practice.c
 * @brief Day 2 Task 4: GDB Debugging Session (50 minutes)
 *
 * This file contains intentionally buggy code for students to practice
 * GDB debugging techniques. Students will learn to use breakpoints,
 * step through code, examine variables, and identify common bugs.
 *
 * INSTRUCTOR NOTE: This file contains intentional bugs for educational purposes!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "monitor.h"

// Global variables for debugging practice
static int debug_counter = 0;
static float sensor_readings[10];

/**
 * Task 4: GDB Debugging Session (50 minutes)
 *
 * TODO: Use GDB to find and fix the bugs in this file
 *
 * Learning objectives:
 * - Master essential GDB commands (break, run, next, step, print, continue, backtrace)
 * - Practice setting breakpoints and examining program state
 * - Learn to identify common C programming bugs
 * - Document debugging sessions for learning
 *
 * GDB Commands to practice:
 * - break function_name    : Set breakpoint at function
 * - break line_number      : Set breakpoint at line
 * - run                    : Start program execution
 * - next                   : Execute next line (don't step into functions)
 * - step                   : Execute next line (step into functions)
 * - print variable         : Print variable value
 * - continue               : Continue execution
 * - backtrace             : Show call stack
 * - list                  : Show source code around current line
 * - info locals           : Show local variables
 * - quit                  : Exit GDB
 */

/**
 * @brief Buggy function #1: Array bounds issue
 * @param values Array of sensor values
 * @param count Number of values to process
 * @return Average of the values
 *
 * BUG: This function has an array bounds error
 * TODO: Use GDB to find the bug and document it in gdb_session_log.md
 */
float calculate_average_buggy(float values[], int count) {
    printf("Calculating average of %d values\n", count);

    float sum = 0.0f;

    // BUG: Off-by-one error in loop condition
    for (int i = 0; i <= count; i++) {  // Should be i < count
        printf("Processing value[%d] = %.2f\n", i, values[i]);
        sum += values[i];
    }

    if (count == 0) {
        return 0.0f;
    }

    return sum / count;
}

/**
 * @brief Buggy function #2: Uninitialized variable
 * @param voltage Input voltage to validate
 * @return true if voltage is valid, false otherwise
 *
 * BUG: This function uses an uninitialized variable
 * TODO: Use GDB to identify the uninitialized variable
 */
bool validate_voltage_buggy(float voltage) {
    bool is_valid;  // BUG: Not initialized
    float min_voltage = 3.0f;
    float max_voltage = 3.6f;

    printf("Validating voltage: %.2fV\n", voltage);

    if (voltage >= min_voltage && voltage <= max_voltage) {
        is_valid = true;
    }
    // BUG: Missing else clause - is_valid remains uninitialized for invalid voltages

    printf("Validation result: %s\n", is_valid ? "PASS" : "FAIL");
    return is_valid;
}

/**
 * @brief Buggy function #3: Logic error in conditional
 * @param temperature Temperature reading
 * @return Status code based on temperature
 *
 * BUG: This function has a logic error in the conditional statements
 * TODO: Use GDB to trace through the logic and find the error
 */
int check_temperature_status_buggy(float temperature) {
    printf("Checking temperature: %.1f°C\n", temperature);

    // BUG: Logic error in conditional ordering
    if (temperature > 85.0f) {
        printf("Status: CRITICAL\n");
        return 3;  // Critical
    } else if (temperature > 75.0f) {
        printf("Status: WARNING\n");
        return 2;  // Warning
    } else if (temperature > 25.0f) {
        printf("Status: NORMAL\n");
        return 1;  // Normal
    } else if (temperature > 0.0f) {  // BUG: This condition is never reached for normal temps
        printf("Status: COLD\n");
        return 0;  // Cold
    }

    printf("Status: INVALID\n");
    return -1;  // Invalid
}

/**
 * @brief Buggy function #4: Infinite loop potential
 * @param target_value Value to search for
 * @return Index of the target value, or -1 if not found
 *
 * BUG: This function can cause an infinite loop
 * TODO: Use GDB to identify the infinite loop condition
 */
int find_value_buggy(float target_value) {
    printf("Searching for value: %.2f\n", target_value);

    int index = 0;

    // BUG: Infinite loop if target_value is not in array
    while (sensor_readings[index] != target_value) {  // Should have bounds check
        printf("Checking index %d: %.2f\n", index, sensor_readings[index]);
        index++;
        // BUG: No bounds checking - will access invalid memory
    }

    printf("Found value at index: %d\n", index);
    return index;
}

/**
 * @brief Buggy function #5: Memory access error
 * @param data Pointer to data array
 * @param size Size of the data array
 *
 * BUG: This function has a null pointer dereference
 * TODO: Use GDB to identify the null pointer access
 */
void process_data_buggy(float *data, int size) {
    printf("Processing %d data points\n", size);

    // BUG: No null pointer check
    for (int i = 0; i < size; i++) {
        printf("Data[%d] = %.2f\n", i, data[i]);  // Will crash if data is NULL
        data[i] *= 2.0f;  // Double each value
    }

    printf("Data processing complete\n");
}

/**
 * @brief Buggy function #6: Assignment vs comparison
 * @param error_code Error code to check
 * @return true if error is critical, false otherwise
 *
 * BUG: This function has an assignment instead of comparison
 * TODO: Use GDB to identify the assignment bug
 */
bool is_critical_error_buggy(int error_code) {
    printf("Checking error code: %d\n", error_code);

    // BUG: Assignment (=) instead of comparison (==)
    if (error_code = 3) {  // Should be ==
        printf("Critical error detected!\n");
        return true;
    }

    printf("Non-critical error\n");
    return false;
}

/**
 * @brief Initialize sensor readings with test data
 */
void init_sensor_data(void) {
    printf("Initializing sensor data...\n");

    for (int i = 0; i < 10; i++) {
        sensor_readings[i] = 3.0f + (float)i * 0.1f;  // 3.0, 3.1, 3.2, ... 3.9
        printf("sensor_readings[%d] = %.1f\n", i, sensor_readings[i]);
    }
}

/**
 * @brief Debugging exercise: Find all bugs using GDB
 *
 * Instructions for students:
 * 1. Compile with debug flags: gcc -g -o debug_practice debug_practice.c
 * 2. Start GDB: gdb ./debug_practice
 * 3. Set breakpoints at each buggy function
 * 4. Step through code and examine variables
 * 5. Document findings in debug/gdb_session_log.md
 */
void run_debugging_exercises(void) {
    printf("=== GDB Debugging Exercises ===\n\n");

    // Initialize test data
    init_sensor_data();

    printf("--- Exercise 1: Array Bounds Bug ---\n");
    // This will cause array bounds violation
    float test_values[] = {3.1f, 3.2f, 3.3f, 3.4f, 3.5f};
    float avg = calculate_average_buggy(test_values, 5);
    printf("Average: %.2f\n\n", avg);

    printf("--- Exercise 2: Uninitialized Variable Bug ---\n");
    // This will use uninitialized variable for invalid voltage
    bool valid1 = validate_voltage_buggy(3.3f);  // Valid voltage
    bool valid2 = validate_voltage_buggy(2.5f);  // Invalid voltage - bug here
    printf("Results: %d, %d\n\n", valid1, valid2);

    printf("--- Exercise 3: Logic Error Bug ---\n");
    // This will show incorrect temperature classification
    int status1 = check_temperature_status_buggy(30.0f);  // Should be NORMAL
    int status2 = check_temperature_status_buggy(80.0f);  // Should be WARNING
    printf("Status codes: %d, %d\n\n", status1, status2);

    printf("--- Exercise 4: Infinite Loop Bug ---\n");
    // WARNING: This will cause infinite loop - use Ctrl+C to break
    // Uncomment the next line to test (be ready to break with Ctrl+C)
    // int index = find_value_buggy(999.0f);  // Value not in array
    printf("Skipping infinite loop test (uncomment to test)\n\n");

    printf("--- Exercise 5: Null Pointer Bug ---\n");
    // This will cause segmentation fault
    // Uncomment the next line to test
    // process_data_buggy(NULL, 5);  // NULL pointer
    printf("Skipping null pointer test (uncomment to test)\n\n");

    printf("--- Exercise 6: Assignment vs Comparison Bug ---\n");
    // This will show incorrect error checking
    bool critical1 = is_critical_error_buggy(1);  // Should be false
    bool critical2 = is_critical_error_buggy(3);  // Should be true
    printf("Critical flags: %d, %d\n\n", critical1, critical2);

    printf("=== Debugging Exercises Complete ===\n");
    printf("Use GDB to find and fix all bugs!\n");
}

/**
 * @brief Main function for debugging practice
 */
int main(void) {
    printf("=== Day 2: GDB Debugging Practice - Task 4 ===\n\n");

    printf("This program contains intentional bugs for debugging practice.\n");
    printf("Use GDB to find and fix the following types of bugs:\n");
    printf("1. Array bounds violations\n");
    printf("2. Uninitialized variables\n");
    printf("3. Logic errors in conditionals\n");
    printf("4. Infinite loops\n");
    printf("5. Null pointer dereferences\n");
    printf("6. Assignment vs comparison errors\n\n");

    printf("Compilation instructions:\n");
    printf("  gcc -g -Wall -o debug_practice debug_practice.c\n\n");

    printf("GDB usage instructions:\n");
    printf("  gdb ./debug_practice\n");
    printf("  (gdb) break main\n");
    printf("  (gdb) run\n");
    printf("  (gdb) next  # Step through code\n");
    printf("  (gdb) print variable_name  # Examine variables\n\n");

    // Run the debugging exercises
    run_debugging_exercises();

    printf("\nRemember to document your debugging session in:\n");
    printf("  debug/gdb_session_log.md\n");

    return 0;
}


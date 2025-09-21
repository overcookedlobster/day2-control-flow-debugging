/**
 * @file test_functions.c
 * @brief Day 2 Task 3: Modular Test Functions (50 minutes)
 *
 * This file implements modular functions for different validation types.
 * Students will learn to create reusable, well-organized functions with
 * proper parameter handling and return values.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/monitor.h"

/**
 * @brief Test voltage levels with comprehensive validation
 * @param voltage Input voltage to test
 * @return true if voltage is acceptable, false otherwise
 */
bool test_voltage_levels(float voltage) {
    printf("=== Voltage Level Test ===\n");
    printf("Input voltage: %.2fV\n", voltage);

    // Check for invalid input
    if (voltage < 0.0f || voltage > 10.0f) {
        printf("FAIL: Invalid voltage reading (%.2fV)\n", voltage);
        return false;
    }

    // Test minimum threshold
    if (voltage < MIN_VOLTAGE) {
        printf("FAIL: Voltage below minimum (%.2fV < %.2fV)\n", voltage, MIN_VOLTAGE);
        return false;
    }

    // Test maximum threshold
    if (voltage > MAX_VOLTAGE) {
        printf("FAIL: Voltage above maximum (%.2fV > %.2fV)\n", voltage, MAX_VOLTAGE);
        return false;
    }

    // Test nominal voltage (±5% tolerance)
    float tolerance = NOMINAL_VOLTAGE * 0.05f;
    float min_nominal = NOMINAL_VOLTAGE - tolerance;
    float max_nominal = NOMINAL_VOLTAGE + tolerance;

    if (voltage >= min_nominal && voltage <= max_nominal) {
        printf("PASS: Voltage within nominal range (%.2fV ± %.2fV)\n", NOMINAL_VOLTAGE, tolerance);
    } else {
        printf("WARNING: Voltage outside nominal range but within limits\n");
    }

    printf("PASS: Voltage test successful\n");
    return true;
}

/**
 * @brief Test current consumption levels
 * @param current Input current to test
 * @return true if current is acceptable, false otherwise
 */
bool test_current_consumption(float current) {
    printf("=== Current Consumption Test ===\n");
    printf("Input current: %.3fA\n", current);

    // Validate input - check for negative current
    if (current < 0.0f) {
        printf("FAIL: Negative current reading (%.3fA)\n", current);
        return false;
    }

    // Check minimum current threshold
    if (current < MIN_CURRENT) {
        printf("FAIL: Current below minimum (%.3fA < %.3fA)\n", current, MIN_CURRENT);
        return false;
    }

    // Check maximum current threshold
    if (current > MAX_CURRENT) {
        printf("FAIL: Current above maximum (%.3fA > %.3fA)\n", current, MAX_CURRENT);
        return false;
    }

    // Classify current consumption
    float tolerance = NOMINAL_CURRENT * 0.1f; // ±10% tolerance
    float min_nominal = NOMINAL_CURRENT - tolerance;
    float max_nominal = NOMINAL_CURRENT + tolerance;

    if (current >= min_nominal && current <= max_nominal) {
        printf("PASS: Current within nominal range (%.3fA ± %.3fA)\n", NOMINAL_CURRENT, tolerance);
    } else if (current < min_nominal) {
        printf("WARNING: Current consumption low but within limits\n");
    } else {
        printf("WARNING: Current consumption high but within limits\n");
    }

    printf("PASS: Current consumption test successful\n");
    return true;
}

/**
 * @brief Test temperature range with thermal management
 * @param temperature Input temperature to test
 * @return true if temperature is safe, false otherwise
 */
bool test_temperature_range(float temperature) {
    printf("=== Temperature Range Test ===\n");
    printf("Input temperature: %.1f°C\n", temperature);

    // Validate input (reasonable temperature range)
    if (temperature < -40.0f || temperature > 150.0f) {
        printf("FAIL: Temperature reading out of sensor range (%.1f°C)\n", temperature);
        return false;
    }

    // Check critical threshold first (highest priority)
    if (temperature > TEMP_CRITICAL) {
        printf("FAIL: Temperature critical - immediate shutdown required (%.1f°C > %.1f°C)\n",
               temperature, TEMP_CRITICAL);
        printf("RECOMMENDATION: Emergency thermal protection activated\n");
        return false;
    }

    // Check warning threshold
    if (temperature > TEMP_WARNING) {
        printf("WARNING: Temperature elevated - monitoring required (%.1f°C > %.1f°C)\n",
               temperature, TEMP_WARNING);
        printf("RECOMMENDATION: Increase cooling, reduce load\n");
        return true; // Still passing but needs attention
    }

    // Normal operating temperature
    printf("PASS: Temperature in normal operating range (%.1f°C)\n", temperature);
    return true;
}

/**
 * @brief Validate register value against expected range
 * @param address Register address
 * @param value Current register value
 * @param min Minimum expected value
 * @param max Maximum expected value
 * @return true if register is valid, false otherwise
 */
bool validate_register(uint32_t address, uint32_t value, uint32_t min, uint32_t max) {
    printf("Validating register 0x%08X\n", address);
    printf("  Current value: 0x%08X\n", value);
    printf("  Expected range: 0x%08X to 0x%08X\n", min, max);

    if (value < min) {
        printf("  FAIL: Value below minimum (0x%08X < 0x%08X)\n", value, min);
        return false;
    }

    if (value > max) {
        printf("  FAIL: Value above maximum (0x%08X > 0x%08X)\n", value, max);
        return false;
    }

    printf("  PASS: Register value within expected range\n");
    return true;
}

/**
 * @brief Run comprehensive test suite on monitor system
 * @param system Pointer to monitor system structure
 * @return Number of tests passed
 */
int run_comprehensive_test(monitor_system_t *system) {
    if (system == NULL) {
        printf("ERROR: Cannot run tests - system pointer is NULL\n");
        return 0;
    }

    printf("=== Comprehensive Test Suite ===\n");
    printf("System Status:\n");
    printf("  - Voltage: %.2fV\n", system->voltage);
    printf("  - Temperature: %.1f°C\n", system->temperature);
    printf("  - Current: %.3fA\n", system->current);
    printf("  - Registers: %d\n", system->num_registers);

    int tests_passed = 0;
    int total_tests = 0;

    // Test 1: Voltage levels
    total_tests++;
    printf("\nTest 1: Voltage Level Test\n");
    if (test_voltage_levels(system->voltage)) {
        tests_passed++;
        printf("Result: PASS\n");
    } else {
        printf("Result: FAIL\n");
    }

    // Test 2: Current consumption
    total_tests++;
    printf("\nTest 2: Current Consumption Test\n");
    if (test_current_consumption(system->current)) {
        tests_passed++;
        printf("Result: PASS\n");
    } else {
        printf("Result: FAIL\n");
    }

    // Test 3: Temperature range
    total_tests++;
    printf("\nTest 3: Temperature Range Test\n");
    if (test_temperature_range(system->temperature)) {
        tests_passed++;
        printf("Result: PASS\n");
    } else {
        printf("Result: FAIL\n");
    }

    // Test 4: Register validation
    printf("\nTest 4: Register Validation Tests\n");
    for (int i = 0; i < system->num_registers; i++) {
        total_tests++;
        printf("  Register %d (%s): ", i, system->registers[i].name);
        if (validate_register(system->registers[i].address,
                             system->registers[i].value,
                             system->registers[i].expected_min,
                             system->registers[i].expected_max)) {
            tests_passed++;
            printf("PASS\n");
        } else {
            printf("FAIL\n");
        }
    }

    // Generate test report
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %d\n", total_tests);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", total_tests - tests_passed);
    printf("Success rate: %.1f%%\n", (float)tests_passed / total_tests * 100.0f);

    if (tests_passed == total_tests) {
        printf("Overall result: ALL TESTS PASSED\n");
    } else if (tests_passed >= total_tests * 0.8f) {
        printf("Overall result: MOSTLY PASSED (some issues detected)\n");
    } else {
        printf("Overall result: MULTIPLE FAILURES (system needs attention)\n");
    }

    return tests_passed;
}

/**
 * @brief Demonstrate function parameter passing and return values
 */
void demonstrate_function_concepts(void) {
    printf("=== Function Concepts Demonstration ===\n");

    // Parameter passing by value
    printf("\n1. Parameter passing by value:\n");
    float test_voltage = 3.3f;
    printf("Original voltage: %.2fV\n", test_voltage);
    bool result = test_voltage_levels(test_voltage);
    printf("After function call: %.2fV (unchanged)\n", test_voltage);
    printf("Function returned: %s\n", result ? "true" : "false");

    // Parameter passing by pointer (reference)
    printf("\n2. Parameter passing by pointer:\n");
    monitor_system_t test_system;
    init_monitor_system(&test_system);
    printf("System address: %p\n", (void*)&test_system);
    int passed_tests = run_comprehensive_test(&test_system);
    printf("Function can modify system through pointer\n");
    printf("Tests passed: %d\n", passed_tests);

    // Function composition
    printf("\n3. Function composition:\n");
    printf("Calling multiple functions in sequence:\n");
    bool v_ok = test_voltage_levels(3.3f);
    bool c_ok = test_current_consumption(0.5f);
    bool t_ok = test_temperature_range(25.0f);
    bool all_ok = v_ok && c_ok && t_ok;
    printf("All tests passed: %s\n", all_ok ? "YES" : "NO");
}

/**
 * @brief Main function demonstrating Task 3 (only when compiled as standalone)
 */
#ifdef TEST_FUNCTIONS_STANDALONE
int main(void) {
    printf("=== Day 2: Test Functions - Task 3 ===\n\n");

    // Demonstrate individual test functions
    printf("--- Individual Test Function Demonstrations ---\n");

    // Test voltage function
    printf("1. Voltage Level Testing:\n");
    float test_voltages[] = {2.8f, 3.3f, 3.8f};
    for (int i = 0; i < 3; i++) {
        bool result = test_voltage_levels(test_voltages[i]);
        printf("   %.1fV: %s\n\n", test_voltages[i], result ? "PASS" : "FAIL");
    }

    // Test current function
    printf("2. Current Consumption Testing:\n");
    float test_currents[] = {0.1f, 0.5f, 1.8f};
    for (int i = 0; i < 3; i++) {
        bool result = test_current_consumption(test_currents[i]);
        printf("   %.1fA: %s\n\n", test_currents[i], result ? "PASS" : "FAIL");
    }

    // Test temperature function
    printf("3. Temperature Range Testing:\n");
    float test_temps[] = {25.0f, 80.0f, 90.0f};
    for (int i = 0; i < 3; i++) {
        bool result = test_temperature_range(test_temps[i]);
        printf("   %.1f°C: %s\n\n", test_temps[i], result ? "PASS" : "FAIL");
    }

    // Test register validation
    printf("4. Register Validation Testing:\n");
    bool reg_result = validate_register(0x40000000, 0x12345678, 0x10000000, 0x20000000);
    printf("   Register test: %s\n\n", reg_result ? "PASS" : "FAIL");

    // Comprehensive test suite
    printf("--- Comprehensive Test Suite ---\n");
    monitor_system_t test_system;
    init_monitor_system(&test_system);

    // Set test values
    test_system.voltage = 3.3f;
    test_system.temperature = 25.0f;
    test_system.current = 0.5f;

    int passed_tests = run_comprehensive_test(&test_system);
    printf("Comprehensive test result: %d tests passed\n\n", passed_tests);

    // Function concepts demonstration
    printf("--- Function Concepts Demonstration ---\n");
    demonstrate_function_concepts();

    printf("\n=== Task 3 Complete ===\n");
    printf("Next: Implement debug_practice.c for Task 4\n");

    return 0;
}
#endif


/**
 * @file test_validation.c
 * @brief Comprehensive validation tests for Day 2 assignments
 *
 * This file provides automated testing for all Day 2 tasks:
 * - Task 1: Conditional Logic
 * - Task 2: Loop Operations
 * - Task 3: Modular Functions
 * - Task 4: Debugging (validation of fixes)
 * - Task 5: Error Handling
 * - Homework: Advanced patterns and error recovery
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "../include/monitor.h"

// Test framework macros
#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s\n", message); \
            return false; \
        } \
    } while(0)

#define TEST_PASS(message) \
    do { \
        printf("PASS: %s\n", message); \
        return true; \
    } while(0)

// Test counters
static int tests_run = 0;
static int tests_passed = 0;

/**
 * @brief Run a single test and update counters
 */
bool run_test(const char* test_name, bool (*test_func)(void)) {
    printf("\n--- Running %s ---\n", test_name);
    tests_run++;

    bool result = test_func();
    if (result) {
        tests_passed++;
        printf("‚úì %s PASSED\n", test_name);
    } else {
        printf("‚úó %s FAILED\n", test_name);
    }

    return result;
}

/**
 * Task 1 Tests: Conditional Logic Validation
 */

bool test_validate_voltage_range(void) {
    // Test valid voltage
    TEST_ASSERT(validate_voltage_range(3.3f), "Valid voltage should pass");

    // Test minimum boundary
    TEST_ASSERT(validate_voltage_range(MIN_VOLTAGE), "Minimum voltage should pass");

    // Test maximum boundary
    TEST_ASSERT(validate_voltage_range(MAX_VOLTAGE), "Maximum voltage should pass");

    // Test below minimum
    TEST_ASSERT(!validate_voltage_range(MIN_VOLTAGE - 0.1f), "Below minimum should fail");

    // Test above maximum
    TEST_ASSERT(!validate_voltage_range(MAX_VOLTAGE + 0.1f), "Above maximum should fail");

    TEST_PASS("Voltage range validation works correctly");
}

bool test_validate_temperature_range(void) {
    // Test normal temperature
    TEST_ASSERT(validate_temperature_range(25.0f), "Normal temperature should pass");

    // Test warning threshold boundary
    TEST_ASSERT(validate_temperature_range(TEMP_WARNING), "Warning threshold should pass");

    // Test critical threshold boundary
    TEST_ASSERT(validate_temperature_range(TEMP_CRITICAL), "Critical threshold should pass");

    // Test above critical
    TEST_ASSERT(!validate_temperature_range(TEMP_CRITICAL + 1.0f), "Above critical should fail");

    TEST_PASS("Temperature range validation works correctly");
}

bool test_validate_current_range(void) {
    // Test normal current
    TEST_ASSERT(validate_current_range(0.5f), "Normal current should pass");

    // Test minimum boundary
    TEST_ASSERT(validate_current_range(MIN_CURRENT), "Minimum current should pass");

    // Test maximum boundary
    TEST_ASSERT(validate_current_range(MAX_CURRENT), "Maximum current should pass");

    // Test below minimum
    TEST_ASSERT(!validate_current_range(MIN_CURRENT - 0.01f), "Below minimum should fail");

    // Test above maximum
    TEST_ASSERT(!validate_current_range(MAX_CURRENT + 0.1f), "Above maximum should fail");

    // Test negative current
    TEST_ASSERT(!validate_current_range(-0.1f), "Negative current should fail");

    TEST_PASS("Current range validation works correctly");
}

bool test_determine_system_status(void) {
    // Test normal conditions
    system_status_t status = determine_system_status(3.3f, 25.0f, 0.5f);
    TEST_ASSERT(status == STATUS_NORMAL, "Normal conditions should return STATUS_NORMAL");

    // Test voltage error
    status = determine_system_status(2.5f, 25.0f, 0.5f);
    TEST_ASSERT(status == STATUS_VOLTAGE_ERROR, "Low voltage should return STATUS_VOLTAGE_ERROR");

    // Test temperature warning
    status = determine_system_status(3.3f, 80.0f, 0.5f);
    TEST_ASSERT(status == STATUS_WARNING, "High temperature should return STATUS_WARNING");

    // Test critical temperature
    status = determine_system_status(3.3f, 90.0f, 0.5f);
    TEST_ASSERT(status == STATUS_CRITICAL, "Critical temperature should return STATUS_CRITICAL");

    TEST_PASS("System status determination works correctly");
}

bool test_check_critical_conditions(void) {
    monitor_system_t system;
    init_monitor_system(&system);

    // Test normal system
    TEST_ASSERT(!check_critical_conditions(&system), "Normal system should not be critical");

    // Test NULL pointer
    TEST_ASSERT(check_critical_conditions(NULL), "NULL pointer should be critical");

    // Test high error count
    system.error_count = MAX_ERRORS;
    TEST_ASSERT(check_critical_conditions(&system), "High error count should be critical");

    TEST_PASS("Critical condition checking works correctly");
}

/**
 * Task 2 Tests: Loop Operations Validation
 */

bool test_scan_all_registers(void) {
    monitor_system_t system;
    init_monitor_system(&system);

    // Test normal scan
    int valid_count = scan_all_registers(&system);
    TEST_ASSERT(valid_count >= 0, "Scan should return non-negative count");
    TEST_ASSERT(valid_count <= system.num_registers, "Valid count should not exceed total registers");

    // Test NULL pointer
    valid_count = scan_all_registers(NULL);
    TEST_ASSERT(valid_count == -1, "NULL pointer should return -1");

    TEST_PASS("Register scanning works correctly");
}

bool test_continuous_monitoring_loop(void) {
    monitor_system_t system;
    init_monitor_system(&system);

    // Test short monitoring (should complete without hanging)
    continuous_monitoring_loop(&system, 1); // 1 second

    // Test NULL pointer (should handle gracefully)
    continuous_monitoring_loop(NULL, 1);

    // Test invalid duration
    continuous_monitoring_loop(&system, 0);
    continuous_monitoring_loop(&system, -1);

    TEST_PASS("Continuous monitoring loop works correctly");
}

bool test_count_valid_registers(void) {
    monitor_system_t system;
    init_monitor_system(&system);

    // Test normal count
    int count = count_valid_registers(&system);
    TEST_ASSERT(count >= 0, "Count should be non-negative");
    TEST_ASSERT(count <= system.num_registers, "Count should not exceed total registers");

    // Test NULL pointer
    int null_count = count_valid_registers(NULL);
    TEST_ASSERT(null_count == 0, "NULL pointer should return 0");

    // Test with invalid registers - store original count first
    int original_count = count;
    system.registers[0].is_valid = false;
    int new_count = count_valid_registers(&system);
    TEST_ASSERT(new_count == original_count - 1, "Invalid register should reduce count");

    TEST_PASS("Register counting works correctly");
}

bool test_update_all_registers(void) {
    monitor_system_t system;
    init_monitor_system(&system);

    // Store original values
    uint32_t original_values[MAX_REGISTERS];
    for (int i = 0; i < system.num_registers; i++) {
        original_values[i] = system.registers[i].value;
    }

    // Update registers
    update_all_registers(&system);

    // Check that values may have changed (simulated hardware)
    // Note: This test may pass even if values don't change due to simulation

    // Test NULL pointer (should handle gracefully)
    update_all_registers(NULL);

    TEST_PASS("Register update works correctly");
}

/**
 * Task 3 Tests: Modular Functions Validation
 */

bool test_voltage_level_function(void) {
    // Test valid voltage
    TEST_ASSERT(test_voltage_levels(3.3f), "Valid voltage test should pass");

    // Test invalid voltage
    TEST_ASSERT(!test_voltage_levels(2.5f), "Invalid voltage test should fail");

    // Test boundary conditions
    TEST_ASSERT(test_voltage_levels(MIN_VOLTAGE), "Minimum voltage should pass");
    TEST_ASSERT(test_voltage_levels(MAX_VOLTAGE), "Maximum voltage should pass");

    TEST_PASS("Voltage level testing works correctly");
}

bool test_current_consumption_function(void) {
    // Test valid current
    TEST_ASSERT(test_current_consumption(0.5f), "Valid current test should pass");

    // Test invalid current
    TEST_ASSERT(!test_current_consumption(3.0f), "Invalid current test should fail");

    // Test boundary conditions
    TEST_ASSERT(test_current_consumption(MIN_CURRENT), "Minimum current should pass");
    TEST_ASSERT(test_current_consumption(MAX_CURRENT), "Maximum current should pass");

    // Test negative current
    TEST_ASSERT(!test_current_consumption(-0.1f), "Negative current should fail");

    TEST_PASS("Current consumption testing works correctly");
}

bool test_temperature_range_function(void) {
    // Test normal temperature
    TEST_ASSERT(test_temperature_range(25.0f), "Normal temperature should pass");

    // Test warning temperature
    TEST_ASSERT(test_temperature_range(80.0f), "Warning temperature should pass");

    // Test critical temperature
    TEST_ASSERT(!test_temperature_range(90.0f), "Critical temperature should fail");

    // Test extreme temperature
    TEST_ASSERT(!test_temperature_range(200.0f), "Extreme temperature should fail");

    TEST_PASS("Temperature range testing works correctly");
}

bool test_validate_register_function(void) {
    // Test valid register
    TEST_ASSERT(validate_register(0x40000000, 0x15000000, 0x10000000, 0x20000000),
                "Valid register should pass");

    // Test below minimum
    TEST_ASSERT(!validate_register(0x40000000, 0x05000000, 0x10000000, 0x20000000),
                "Below minimum should fail");

    // Test above maximum
    TEST_ASSERT(!validate_register(0x40000000, 0x25000000, 0x10000000, 0x20000000),
                "Above maximum should fail");

    // Test boundary conditions
    TEST_ASSERT(validate_register(0x40000000, 0x10000000, 0x10000000, 0x20000000),
                "Minimum boundary should pass");
    TEST_ASSERT(validate_register(0x40000000, 0x20000000, 0x10000000, 0x20000000),
                "Maximum boundary should pass");

    TEST_PASS("Register validation works correctly");
}

bool test_run_comprehensive_test(void) {
    monitor_system_t system;
    init_monitor_system(&system);

    // Test normal system
    int passed = run_comprehensive_test(&system);
    TEST_ASSERT(passed >= 0, "Comprehensive test should return non-negative count");

    // Test NULL pointer
    passed = run_comprehensive_test(NULL);
    TEST_ASSERT(passed == 0, "NULL pointer should return 0");

    TEST_PASS("Comprehensive test suite works correctly");
}

/**
 * Task 5 Tests: Error Handling Validation
 */

bool test_handle_error(void) {
    // Test all error types (should not crash)
    handle_error(ERROR_NONE);
    handle_error(ERROR_VOLTAGE_LOW);
    handle_error(ERROR_VOLTAGE_HIGH);
    handle_error(ERROR_TEMPERATURE_HIGH);
    handle_error(ERROR_CURRENT_LOW);
    handle_error(ERROR_CURRENT_HIGH);
    handle_error(ERROR_COMMUNICATION);
    handle_error(ERROR_TIMEOUT);
    handle_error(ERROR_INVALID_DATA);

    // Test unknown error
    handle_error((error_code_t)999);

    TEST_PASS("Error handling works correctly");
}

bool test_get_error_message(void) {
    // Test all error types
    const char* msg = get_error_message(ERROR_NONE);
    TEST_ASSERT(msg != NULL, "Error message should not be NULL");
    TEST_ASSERT(strlen(msg) > 0, "Error message should not be empty");

    msg = get_error_message(ERROR_VOLTAGE_LOW);
    TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Voltage low message should be valid");

    msg = get_error_message(ERROR_TEMPERATURE_HIGH);
    TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Temperature high message should be valid");

    // Test unknown error
    msg = get_error_message((error_code_t)999);
    TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Unknown error message should be valid");

    TEST_PASS("Error message retrieval works correctly");
}

bool test_attempt_error_recovery(void) {
    monitor_system_t system;
    init_monitor_system(&system);

    // Test recovery attempts (should not crash)
    bool result = attempt_error_recovery(&system, ERROR_VOLTAGE_LOW);
    // Result can be true or false, just check it doesn't crash

    result = attempt_error_recovery(&system, ERROR_COMMUNICATION);
    // Result can be true or false, just check it doesn't crash

    // Test NULL pointer
    result = attempt_error_recovery(NULL, ERROR_VOLTAGE_LOW);
    TEST_ASSERT(!result, "NULL pointer recovery should fail");

    TEST_PASS("Error recovery works correctly");
}

/**
 * Integration Tests
 */

bool test_system_integration(void) {
    monitor_system_t system;
    init_monitor_system(&system);

    // Test complete workflow
    printf("Testing complete system workflow...\n");

    // 1. Initialize system
    TEST_ASSERT(system.system_active, "System should be active after init");

    // 2. Scan registers
    int valid_regs = scan_all_registers(&system);
    TEST_ASSERT(valid_regs >= 0, "Register scan should succeed");

    // 3. Run comprehensive tests
    int passed_tests = run_comprehensive_test(&system);
    TEST_ASSERT(passed_tests >= 0, "Comprehensive test should succeed");

    // 4. Test error handling
    handle_error(ERROR_VOLTAGE_LOW);
    const char* msg = get_error_message(ERROR_VOLTAGE_LOW);
    TEST_ASSERT(msg != NULL, "Error message should be available");

    // 5. Test system status
    system_status_t status = determine_system_status(3.3f, 25.0f, 0.5f);
    TEST_ASSERT(status == STATUS_NORMAL, "Normal conditions should give normal status");

    TEST_PASS("System integration works correctly");
}

/**
 * Performance and Edge Case Tests
 */

bool test_edge_cases(void) {
    // Test extreme values
    TEST_ASSERT(!validate_voltage_range(0.0f), "Zero voltage should fail");
    TEST_ASSERT(!validate_voltage_range(-1.0f), "Negative voltage should fail");
    TEST_ASSERT(!validate_voltage_range(100.0f), "Extreme voltage should fail");

    TEST_ASSERT(!validate_current_range(-1.0f), "Negative current should fail");
    TEST_ASSERT(!validate_current_range(1000.0f), "Extreme current should fail");

    // Test temperature range - extreme cold is actually valid in this implementation
    // because validate_temperature_range only checks upper bounds
    TEST_ASSERT(validate_temperature_range(-100.0f), "Extreme cold should pass (implementation only checks upper bounds)");
    TEST_ASSERT(!validate_temperature_range(200.0f), "Extreme heat should fail");

    // Test boundary precision
    float epsilon = 0.001f;
    TEST_ASSERT(validate_voltage_range(MIN_VOLTAGE + epsilon), "Just above minimum should pass");
    TEST_ASSERT(validate_voltage_range(MAX_VOLTAGE - epsilon), "Just below maximum should pass");

    TEST_PASS("Edge case handling works correctly");
}

bool test_memory_safety(void) {
    // Test system with NULL pointers
    monitor_system_t* null_system = NULL;
    TEST_ASSERT(scan_all_registers(null_system) == -1, "NULL system scan should fail");
    TEST_ASSERT(count_valid_registers(null_system) == 0, "NULL system count should be 0");

    // Test system cleanup
    monitor_system_t system;
    init_monitor_system(&system);
    cleanup_monitor_system(&system);
    TEST_ASSERT(!system.system_active, "System should be inactive after cleanup");

    // Test extreme values that should be handled gracefully
    TEST_ASSERT(!validate_voltage_range(1000.0f), "Extreme voltage should fail");
    TEST_ASSERT(!validate_current_range(1000.0f), "Extreme current should fail");

    TEST_PASS("Memory safety works correctly");
}

/**
 * Main test runner
 */
int main(void) {
    printf("=== Day 2 Validation Test Suite ===\n");
    printf("Testing all implemented functions for correctness...\n\n");

    // Task 1: Conditional Logic Tests
    printf("\n=== Task 1: Conditional Logic Tests ===\n");
    run_test("Voltage Range Validation", test_validate_voltage_range);
    run_test("Temperature Range Validation", test_validate_temperature_range);
    run_test("Current Range Validation", test_validate_current_range);
    run_test("System Status Determination", test_determine_system_status);
    run_test("Critical Conditions Check", test_check_critical_conditions);

    // Task 2: Loop Operations Tests
    printf("\n=== Task 2: Loop Operations Tests ===\n");
    run_test("Register Scanning", test_scan_all_registers);
    run_test("Continuous Monitoring", test_continuous_monitoring_loop);
    run_test("Register Counting", test_count_valid_registers);
    run_test("Register Updates", test_update_all_registers);

    // Task 3: Modular Functions Tests
    printf("\n=== Task 3: Modular Functions Tests ===\n");
    run_test("Voltage Level Testing", test_voltage_level_function);
    run_test("Current Consumption Testing", test_current_consumption_function);
    run_test("Temperature Range Testing", test_temperature_range_function);
    run_test("Register Validation", test_validate_register_function);
    run_test("Comprehensive Test Suite", test_run_comprehensive_test);

    // Task 5: Error Handling Tests
    printf("\n=== Task 5: Error Handling Tests ===\n");
    run_test("Error Handling", test_handle_error);
    run_test("Error Message Retrieval", test_get_error_message);
    run_test("Error Recovery", test_attempt_error_recovery);

    // Integration and Edge Case Tests
    printf("\n=== Integration and Edge Case Tests ===\n");
    run_test("System Integration", test_system_integration);
    run_test("Edge Cases", test_edge_cases);
    run_test("Memory Safety", test_memory_safety);

    // Final Results
    printf("\n=== Test Results Summary ===\n");
    printf("Total tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_run - tests_passed);
    printf("Success rate: %.1f%%\n", (float)tests_passed / tests_run * 100.0f);

    if (tests_passed == tests_run) {
        printf("\nÌ†ºÌæâ ALL TESTS PASSED! Ì†ºÌæâ\n");
        printf("Your implementation is working correctly!\n");
        return 0;
    } else {
        printf("\n‚ùå SOME TESTS FAILED ‚ùå\n");
        printf("Please review the failed tests and fix the implementation.\n");
        return 1;
    }
}


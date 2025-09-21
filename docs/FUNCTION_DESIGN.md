# Function Design Guidelines for Day 2

## Overview
This guide covers best practices for designing and implementing functions in C, with examples from the register monitoring system. You'll learn to create modular, reusable, and maintainable functions.

## Table of Contents
1. [Function Fundamentals](#function-fundamentals)
2. [Function Design Principles](#function-design-principles)
3. [Parameter Passing](#parameter-passing)
4. [Return Values](#return-values)
5. [Function Organization](#function-organization)
6. [Best Practices](#best-practices)

## Function Fundamentals

### Basic Function Structure
```c
return_type function_name(parameter_list) {
    // Function body
    return value; // if return_type is not void
}
```

### Function Declaration vs Definition
```c
// Declaration (in header file)
bool validate_voltage_range(float voltage);

// Definition (in source file)
bool validate_voltage_range(float voltage) {
    if (voltage < MIN_VOLTAGE || voltage > MAX_VOLTAGE) {
        return false;
    }
    return true;
}
```

## Function Design Principles

### 1. Single Responsibility Principle
Each function should do one thing well:

```c
// Good - single responsibility
bool validate_voltage_range(float voltage) {
    return (voltage >= MIN_VOLTAGE && voltage <= MAX_VOLTAGE);
}

bool validate_temperature_range(float temperature) {
    return (temperature <= TEMP_CRITICAL);
}

bool validate_current_range(float current) {
    return (current >= MIN_CURRENT && current <= MAX_CURRENT);
}

// Bad - multiple responsibilities
bool validate_all_parameters(float voltage, float temp, float current) {
    // Validates voltage, temperature, current, logs errors,
    // updates system state, sends alerts...
    // Too many responsibilities!
}
```

### 2. Clear and Descriptive Names
```c
// Good names
bool test_voltage_levels(float voltage);
int scan_all_registers(monitor_system_t *system);
void handle_critical_error(error_code_t error);

// Bad names
bool check(float x);
int do_stuff(void *ptr);
void fix(int code);
```

### 3. Appropriate Function Size
```c
// Good - concise and focused
bool is_voltage_valid(float voltage) {
    return (voltage >= MIN_VOLTAGE && voltage <= MAX_VOLTAGE);
}

// Good - reasonable size for complex logic
system_status_t determine_system_status(float voltage, float temperature, float current) {
    // 20-30 lines of well-organized logic
    if (temperature > TEMP_CRITICAL) {
        return STATUS_CRITICAL;
    }

    if (voltage < MIN_VOLTAGE || voltage > MAX_VOLTAGE) {
        return STATUS_VOLTAGE_ERROR;
    }

    // Additional checks...
    return STATUS_NORMAL;
}
```

## Parameter Passing

### Pass by Value
For simple data types and when you don't need to modify the original:

```c
bool test_voltage_levels(float voltage) {
    // voltage is a copy - original unchanged
    if (voltage < 0) {
        voltage = 0; // Only affects local copy
    }
    return validate_voltage_range(voltage);
}
```

### Pass by Pointer
For structures, arrays, or when you need to modify the original:

```c
// Modify original structure
void update_system_voltage(monitor_system_t *system, float new_voltage) {
    if (system != NULL) {
        system->voltage = new_voltage;
    }
}

// Read-only access to structure
void print_system_state(const monitor_system_t *system) {
    if (system != NULL) {
        printf("Voltage: %.2fV\n", system->voltage);
        // system->voltage = 0; // Compiler error - const parameter
    }
}
```

### Array Parameters
```c
// Arrays are always passed by reference
float calculate_average(float values[], int count) {
    float sum = 0.0f;
    for (int i = 0; i < count; i++) {
        sum += values[i]; // Accessing original array
    }
    return sum / count;
}

// Alternative syntax (equivalent)
float calculate_average(float *values, int count) {
    // Same implementation
}
```

### Parameter Validation
```c
bool validate_register(uint32_t address, uint32_t value, uint32_t min, uint32_t max) {
    // Validate parameters
    if (min > max) {
        printf("ERROR: Invalid range - min > max\n");
        return false;
    }

    // Perform validation
    return (value >= min && value <= max);
}

int scan_all_registers(monitor_system_t *system) {
    // Check for NULL pointer
    if (system == NULL) {
        printf("ERROR: System pointer is NULL\n");
        return -1;
    }

    // Check for valid register count
    if (system->num_registers <= 0) {
        printf("ERROR: No registers to scan\n");
        return 0;
    }

    // Proceed with scanning...
}
```

## Return Values

### Boolean Returns
For validation and status functions:

```c
bool test_voltage_levels(float voltage) {
    if (voltage < MIN_VOLTAGE) {
        printf("FAIL: Voltage too low\n");
        return false;
    }

    if (voltage > MAX_VOLTAGE) {
        printf("FAIL: Voltage too high\n");
        return false;
    }

    printf("PASS: Voltage within range\n");
    return true;
}
```

### Integer Returns
For counts, status codes, or error indicators:

```c
// Return count (positive) or error code (negative)
int scan_all_registers(monitor_system_t *system) {
    if (system == NULL) {
        return -1; // Error indicator
    }

    int valid_count = 0;
    for (int i = 0; i < system->num_registers; i++) {
        if (validate_register_at_index(system, i)) {
            valid_count++;
        }
    }

    return valid_count; // Success - return count
}
```

### Enumerated Returns
For status or error codes:

```c
system_status_t determine_system_status(float voltage, float temperature, float current) {
    if (temperature > TEMP_CRITICAL) {
        return STATUS_CRITICAL;
    }

    if (voltage < MIN_VOLTAGE || voltage > MAX_VOLTAGE) {
        return STATUS_VOLTAGE_ERROR;
    }

    if (temperature > TEMP_WARNING) {
        return STATUS_WARNING;
    }

    return STATUS_NORMAL;
}
```

### Pointer Returns
For dynamically allocated memory or optional results:

```c
monitor_system_t* create_monitor_system(int num_registers) {
    if (num_registers <= 0 || num_registers > MAX_REGISTERS) {
        return NULL; // Invalid parameter
    }

    monitor_system_t *system = malloc(sizeof(monitor_system_t));
    if (system == NULL) {
        return NULL; // Allocation failed
    }

    // Initialize system...
    init_monitor_system(system);
    return system;
}
```

## Function Organization

### Grouping Related Functions
```c
// Voltage-related functions
bool validate_voltage_range(float voltage);
bool test_voltage_levels(float voltage);
void adjust_voltage(monitor_system_t *system, float target);

// Temperature-related functions
bool validate_temperature_range(float temperature);
bool test_temperature_range(float temperature);
void activate_cooling(monitor_system_t *system);

// Register-related functions
bool validate_register(uint32_t address, uint32_t value, uint32_t min, uint32_t max);
int scan_all_registers(monitor_system_t *system);
void update_all_registers(monitor_system_t *system);
```

### Function Composition
Building complex functionality from simple functions:

```c
// Simple building blocks
bool is_voltage_valid(float voltage) {
    return (voltage >= MIN_VOLTAGE && voltage <= MAX_VOLTAGE);
}

bool is_temperature_safe(float temperature) {
    return (temperature <= TEMP_CRITICAL);
}

bool is_current_acceptable(float current) {
    return (current >= MIN_CURRENT && current <= MAX_CURRENT);
}

// Composed function
bool is_system_healthy(const monitor_system_t *system) {
    return is_voltage_valid(system->voltage) &&
           is_temperature_safe(system->temperature) &&
           is_current_acceptable(system->current);
}

// Higher-level composed function
int run_comprehensive_test(monitor_system_t *system) {
    int tests_passed = 0;

    if (test_voltage_levels(system->voltage)) {
        tests_passed++;
    }

    if (test_current_consumption(system->current)) {
        tests_passed++;
    }

    if (test_temperature_range(system->temperature)) {
        tests_passed++;
    }

    return tests_passed;
}
```

## Best Practices

### 1. Use const for Read-Only Parameters
```c
// Good - indicates parameter won't be modified
void print_system_state(const monitor_system_t *system) {
    printf("Voltage: %.2fV\n", system->voltage);
}

// Good - const for read-only arrays
float calculate_average(const float values[], int count) {
    // Can read values but not modify them
}
```

### 2. Validate Input Parameters
```c
bool validate_register(uint32_t address, uint32_t value, uint32_t min, uint32_t max) {
    // Parameter validation
    if (min > max) {
        printf("ERROR: Invalid range specification\n");
        return false;
    }

    if (address == 0) {
        printf("ERROR: Invalid register address\n");
        return false;
    }

    // Actual validation logic
    return (value >= min && value <= max);
}
```

### 3. Use Meaningful Return Values
```c
// Good - clear return value meanings
typedef enum {
    SCAN_SUCCESS = 0,
    SCAN_ERROR_NULL_POINTER = -1,
    SCAN_ERROR_NO_REGISTERS = -2,
    SCAN_ERROR_HARDWARE_FAILURE = -3
} scan_result_t;

scan_result_t scan_registers_with_status(monitor_system_t *system) {
    if (system == NULL) {
        return SCAN_ERROR_NULL_POINTER;
    }

    if (system->num_registers == 0) {
        return SCAN_ERROR_NO_REGISTERS;
    }

    // Perform scan...
    return SCAN_SUCCESS;
}
```

### 4. Keep Functions Focused
```c
// Good - focused function
bool is_register_address_valid(uint32_t address) {
    return (address >= MIN_REGISTER_ADDR && address <= MAX_REGISTER_ADDR);
}

// Good - focused function
bool is_register_value_in_range(uint32_t value, uint32_t min, uint32_t max) {
    return (value >= min && value <= max);
}

// Good - composed from focused functions
bool validate_register(uint32_t address, uint32_t value, uint32_t min, uint32_t max) {
    return is_register_address_valid(address) &&
           is_register_value_in_range(value, min, max);
}
```

### 5. Use Static for Internal Functions
```c
// Internal helper function - not exposed in header
static bool is_power_stable(const monitor_system_t *system) {
    return (system->voltage >= MIN_VOLTAGE * 0.95f &&
            system->voltage <= MAX_VOLTAGE * 1.05f);
}

// Public function that uses internal helper
bool system_ready_for_operation(const monitor_system_t *system) {
    return is_power_stable(system) &&
           system->temperature < TEMP_WARNING &&
           system->error_count == 0;
}
```

## Common Function Design Mistakes

### 1. Functions That Do Too Much
```c
// Bad - too many responsibilities
void process_system_data(monitor_system_t *system) {
    // Reads sensors
    // Validates data
    // Updates display
    // Logs to file
    // Sends network updates
    // Handles errors
    // Updates configuration
    // ... (100+ lines)
}
```

### 2. Poor Parameter Design
```c
// Bad - too many parameters
bool validate_system(float v1, float v2, float v3, float t1, float t2,
                    float c1, float c2, int flag1, int flag2, char *msg);

// Better - use structure
bool validate_system(const system_readings_t *readings,
                    const validation_config_t *config);
```

### 3. Inconsistent Return Values
```c
// Bad - inconsistent error handling
int function1() { return -1; }  // Returns -1 on error
int function2() { return 0; }   // Returns 0 on error
int function3() { return 999; } // Returns 999 on error
```

### 4. Side Effects Without Documentation
```c
// Bad - hidden side effects
int calculate_average(float values[], int count) {
    // Hidden side effect - modifies input array!
    for (int i = 0; i < count; i++) {
        values[i] = fabs(values[i]); // Modifies original data
    }

    // Calculate average...
}
```

## Summary

Good function design is essential for maintainable code:

1. **Single Responsibility**: One function, one purpose
2. **Clear Names**: Function names should describe what they do
3. **Parameter Validation**: Check inputs before processing
4. **Consistent Returns**: Use consistent return value conventions
5. **Appropriate Size**: Keep functions focused and reasonably sized
6. **Composition**: Build complex functionality from simple functions

Practice these principles with the register monitoring examples to develop good function design habits!


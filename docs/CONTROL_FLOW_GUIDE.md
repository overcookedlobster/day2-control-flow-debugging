# Control Flow Guide for Day 2

## Overview
This guide covers the fundamental control flow constructs in C programming, with practical examples from the register monitoring system. You'll learn to use conditionals, loops, and switch statements effectively.

## Table of Contents
1. [Conditional Statements](#conditional-statements)
2. [Loop Structures](#loop-structures)
3. [Switch Statements](#switch-statements)
4. [Best Practices](#best-practices)
5. [Common Pitfalls](#common-pitfalls)

## Conditional Statements

### Basic if-else Structure
```c
if (condition) {
    // Execute if condition is true
} else {
    // Execute if condition is false
}
```

### Multi-level Conditionals
```c
if (voltage < MIN_VOLTAGE) {
    printf("ERROR: Voltage too low\n");
    return false;
} else if (voltage > MAX_VOLTAGE) {
    printf("ERROR: Voltage too high\n");
    return false;
} else {
    printf("PASS: Voltage within range\n");
    return true;
}
```

### Logical Operators
- `&&` (AND): Both conditions must be true
- `||` (OR): At least one condition must be true
- `!` (NOT): Negates the condition

```c
// Complex condition with logical operators
if (voltage >= MIN_VOLTAGE && voltage <= MAX_VOLTAGE &&
    temperature < TEMP_CRITICAL && current > 0.0f) {
    status = STATUS_NORMAL;
} else {
    status = STATUS_ERROR;
}
```

### Nested Conditionals
```c
if (system_active) {
    if (temperature > TEMP_CRITICAL) {
        printf("CRITICAL: Emergency shutdown required\n");
        emergency_shutdown();
    } else if (temperature > TEMP_WARNING) {
        printf("WARNING: Temperature elevated\n");
        activate_cooling();
    } else {
        printf("INFO: Temperature normal\n");
    }
} else {
    printf("System inactive\n");
}
```

## Loop Structures

### For Loops
Best for when you know the number of iterations:

```c
// Basic for loop
for (int i = 0; i < num_registers; i++) {
    printf("Register %d: 0x%08X\n", i, registers[i].value);
}

// For loop with step
for (int i = 0; i < 100; i += 10) {
    printf("Value: %d\n", i);
}

// Nested for loops
for (int chip = 0; chip < num_chips; chip++) {
    for (int reg = 0; reg < registers_per_chip; reg++) {
        validate_register(chip, reg);
    }
}
```

### While Loops
Best for condition-based iteration:

```c
// Basic while loop
while (system_active && error_count < MAX_ERRORS) {
    update_system_state();
    check_conditions();
    delay_ms(MONITOR_INTERVAL);
}

// While loop with complex condition
while (temperature > TEMP_WARNING &&
       cooling_attempts < MAX_COOLING_ATTEMPTS &&
       time_elapsed < COOLING_TIMEOUT) {
    activate_cooling();
    delay_ms(1000);
    temperature = read_temperature();
    cooling_attempts++;
    time_elapsed++;
}
```

### Do-While Loops
Execute at least once:

```c
do {
    user_input = get_user_input();
    process_input(user_input);
} while (user_input != EXIT_COMMAND);
```

### Loop Control Statements

#### Break Statement
Exit loop immediately:

```c
for (int i = 0; i < num_registers; i++) {
    if (registers[i].critical_error) {
        printf("Critical error in register %d, aborting scan\n", i);
        break; // Exit the loop
    }
    validate_register(i);
}
```

#### Continue Statement
Skip to next iteration:

```c
for (int i = 0; i < num_registers; i++) {
    if (!registers[i].enabled) {
        continue; // Skip disabled registers
    }

    // Process enabled register
    validate_register(i);
}
```

## Switch Statements

### Basic Switch Structure
```c
switch (error_code) {
    case ERROR_VOLTAGE_LOW:
        printf("Voltage below threshold\n");
        adjust_voltage_up();
        break;

    case ERROR_VOLTAGE_HIGH:
        printf("Voltage above threshold\n");
        adjust_voltage_down();
        break;

    case ERROR_TEMPERATURE_HIGH:
        printf("Temperature critical\n");
        emergency_cooling();
        break;

    default:
        printf("Unknown error: %d\n", error_code);
        break;
}
```

### Switch with Fall-through
```c
switch (priority_level) {
    case PRIORITY_CRITICAL:
        log_to_emergency_system();
        // Fall through to high priority handling

    case PRIORITY_HIGH:
        send_immediate_alert();
        // Fall through to normal handling

    case PRIORITY_NORMAL:
        log_to_file();
        break;

    case PRIORITY_LOW:
        // Only log to buffer
        log_to_buffer();
        break;

    default:
        printf("Invalid priority level\n");
        break;
}
```

### Switch with Multiple Cases
```c
switch (system_status) {
    case STATUS_NORMAL:
    case STATUS_WARNING:
        // Handle both normal and warning the same way
        continue_operation();
        break;

    case STATUS_CRITICAL:
    case STATUS_VOLTAGE_ERROR:
    case STATUS_TEMPERATURE_ERROR:
        // Handle all error conditions
        initiate_recovery();
        break;

    default:
        printf("Unknown status\n");
        break;
}
```

## Best Practices

### 1. Use Meaningful Conditions
```c
// Bad
if (x > 5) {
    // What does 5 represent?
}

// Good
if (temperature > TEMP_WARNING) {
    // Clear what we're checking
}
```

### 2. Avoid Deep Nesting
```c
// Bad - too deeply nested
if (system_active) {
    if (voltage_ok) {
        if (temperature_ok) {
            if (current_ok) {
                // Deep nesting is hard to read
            }
        }
    }
}

// Good - early returns
if (!system_active) return false;
if (!voltage_ok) return false;
if (!temperature_ok) return false;
if (!current_ok) return false;

// All conditions met
return true;
```

### 3. Use Logical Operators Effectively
```c
// Combine related conditions
if (voltage >= MIN_VOLTAGE && voltage <= MAX_VOLTAGE) {
    // Voltage is in valid range
}

// Use parentheses for clarity
if ((temperature > TEMP_WARNING) ||
    (current > MAX_CURRENT && voltage < MIN_VOLTAGE)) {
    // Complex condition made clear
}
```

### 4. Initialize Loop Variables Properly
```c
// Good initialization
for (int i = 0; i < array_size; i++) {
    // i is properly initialized and scoped
}

// Be careful with while loops
int attempts = 0;
while (attempts < MAX_ATTEMPTS && !success) {
    success = try_operation();
    attempts++; // Don't forget to update!
}
```

## Common Pitfalls

### 1. Assignment vs Comparison
```c
// Bug - assignment instead of comparison
if (error_code = CRITICAL_ERROR) {  // Wrong!
    handle_critical_error();
}

// Correct - comparison
if (error_code == CRITICAL_ERROR) {  // Right!
    handle_critical_error();
}
```

### 2. Off-by-One Errors
```c
// Bug - accessing array[size]
for (int i = 0; i <= array_size; i++) {  // Wrong!
    process(array[i]);
}

// Correct
for (int i = 0; i < array_size; i++) {   // Right!
    process(array[i]);
}
```

### 3. Infinite Loops
```c
// Bug - condition never changes
while (temperature > TEMP_WARNING) {
    printf("Temperature high\n");
    // Forgot to update temperature!
}

// Correct
while (temperature > TEMP_WARNING) {
    activate_cooling();
    delay_ms(1000);
    temperature = read_temperature(); // Update condition!
}
```

### 4. Missing Break Statements
```c
switch (error_type) {
    case ERROR_MINOR:
        log_error();
        // Missing break - falls through!
    case ERROR_MAJOR:
        shutdown_system(); // Executes for minor errors too!
        break;
}
```

### 5. Uninitialized Variables in Conditionals
```c
bool is_valid; // Not initialized!

if (some_condition) {
    is_valid = true;
}
// If some_condition is false, is_valid is uninitialized

if (is_valid) { // Using uninitialized variable!
    proceed();
}
```

## Practice Exercises

### Exercise 1: Voltage Validation
Write a function that validates voltage with multiple thresholds:
- Below 2.5V: Critical error
- 2.5V - 3.0V: Warning
- 3.0V - 3.6V: Normal
- Above 3.6V: Error

### Exercise 2: Multi-Register Scan
Write a loop that scans registers but:
- Skips disabled registers (continue)
- Stops on critical errors (break)
- Counts valid registers

### Exercise 3: Error Classification
Write a switch statement that handles different error types and implements appropriate recovery actions.

## Summary

Control flow is fundamental to programming logic. Master these concepts:

1. **Conditionals**: Use if-else for decision making
2. **Loops**: Use for/while for repetition
3. **Switch**: Use for multiple discrete choices
4. **Best Practices**: Write clear, maintainable code
5. **Avoid Pitfalls**: Watch for common mistakes

Practice with the register monitoring examples to build confidence with these essential programming constructs!


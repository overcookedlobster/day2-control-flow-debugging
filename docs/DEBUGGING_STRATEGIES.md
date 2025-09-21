# Debugging Strategies for Day 2

## Overview
This guide provides systematic approaches to debugging C programs, with emphasis on the techniques needed for Day 2's debugging exercises. You'll learn methodical debugging strategies that will serve you throughout your programming career.

## Table of Contents
1. [Debugging Mindset](#debugging-mindset)
2. [Systematic Debugging Process](#systematic-debugging-process)
3. [Common Bug Categories](#common-bug-categories)
4. [Debugging Tools and Techniques](#debugging-tools-and-techniques)
5. [Prevention Strategies](#prevention-strategies)

## Debugging Mindset

### The Scientific Method Applied to Debugging
1. **Observe** the problem behavior
2. **Hypothesize** about the cause
3. **Test** your hypothesis
4. **Analyze** the results
5. **Repeat** until solved

### Key Principles
- **Stay calm and methodical** - Panic leads to random changes
- **Reproduce the bug consistently** - Intermittent bugs are harder to fix
- **Change one thing at a time** - Multiple changes obscure the cause
- **Document your findings** - Keep track of what you've tried
- **Think like the computer** - Follow the exact execution path

## Systematic Debugging Process

### Step 1: Understand the Problem
```
Questions to ask:
- What is the expected behavior?
- What is the actual behavior?
- When does the problem occur?
- What are the exact steps to reproduce it?
- What changed recently that might have caused this?
```

### Step 2: Gather Information
```c
// Add diagnostic output
printf("DEBUG: Entering function with voltage=%.2f\n", voltage);
printf("DEBUG: Loop iteration %d, array[%d]=%.2f\n", i, i, array[i]);
printf("DEBUG: Condition result: %s\n", condition ? "true" : "false");
```

### Step 3: Form Hypotheses
```
Common hypotheses for C programs:
- Array bounds violation
- Uninitialized variable
- Logic error in conditionals
- Memory management issue
- Off-by-one error
- Null pointer dereference
```

### Step 4: Test Systematically
```c
// Test boundary conditions
test_function(0);           // Minimum value
test_function(MAX_VALUE);   // Maximum value
test_function(-1);          // Below minimum
test_function(MAX_VALUE+1); // Above maximum

// Test with known good/bad inputs
test_function(KNOWN_GOOD_INPUT);
test_function(KNOWN_BAD_INPUT);
```

### Step 5: Isolate the Problem
```c
// Binary search approach - comment out half the code
void problematic_function() {
    // First half of function
    printf("DEBUG: Reached checkpoint 1\n");

    // Second half of function
    printf("DEBUG: Reached checkpoint 2\n");

    // Narrow down to the problematic section
}
```

## Common Bug Categories

### 1. Array Bounds Violations
**Symptoms:** Crashes, corrupted data, unpredictable behavior

**Detection Strategy:**
```c
// Add bounds checking
void safe_array_access(float array[], int size, int index) {
    if (index < 0 || index >= size) {
        printf("ERROR: Array bounds violation - index %d, size %d\n", index, size);
        return;
    }

    // Safe to access array[index]
    printf("array[%d] = %.2f\n", index, array[index]);
}

// Debug loop bounds
for (int i = 0; i < size; i++) {  // Note: < not <=
    printf("DEBUG: Processing element %d/%d\n", i+1, size);
    process_element(array[i]);
}
```

**GDB Commands:**
```gdb
(gdb) print array_size
(gdb) print loop_index
(gdb) print &array[0]
(gdb) print &array[array_size-1]  # Last valid element
```

### 2. Uninitialized Variables
**Symptoms:** Random behavior, inconsistent results

**Detection Strategy:**
```c
// Initialize all variables
bool is_valid = false;  // Always initialize!
int count = 0;
float sum = 0.0f;

// Use compiler warnings
// gcc -Wall -Wuninitialized will catch many cases

// Add assertions for critical variables
assert(is_valid == true || is_valid == false);  // Catches uninitialized bools
```

**GDB Commands:**
```gdb
(gdb) print variable_name
# Look for garbage values like 0x41414141 or very large numbers
```

### 3. Logic Errors in Conditionals
**Symptoms:** Wrong execution path, incorrect results

**Detection Strategy:**
```c
// Add debug output for conditions
if (temperature > TEMP_WARNING) {
    printf("DEBUG: Temperature %.1f > %.1f (WARNING)\n", temperature, TEMP_WARNING);
    handle_warning();
} else if (temperature > TEMP_CRITICAL) {  // BUG: This will never execute!
    printf("DEBUG: Temperature %.1f > %.1f (CRITICAL)\n", temperature, TEMP_CRITICAL);
    handle_critical();
}

// Correct version:
if (temperature > TEMP_CRITICAL) {
    printf("DEBUG: Temperature %.1f > %.1f (CRITICAL)\n", temperature, TEMP_CRITICAL);
    handle_critical();
} else if (temperature > TEMP_WARNING) {
    printf("DEBUG: Temperature %.1f > %.1f (WARNING)\n", temperature, TEMP_WARNING);
    handle_warning();
}
```

**GDB Commands:**
```gdb
(gdb) print temperature
(gdb) print TEMP_WARNING
(gdb) print TEMP_CRITICAL
(gdb) step  # Step through each condition
```

### 4. Assignment vs Comparison
**Symptoms:** Unexpected variable changes, always-true conditions

**Detection Strategy:**
```c
// Common mistake
if (error_code = CRITICAL_ERROR) {  // Assignment!
    handle_critical();
}

// Correct version
if (error_code == CRITICAL_ERROR) {  // Comparison!
    handle_critical();
}

// Defensive programming - put constant first
if (CRITICAL_ERROR == error_code) {  // Compiler error if you use =
    handle_critical();
}
```

### 5. Infinite Loops
**Symptoms:** Program hangs, high CPU usage

**Detection Strategy:**
```c
// Add loop counters and limits
int safety_counter = 0;
const int MAX_ITERATIONS = 1000;

while (condition && safety_counter < MAX_ITERATIONS) {
    // Loop body

    safety_counter++;
    if (safety_counter % 100 == 0) {
        printf("DEBUG: Loop iteration %d\n", safety_counter);
    }
}

if (safety_counter >= MAX_ITERATIONS) {
    printf("ERROR: Loop exceeded maximum iterations\n");
}
```

### 6. Null Pointer Dereferences
**Symptoms:** Segmentation faults, crashes

**Detection Strategy:**
```c
// Always check pointers before use
void process_data(float *data, int size) {
    if (data == NULL) {
        printf("ERROR: NULL pointer passed to process_data\n");
        return;
    }

    if (size <= 0) {
        printf("ERROR: Invalid size %d\n", size);
        return;
    }

    // Safe to use data
    for (int i = 0; i < size; i++) {
        printf("data[%d] = %.2f\n", i, data[i]);
    }
}
```

## Debugging Tools and Techniques

### 1. Printf Debugging
```c
// Strategic printf placement
void debug_function(int input) {
    printf("ENTER: debug_function(input=%d)\n", input);

    int result = complex_calculation(input);
    printf("DEBUG: complex_calculation returned %d\n", result);

    if (result > 0) {
        printf("DEBUG: Taking positive path\n");
        handle_positive(result);
    } else {
        printf("DEBUG: Taking non-positive path\n");
        handle_non_positive(result);
    }

    printf("EXIT: debug_function\n");
}
```

### 2. Conditional Compilation
```c
#define DEBUG 1

#if DEBUG
    #define DBG_PRINT(fmt, ...) printf("DEBUG: " fmt, ##__VA_ARGS__)
#else
    #define DBG_PRINT(fmt, ...) // No-op in release builds
#endif

void function() {
    DBG_PRINT("Entering function\n");
    DBG_PRINT("Variable x = %d\n", x);
    // Debug output only in debug builds
}
```

### 3. Assertions
```c
#include <assert.h>

void validate_register(uint32_t address, uint32_t value, uint32_t min, uint32_t max) {
    // Precondition checks
    assert(min <= max);
    assert(address != 0);

    bool result = (value >= min && value <= max);

    // Postcondition check
    assert(result == true || result == false);  // Catches uninitialized returns
}
```

### 4. Static Analysis Tools
```bash
# Compiler warnings
gcc -Wall -Wextra -Wpedantic -Wuninitialized program.c

# Static analysis
cppcheck --enable=all program.c

# Memory checking
valgrind --leak-check=full ./program
```

### 5. GDB Systematic Approach
```gdb
# 1. Set strategic breakpoints
(gdb) break main
(gdb) break problematic_function
(gdb) break file.c:suspicious_line

# 2. Run and examine state
(gdb) run
(gdb) info locals
(gdb) print variable_name
(gdb) backtrace

# 3. Step through systematically
(gdb) next    # Don't step into functions
(gdb) step    # Step into functions
(gdb) finish  # Run until function returns

# 4. Watch variables change
(gdb) watch important_variable
(gdb) continue

# 5. Examine memory
(gdb) x/10x array_pointer
(gdb) print *structure_pointer
```

## Prevention Strategies

### 1. Defensive Programming
```c
// Always validate inputs
bool safe_divide(float numerator, float denominator, float *result) {
    if (result == NULL) {
        return false;  // Invalid output pointer
    }

    if (denominator == 0.0f) {
        return false;  // Division by zero
    }

    *result = numerator / denominator;
    return true;
}

// Use const for read-only parameters
void print_array(const float array[], int size) {
    // array cannot be modified
}
```

### 2. Clear Variable Initialization
```c
// Initialize at declaration
int count = 0;
bool is_valid = false;
float sum = 0.0f;
char buffer[256] = {0};  // Zero-initialize array

// Initialize structures
monitor_system_t system = {0};  // Zero-initialize all fields
```

### 3. Consistent Error Handling
```c
typedef enum {
    RESULT_SUCCESS = 0,
    RESULT_ERROR_NULL_POINTER = -1,
    RESULT_ERROR_INVALID_INPUT = -2,
    RESULT_ERROR_OUT_OF_RANGE = -3
} result_code_t;

result_code_t consistent_function(const input_t *input, output_t *output) {
    if (input == NULL || output == NULL) {
        return RESULT_ERROR_NULL_POINTER;
    }

    if (!is_input_valid(input)) {
        return RESULT_ERROR_INVALID_INPUT;
    }

    // Process...
    return RESULT_SUCCESS;
}
```

### 4. Code Review Checklist
```
□ All variables initialized before use?
□ Array bounds checked in all loops?
□ Null pointers checked before dereferencing?
□ Function parameters validated?
□ Return values checked for error conditions?
□ Memory allocated is freed?
□ Loop termination conditions correct?
□ Switch statements have default cases?
```

## Debugging Exercise Workflow

### For Day 2 Debug Practice:
1. **Compile with debug flags:** `gcc -g -Wall -O0`
2. **Read the code** to understand intended behavior
3. **Run the program** to observe actual behavior
4. **Start GDB** and set breakpoints at suspicious functions
5. **Step through systematically** using next/step
6. **Examine variables** at each step
7. **Form hypothesis** about the bug
8. **Test the fix** by modifying and recompiling
9. **Document the bug** and solution

### Example Debug Session:
```gdb
# Start debugging session
gdb ./debug_practice

# Set breakpoints at all buggy functions
(gdb) break calculate_average_buggy
(gdb) break validate_voltage_buggy
(gdb) break check_temperature_status_buggy
(gdb) break find_value_buggy
(gdb) break process_data_buggy
(gdb) break is_critical_error_buggy

# Run and debug each function systematically
(gdb) run
# When breakpoint hits, examine state and step through
```

## Summary

Effective debugging requires:

1. **Systematic approach** - Don't debug randomly
2. **Right tools** - Use GDB, compiler warnings, static analysis
3. **Good habits** - Initialize variables, validate inputs, check bounds
4. **Documentation** - Keep track of what you've tried
5. **Prevention** - Write defensive code to avoid bugs

Remember: The best debugging strategy is to write code that doesn't have bugs in the first place. But when bugs do occur, systematic debugging will help you find and fix them efficiently.

Practice these techniques with the Day 2 debugging exercises to build your debugging skills!


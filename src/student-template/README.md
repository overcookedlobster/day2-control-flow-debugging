# Day 2 Student Assignment: Control Flow and Debugging

## Overview
Welcome to Day 2 of the C Programming Course! Today you'll learn essential control flow concepts through a practical embedded systems project: building an automated register monitoring system.

## Learning Objectives
By the end of this assignment, you will:
- Master conditional statements (if-else) for validation logic
- Implement loops for repetitive testing scenarios
- Create and call functions effectively for modular testing
- Use GDB to debug programs step-by-step
- Design automated test sequences for hardware validation
- Handle multiple validation scenarios with switch statements

## Assignment Structure
```
student-template/
â”œâ”€â”€ src/                    # Your implementation files
â”‚   â”œâ”€â”€ register_monitor.c  # Tasks 1, 2 & 5 - TODO: Implement
â”‚   â”œâ”€â”€ test_functions.c    # Task 3 - TODO: Implement
â”‚   â”œâ”€â”€ debug_practice.c    # Task 4 - Debug the bugs!
â”‚   â””â”€â”€ monitor_utils.c     # Utility functions (provided)
â”œâ”€â”€ include/
â”‚   â””â”€â”€ monitor.h           # Header file (provided)
â”œâ”€â”€ tests/                  # Test files (provided)
â”œâ”€â”€ docs/                   # Learning guides (provided)
â”œâ”€â”€ debug/                  # GDB resources (provided)
â”œâ”€â”€ config/                 # Configuration files (provided)
â”œâ”€â”€ Makefile               # Build system (provided)
â””â”€â”€ README.md              # This file
```

## Tasks Overview

### í ¼í¾¯ Task 1: Conditional Validation Logic (50 minutes)
**File:** `src/register_monitor.c`

**What you'll implement:**
- `validate_voltage_range()` - Check if voltage is within safe limits
- `validate_temperature_range()` - Multi-level temperature validation
- `validate_current_range()` - Current consumption validation
- `determine_system_status()` - Complex status determination
- `check_critical_conditions()` - Compound conditional checking

**Key concepts:**
- Complex if-else chains with logical operators (&&, ||, !)
- Nested conditionals for decision trees
- Range checking and boundary validation
- Priority-based status determination

### í ½í´„ Task 2: Loop-Based Testing (50 minutes)
**File:** `src/register_monitor.c`

**What you'll implement:**
- `scan_all_registers()` - For loop with break/continue control
- `continuous_monitoring_loop()` - While loop with time limits
- `count_valid_registers()` - Simple counting loop
- `update_all_registers()` - Register update loop

**Key concepts:**
- For loops for sequential operations
- While loops for continuous monitoring
- Loop control with break and continue statements
- Nested loops for complex scenarios

### í ¾í·© Task 3: Modular Test Functions (50 minutes)
**File:** `src/test_functions.c`

**What you'll implement:**
- `test_voltage_levels()` - Comprehensive voltage testing
- `test_current_consumption()` - Current validation with tolerance
- `test_temperature_range()` - Temperature testing with recommendations
- `validate_register()` - Register validation function
- `run_comprehensive_test()` - Function composition example
- `demonstrate_function_concepts()` - Parameter passing demo

**Key concepts:**
- Single responsibility principle
- Parameter validation and error handling
- Function composition and organization
- Parameter passing by value vs pointer

### í ½í°› Task 4: GDB Debugging Session (50 minutes)
**File:** `src/debug_practice.c`

**What you'll debug:**
This file contains 6 intentional bugs for you to find and fix using GDB:
1. **Array bounds violation** - Off-by-one error in loop
2. **Uninitialized variable** - Variable used before initialization
3. **Logic error** - Incorrect conditional ordering
4. **Infinite loop** - Missing bounds check
5. **Null pointer dereference** - Missing null check
6. **Assignment vs comparison** - Using = instead of ==

**Key concepts:**
- Systematic debugging methodology
- GDB command proficiency (break, run, next, step, print, continue)
- Common C programming pitfalls
- Bug identification and documentation

### í ½í´§ Task 5: Switch-Based Error Handling (30 minutes)
**File:** `src/register_monitor.c`

**What you'll implement:**
- `handle_error()` - Switch-based error classification
- `get_error_message()` - Error message lookup
- `attempt_error_recovery()` - Recovery strategies

**Key concepts:**
- Switch statement usage and best practices
- Comprehensive error classification
- Recovery strategy implementation
- Error logging and reporting

## Getting Started

### Prerequisites
- GCC compiler with C11 support
- Make build system
- GDB debugger
- Linux/Unix environment (or WSL on Windows)

### Setup Instructions
1. **Clone or download** this assignment template
2. **Navigate** to the student-template directory
3. **Read the documentation** in the `docs/` folder
4. **Start with Task 1** in `src/register_monitor.c`

### Build Commands
```bash
# Build all programs
make all

# Build with debug information (recommended)
make debug

# Test your implementations
make test

# Start GDB debugging session for Task 4
make gdb-session

# Clean build artifacts
make clean
```

## Implementation Guidelines

### í ¼í¾¯ Task 1: Conditional Logic
Look for TODO comments in `validate_voltage_range()` and related functions. Each function has:
- Clear step-by-step instructions
- Example framework code (commented out)
- Expected behavior description
- Constants defined in `monitor.h`

**Example approach for voltage validation:**
```c
bool validate_voltage_range(float voltage) {
    if (voltage < MIN_VOLTAGE) {
        printf("ERROR: Voltage too low: %.2fV\n", voltage);
        return false;
    }
    // Add more conditions...
    return true;
}
```

### í ½í´„ Task 2: Loop Operations
Implement loops in `scan_all_registers()` and related functions:
- Use `for` loops for known iterations
- Use `while` loops for condition-based monitoring
- Practice `break` and `continue` statements
- Handle edge cases and error conditions

**Example approach for register scanning:**
```c
int scan_all_registers(monitor_system_t *system) {
    for (int i = 0; i < system->num_registers; i++) {
        // Read and validate each register
        // Use continue for non-critical errors
        // Use break for critical failures
    }
    return valid_count;
}
```

### í ¾í·© Task 3: Modular Functions
Create well-designed functions in `test_functions.c`:
- Each function should have a single responsibility
- Validate input parameters
- Return meaningful values
- Use clear, descriptive names

**Example approach for voltage testing:**
```c
bool test_voltage_levels(float voltage) {
    printf("=== Voltage Level Test ===\n");

    // Input validation
    if (voltage < 0.0f) {
        printf("FAIL: Invalid voltage\n");
        return false;
    }

    // Range checking
    // Tolerance checking
    // Result reporting

    return true;
}
```

### í ½í°› Task 4: GDB Debugging
The `debug_practice.c` file is already complete but contains intentional bugs:

1. **Compile with debug flags:**
   ```bash
   gcc -g -Wall -o debug_practice debug_practice.c monitor_utils.c
   ```

2. **Start GDB:**
   ```bash
   gdb ./debug_practice
   ```

3. **Essential GDB commands:**
   ```gdb
   (gdb) break main          # Set breakpoint
   (gdb) run                 # Start program
   (gdb) next                # Execute next line
   (gdb) step                # Step into functions
   (gdb) print variable      # Examine variables
   (gdb) continue            # Continue execution
   (gdb) backtrace          # Show call stack
   ```

4. **Document your findings** in `debug/gdb_session_log.md`

### í ½í´§ Task 5: Error Handling
Implement switch statements in error handling functions:
- Use `switch` for multiple discrete choices
- Include `default` case for unknown errors
- Implement recovery strategies where possible
- Log errors for debugging

**Example approach:**
```c
void handle_error(error_code_t error_code) {
    switch (error_code) {
        case ERROR_VOLTAGE_LOW:
            printf("ERROR: Voltage too low\n");
            // Recovery action
            break;

        case ERROR_TEMPERATURE_HIGH:
            printf("ERROR: Temperature critical\n");
            // Emergency action
            break;

        default:
            printf("ERROR: Unknown error\n");
            break;
    }
}
```

## Testing Your Implementation

### Running Individual Tasks
```bash
# Test Tasks 1, 2 & 5
./bin/register_monitor

# Test Task 3
./bin/test_functions

# Debug Task 4
gdb ./bin/debug_practice
```

### Expected Output
Each program should produce detailed output showing:
- Function execution flow
- Validation results
- Error handling demonstrations
- Test summaries

### Validation
Your implementation will be tested against:
- **Functionality**: Correct behavior for all inputs
- **Code Quality**: Clean, readable, well-commented code
- **Edge Cases**: Proper handling of boundary conditions
- **Error Handling**: Appropriate responses to invalid inputs

## Learning Resources

### í ½í³š Documentation (in `docs/` folder)
- `CONTROL_FLOW_GUIDE.md` - Complete guide to conditionals and loops
- `FUNCTION_DESIGN.md` - Best practices for function design
- `DEBUGGING_STRATEGIES.md` - Systematic debugging methodology

### í ½í´§ Debug Resources (in `debug/` folder)
- `gdb_commands.txt` - GDB command reference
- `gdb_session_log.md` - Template for documenting your debugging session

### âš™ï¸ Configuration (in `config/` folder)
- `register_map.txt` - Hardware register definitions
- `test_scenarios.txt` - Test cases and expected results

## Common Pitfalls to Avoid

### âŒ Conditional Logic Mistakes
- Using `=` instead of `==` in conditions
- Incorrect operator precedence with `&&` and `||`
- Missing `else` cases for uninitialized variables
- Off-by-one errors in range checking

### âŒ Loop Mistakes
- Off-by-one errors (`<=` vs `<`)
- Infinite loops (forgetting to update loop variables)
- Missing `break` statements in switch cases
- Accessing arrays out of bounds

### âŒ Function Design Mistakes
- Functions doing too many things
- Not validating input parameters
- Inconsistent return value conventions
- Poor naming conventions

### âŒ Debugging Mistakes
- Not compiling with `-g` flag
- Not setting breakpoints before running
- Not examining variable values at each step
- Not documenting findings

## Assessment Criteria

### Functionality (60%)
- âœ… All required functions implemented correctly
- âœ… Proper handling of edge cases and error conditions
- âœ… Expected output matching specifications
- âœ… All bugs found and documented in Task 4

### Code Quality (25%)
- âœ… Clean, readable code with proper formatting
- âœ… Appropriate comments and documentation
- âœ… Consistent naming conventions
- âœ… Proper error handling throughout

### Understanding (15%)
- âœ… Efficient algorithms and appropriate data structures
- âœ… Professional coding practices
- âœ… Demonstration of concept mastery
- âœ… Thoughtful debugging documentation

## Getting Help

### í ¼í¶˜ If You're Stuck
1. **Read the TODO comments** - They contain step-by-step instructions
2. **Check the example framework** - Uncomment and complete the provided code
3. **Consult the documentation** - Guides in `docs/` folder explain concepts
4. **Use GDB for debugging** - Step through your code to understand behavior
5. **Ask for help** - Don't hesitate to ask instructors or classmates

### í ¼í¾¯ Success Tips
1. **Start early** - Don't wait until the last minute
2. **Test frequently** - Build and test after each function
3. **Read error messages** - Compiler warnings often point to issues
4. **Use printf debugging** - Add diagnostic output to understand flow
5. **Document your work** - Especially for the GDB debugging session

## Submission Guidelines

### í ½í³‹ What to Submit
- All implemented source files in `src/` directory
- Completed `debug/gdb_session_log.md` with your debugging findings
- Any additional documentation or notes you created

### âœ… Before Submission Checklist
- [ ] All programs compile without errors
- [ ] All functions implemented and working
- [ ] GDB debugging session completed and documented
- [ ] Code is clean and well-commented
- [ ] All tests pass when run

Good luck with your assignment! Remember, the goal is to learn these fundamental concepts through hands-on practice. Take your time, experiment, and don't be afraid to make mistakes - that's how we learn! í ½íº€


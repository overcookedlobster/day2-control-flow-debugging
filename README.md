# Day 2 Student Assignment: Control Flow and Debugging
## Automated Register Monitoring System

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

## Assignment Details
**Theme:** Automated Register Monitoring System
**Focus:** Control flow, loops, functions, GDB debugging
**Time Allocation:** 3.5 hours in-class + 2 hours homework
**Total Points:** 150 points

## Project Structure
```
day2-control-flow-debugging/
├── src/                        # Your implementation files
│   ├── register_monitor.c      # Tasks 1, 2 & 5 - TODO: Implement
│   ├── test_functions.c        # Task 3 - TODO: Implement
│   ├── debug_practice.c        # Task 4 - Debug the bugs!
│   ├── monitor_utils.c         # Utility functions (provided)
│   ├── multi_chip_monitor.c    # Homework: Multi-chip testing
│   └── error_recovery.c        # Homework: Error recovery systems
├── include/
│   └── monitor.h               # Header file (provided)
├── tests/                      # Test files (provided)
│   └── test_validation.c       # Automated test framework
├── docs/                       # Learning guides (provided)
│   ├── CONTROL_FLOW_GUIDE.md   # Control flow best practices
│   ├── FUNCTION_DESIGN.md      # Function design guidelines
│   └── DEBUGGING_STRATEGIES.md # Debugging methodology
├── config/                     # Configuration files (provided)
│   ├── register_map.txt        # Register address definitions
│   └── test_scenarios.txt      # Test case configurations
├── .github/                    # GitHub Classroom configuration
│   ├── classroom/
│   │   └── autograding.json    # Autograder configuration
│   └── workflows/
│       └── classroom.yml       # GitHub Actions workflow
├── Makefile                    # Build system (provided)
└── README.md                   # This file
```

##  In-Class Tasks (3.5 hours)

### Task 1: Conditional Validation Logic (50 minutes)
**Objective:** Implement complex if-else chains with logical operators for register status checking

**What you'll implement:**
- Multi-level conditional logic for register validation
- Logical operators (&&, ||, !) for complex conditions
- Nested if-else structures for decision trees
- Range checking and boundary validation

**Files to work on:**
- `src/register_monitor.c` (Part 1)
- `include/monitor.h`

**Key concepts:**
```c
// Complex conditional logic example
if (voltage >= MIN_VOLTAGE && voltage <= MAX_VOLTAGE) {
    if (temperature < TEMP_WARNING) {
        status = STATUS_NORMAL;
    } else if (temperature < TEMP_CRITICAL) {
        status = STATUS_WARNING;
    } else {
        status = STATUS_CRITICAL;
    }
} else {
    status = STATUS_VOLTAGE_ERROR;
}
```

### Task 2: Loop-Based Testing (50 minutes)
**Objective:** Implement for/while loops for sequential and continuous monitoring

**What you'll implement:**
- For loops for sequential register testing
- While loops for continuous monitoring
- Loop control with break and continue
- Nested loops for multi-dimensional testing

**Files to work on:**
- `src/register_monitor.c` (Part 2)
- `include/monitor.h`

**Key concepts:**
```c
// Sequential register testing
for (int reg = 0; reg < NUM_REGISTERS; reg++) {
    uint32_t value = read_register(reg);
    if (validate_register(reg, value)) {
        printf("Register %d: PASS\n", reg);
    } else {
        printf("Register %d: FAIL\n", reg);
        error_count++;
    }
}

// Continuous monitoring
while (system_active && error_count < MAX_ERRORS) {
    update_all_registers();
    if (check_critical_conditions()) {
        handle_critical_error();
        break;
    }
    sleep(MONITOR_INTERVAL);
}
```

### Task 3: Function-Based Test Organization (50 minutes)
**Objective:** Create modular functions for different validation types

**What you'll implement:**
- Separate functions for voltage, current, temperature testing
- Function parameters and return values
- Function prototypes and proper organization
- Modular test design following validation patterns

**Files to work on:**
- `src/test_functions.c`
- `include/monitor.h`

**Key concepts:**
```c
// Function prototypes
bool test_voltage_levels(float voltage);
bool test_current_consumption(float current);
bool test_temperature_range(float temperature);
int run_comprehensive_test(void);

// Function implementation
bool test_voltage_levels(float voltage) {
    if (voltage < MIN_VOLTAGE) {
        printf("ERROR: Voltage too low: %.2fV\n", voltage);
        return false;
    }
    if (voltage > MAX_VOLTAGE) {
        printf("ERROR: Voltage too high: %.2fV\n", voltage);
        return false;
    }
    printf("PASS: Voltage within range: %.2fV\n", voltage);
    return true;
}
```

### Task 4: GDB Debugging Session (50 minutes)
**Objective:** Hands-on GDB usage with intentionally buggy validation code

**What you'll practice:**
- Setting breakpoints with `break`
- Running programs with `run`
- Stepping through code with `next` and `step`
- Examining variables with `print`
- Continuing execution with `continue`
- Viewing call stack with `backtrace`

**Files to work on:**
- `src/debug_practice.c` (contains intentional bugs)
- `debug/gdb_session_log.md` (document your debugging session)

**GDB Commands to master:**
```bash
# Compile with debug info
gcc -g -o debug_practice debug_practice.c

# Start GDB
gdb ./debug_practice

# Essential GDB commands
(gdb) break main          # Set breakpoint at main
(gdb) run                 # Start program
(gdb) next                # Execute next line
(gdb) step                # Step into functions
(gdb) print variable_name # Print variable value
(gdb) continue            # Continue execution
(gdb) backtrace           # Show call stack
(gdb) quit                # Exit GDB
```

### Task 5: Switch-Based Error Handling (30 minutes)
**Objective:** Implement switch statements for different error types

**What you'll implement:**
- Switch statements for error classification
- Comprehensive error reporting system
- Default case handling
- Error recovery strategies

**Files to work on:**
- `src/register_monitor.c` (Part 3)
- `include/monitor.h`

**Key concepts:**
```c
void handle_error(int error_code) {
    switch (error_code) {
        case ERROR_VOLTAGE_LOW:
            printf("ERROR: Voltage below minimum threshold\n");
            // Implement recovery action
            break;
        case ERROR_VOLTAGE_HIGH:
            printf("ERROR: Voltage above maximum threshold\n");
            // Implement recovery action
            break;
        case ERROR_TEMPERATURE_HIGH:
            printf("ERROR: Temperature critical\n");
            // Implement emergency shutdown
            break;
        case ERROR_COMMUNICATION:
            printf("ERROR: Communication failure\n");
            // Implement retry mechanism
            break;
        default:
            printf("ERROR: Unknown error code: %d\n", error_code);
            break;
    }
}
```

##  Homework Extensions (2 hours)

### Homework 1: Advanced Loop Patterns (60 minutes)
**Objective:** Implement nested loops and optimization techniques

**What you'll implement:**
- Nested loops for multi-dimensional testing
- Loop optimization for performance
- Complex iteration patterns
- Early termination strategies

**Files to work on:**
- `src/multi_chip_monitor.c`

### Homework 2: Error Recovery Systems (60 minutes)
**Objective:** Implement automatic retry and graceful degradation

**What you'll implement:**
- Automatic retry mechanisms
- Graceful degradation strategies
- Error logging and reporting
- System recovery procedures

**Files to work on:**
- `src/error_recovery.c`

## Getting Started

### Prerequisites
- GCC compiler with C11 support
- Make build system
- GDB debugger
- Linux/Unix environment (or WSL on Windows)

### Setup Instructions
1. **Clone your repository** from GitHub Classroom
2. **Navigate** to the project directory
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

### Quick Start
```bash
# 1. Build the project
make debug

# 2. Run tests to see current status
make test

# 3. Start with Task 1 - open src/register_monitor.c
# 4. Look for TODO comments and implement step by step
# 5. Test your work: make test-conditionals
```

##  Assessment Criteria (150 points total)

### Control Flow Implementation (50 points)
- **Conditional Logic (15 pts):** Correct if-else implementation
- **Loop Structures (15 pts):** Proper for/while loop usage
- **Switch Statements (10 pts):** Effective error handling
- **Logic Flow (10 pts):** Clear, efficient program flow

### Function Design (40 points)
- **Modular Design (15 pts):** Well-organized, reusable functions
- **Parameter Handling (10 pts):** Proper parameter usage
- **Return Values (10 pts):** Appropriate return value handling
- **Function Organization (5 pts):** Clear function structure

### GDB Debugging (30 points)
- **Debugging Session (15 pts):** Documented GDB usage
- **Bug Identification (10 pts):** Successfully found bugs
- **Debugging Techniques (5 pts):** Proper GDB command usage

### Code Quality (20 points)
- **Code Style (8 pts):** Consistent formatting and naming
- **Comments (6 pts):** Clear, helpful documentation
- **Organization (6 pts):** Logical code structure

### GitHub Workflow (10 points)
- **Commit Quality (5 pts):** Frequent, descriptive commits
- **Repository Organization (3 pts):** Clean file structure
- **Documentation (2 pts):** Updated README and comments

##  Build System

### Available Make Targets
```bash
make all              # Build all programs
make clean            # Clean build artifacts
make test             # Run all tests
make debug            # Build with debug flags
make test-conditionals # Test conditional logic
make test-loops       # Test loop implementations
make test-functions   # Test function implementations
make gdb-session      # Start GDB debugging session
make help             # Show all available targets
```

### Debugging Support
```bash
# Build with debug information
make debug

# Run specific program with GDB
make gdb-session

# Run with Valgrind (memory checking)
make valgrind
```

##  Learning Resources

### Control Flow Fundamentals
- Review `docs/CONTROL_FLOW_GUIDE.md` for comprehensive tutorial
- Practice with simple examples before complex validation logic
- Understand operator precedence and logical operators

### Function Design
- Read `docs/FUNCTION_DESIGN.md` for best practices
- Learn about function scope and parameter passing
- Practice modular programming principles

### GDB Debugging
- Study `debug/debugging_guide.md` for step-by-step instructions
- Practice with `debug/gdb_commands.txt` reference
- Document your debugging sessions in `debug/gdb_session_log.md`

##  Common Pitfalls to Avoid

### Control Flow Issues
- Off-by-one errors in loop conditions
- Infinite loops due to incorrect termination conditions
- Missing break statements in switch cases
- Incorrect logical operator usage (= vs ==)

### Function Design Problems
- Functions that do too much (lack of modularity)
- Inconsistent parameter ordering
- Missing return statements
- Unclear function naming

### Debugging Challenges
- Not compiling with debug flags (-g)
- Setting breakpoints in wrong locations
- Not examining variable values during debugging
- Giving up too quickly on complex bugs

##  Success Criteria

By the end of Day 2, you should be able to:
- ✅ Write complex conditional logic for validation scenarios
- ✅ Implement efficient loops for repetitive testing
- ✅ Design modular functions for different test types
- ✅ Use GDB effectively to debug C programs
- ✅ Handle errors gracefully with switch statements
- ✅ Organize code in a professional, maintainable way

##  Next Steps

Day 2 prepares you for:
- **Day 3:** Memory management and data structures
- **Day 4:** Modular programming and cross-compilation
- **Day 5:** Hardware peripheral programming

The control flow and debugging skills you learn today are fundamental to all programming!

---

**Remember:** Good debugging skills will save you hours of frustration. Take time to master GDB - it's one of the most valuable tools in a C programmer's toolkit!


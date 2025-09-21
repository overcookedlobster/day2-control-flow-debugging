# GitHub Classroom Autograding Guide

## Overview
This repository is configured for GitHub Classroom with comprehensive autograding for Day 2: Control Flow and Debugging assignment.

## Autograding Configuration

### Test Structure (200 points total)

1. **Build System Check (10 points)**
   - Verifies project builds successfully
   - Installs required dependencies (gcc, gdb, valgrind, etc.)

2. **Task 1: Conditional Logic (20 points)**
   - Tests voltage, temperature, and current validation functions
   - Verifies complex if-else logic implementation

3. **Task 2: Loop Operations (20 points)**
   - Tests register scanning and continuous monitoring loops
   - Verifies for/while loop implementations

4. **Task 3: Modular Functions (20 points)**
   - Tests individual test functions for voltage, current, temperature
   - Verifies function parameter handling and return values

5. **Task 4: Debug Practice (10 points)**
   - Verifies debug practice program compiles and runs
   - Tests intentional bugs for GDB practice

6. **Task 5: Error Handling (20 points)**
   - Tests switch-based error handling implementation
   - Verifies error recovery mechanisms

7. **Comprehensive Validation Suite (50 points)**
   - Runs complete test suite with 20+ test cases
   - Must achieve 100% pass rate for full points

8. **Homework 1: Multi-Chip Monitoring (15 points)**
   - Tests advanced loop patterns and nested loops
   - Verifies multi-chip system implementation

9. **Homework 2: Error Recovery Systems (15 points)**
   - Tests retry mechanisms and graceful degradation
   - Verifies comprehensive error recovery

10. **Code Quality and Style (5 points)**
    - Runs astyle for code formatting check
    - Verifies consistent coding standards

11. **Static Analysis (5 points)**
    - Runs cppcheck for static code analysis
    - Identifies potential code issues

12. **Memory Safety (10 points)**
    - Runs valgrind for memory leak detection
    - Verifies proper memory management

## How Autograding Works

### GitHub Actions Workflow
- Triggers on push to main/master branch
- Sets up Ubuntu environment with required tools
- Builds project using Makefile
- Runs comprehensive test suite
- Executes autograding tests
- Generates detailed test report

### Test Execution
Each test has:
- **Setup**: Environment preparation
- **Run**: Command to execute
- **Expected Output**: String to match in output
- **Timeout**: Maximum execution time
- **Points**: Score for passing the test

### Grading Criteria
- **Compilation**: Must build without errors (10 points)
- **Functionality**: Each task must produce expected output
- **Comprehensive Tests**: All validation tests must pass (50 points)
- **Code Quality**: Style and analysis checks
- **Advanced Features**: Homework assignments for extra credit

## Student Instructions

### Prerequisites
- C compiler (gcc)
- Make build system
- GDB debugger
- Git for version control

### Getting Started
1. Accept GitHub Classroom assignment
2. Clone your repository
3. Read README.md for detailed instructions
4. Start with Task 1 in `src/register_monitor.c`

### Development Workflow
1. **Implement**: Write code for each task
2. **Test Locally**: Use `make test-validation`
3. **Debug**: Use `make gdb-session` for debugging
4. **Commit**: Push changes to trigger autograding
5. **Review**: Check GitHub Actions results

### Local Testing Commands
```bash
# Build all programs
make debug

# Run comprehensive tests
make test-validation

# Test individual components
make test-conditionals
make test-loops
make test-functions

# Debug practice
make gdb-session

# Code quality checks
make style-check
make static-analysis
make valgrind
```

### Common Issues and Solutions

#### Build Failures
- Check for syntax errors in C code
- Ensure all required functions are implemented
- Verify header file includes

#### Test Failures
- Review test output for specific failures
- Use local testing to debug issues
- Check function return values and output format

#### Timeout Issues
- Avoid infinite loops in monitoring functions
- Use proper loop termination conditions
- Test with shorter durations locally

#### Memory Issues
- Initialize all variables
- Check for null pointer dereferences
- Use valgrind locally to detect memory leaks

## Instructor Notes

### Customizing Tests
- Modify `.github/classroom/autograding.json` for different point values
- Update expected output strings for different implementations
- Adjust timeout values based on system performance

### Adding New Tests
1. Add test entry to autograding.json
2. Create corresponding make target if needed
3. Update point totals
4. Test thoroughly before deployment

### Monitoring Student Progress
- Check GitHub Actions logs for detailed test results
- Review commit history for development progress
- Use test artifacts for debugging student issues

### Common Student Mistakes
1. **Infinite Loops**: In continuous monitoring functions
2. **Missing Output**: Not printing expected strings
3. **Compilation Errors**: Syntax or linking issues
4. **Logic Errors**: Incorrect conditional statements
5. **Memory Issues**: Uninitialized variables or null pointers

## Troubleshooting

### Autograding Not Running
- Check GitHub Actions permissions
- Verify workflow file syntax
- Ensure autograding.json is valid JSON

### Tests Failing Unexpectedly
- Check for environment differences
- Verify timeout values are sufficient
- Review expected output strings

### Performance Issues
- Increase timeout values if needed
- Optimize test execution order
- Consider parallel test execution

## Support Resources

### For Students
- README.md: Complete assignment instructions
- docs/: Learning guides and best practices
- GitHub Issues: Report problems or ask questions

### For Instructors
- GitHub Classroom dashboard: Monitor progress
- Actions logs: Detailed test execution results
- Repository insights: Code quality metrics

## Version History
- v1.0: Initial autograding setup
- v1.1: Enhanced test coverage and error handling
- v1.2: Added memory safety and code quality checks


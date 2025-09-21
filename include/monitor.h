#ifndef MONITOR_H
#define MONITOR_H

#include <stdint.h>
#include <stdbool.h>

// System constants
#define MAX_REGISTERS 16
#define MAX_ERRORS 10
#define MONITOR_INTERVAL 1000  // milliseconds

// Voltage thresholds (in Volts)
#define MIN_VOLTAGE 3.0f
#define MAX_VOLTAGE 3.6f
#define NOMINAL_VOLTAGE 3.3f

// Temperature thresholds (in Celsius)
#define TEMP_WARNING 75.0f
#define TEMP_CRITICAL 85.0f
#define TEMP_NORMAL 25.0f

// Current thresholds (in Amperes)
#define MIN_CURRENT 0.05f
#define MAX_CURRENT 2.0f
#define NOMINAL_CURRENT 0.5f

// Status codes
typedef enum {
    STATUS_NORMAL = 0,
    STATUS_WARNING = 1,
    STATUS_CRITICAL = 2,
    STATUS_VOLTAGE_ERROR = 3,
    STATUS_TEMPERATURE_ERROR = 4,
    STATUS_CURRENT_ERROR = 5,
    STATUS_COMMUNICATION_ERROR = 6
} system_status_t;

// Error codes
typedef enum {
    ERROR_NONE = 0,
    ERROR_VOLTAGE_LOW = 1,
    ERROR_VOLTAGE_HIGH = 2,
    ERROR_TEMPERATURE_HIGH = 3,
    ERROR_CURRENT_LOW = 4,
    ERROR_CURRENT_HIGH = 5,
    ERROR_COMMUNICATION = 6,
    ERROR_TIMEOUT = 7,
    ERROR_INVALID_DATA = 8
} error_code_t;

// Register information structure
typedef struct {
    uint32_t address;
    uint32_t value;
    uint32_t expected_min;
    uint32_t expected_max;
    bool is_valid;
    char name[32];
} register_info_t;

// System monitoring structure
typedef struct {
    float voltage;
    float temperature;
    float current;
    system_status_t status;
    int error_count;
    bool system_active;
    register_info_t registers[MAX_REGISTERS];
    int num_registers;
} monitor_system_t;

// Function prototypes for Task 1: Conditional Logic
bool validate_voltage_range(float voltage);
bool validate_temperature_range(float temperature);
bool validate_current_range(float current);
system_status_t determine_system_status(float voltage, float temperature, float current);
bool check_critical_conditions(const monitor_system_t *system);

// Function prototypes for Task 2: Loop Operations
int scan_all_registers(monitor_system_t *system);
void continuous_monitoring_loop(monitor_system_t *system, int duration_seconds);
int count_valid_registers(const monitor_system_t *system);
void update_all_registers(monitor_system_t *system);
bool monitor_until_stable(monitor_system_t *system, int max_iterations);

// Function prototypes for Task 3: Modular Functions
bool test_voltage_levels(float voltage);
bool test_current_consumption(float current);
bool test_temperature_range(float temperature);
int run_comprehensive_test(monitor_system_t *system);
bool validate_register(uint32_t address, uint32_t value, uint32_t min, uint32_t max);

// Function prototypes for Task 4: Debugging Support
void print_system_state(const monitor_system_t *system);
void log_error(error_code_t error, const char *message);
void debug_register_dump(const monitor_system_t *system);
bool simulate_hardware_failure(monitor_system_t *system, error_code_t error_type);

// Function prototypes for Task 5: Error Handling
void handle_error(error_code_t error_code);
const char* get_error_message(error_code_t error_code);
bool attempt_error_recovery(monitor_system_t *system, error_code_t error_code);
void emergency_shutdown(monitor_system_t *system);

// Utility functions
void init_monitor_system(monitor_system_t *system);
void cleanup_monitor_system(monitor_system_t *system);
uint32_t read_register(uint32_t address);
bool write_register(uint32_t address, uint32_t value);
void delay_ms(int milliseconds);

// Homework function prototypes
int multi_chip_monitoring(int num_chips);
bool implement_retry_mechanism(monitor_system_t *system, int max_retries);
void graceful_degradation(monitor_system_t *system);

#endif // MONITOR_H


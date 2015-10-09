/// @file 
/// File generated from can2C.py
/// Simply run the python script everytime the yaml CAN specification is changed
/// This is located at: centaurus3/Utility Programs/cangen/can2C.py

#ifndef GENERATED_CAN_HEADER
#define GENERATED_CAN_HEADER

#include "d1k.h"
#include "d1k_can.h"

typedef union {
	uint8_t b[4];
    uint32_t i;
	float f;
} can_float_union_t;

/// @brief enumeration for CAN packet IDs
typedef enum {
    ///Low frequency BP status update.
    CAN_PACKET_BP_STATE = 0x200,

    ///State of charge and pack voltage
    CAN_PACKET_BP_SOC = 0x201,

    ///current draw from battery
    CAN_PACKET_BP_CURRENT = 0x202,

    ///write state of charge, use with caution
    CAN_PACKET_BP_SOC_SET = 0x205,

    ///min and max cell voltages and temperatures
    CAN_PACKET_BP_MINMAX = 0x206,

    ///Information for car startup problems.
    CAN_PACKET_BP_STARTUP_ERROR = 0x20a,

    ///sets discharge parameters
    CAN_PACKET_BP_DISCHARGE_SET = 0x20b,

    ///enables BP debug
    CAN_PACKET_BP_DEBUG_EN = 0x2c,

    ///sets BP debug params
    CAN_PACKET_BP_DEBUG_PARAM_SET = 0x2d,

    ///Voltage and temperature for a single module
    CAN_PACKET_BP_MODULE = 0x210,

    ///Emergency packet to kill the car
    CAN_PACKET_KILL = 0x20,

    ///Heartbeat packet for the dashboard
    CAN_PACKET_DASHBOARD_STATUS = 0x300,

    ///displays a message on the driver display. Message is null-terminated.
    CAN_PACKET_DASHBOARD_DISPLAY_MESSAGE = 0x310,

    ///Sent by strategy to inform the driver. Negative values represent invalid (disable).
    CAN_PACKET_SPEED_ZONE_INFO = 0x321,

    ///Sent by dashboard ONLY after driver conformation. Sets the cruise control target speed. Magic set to (0x501A4CA4) for safety.
    CAN_PACKET_CRUISE_CONTROL_SPEED_SET = 0x322,

    ///status of switches and pedals
    CAN_PACKET_DRIVER_INFORMATION_DI = 0x50,

    ///status of all di switches
    CAN_PACKET_C3_DI_SWITCHES = 0x15,

    ///Drive command.  This must be sent at a minimum of 4 hz.
    CAN_PACKET_FULLCURRENT_DRIVE = 0x400,

    ///Sets the tire radius for speed calculations.
    CAN_PACKET_FULLCURRENT_TIRE_RADIUS = 0x401,

    ///D & Q axis currents
    CAN_PACKET_FULLCURRENT_IQ_ID = 0x420,

    ///Position of the rotor & speed of the rotor (RPM)
    CAN_PACKET_FULLCURRENT_POSITION_SPEED = 0x422,

    ///Errors and warnings present on the controller.
    CAN_PACKET_FULLCURRENT_EVENTSTATE = 0x424,

    ///Gets the calibration state of the motor.
    CAN_PACKET_FULLCURRENT_CALIBRATION = 0x426,

    ///Instructs the motor to calibrate
    CAN_PACKET_FULLCURRENT_CALIBRATE = 0x428,

    ///Sets the scale of the current command.
    CAN_PACKET_FULLCURRENT_CURRENTLIMIT = 0x42a,

    ///actual on/off state of front lights
    CAN_PACKET_LIGHTS_STATE_FRONT = 0xb0,

    ///actual on/off state of rear lights
    CAN_PACKET_LIGHTS_STATE_REAR = 0xb1,

    ///current that all of the lights draw
    CAN_PACKET_LIGHTS_CURRENT = 0xb5,

    ///status of everything on the wheel
    CAN_PACKET_DRIVER_INFORMATION_WHEEL = 0x51,

    ///Turn signals from the steering wheel
    CAN_PACKET_C3_LIGHTS_BLINKERS = 0xa0,

    ///GPS location of the car
    CAN_PACKET_TELEMETRY_LATLON = 0x6c0,

    ///GPS velocity vector of the car
    CAN_PACKET_TELEMETRY_HEADING = 0x6c1,

    ///Tracker data (on RTR only)
    CAN_PACKET_TRACKER_DATA = 0x600,

    ///enable/disable trackers
    CAN_PACKET_TRACKER_ENABLE = 0x610,

    ///Tritium Motor Drive Command
    CAN_PACKET_TRITIUM_MOTOR_DRIVE = 0x121,

    ///Tritium Motor Power Command
    CAN_PACKET_TRITIUM_MOTOR_POWER = 0x122,

    ///Tritium Reset Command
    CAN_PACKET_TRITIUM_RESET = 0x123,

    ///Tritium Identification Information
    CAN_PACKET_TRITIUM_IDENTIFICATION = 0x100,

    ///Tritium Status Information
    CAN_PACKET_TRITIUM_STATUS = 0x101,

    ///Tritium Bus Information
    CAN_PACKET_TRITIUM_BUS = 0x102,

    ///Tritium Velocity Measurement
    CAN_PACKET_TRITIUM_VELOCITY = 0x103,

    ///Tritium Phase Current Measurement
    CAN_PACKET_TRITIUM_PHASE_CURRENT = 0x104,

    ///Tritium Motor Voltage Vector Measurement
    CAN_PACKET_TRITIUM_MOTOR_VOLTAGE_VECTOR = 0x105,

    ///Tritium Motor Current Vector Measurement
    CAN_PACKET_TRITIUM_MOTOR_CURRENT_VECTOR = 0x106,

    ///Tritium Motor BackEMF Measurement / Prediction
    CAN_PACKET_TRITIUM_MOTOR_BACKEMF = 0x107,

    ///Tritium 15 and 1.65 Voltage Rail Measurement
    CAN_PACKET_TRITIUM_15_165_VOLTAGE_RAIL = 0x108,

    ///Tritium 2.5V and 1.2V Voltage Rail Measurement
    CAN_PACKET_TRITIUM_25_12_VOLTAGE_RAIL = 0x109,

} CANPacketId;


///@brief Low frequency BP status update.
typedef struct can_packet_bp_state_struct{
        uint16_t primary_voltage;
        struct {
            uint8_t contactor_mid :1;

            uint8_t contactor_bus :1;

            uint8_t contactor_bus_pre :1;

            uint8_t contactor_tracker :1;

            /// 0x0:fivecell\n
            /// 0x1:bus_powered\n
            uint8_t power_source :1;

        };
        float battery_voltage;
} can_packet_bp_state;

/// the length of the bp_state packet
#define CAN_LENGTH_BP_STATE sizeof(can_packet_bp_state)

/** initial values for the bp_state packet
 *  To be used like:
 *  @code
 *  CANPacket my_bp_state_packet = CAN_INIT_BP_STATE;
 *  @endcode **/
#define CAN_INIT_BP_STATE {.id=CAN_PACKET_BP_STATE, .length=CAN_LENGTH_BP_STATE}

///@brief State of charge and pack voltage
typedef struct can_packet_bp_soc_struct{
        float soc;
        uint16_t voltage;
} can_packet_bp_soc;

/// the length of the bp_soc packet
#define CAN_LENGTH_BP_SOC sizeof(can_packet_bp_soc)

/** initial values for the bp_soc packet
 *  To be used like:
 *  @code
 *  CANPacket my_bp_soc_packet = CAN_INIT_BP_SOC;
 *  @endcode **/
#define CAN_INIT_BP_SOC {.id=CAN_PACKET_BP_SOC, .length=CAN_LENGTH_BP_SOC}

///@brief current draw from battery
typedef struct can_packet_bp_current_struct{
        int32_t main;
        int32_t tracker;
} can_packet_bp_current;

/// the length of the bp_current packet
#define CAN_LENGTH_BP_CURRENT sizeof(can_packet_bp_current)

/** initial values for the bp_current packet
 *  To be used like:
 *  @code
 *  CANPacket my_bp_current_packet = CAN_INIT_BP_CURRENT;
 *  @endcode **/
#define CAN_INIT_BP_CURRENT {.id=CAN_PACKET_BP_CURRENT, .length=CAN_LENGTH_BP_CURRENT}

///@brief write state of charge, use with caution
typedef struct can_packet_bp_soc_set_struct{
        float soc;
} can_packet_bp_soc_set;

/// the length of the bp_soc_set packet
#define CAN_LENGTH_BP_SOC_SET sizeof(can_packet_bp_soc_set)

/** initial values for the bp_soc_set packet
 *  To be used like:
 *  @code
 *  CANPacket my_bp_soc_set_packet = CAN_INIT_BP_SOC_SET;
 *  @endcode **/
#define CAN_INIT_BP_SOC_SET {.id=CAN_PACKET_BP_SOC_SET, .length=CAN_LENGTH_BP_SOC_SET}

///@brief min and max cell voltages and temperatures
typedef struct can_packet_bp_minmax_struct{
        int16_t max_temp;
        int16_t min_temp;
        uint16_t max_cell;
        uint16_t min_cell;
} can_packet_bp_minmax;

/// the length of the bp_minmax packet
#define CAN_LENGTH_BP_MINMAX sizeof(can_packet_bp_minmax)

/** initial values for the bp_minmax packet
 *  To be used like:
 *  @code
 *  CANPacket my_bp_minmax_packet = CAN_INIT_BP_MINMAX;
 *  @endcode **/
#define CAN_INIT_BP_MINMAX {.id=CAN_PACKET_BP_MINMAX, .length=CAN_LENGTH_BP_MINMAX}

///@brief Information for car startup problems.
typedef struct can_packet_bp_startup_error_struct{
        uint16_t data1;
        uint16_t data2;
        uint8_t error_code;
} can_packet_bp_startup_error;

/// the length of the bp_startup_error packet
#define CAN_LENGTH_BP_STARTUP_ERROR sizeof(can_packet_bp_startup_error)

/** initial values for the bp_startup_error packet
 *  To be used like:
 *  @code
 *  CANPacket my_bp_startup_error_packet = CAN_INIT_BP_STARTUP_ERROR;
 *  @endcode **/
#define CAN_INIT_BP_STARTUP_ERROR {.id=CAN_PACKET_BP_STARTUP_ERROR, .length=CAN_LENGTH_BP_STARTUP_ERROR}

///@brief sets discharge parameters
typedef struct can_packet_bp_discharge_set_struct{
        uint16_t target_voltage;
        uint8_t discharge_rate;
        struct {
            uint8_t discharge_enable :1;

        };
} can_packet_bp_discharge_set;

/// the length of the bp_discharge_set packet
#define CAN_LENGTH_BP_DISCHARGE_SET sizeof(can_packet_bp_discharge_set)

/** initial values for the bp_discharge_set packet
 *  To be used like:
 *  @code
 *  CANPacket my_bp_discharge_set_packet = CAN_INIT_BP_DISCHARGE_SET;
 *  @endcode **/
#define CAN_INIT_BP_DISCHARGE_SET {.id=CAN_PACKET_BP_DISCHARGE_SET, .length=CAN_LENGTH_BP_DISCHARGE_SET}

///@brief enables BP debug
typedef struct can_packet_bp_debug_en_struct{
        uint32_t check1;
        uint32_t check2;
} can_packet_bp_debug_en;

/// the length of the bp_debug_en packet
#define CAN_LENGTH_BP_DEBUG_EN sizeof(can_packet_bp_debug_en)

/** initial values for the bp_debug_en packet
 *  To be used like:
 *  @code
 *  CANPacket my_bp_debug_en_packet = CAN_INIT_BP_DEBUG_EN;
 *  @endcode **/
#define CAN_INIT_BP_DEBUG_EN {.id=CAN_PACKET_BP_DEBUG_EN, .length=CAN_LENGTH_BP_DEBUG_EN}

///@brief sets BP debug params
typedef struct can_packet_bp_debug_param_set_struct{
        struct {
            uint8_t enable :1;

        };
        uint8_t sv;
        uint8_t st;
        uint32_t check;
} can_packet_bp_debug_param_set;

/// the length of the bp_debug_param_set packet
#define CAN_LENGTH_BP_DEBUG_PARAM_SET sizeof(can_packet_bp_debug_param_set)

/** initial values for the bp_debug_param_set packet
 *  To be used like:
 *  @code
 *  CANPacket my_bp_debug_param_set_packet = CAN_INIT_BP_DEBUG_PARAM_SET;
 *  @endcode **/
#define CAN_INIT_BP_DEBUG_PARAM_SET {.id=CAN_PACKET_BP_DEBUG_PARAM_SET, .length=CAN_LENGTH_BP_DEBUG_PARAM_SET}

///@brief Voltage and temperature for a single module
typedef struct can_packet_bp_module_struct{
        uint16_t voltage;
        uint8_t discharge;
        uint8_t discharge_commanded;
        int16_t temperature;
        uint8_t error_count;
} can_packet_bp_module;

/// the length of the bp_module packet
#define CAN_LENGTH_BP_MODULE sizeof(can_packet_bp_module)

/** initial values for the bp_module packet
 *  To be used like:
 *  @code
 *  CANPacket my_bp_module_packet = CAN_INIT_BP_MODULE;
 *  @endcode **/
#define CAN_INIT_BP_MODULE {.id=CAN_PACKET_BP_MODULE, .length=CAN_LENGTH_BP_MODULE}

///@brief Emergency packet to kill the car
typedef struct can_packet_kill_struct{
        uint8_t board_id;
        uint8_t error_code;
} can_packet_kill;

/// the length of the kill packet
#define CAN_LENGTH_KILL sizeof(can_packet_kill)

/** initial values for the kill packet
 *  To be used like:
 *  @code
 *  CANPacket my_kill_packet = CAN_INIT_KILL;
 *  @endcode **/
#define CAN_INIT_KILL {.id=CAN_PACKET_KILL, .length=CAN_LENGTH_KILL}

///@brief Heartbeat packet for the dashboard
typedef struct can_packet_dashboard_status_struct{
        uint16_t battery_voltage;
        int16_t battery_current;
        uint8_t battery_soc;
        struct {
            /// 0x0:battery_powered\n
            /// 0x1:bus_powered\n
            uint8_t power_source :1;

            uint8_t nexus_screen_on :1;

            uint8_t nexus_usb_connected :1;

        };
} can_packet_dashboard_status;

/// the length of the dashboard_status packet
#define CAN_LENGTH_DASHBOARD_STATUS sizeof(can_packet_dashboard_status)

/** initial values for the dashboard_status packet
 *  To be used like:
 *  @code
 *  CANPacket my_dashboard_status_packet = CAN_INIT_DASHBOARD_STATUS;
 *  @endcode **/
#define CAN_INIT_DASHBOARD_STATUS {.id=CAN_PACKET_DASHBOARD_STATUS, .length=CAN_LENGTH_DASHBOARD_STATUS}

///@brief displays a message on the driver display. Message is null-terminated.
typedef struct can_packet_dashboard_display_message_struct{
        uint8_t char7;
} can_packet_dashboard_display_message;

/// the length of the dashboard_display_message packet
#define CAN_LENGTH_DASHBOARD_DISPLAY_MESSAGE sizeof(can_packet_dashboard_display_message)

/** initial values for the dashboard_display_message packet
 *  To be used like:
 *  @code
 *  CANPacket my_dashboard_display_message_packet = CAN_INIT_DASHBOARD_DISPLAY_MESSAGE;
 *  @endcode **/
#define CAN_INIT_DASHBOARD_DISPLAY_MESSAGE {.id=CAN_PACKET_DASHBOARD_DISPLAY_MESSAGE, .length=CAN_LENGTH_DASHBOARD_DISPLAY_MESSAGE}

///@brief Sent by strategy to inform the driver. Negative values represent invalid (disable).
typedef struct can_packet_speed_zone_info_struct{
        float speed_limit;
        float target_speed;
} can_packet_speed_zone_info;

/// the length of the speed_zone_info packet
#define CAN_LENGTH_SPEED_ZONE_INFO sizeof(can_packet_speed_zone_info)

/** initial values for the speed_zone_info packet
 *  To be used like:
 *  @code
 *  CANPacket my_speed_zone_info_packet = CAN_INIT_SPEED_ZONE_INFO;
 *  @endcode **/
#define CAN_INIT_SPEED_ZONE_INFO {.id=CAN_PACKET_SPEED_ZONE_INFO, .length=CAN_LENGTH_SPEED_ZONE_INFO}

///@brief Sent by dashboard ONLY after driver conformation. Sets the cruise control target speed. Magic set to (0x501A4CA4) for safety.
typedef struct can_packet_cruise_control_speed_set_struct{
        float target_speed;
        uint32_t magic;
} can_packet_cruise_control_speed_set;

/// the length of the cruise_control_speed_set packet
#define CAN_LENGTH_CRUISE_CONTROL_SPEED_SET sizeof(can_packet_cruise_control_speed_set)

/** initial values for the cruise_control_speed_set packet
 *  To be used like:
 *  @code
 *  CANPacket my_cruise_control_speed_set_packet = CAN_INIT_CRUISE_CONTROL_SPEED_SET;
 *  @endcode **/
#define CAN_INIT_CRUISE_CONTROL_SPEED_SET {.id=CAN_PACKET_CRUISE_CONTROL_SPEED_SET, .length=CAN_LENGTH_CRUISE_CONTROL_SPEED_SET}

///@brief status of switches and pedals
typedef struct can_packet_driver_information_di_struct{
        struct {
            uint8_t hazards :1;

            uint8_t cruise :1;

            uint8_t boosh :1;

            uint8_t dash_lights :1;

            uint8_t display :1;

            uint8_t magic :1;

        };
        struct {
            uint8_t headlights_low :1;

            uint8_t headlights_high :1;

            uint8_t direction_fwd :1;

            uint8_t direction_rev :1;

            uint8_t mechanical_brakes :1;

            uint8_t any_brakes :1;

        };
        uint8_t accelerator;
        uint8_t brakes;
} can_packet_driver_information_di;

/// the length of the driver_information_di packet
#define CAN_LENGTH_DRIVER_INFORMATION_DI sizeof(can_packet_driver_information_di)

/** initial values for the driver_information_di packet
 *  To be used like:
 *  @code
 *  CANPacket my_driver_information_di_packet = CAN_INIT_DRIVER_INFORMATION_DI;
 *  @endcode **/
#define CAN_INIT_DRIVER_INFORMATION_DI {.id=CAN_PACKET_DRIVER_INFORMATION_DI, .length=CAN_LENGTH_DRIVER_INFORMATION_DI}

///@brief status of all di switches
typedef struct can_packet_c3_di_switches_struct{
        struct {
            uint8_t boosh :1;

            uint8_t magic :1;

            uint8_t override :1;

            uint8_t hazards :1;

            uint8_t brake :1;

            /// 0x0:reverse\n
            /// 0x1:forward\n
            uint8_t direction :1;

            /// 0x0:motor disabled\n
            /// 0x1:motor enabled\n
            uint8_t motor_enable :1;

        };
} can_packet_c3_di_switches;

/// the length of the c3_di_switches packet
#define CAN_LENGTH_C3_DI_SWITCHES sizeof(can_packet_c3_di_switches)

/** initial values for the c3_di_switches packet
 *  To be used like:
 *  @code
 *  CANPacket my_c3_di_switches_packet = CAN_INIT_C3_DI_SWITCHES;
 *  @endcode **/
#define CAN_INIT_C3_DI_SWITCHES {.id=CAN_PACKET_C3_DI_SWITCHES, .length=CAN_LENGTH_C3_DI_SWITCHES}

///@brief Drive command.  This must be sent at a minimum of 4 hz.
typedef struct can_packet_fullcurrent_drive_struct{
        uint8_t drivemode;
        float motor_current;
} can_packet_fullcurrent_drive;

/// the length of the fullcurrent_drive packet
#define CAN_LENGTH_FULLCURRENT_DRIVE sizeof(can_packet_fullcurrent_drive)

/** initial values for the fullcurrent_drive packet
 *  To be used like:
 *  @code
 *  CANPacket my_fullcurrent_drive_packet = CAN_INIT_FULLCURRENT_DRIVE;
 *  @endcode **/
#define CAN_INIT_FULLCURRENT_DRIVE {.id=CAN_PACKET_FULLCURRENT_DRIVE, .length=CAN_LENGTH_FULLCURRENT_DRIVE}

///@brief Sets the tire radius for speed calculations.
typedef struct can_packet_fullcurrent_tire_radius_struct{
        float radius;
} can_packet_fullcurrent_tire_radius;

/// the length of the fullcurrent_tire_radius packet
#define CAN_LENGTH_FULLCURRENT_TIRE_RADIUS sizeof(can_packet_fullcurrent_tire_radius)

/** initial values for the fullcurrent_tire_radius packet
 *  To be used like:
 *  @code
 *  CANPacket my_fullcurrent_tire_radius_packet = CAN_INIT_FULLCURRENT_TIRE_RADIUS;
 *  @endcode **/
#define CAN_INIT_FULLCURRENT_TIRE_RADIUS {.id=CAN_PACKET_FULLCURRENT_TIRE_RADIUS, .length=CAN_LENGTH_FULLCURRENT_TIRE_RADIUS}

///@brief D & Q axis currents
typedef struct can_packet_fullcurrent_iq_id_struct{
        float iq;
        float id;
} can_packet_fullcurrent_iq_id;

/// the length of the fullcurrent_iq_id packet
#define CAN_LENGTH_FULLCURRENT_IQ_ID sizeof(can_packet_fullcurrent_iq_id)

/** initial values for the fullcurrent_iq_id packet
 *  To be used like:
 *  @code
 *  CANPacket my_fullcurrent_iq_id_packet = CAN_INIT_FULLCURRENT_IQ_ID;
 *  @endcode **/
#define CAN_INIT_FULLCURRENT_IQ_ID {.id=CAN_PACKET_FULLCURRENT_IQ_ID, .length=CAN_LENGTH_FULLCURRENT_IQ_ID}

///@brief Position of the rotor & speed of the rotor (RPM)
typedef struct can_packet_fullcurrent_position_speed_struct{
        float position;
        float speed;
} can_packet_fullcurrent_position_speed;

/// the length of the fullcurrent_position_speed packet
#define CAN_LENGTH_FULLCURRENT_POSITION_SPEED sizeof(can_packet_fullcurrent_position_speed)

/** initial values for the fullcurrent_position_speed packet
 *  To be used like:
 *  @code
 *  CANPacket my_fullcurrent_position_speed_packet = CAN_INIT_FULLCURRENT_POSITION_SPEED;
 *  @endcode **/
#define CAN_INIT_FULLCURRENT_POSITION_SPEED {.id=CAN_PACKET_FULLCURRENT_POSITION_SPEED, .length=CAN_LENGTH_FULLCURRENT_POSITION_SPEED}

///@brief Errors and warnings present on the controller.
typedef struct can_packet_fullcurrent_eventstate_struct{
        uint32_t errors;
        uint32_t warnings;
} can_packet_fullcurrent_eventstate;

/// the length of the fullcurrent_eventstate packet
#define CAN_LENGTH_FULLCURRENT_EVENTSTATE sizeof(can_packet_fullcurrent_eventstate)

/** initial values for the fullcurrent_eventstate packet
 *  To be used like:
 *  @code
 *  CANPacket my_fullcurrent_eventstate_packet = CAN_INIT_FULLCURRENT_EVENTSTATE;
 *  @endcode **/
#define CAN_INIT_FULLCURRENT_EVENTSTATE {.id=CAN_PACKET_FULLCURRENT_EVENTSTATE, .length=CAN_LENGTH_FULLCURRENT_EVENTSTATE}

///@brief Gets the calibration state of the motor.
typedef struct can_packet_fullcurrent_calibration_struct{
        uint8_t calibration_enabled;
        uint8_t calibration_successful;
        uint8_t phases_a_and_b_flipped;
        uint8_t motor_sensor_reversed;
        float position_offset;
} can_packet_fullcurrent_calibration;

/// the length of the fullcurrent_calibration packet
#define CAN_LENGTH_FULLCURRENT_CALIBRATION sizeof(can_packet_fullcurrent_calibration)

/** initial values for the fullcurrent_calibration packet
 *  To be used like:
 *  @code
 *  CANPacket my_fullcurrent_calibration_packet = CAN_INIT_FULLCURRENT_CALIBRATION;
 *  @endcode **/
#define CAN_INIT_FULLCURRENT_CALIBRATION {.id=CAN_PACKET_FULLCURRENT_CALIBRATION, .length=CAN_LENGTH_FULLCURRENT_CALIBRATION}

///@brief Instructs the motor to calibrate
typedef struct can_packet_fullcurrent_calibrate_struct{
        uint8_t penis_size;
} can_packet_fullcurrent_calibrate;

/// the length of the fullcurrent_calibrate packet
#define CAN_LENGTH_FULLCURRENT_CALIBRATE sizeof(can_packet_fullcurrent_calibrate)

/** initial values for the fullcurrent_calibrate packet
 *  To be used like:
 *  @code
 *  CANPacket my_fullcurrent_calibrate_packet = CAN_INIT_FULLCURRENT_CALIBRATE;
 *  @endcode **/
#define CAN_INIT_FULLCURRENT_CALIBRATE {.id=CAN_PACKET_FULLCURRENT_CALIBRATE, .length=CAN_LENGTH_FULLCURRENT_CALIBRATE}

///@brief Sets the scale of the current command.
typedef struct can_packet_fullcurrent_currentLimit_struct{
        float currentLimit;
} can_packet_fullcurrent_currentLimit;

/// the length of the fullcurrent_currentLimit packet
#define CAN_LENGTH_FULLCURRENT_CURRENTLIMIT sizeof(can_packet_fullcurrent_currentLimit)

/** initial values for the fullcurrent_currentLimit packet
 *  To be used like:
 *  @code
 *  CANPacket my_fullcurrent_currentLimit_packet = CAN_INIT_FULLCURRENT_CURRENTLIMIT;
 *  @endcode **/
#define CAN_INIT_FULLCURRENT_CURRENTLIMIT {.id=CAN_PACKET_FULLCURRENT_CURRENTLIMIT, .length=CAN_LENGTH_FULLCURRENT_CURRENTLIMIT}

///@brief actual on/off state of front lights
typedef struct can_packet_lights_state_front_struct{
        struct {
            uint8_t turn_left :1;

            uint8_t turn_right :1;

            uint8_t headlight_left :1;

            uint8_t headlight_right :1;

        };
} can_packet_lights_state_front;

/// the length of the lights_state_front packet
#define CAN_LENGTH_LIGHTS_STATE_FRONT sizeof(can_packet_lights_state_front)

/** initial values for the lights_state_front packet
 *  To be used like:
 *  @code
 *  CANPacket my_lights_state_front_packet = CAN_INIT_LIGHTS_STATE_FRONT;
 *  @endcode **/
#define CAN_INIT_LIGHTS_STATE_FRONT {.id=CAN_PACKET_LIGHTS_STATE_FRONT, .length=CAN_LENGTH_LIGHTS_STATE_FRONT}

///@brief actual on/off state of rear lights
typedef struct can_packet_lights_state_rear_struct{
        struct {
            uint8_t turn_left :1;

            uint8_t turn_right :1;

            uint8_t brake_left :1;

            uint8_t brake_right :1;

        };
} can_packet_lights_state_rear;

/// the length of the lights_state_rear packet
#define CAN_LENGTH_LIGHTS_STATE_REAR sizeof(can_packet_lights_state_rear)

/** initial values for the lights_state_rear packet
 *  To be used like:
 *  @code
 *  CANPacket my_lights_state_rear_packet = CAN_INIT_LIGHTS_STATE_REAR;
 *  @endcode **/
#define CAN_INIT_LIGHTS_STATE_REAR {.id=CAN_PACKET_LIGHTS_STATE_REAR, .length=CAN_LENGTH_LIGHTS_STATE_REAR}

///@brief current that all of the lights draw
typedef struct can_packet_lights_current_struct{
        uint8_t channel_1;
        uint8_t channel_2;
        uint8_t channel_3;
        uint8_t channel_4;
        uint8_t channel_5;
        uint8_t channel_6;
} can_packet_lights_current;

/// the length of the lights_current packet
#define CAN_LENGTH_LIGHTS_CURRENT sizeof(can_packet_lights_current)

/** initial values for the lights_current packet
 *  To be used like:
 *  @code
 *  CANPacket my_lights_current_packet = CAN_INIT_LIGHTS_CURRENT;
 *  @endcode **/
#define CAN_INIT_LIGHTS_CURRENT {.id=CAN_PACKET_LIGHTS_CURRENT, .length=CAN_LENGTH_LIGHTS_CURRENT}

///@brief status of everything on the wheel
typedef struct can_packet_driver_information_wheel_struct{
        struct {
            uint8_t horn :1;

            uint8_t cruise_set :1;

            uint8_t cruise_release :1;

            uint8_t turn_left :1;

            uint8_t turn_right :1;

        };
        uint8_t accelerator;
        uint8_t regen;
} can_packet_driver_information_wheel;

/// the length of the driver_information_wheel packet
#define CAN_LENGTH_DRIVER_INFORMATION_WHEEL sizeof(can_packet_driver_information_wheel)

/** initial values for the driver_information_wheel packet
 *  To be used like:
 *  @code
 *  CANPacket my_driver_information_wheel_packet = CAN_INIT_DRIVER_INFORMATION_WHEEL;
 *  @endcode **/
#define CAN_INIT_DRIVER_INFORMATION_WHEEL {.id=CAN_PACKET_DRIVER_INFORMATION_WHEEL, .length=CAN_LENGTH_DRIVER_INFORMATION_WHEEL}

///@brief Turn signals from the steering wheel
typedef struct can_packet_c3_lights_blinkers_struct{
        struct {
            uint8_t left_blinker :1;

            uint8_t right_blinker :1;

        };
} can_packet_c3_lights_blinkers;

/// the length of the c3_lights_blinkers packet
#define CAN_LENGTH_C3_LIGHTS_BLINKERS sizeof(can_packet_c3_lights_blinkers)

/** initial values for the c3_lights_blinkers packet
 *  To be used like:
 *  @code
 *  CANPacket my_c3_lights_blinkers_packet = CAN_INIT_C3_LIGHTS_BLINKERS;
 *  @endcode **/
#define CAN_INIT_C3_LIGHTS_BLINKERS {.id=CAN_PACKET_C3_LIGHTS_BLINKERS, .length=CAN_LENGTH_C3_LIGHTS_BLINKERS}

///@brief GPS location of the car
typedef struct can_packet_telemetry_latlon_struct{
        float lat;
        float lon;
} can_packet_telemetry_latlon;

/// the length of the telemetry_latlon packet
#define CAN_LENGTH_TELEMETRY_LATLON sizeof(can_packet_telemetry_latlon)

/** initial values for the telemetry_latlon packet
 *  To be used like:
 *  @code
 *  CANPacket my_telemetry_latlon_packet = CAN_INIT_TELEMETRY_LATLON;
 *  @endcode **/
#define CAN_INIT_TELEMETRY_LATLON {.id=CAN_PACKET_TELEMETRY_LATLON, .length=CAN_LENGTH_TELEMETRY_LATLON}

///@brief GPS velocity vector of the car
typedef struct can_packet_telemetry_heading_struct{
        float heading;
        float velocity;
} can_packet_telemetry_heading;

/// the length of the telemetry_heading packet
#define CAN_LENGTH_TELEMETRY_HEADING sizeof(can_packet_telemetry_heading)

/** initial values for the telemetry_heading packet
 *  To be used like:
 *  @code
 *  CANPacket my_telemetry_heading_packet = CAN_INIT_TELEMETRY_HEADING;
 *  @endcode **/
#define CAN_INIT_TELEMETRY_HEADING {.id=CAN_PACKET_TELEMETRY_HEADING, .length=CAN_LENGTH_TELEMETRY_HEADING}

///@brief Tracker data (on RTR only)
typedef struct can_packet_tracker_data_struct{
        uint16_t array_voltage;
        uint16_t array_current;
        uint16_t battery_voltage;
        uint16_t temperature;
} can_packet_tracker_data;

/// the length of the tracker_data packet
#define CAN_LENGTH_TRACKER_DATA sizeof(can_packet_tracker_data)

/** initial values for the tracker_data packet
 *  To be used like:
 *  @code
 *  CANPacket my_tracker_data_packet = CAN_INIT_TRACKER_DATA;
 *  @endcode **/
#define CAN_INIT_TRACKER_DATA {.id=CAN_PACKET_TRACKER_DATA, .length=CAN_LENGTH_TRACKER_DATA}

///@brief enable/disable trackers
typedef struct can_packet_tracker_enable_struct{
        uint8_t enable;
} can_packet_tracker_enable;

/// the length of the tracker_enable packet
#define CAN_LENGTH_TRACKER_ENABLE sizeof(can_packet_tracker_enable)

/** initial values for the tracker_enable packet
 *  To be used like:
 *  @code
 *  CANPacket my_tracker_enable_packet = CAN_INIT_TRACKER_ENABLE;
 *  @endcode **/
#define CAN_INIT_TRACKER_ENABLE {.id=CAN_PACKET_TRACKER_ENABLE, .length=CAN_LENGTH_TRACKER_ENABLE}

///@brief Tritium Motor Drive Command
typedef struct can_packet_tritium_motor_drive_struct{
        float motor_velocity;
        float motor_current;
} can_packet_tritium_motor_drive;

/// the length of the tritium_motor_drive packet
#define CAN_LENGTH_TRITIUM_MOTOR_DRIVE sizeof(can_packet_tritium_motor_drive)

/** initial values for the tritium_motor_drive packet
 *  To be used like:
 *  @code
 *  CANPacket my_tritium_motor_drive_packet = CAN_INIT_TRITIUM_MOTOR_DRIVE;
 *  @endcode **/
#define CAN_INIT_TRITIUM_MOTOR_DRIVE {.id=CAN_PACKET_TRITIUM_MOTOR_DRIVE, .length=CAN_LENGTH_TRITIUM_MOTOR_DRIVE}

///@brief Tritium Motor Power Command
typedef struct can_packet_tritium_motor_power_struct{
        float reserved;
        float bus_current;
} can_packet_tritium_motor_power;

/// the length of the tritium_motor_power packet
#define CAN_LENGTH_TRITIUM_MOTOR_POWER sizeof(can_packet_tritium_motor_power)

/** initial values for the tritium_motor_power packet
 *  To be used like:
 *  @code
 *  CANPacket my_tritium_motor_power_packet = CAN_INIT_TRITIUM_MOTOR_POWER;
 *  @endcode **/
#define CAN_INIT_TRITIUM_MOTOR_POWER {.id=CAN_PACKET_TRITIUM_MOTOR_POWER, .length=CAN_LENGTH_TRITIUM_MOTOR_POWER}

///@brief Tritium Reset Command
typedef struct can_packet_tritium_reset_struct{
        float unused1;
        float unused2;
} can_packet_tritium_reset;

/// the length of the tritium_reset packet
#define CAN_LENGTH_TRITIUM_RESET sizeof(can_packet_tritium_reset)

/** initial values for the tritium_reset packet
 *  To be used like:
 *  @code
 *  CANPacket my_tritium_reset_packet = CAN_INIT_TRITIUM_RESET;
 *  @endcode **/
#define CAN_INIT_TRITIUM_RESET {.id=CAN_PACKET_TRITIUM_RESET, .length=CAN_LENGTH_TRITIUM_RESET}

///@brief Tritium Identification Information
typedef struct can_packet_tritium_identification_struct{
        uint32_t tritium_id;
        uint32_t serial_number;
} can_packet_tritium_identification;

/// the length of the tritium_identification packet
#define CAN_LENGTH_TRITIUM_IDENTIFICATION sizeof(can_packet_tritium_identification)

/** initial values for the tritium_identification packet
 *  To be used like:
 *  @code
 *  CANPacket my_tritium_identification_packet = CAN_INIT_TRITIUM_IDENTIFICATION;
 *  @endcode **/
#define CAN_INIT_TRITIUM_IDENTIFICATION {.id=CAN_PACKET_TRITIUM_IDENTIFICATION, .length=CAN_LENGTH_TRITIUM_IDENTIFICATION}

///@brief Tritium Status Information
typedef struct can_packet_tritium_status_struct{
        uint16_t limit_flags;
        uint16_t error_flags;
        uint16_t active_motor;
        uint16_t reserved;
} can_packet_tritium_status;

/// the length of the tritium_status packet
#define CAN_LENGTH_TRITIUM_STATUS sizeof(can_packet_tritium_status)

/** initial values for the tritium_status packet
 *  To be used like:
 *  @code
 *  CANPacket my_tritium_status_packet = CAN_INIT_TRITIUM_STATUS;
 *  @endcode **/
#define CAN_INIT_TRITIUM_STATUS {.id=CAN_PACKET_TRITIUM_STATUS, .length=CAN_LENGTH_TRITIUM_STATUS}

///@brief Tritium Bus Information
typedef struct can_packet_tritium_bus_struct{
        float bus_voltage;
        float bus_current;
} can_packet_tritium_bus;

/// the length of the tritium_bus packet
#define CAN_LENGTH_TRITIUM_BUS sizeof(can_packet_tritium_bus)

/** initial values for the tritium_bus packet
 *  To be used like:
 *  @code
 *  CANPacket my_tritium_bus_packet = CAN_INIT_TRITIUM_BUS;
 *  @endcode **/
#define CAN_INIT_TRITIUM_BUS {.id=CAN_PACKET_TRITIUM_BUS, .length=CAN_LENGTH_TRITIUM_BUS}

///@brief Tritium Velocity Measurement
typedef struct can_packet_tritium_velocity_struct{
        float motor_velocity;
        float vehicle_velocity;
} can_packet_tritium_velocity;

/// the length of the tritium_velocity packet
#define CAN_LENGTH_TRITIUM_VELOCITY sizeof(can_packet_tritium_velocity)

/** initial values for the tritium_velocity packet
 *  To be used like:
 *  @code
 *  CANPacket my_tritium_velocity_packet = CAN_INIT_TRITIUM_VELOCITY;
 *  @endcode **/
#define CAN_INIT_TRITIUM_VELOCITY {.id=CAN_PACKET_TRITIUM_VELOCITY, .length=CAN_LENGTH_TRITIUM_VELOCITY}

///@brief Tritium Phase Current Measurement
typedef struct can_packet_tritium_phase_current_struct{
        float phase_b_current;
        float phase_a_current;
} can_packet_tritium_phase_current;

/// the length of the tritium_phase_current packet
#define CAN_LENGTH_TRITIUM_PHASE_CURRENT sizeof(can_packet_tritium_phase_current)

/** initial values for the tritium_phase_current packet
 *  To be used like:
 *  @code
 *  CANPacket my_tritium_phase_current_packet = CAN_INIT_TRITIUM_PHASE_CURRENT;
 *  @endcode **/
#define CAN_INIT_TRITIUM_PHASE_CURRENT {.id=CAN_PACKET_TRITIUM_PHASE_CURRENT, .length=CAN_LENGTH_TRITIUM_PHASE_CURRENT}

///@brief Tritium Motor Voltage Vector Measurement
typedef struct can_packet_tritium_motor_voltage_vector_struct{
        float vq;
        float vd;
} can_packet_tritium_motor_voltage_vector;

/// the length of the tritium_motor_voltage_vector packet
#define CAN_LENGTH_TRITIUM_MOTOR_VOLTAGE_VECTOR sizeof(can_packet_tritium_motor_voltage_vector)

/** initial values for the tritium_motor_voltage_vector packet
 *  To be used like:
 *  @code
 *  CANPacket my_tritium_motor_voltage_vector_packet = CAN_INIT_TRITIUM_MOTOR_VOLTAGE_VECTOR;
 *  @endcode **/
#define CAN_INIT_TRITIUM_MOTOR_VOLTAGE_VECTOR {.id=CAN_PACKET_TRITIUM_MOTOR_VOLTAGE_VECTOR, .length=CAN_LENGTH_TRITIUM_MOTOR_VOLTAGE_VECTOR}

///@brief Tritium Motor Current Vector Measurement
typedef struct can_packet_tritium_motor_current_vector_struct{
        float iq;
        float id;
} can_packet_tritium_motor_current_vector;

/// the length of the tritium_motor_current_vector packet
#define CAN_LENGTH_TRITIUM_MOTOR_CURRENT_VECTOR sizeof(can_packet_tritium_motor_current_vector)

/** initial values for the tritium_motor_current_vector packet
 *  To be used like:
 *  @code
 *  CANPacket my_tritium_motor_current_vector_packet = CAN_INIT_TRITIUM_MOTOR_CURRENT_VECTOR;
 *  @endcode **/
#define CAN_INIT_TRITIUM_MOTOR_CURRENT_VECTOR {.id=CAN_PACKET_TRITIUM_MOTOR_CURRENT_VECTOR, .length=CAN_LENGTH_TRITIUM_MOTOR_CURRENT_VECTOR}

///@brief Tritium Motor BackEMF Measurement / Prediction
typedef struct can_packet_tritium_motor_backemf_struct{
        float bemfq;
        float bemfd;
} can_packet_tritium_motor_backemf;

/// the length of the tritium_motor_backemf packet
#define CAN_LENGTH_TRITIUM_MOTOR_BACKEMF sizeof(can_packet_tritium_motor_backemf)

/** initial values for the tritium_motor_backemf packet
 *  To be used like:
 *  @code
 *  CANPacket my_tritium_motor_backemf_packet = CAN_INIT_TRITIUM_MOTOR_BACKEMF;
 *  @endcode **/
#define CAN_INIT_TRITIUM_MOTOR_BACKEMF {.id=CAN_PACKET_TRITIUM_MOTOR_BACKEMF, .length=CAN_LENGTH_TRITIUM_MOTOR_BACKEMF}

///@brief Tritium 15 and 1.65 Voltage Rail Measurement
typedef struct can_packet_tritium_15_165_voltage_rail_struct{
        float reference_165v;
        float supply_15v;
} can_packet_tritium_15_165_voltage_rail;

/// the length of the tritium_15_165_voltage_rail packet
#define CAN_LENGTH_TRITIUM_15_165_VOLTAGE_RAIL sizeof(can_packet_tritium_15_165_voltage_rail)

/** initial values for the tritium_15_165_voltage_rail packet
 *  To be used like:
 *  @code
 *  CANPacket my_tritium_15_165_voltage_rail_packet = CAN_INIT_TRITIUM_15_165_VOLTAGE_RAIL;
 *  @endcode **/
#define CAN_INIT_TRITIUM_15_165_VOLTAGE_RAIL {.id=CAN_PACKET_TRITIUM_15_165_VOLTAGE_RAIL, .length=CAN_LENGTH_TRITIUM_15_165_VOLTAGE_RAIL}

///@brief Tritium 2.5V and 1.2V Voltage Rail Measurement
typedef struct can_packet_tritium_25_12_voltage_rail_struct{
        float supply_12v;
        float supply_25v;
} can_packet_tritium_25_12_voltage_rail;

/// the length of the tritium_25_12_voltage_rail packet
#define CAN_LENGTH_TRITIUM_25_12_VOLTAGE_RAIL sizeof(can_packet_tritium_25_12_voltage_rail)

/** initial values for the tritium_25_12_voltage_rail packet
 *  To be used like:
 *  @code
 *  CANPacket my_tritium_25_12_voltage_rail_packet = CAN_INIT_TRITIUM_25_12_VOLTAGE_RAIL;
 *  @endcode **/
#define CAN_INIT_TRITIUM_25_12_VOLTAGE_RAIL {.id=CAN_PACKET_TRITIUM_25_12_VOLTAGE_RAIL, .length=CAN_LENGTH_TRITIUM_25_12_VOLTAGE_RAIL}

/// @brief the structure for a CAN packet
typedef struct CANPacket_struct{

    /// @brief number of bytes in the data of this packet
    uint8_t length;

    bool rtr;

    /// @brief the id for this packet
    CANPacketId id;
    union{
        ///@brief the data without any structure
        uint8_t data[8];

        ///@brief Low frequency BP status update.
        can_packet_bp_state bp_state;

        ///@brief State of charge and pack voltage
        can_packet_bp_soc bp_soc;

        ///@brief current draw from battery
        can_packet_bp_current bp_current;

        ///@brief write state of charge, use with caution
        can_packet_bp_soc_set bp_soc_set;

        ///@brief min and max cell voltages and temperatures
        can_packet_bp_minmax bp_minmax;

        ///@brief Information for car startup problems.
        can_packet_bp_startup_error bp_startup_error;

        ///@brief sets discharge parameters
        can_packet_bp_discharge_set bp_discharge_set;

        ///@brief enables BP debug
        can_packet_bp_debug_en bp_debug_en;

        ///@brief sets BP debug params
        can_packet_bp_debug_param_set bp_debug_param_set;

        ///@brief Voltage and temperature for a single module
        can_packet_bp_module bp_module;

        ///@brief Emergency packet to kill the car
        can_packet_kill kill;

        ///@brief Heartbeat packet for the dashboard
        can_packet_dashboard_status dashboard_status;

        ///@brief displays a message on the driver display. Message is null-terminated.
        can_packet_dashboard_display_message dashboard_display_message;

        ///@brief Sent by strategy to inform the driver. Negative values represent invalid (disable).
        can_packet_speed_zone_info speed_zone_info;

        ///@brief Sent by dashboard ONLY after driver conformation. Sets the cruise control target speed. Magic set to (0x501A4CA4) for safety.
        can_packet_cruise_control_speed_set cruise_control_speed_set;

        ///@brief status of switches and pedals
        can_packet_driver_information_di driver_information_di;

        ///@brief status of all di switches
        can_packet_c3_di_switches c3_di_switches;

        ///@brief Drive command.  This must be sent at a minimum of 4 hz.
        can_packet_fullcurrent_drive fullcurrent_drive;

        ///@brief Sets the tire radius for speed calculations.
        can_packet_fullcurrent_tire_radius fullcurrent_tire_radius;

        ///@brief D & Q axis currents
        can_packet_fullcurrent_iq_id fullcurrent_iq_id;

        ///@brief Position of the rotor & speed of the rotor (RPM)
        can_packet_fullcurrent_position_speed fullcurrent_position_speed;

        ///@brief Errors and warnings present on the controller.
        can_packet_fullcurrent_eventstate fullcurrent_eventstate;

        ///@brief Gets the calibration state of the motor.
        can_packet_fullcurrent_calibration fullcurrent_calibration;

        ///@brief Instructs the motor to calibrate
        can_packet_fullcurrent_calibrate fullcurrent_calibrate;

        ///@brief Sets the scale of the current command.
        can_packet_fullcurrent_currentLimit fullcurrent_currentLimit;

        ///@brief actual on/off state of front lights
        can_packet_lights_state_front lights_state_front;

        ///@brief actual on/off state of rear lights
        can_packet_lights_state_rear lights_state_rear;

        ///@brief current that all of the lights draw
        can_packet_lights_current lights_current;

        ///@brief status of everything on the wheel
        can_packet_driver_information_wheel driver_information_wheel;

        ///@brief Turn signals from the steering wheel
        can_packet_c3_lights_blinkers c3_lights_blinkers;

        ///@brief GPS location of the car
        can_packet_telemetry_latlon telemetry_latlon;

        ///@brief GPS velocity vector of the car
        can_packet_telemetry_heading telemetry_heading;

        ///@brief Tracker data (on RTR only)
        can_packet_tracker_data tracker_data;

        ///@brief enable/disable trackers
        can_packet_tracker_enable tracker_enable;

        ///@brief Tritium Motor Drive Command
        can_packet_tritium_motor_drive tritium_motor_drive;

        ///@brief Tritium Motor Power Command
        can_packet_tritium_motor_power tritium_motor_power;

        ///@brief Tritium Reset Command
        can_packet_tritium_reset tritium_reset;

        ///@brief Tritium Identification Information
        can_packet_tritium_identification tritium_identification;

        ///@brief Tritium Status Information
        can_packet_tritium_status tritium_status;

        ///@brief Tritium Bus Information
        can_packet_tritium_bus tritium_bus;

        ///@brief Tritium Velocity Measurement
        can_packet_tritium_velocity tritium_velocity;

        ///@brief Tritium Phase Current Measurement
        can_packet_tritium_phase_current tritium_phase_current;

        ///@brief Tritium Motor Voltage Vector Measurement
        can_packet_tritium_motor_voltage_vector tritium_motor_voltage_vector;

        ///@brief Tritium Motor Current Vector Measurement
        can_packet_tritium_motor_current_vector tritium_motor_current_vector;

        ///@brief Tritium Motor BackEMF Measurement / Prediction
        can_packet_tritium_motor_backemf tritium_motor_backemf;

        ///@brief Tritium 15 and 1.65 Voltage Rail Measurement
        can_packet_tritium_15_165_voltage_rail tritium_15_165_voltage_rail;

        ///@brief Tritium 2.5V and 1.2V Voltage Rail Measurement
        can_packet_tritium_25_12_voltage_rail tritium_25_12_voltage_rail;

    };
} CANPacket;

void orbit_bp_state(uint16_t primary_voltage, uint8_t status_contactor_mid, uint8_t status_contactor_bus, uint8_t status_contactor_bus_pre, uint8_t status_contactor_tracker, uint8_t status_power_source, float battery_voltage);
void orbit_bp_soc(float soc, uint16_t voltage);
void orbit_bp_current(int32_t main, int32_t tracker);
void orbit_bp_soc_set(float soc);
void orbit_bp_minmax(int16_t max_temp, int16_t min_temp, uint16_t max_cell, uint16_t min_cell);
void orbit_bp_startup_error(uint16_t data1, uint16_t data2, uint8_t error_code);
void orbit_bp_discharge_set(uint16_t target_voltage, uint8_t discharge_rate, uint8_t flags_discharge_enable);
void orbit_bp_debug_en(uint32_t check1, uint32_t check2);
void orbit_bp_debug_param_set(uint8_t f_enable, uint8_t sv, uint8_t st, uint32_t check);
void orbit_bp_module(int _n, uint16_t voltage, uint8_t discharge, uint8_t discharge_commanded, int16_t temperature, uint8_t error_count);
void orbit_kill(uint8_t board_id, uint8_t error_code);
void orbit_dashboard_status(uint16_t battery_voltage, int16_t battery_current, uint8_t battery_soc, uint8_t status_power_source, uint8_t status_nexus_screen_on, uint8_t status_nexus_usb_connected);
void orbit_dashboard_display_message(int _n, uint8_t char7);
void orbit_speed_zone_info(float speed_limit, float target_speed);
void orbit_cruise_control_speed_set(float target_speed, uint32_t magic);
void orbit_driver_information_di(uint8_t switches_hazards, uint8_t switches_cruise, uint8_t switches_boosh, uint8_t switches_dash_lights, uint8_t switches_display, uint8_t switches_magic, uint8_t bits1_headlights_low, uint8_t bits1_headlights_high, uint8_t bits1_direction_fwd, uint8_t bits1_direction_rev, uint8_t bits1_mechanical_brakes, uint8_t bits1_any_brakes, uint8_t accelerator, uint8_t brakes);
void orbit_c3_di_switches(uint8_t switches_boosh, uint8_t switches_magic, uint8_t switches_override, uint8_t switches_hazards, uint8_t switches_brake, uint8_t switches_direction, uint8_t switches_motor_enable);
void orbit_fullcurrent_drive(uint8_t drivemode, float motor_current);
void orbit_fullcurrent_tire_radius(float radius);
void orbit_fullcurrent_iq_id(int _n, float iq, float id);
void orbit_fullcurrent_position_speed(int _n, float position, float speed);
void orbit_fullcurrent_eventstate(int _n, uint32_t errors, uint32_t warnings);
void orbit_fullcurrent_calibration(int _n, uint8_t calibration_enabled, uint8_t calibration_successful, uint8_t phases_a_and_b_flipped, uint8_t motor_sensor_reversed, float position_offset);
void orbit_fullcurrent_calibrate(int _n, uint8_t penis_size);
void orbit_fullcurrent_currentLimit(float currentLimit);
void orbit_lights_state_front(uint8_t light_turn_left, uint8_t light_turn_right, uint8_t light_headlight_left, uint8_t light_headlight_right);
void orbit_lights_state_rear(uint8_t light_turn_left, uint8_t light_turn_right, uint8_t light_brake_left, uint8_t light_brake_right);
void orbit_lights_current(uint8_t channel_1, uint8_t channel_2, uint8_t channel_3, uint8_t channel_4, uint8_t channel_5, uint8_t channel_6);
void orbit_driver_information_wheel(uint8_t wheel_switches_horn, uint8_t wheel_switches_cruise_set, uint8_t wheel_switches_cruise_release, uint8_t wheel_switches_turn_left, uint8_t wheel_switches_turn_right, uint8_t accelerator, uint8_t regen);
void orbit_c3_lights_blinkers(uint8_t flags_left_blinker, uint8_t flags_right_blinker);
void orbit_telemetry_latlon(float lat, float lon);
void orbit_telemetry_heading(float heading, float velocity);
void orbit_tracker_data(int _n, uint16_t array_voltage, uint16_t array_current, uint16_t battery_voltage, uint16_t temperature);
void orbit_tracker_enable(int _n, uint8_t enable);
void orbit_tritium_motor_drive(float motor_velocity, float motor_current);
void orbit_tritium_motor_power(float reserved, float bus_current);
void orbit_tritium_reset(float unused1, float unused2);
void orbit_tritium_identification(uint32_t tritium_id, uint32_t serial_number);
void orbit_tritium_status(uint16_t limit_flags, uint16_t error_flags, uint16_t active_motor, uint16_t reserved);
void orbit_tritium_bus(float bus_voltage, float bus_current);
void orbit_tritium_velocity(float motor_velocity, float vehicle_velocity);
void orbit_tritium_phase_current(float phase_b_current, float phase_a_current);
void orbit_tritium_motor_voltage_vector(float vq, float vd);
void orbit_tritium_motor_current_vector(float iq, float id);
void orbit_tritium_motor_backemf(float bemfq, float bemfd);
void orbit_tritium_15_165_voltage_rail(float reference_165v, float supply_15v);
void orbit_tritium_25_12_voltage_rail(float supply_12v, float supply_25v);
#endif //GENERATED_CAN_HEADER


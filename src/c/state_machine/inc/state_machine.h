/** \file state_machine.h
 *  \brief State machine example header file.
 *
 *  This is the main source file of a fictitious firmware implementation as
 *  a state machine (automaton).
 *  \author Fernando C. Guimaraes
 *  \date 2020-12-11
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <stdint.h>

#define STATE_FUNCTIONS_ARGS      struct output_data *ex_data, struct input_data in_data
#define IN_BUFFER_SIZE            8
#define OUT_BUFFER_SIZE           4

struct output_data
{
    int32_t speed_motor;
    uint8_t light_motor_on;
    uint8_t light_motor_cw;
    uint8_t light_motor_ccw;
    uint8_t light_buffer_empty;
};

struct input_data
{
    uint8_t btn_off;
    uint8_t btn_on;
    uint8_t btn_emergency_stop;
    uint8_t sensor_level;
};

typedef enum
{
   ST_NULL = 0,
   ST_MOTOR_OFF_BUFFER_EMPTY,
   ST_MOTOR_CCW_BUFFER_EMPTY,
   ST_MOTOR_CW_BUFFER_EMPTY,
   ST_MOTOR_OFF_BUFFER_OK,
   ST_MOTOR_CCW_BUFFER_OK,
   ST_MOTOR_CW_BUFFER_OK,
   NUMBER_ST
} state_id;

typedef enum
{
   EV_NONE = 0,
   EV_BTN_OFF,
   EV_BTN_ON,
   EV_BTN_PE,
   EV_EMPTY_BUFFER,
   EV_SWITCH_MOTOR_CCW,
   EV_SWITCH_MOTOR_CW,
   NUMBER_EV
} event_id;

typedef struct
{
   state_id st_id;
   int (*state_exe)(STATE_FUNCTIONS_ARGS);
} state_handler;

// Public function used to tell the SM what the new incoming event is. It can be called by an interrupt
// function or a polling one responsible for inputs reading. External interrupts are triggered by
// rise or falling edges of GPIOs and other peripherals. The polling method only reads the GPIOs states
// at the reading moment.
int sm_set_event(event_id event);

// Public function used to setup whatever the SM needs (peripherals registers, memory allocation, etc.)
void sm_setup(void);

// Public function which executes the current state function.
int sm_run(void);

// Public function used to turn things off, to deallocate memory, etc.
void sm_close(void);


#endif /* STATE_MACHINE_H_ */

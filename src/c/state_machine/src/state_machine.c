/** \file state_machine.c
 *  \brief State machine example source file.
 *
 *  This is the main source file of a fictitious firmware implementation as
 *  a state machine (automaton).
 *  \author Fernando C. Guimaraes
 *  \date 2020-12-11
 */
#include <dev_helper.h>
#include <state_machine.h>

/********************* Declaration of function headers ****************************/

// State Actions
static int action_turn_system_off(STATE_FUNCTIONS_ARGS);
static int action_motor_off_buffer_empty(STATE_FUNCTIONS_ARGS);
static int action_motor_ccw_buffer_empty(STATE_FUNCTIONS_ARGS);
static int action_motor_cw_buffer_empty(STATE_FUNCTIONS_ARGS);
static int action_motor_off_buffer_ok(STATE_FUNCTIONS_ARGS);
static int action_motor_ccw_buffer_ok(STATE_FUNCTIONS_ARGS);
static int action_motor_cw_buffer_ok(STATE_FUNCTIONS_ARGS);

// Emulate some real system behavior
static int simulate_system(STATE_FUNCTIONS_ARGS);

// Declaration of a private 'get' function used to return the last event.
static event_id sm_private_get_event(void);

// Declaration of a private 'set' function used by internal states to allow
// external events recognition.
static void sm_private_set_event(event_id event);

/********************* Global variables *******************************************/

static event_id _event = EV_NONE; // current event
static state_id _state = ST_MOTOR_OFF_BUFFER_EMPTY; // initial state (it could be declared inside sm_run)

// Function pointers array
// ----> Change this array accordingly to your automaton model
const static state_handler state_handler_container[NUMBER_ST] =
{
   [ST_NULL] = {ST_NULL, action_motor_off_buffer_ok},
   [ST_MOTOR_OFF_BUFFER_EMPTY] = {ST_MOTOR_OFF_BUFFER_EMPTY, action_motor_off_buffer_empty},
   [ST_MOTOR_CCW_BUFFER_EMPTY] = {ST_MOTOR_CCW_BUFFER_EMPTY, action_motor_ccw_buffer_empty},
   [ST_MOTOR_CW_BUFFER_EMPTY] = {ST_MOTOR_CW_BUFFER_EMPTY, action_motor_cw_buffer_empty},
   [ST_MOTOR_OFF_BUFFER_OK] = {ST_MOTOR_OFF_BUFFER_OK, action_motor_off_buffer_ok},
   [ST_MOTOR_CCW_BUFFER_OK] = {ST_MOTOR_CCW_BUFFER_OK, action_motor_ccw_buffer_ok},
   [ST_MOTOR_CW_BUFFER_OK] = {ST_MOTOR_CW_BUFFER_OK, action_motor_cw_buffer_ok}
};


/********************* Implementation of functions *************************************/

/********************* State functions *************************************************/

// ----> Implement your own state functions

static int action_turn_system_off(STATE_FUNCTIONS_ARGS)
{
   ex_data->speed_motor = 0;
   ex_data->light_motor_on = 0;
   ex_data->light_motor_cw = 0;
   ex_data->light_motor_ccw = 0;
   ex_data->light_buffer_empty = 0;

   return 0;
}

static int action_motor_off_buffer_empty(STATE_FUNCTIONS_ARGS)
{
   ex_data->speed_motor = 0;
   ex_data->light_motor_on = 0;
   ex_data->light_motor_cw = 0;
   ex_data->light_motor_ccw = 0;
   ex_data->light_buffer_empty = 1;

   return 0;
}

static int action_motor_ccw_buffer_empty(STATE_FUNCTIONS_ARGS)
{
   ex_data->speed_motor = -100;
   ex_data->light_motor_on = 1;
   ex_data->light_motor_cw = 0;
   ex_data->light_motor_ccw = 1;
   ex_data->light_buffer_empty = 1;

   return 0;
}

static int action_motor_cw_buffer_empty(STATE_FUNCTIONS_ARGS)
{
   ex_data->speed_motor = 100;
   ex_data->light_motor_on = 1;
   ex_data->light_motor_cw = 1;
   ex_data->light_motor_ccw = 0;
   ex_data->light_buffer_empty = 1;

   return 0;
}

static int action_motor_off_buffer_ok(STATE_FUNCTIONS_ARGS)
{
   ex_data->speed_motor = 0;
   ex_data->light_motor_on = 0;
   ex_data->light_motor_cw = 0;
   ex_data->light_motor_ccw = 0;
   ex_data->light_buffer_empty = 0;

   return 0;
}

static int action_motor_ccw_buffer_ok(STATE_FUNCTIONS_ARGS)
{
   ex_data->speed_motor = 0;
   ex_data->light_motor_on = 0;
   ex_data->light_motor_cw = 0;
   ex_data->light_motor_ccw = 0;
   ex_data->light_buffer_empty = 0;

   return 0;
}

static int action_motor_cw_buffer_ok(STATE_FUNCTIONS_ARGS)
{
   ex_data->speed_motor = 0;
   ex_data->light_motor_on = 0;
   ex_data->light_motor_cw = 0;
   ex_data->light_motor_ccw = 0;
   ex_data->light_buffer_empty = 0;

   return 0;
}

/********************* 'get' and 'set' functions *********************************************/

// sm_private_get_event implementation.
static event_id sm_private_get_event(void)
{
   event_id new_event;

   PDEBUG("sm_private_get_event execution\n");

   // Read the current event and reset it.
   new_event = _event;
   _event = EV_NONE;

   return new_event;
}

// sm_private_set_event implementation.
static void sm_private_set_event(event_id event)
{
   PDEBUG("sm_private_set_event execution\n");

   // The value of event is not checked in this private 'set' function. The SM programmers should know what
   // they're doing.
   _event = event;
}

// sm_set_event implementation.
int sm_set_event(event_id event)
{
   int ret = -1; // return -1 if event is ignored.

   PDEBUG("sm_set_event_event execution\n");

   // First check if event value is valid. Obs.: EV_NONE is not recognized because it would be useless.
   if((event > 0) && (event < NUMBER_EV)) {
      PDEBUG("Valid Event Detected\n");

      _event = event;
      ret = 0;
   }

   return ret;
}

/********************* Public SM functions *********************************************/

// sm_setup implementation.
void sm_setup(void)
{
   // Nothing to do in this fictitious firmware, just print a msg.
   PDEBUG("sm_setup execution\n");
}

// sm_run implementation
int sm_run(void)
{
   int ret = 0;
   event_id event = EV_NONE;
   state_id next_state = _state; // Current state

   // Dynamic memory allocation could be used (in setup function), but for microcontrollers with
   // bare-metal, monolithic firmware, this is usually better.
   static struct output_data ex_data;
   static struct input_data in_data;

   // Transition function array: given a state and an event, it returns the next state.
   // Be careful! The events ordering defined in 'typedef enum event_id' must be obeyed.
   // ----> Change this transition function accordingly to your automaton model!
   static state_id next_state_matrice[NUMBER_ST][NUMBER_EV];

   // EV_BTN_OFF: Force Stop
   next_state_matrice[ST_MOTOR_OFF_BUFFER_EMPTY][EV_BTN_OFF] = ST_MOTOR_OFF_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CCW_BUFFER_EMPTY][EV_BTN_OFF] = ST_MOTOR_OFF_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CW_BUFFER_EMPTY][EV_BTN_OFF] = ST_MOTOR_OFF_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CCW_BUFFER_OK][EV_BTN_OFF] = ST_MOTOR_OFF_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_OFF_BUFFER_OK][EV_BTN_OFF] = ST_MOTOR_OFF_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CW_BUFFER_OK][EV_BTN_OFF] = ST_MOTOR_OFF_BUFFER_EMPTY;

   // EV_BTN_OFF: Force Stop
   next_state_matrice[ST_MOTOR_OFF_BUFFER_EMPTY][EV_BTN_PE] = ST_MOTOR_OFF_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CCW_BUFFER_EMPTY][EV_BTN_PE] = ST_MOTOR_OFF_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CW_BUFFER_EMPTY][EV_BTN_PE] = ST_MOTOR_OFF_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CCW_BUFFER_OK][EV_BTN_PE] = ST_MOTOR_OFF_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_OFF_BUFFER_OK][EV_BTN_PE] = ST_MOTOR_OFF_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CW_BUFFER_OK][EV_BTN_PE] = ST_MOTOR_OFF_BUFFER_EMPTY;

   // EV_BTN_ON: Start System and Follow Buffer State
   next_state_matrice[ST_MOTOR_OFF_BUFFER_EMPTY][EV_BTN_ON] = ST_MOTOR_CW_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CCW_BUFFER_EMPTY][EV_BTN_ON] = ST_MOTOR_CCW_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CW_BUFFER_EMPTY][EV_BTN_ON] = ST_MOTOR_CW_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_OFF_BUFFER_OK][EV_BTN_ON] = ST_MOTOR_CW_BUFFER_OK;
   next_state_matrice[ST_MOTOR_CCW_BUFFER_OK][EV_BTN_ON] = ST_MOTOR_CCW_BUFFER_OK;
   next_state_matrice[ST_MOTOR_CW_BUFFER_OK][EV_BTN_ON] = ST_MOTOR_CW_BUFFER_OK;

   // EV_EMPTY_BUFFER: Turn On Motor
   next_state_matrice[ST_MOTOR_OFF_BUFFER_EMPTY][EV_EMPTY_BUFFER] = ST_MOTOR_CW_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CCW_BUFFER_EMPTY][EV_EMPTY_BUFFER] = ST_MOTOR_CW_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CW_BUFFER_EMPTY][EV_EMPTY_BUFFER] = ST_MOTOR_CW_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CCW_BUFFER_OK][EV_EMPTY_BUFFER] = ST_MOTOR_CCW_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_OFF_BUFFER_OK][EV_EMPTY_BUFFER] = ST_MOTOR_CW_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CW_BUFFER_OK][EV_EMPTY_BUFFER] = ST_MOTOR_CW_BUFFER_EMPTY;

   // EV_SWITCH_MOTOR_CW: CCW -> CW
   next_state_matrice[ST_MOTOR_OFF_BUFFER_EMPTY][EV_SWITCH_MOTOR_CW] = ST_MOTOR_OFF_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CCW_BUFFER_EMPTY][EV_SWITCH_MOTOR_CW] = ST_MOTOR_CW_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CW_BUFFER_EMPTY][EV_SWITCH_MOTOR_CW] = ST_MOTOR_CW_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_OFF_BUFFER_OK][EV_SWITCH_MOTOR_CW] = ST_MOTOR_OFF_BUFFER_OK;
   next_state_matrice[ST_MOTOR_CCW_BUFFER_OK][EV_SWITCH_MOTOR_CW] = ST_MOTOR_CW_BUFFER_OK;
   next_state_matrice[ST_MOTOR_CW_BUFFER_OK][EV_SWITCH_MOTOR_CW] = ST_MOTOR_CW_BUFFER_OK;

   // EV_SWITCH_MOTOR_CCW: CW -> CCW
   next_state_matrice[ST_MOTOR_OFF_BUFFER_EMPTY][EV_SWITCH_MOTOR_CCW] = ST_MOTOR_OFF_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CCW_BUFFER_EMPTY][EV_SWITCH_MOTOR_CCW] = ST_MOTOR_CCW_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CW_BUFFER_EMPTY][EV_SWITCH_MOTOR_CCW] = ST_MOTOR_CCW_BUFFER_EMPTY;
   next_state_matrice[ST_MOTOR_CCW_BUFFER_OK][EV_SWITCH_MOTOR_CCW] = ST_MOTOR_CCW_BUFFER_OK;
   next_state_matrice[ST_MOTOR_OFF_BUFFER_OK][EV_SWITCH_MOTOR_CCW] = ST_MOTOR_OFF_BUFFER_OK;
   next_state_matrice[ST_MOTOR_CW_BUFFER_OK][EV_SWITCH_MOTOR_CCW] = ST_MOTOR_CCW_BUFFER_OK;

   PDEBUG("sm_run execution\n");

   // At each cycle, get the new event, which was set by 'external world' or by a SM internal state
   event = sm_private_get_event();

   // In order to save power, do something if there is a new event only.
   if(event != EV_NONE) {
      // Get the next state to be executed
      next_state = next_state_matrice[_state][event];

      if(next_state != ST_NULL) {
         // Update the current state variable
         _state = next_state;

         // Execute next state function, return 0 if everything went OK or -1 otherwise
         ret = state_handler_container[_state].state_exe(&ex_data, in_data);

         // Display Something to emulate a real system behavior
         simulate_system(&ex_data, in_data);
      }
      else {
         // If the new event is not recognized in current state, f returns a 'NULL' state, and the function
         // returns an error code.
         return -1;
      }
   }

   return ret;
}

// Display System Status as JSON
static int simulate_system(STATE_FUNCTIONS_ARGS)
{
   printf("systemOutput:\n{\n");
   printf("\tspeed_motor: %d,\n,",ex_data->speed_motor);
   printf("\tlight_motor_on: %s,\n",(ex_data->light_motor_on)?"on":"off");
   printf("\tlight_motor_cw: %s,\n",(ex_data->light_motor_cw)?"on":"off");
   printf("\tlight_motor_ccw: %s,\n",(ex_data->light_motor_ccw)?"on":"off");
   printf("\tlight_buffer_empty: %s\n",(ex_data->light_buffer_empty)?"blinking":"off");
   printf("},\n");

   return 0;
}


// sm_close implementation.
void sm_close(void)
{
   // Nothing to do in this fictitious firmware, just print a msg.
   PDEBUG("sm_close execution\n");
}


/*
 * Unicorn 3D Printer Firmware
 * planner.h
*/
#ifndef _PLANNER_H
#define _PLANNER_H

extern unsigned long minsegmenttime;

extern unsigned long axis_steps_per_sqr_second[];

extern unsigned short virtual_steps_x;
extern unsigned short virtual_steps_y;
extern unsigned short virtual_steps_z;

extern float feedrate;
extern float next_feedrate;
extern float saved_feedrate;

extern unsigned char is_homing;

/*
 * Minimum planner junction speed.
 * Set the default minimum speed the planner plans for at the end of the buffer
 * and all stops. 
 * This should not be much greater than zero and should only be changed if 
 * unwanted behavior is observed on a user's machine when running at very slow speeds.
 */
#define MINIMUM_PLANNER_SPEED 0.05     // mm/sec

/*
 * This struct is used when buffering the setup for each linear movement "nominal" values
 * are as specified in the source g-code and may never actually be reached 
 * if acceleration management is active.
 */

#define BLOCK_M_CMD   	1<<1
#define BLOCK_G_CMD   	1<<0
#define BLOCK_NONE_CMD  0
typedef struct {
    unsigned char type; //M gcmd = 1, other is 0
    /* Fields used by the bresenham algorithm for the tracing the line */
    long steps_x;                      // Step count along each axis
    long steps_y;
    long steps_z;
    long steps_e; 
    unsigned long step_event_count;    // The number of step events required to complete this block
    long accelerate_until;             // The index of the step event on which to stop acceleration
    long decelerate_after;             // The index of the step event on which to start decelerating
    long acceleration_rate;            // The acceleration rate used for acceleration calculation
    unsigned char direction_bits;      // The direction bit set for this block
    unsigned char active_extruder;     // Selects the active extruder

    /* Fields used by the motion planner to manage acceleration */
    float nominal_speed;               // The nominal speed for this block in mm/sec
    float entry_speed;                 // Entry speed at previous-current junction in mm/sec
    float max_entry_speed;             // Maximum allowable junction entry speed in mm/sec
    float millimeters;                 // The total travel of this block in mm
    float acceleration;                // Acceleration mm/sec^2
    unsigned char recalculate_flag;    // Planner flag to recalculate trapezoids on entry junction 
    unsigned char nominal_length_flag; // Planner flag for nominal speed always reached

    /* Setting for the trapezoid generator */
    unsigned long nominal_rate;        // The nominal step rate for this in step_events/sec
    unsigned long initial_rate;        // The jerk-adjusted step rate at start of blcok
    unsigned long final_rate;          // The minimal rate at exit
    unsigned long acceleration_st;     // Acceleration step/sec^2
    unsigned long fan_speed;
    volatile char busy;
} block_t;

#if defined (__cplusplus)
extern "C" {
#endif
/*
 * Initialize and delete the motion plan subsystem
 */
extern int  plan_init(void);
extern void plan_exit(void);
/*
 * Stop Planner
 */
extern void plan_start(void);
extern void plan_stop(void);
/*
 * Add a new linear movement to the buffer.
 * x, y and z is the signed, absolute target position in millimaters.
 * Feed rate specfies the speed of the motion.
 */
extern void plan_buffer_line(float x, float y, float z, const float e, 
                             float feed_rate, const uint8_t extruder);
/* 
 * Get the current block, Return NULL if buffer empty 
 */
extern block_t *plan_get_current_block(void);
/*
 * Discard current block
 */
extern void plan_discard_current_block(void);
/*
 * Get current filled block size
 */
extern int plan_get_block_size(void);
/*
 * Set position
 * Used for G92 instructions.
 */
extern void plan_set_position(float x, float y, float z, const float e);
extern void plan_set_position_no_delta_autolevel(float x, float y, float z, const float e);
extern void plan_set_e_position(const float e);
extern void put_mcode_to_fifo();

#if defined (__cplusplus)
}
#endif
#endif

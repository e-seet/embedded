// Lab 6
/*
pseudo code
// Initialize PID controller parameters
Kp = 1.0
Ki = 0.1
Kd = 0.01

// Initialize variables
setpoint = 100.0
current_value = 0.0
integral = 0.0
prev_error = 0.0

// Initialize simulation parameters
time_step = 0.1
num_iterations = 100

// Main control loop
for i = 0 to num_iterations - 1:
    // Compute error
    error = setpoint - current_value

    // Update integral term
    integral += error

    // Compute derivative term
    derivative = error - prev_error

    // Compute control signal
    control_signal = Kp * error + Ki * integral + Kd * derivative

    // Update previous error
    prev_error = error

    // Simulate motor dynamics (for demonstration purposes)
    motor_response = control_signal * 0.1

    // Update current position
    current_value += motor_response

    // Display results
    Print "Iteration ", i, ": Control Signal = ", control_signal, ", Current Position = ", current_value

    // Sleep for the time step (for demonstration purposes)
    Sleep for time_step seconds
*/

// End of main control loop

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>  // For usleep
#include "unistd.h"  // For usleep

// origianl code
// float Kp = 2.0; 
// float Ki = 0.2; 
// float Kd = 0.02; 

// pseudo code
float Kp = 1.0; 
float Ki = 0.1; 
float Kd = 0.01; 

// original
// Function to compute the control signal
// float compute_pid(float setpoint, float current_value, float *integral, float *prev_error) {

//     float error = current_value - setpoint;
    
//     *integral += error;
    
//     float derivative = error - *prev_error;
    
//     float control_signal = Kp * error + Ki * (*integral) + Kd * derivative * 0.1;
    
//     *prev_error = current_value;
    
//     return control_signal;
// }

// from pseudo code
// Function to compute the control signal
float compute_pid(float setpoint, float current_value, float *integral, float *prev_error) {

    float error = setpoint - current_value;
    
    *integral += error;
    
    float derivative = error - *prev_error;
    
	// original code
	// float control_signal = Kp * error + Ki * (*integral) + Kd * derivative * 0.1;
	//pseduo code
    float control_signal = (Kp * error) + (Ki * (*integral)) + (Kd * derivative);
    
    *prev_error = error;	
    
    return control_signal;
}

int main() {
    stdio_init_all();

    float setpoint = 100.0;  // Desired position
    float current_value = 0.0;  // Current position
    float integral = 0.0;  // Integral term
    float prev_error = 0.0;  // Previous error term
    
    float time_step = 0.1;  
    int num_iterations = 100; 
    // float error;

    // Simulate the control loop
    for (int i = 0; i < num_iterations; i++) 
	{
		float control_signal = compute_pid(setpoint, current_value, &integral, &prev_error);

		// Motor response model ( origina lcode)
        // float motor_response = control_signal * 0.05;  
		// pseudo code
		float motor_response = control_signal * 0.1;

		current_value += motor_response;
        printf("Iteration %d: Control Signal = %f, Current Position = %f\n", i, control_signal, current_value);
        prev_error = current_value;

		// to check
		// usleep((useconds_t)(time_step * 1000000));
		 sleep_ms((int)(time_step * 1000));  // Convert seconds to milliseconds
    }
    
    return 0;
}
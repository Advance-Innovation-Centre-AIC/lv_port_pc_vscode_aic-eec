/*******************************************************************************
 * File: part4_examples.h
 * Description: Part 4 Examples - IPC, Logging, Event Bus (PC Simulator)
 *
 * Part 4 focuses on advanced topics:
 * - Inter-Processor Communication (IPC) concepts
 * - Thread-safe Logging
 * - Event-driven Architecture
 * - UI Layout Helpers
 *
 * Part of BiiL Course: Embedded C for IoT
 ******************************************************************************/

#ifndef PART4_EXAMPLES_H
#define PART4_EXAMPLES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Example 1: IPC Ping Test (Simulated) */
void part4_ex1_ipc_ping(void);

/* Example 2: IPC Remote Logging (Simulated) */
void part4_ex2_ipc_log(void);

/* Example 3: IPC Sensor Data (Simulated) */
void part4_ex3_ipc_sensor(void);

/* Example 4: Event Bus Demo */
void part4_ex4_event_bus(void);

/* Example 5: HW IPC LED Control (Simulated) */
void part4_ex5_hw_ipc_led(void);

/* Example 6: HW IPC Button Events (Simulated) */
void part4_ex6_hw_ipc_button(void);

/* Example 7: HW IPC Dashboard (Simulated) */
void part4_ex7_hw_ipc_dashboard(void);

/* Example 8: Advanced Features */
void part4_ex8_advanced(void);

/* Example 9: CAPSENSE via IPC (Simulated) */
void part4_ex9_capsense_ipc(void);

#ifdef __cplusplus
}
#endif

#endif /* PART4_EXAMPLES_H */

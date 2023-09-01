/*
 * scheduler.h
 *
 *  Created on: 10/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *      Description: Module runs a scheduler kernel that completes tasks based on the system ticks interrupt.
 *      If a task has completed enough ticks, it is ready to run the function, then reset back to the starting tick values.
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

//*******************************************************************************************
// Header Files
//*******************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

//*******************************************************************************************
// Constants
//*******************************************************************************************
#define NUMTASKS 5

//*******************************************************************************************
// Structs
//*******************************************************************************************
typedef struct {
    void(*taskToComplete)(void);
    uint16_t currentTicks;
    size_t numTicksCycle;
    bool completeFunc;
} task_t;

//*******************************************************************************************
// Creates a dynamically allocated task list for the scheduler to iterate through
//*******************************************************************************************
void initialiseTaskList(void);

//*******************************************************************************************
// Initialise each task for the scheduler
//
// @param functionToRun Pointer to the function the task completes.
//
// @param cycleLength Number of ticks to complete before being ready.
//*******************************************************************************************
void
initialiseTask(void(*functionToRun)(void), size_t cycleLength, size_t index);

//*******************************************************************************************
// Starts the while loop to run the scheduler. Complete tasks when the boolean in the struct
// is true.
//*******************************************************************************************
void
startScheduler(void);

//*******************************************************************************************
// Updates the system ticks for the scheduler and sets a task to run if the full cycle
// for a task is complete
//*******************************************************************************************
void
updateScheduleTicks(void);

#endif /* SCHEDULER_H_ */

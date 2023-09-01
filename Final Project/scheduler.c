/*
 * scheduler.c
 *
 *  Created on: 10/05/2023
 *      Author: Amber Waymouth (awa155)
 *              Arabella Cryer (acr151)
 *      Description: Module runs a scheduler kernel that completes tasks based on the system ticks interrupt.
 *      If a task has completed enough ticks, it is ready to run the function, then reset back to the starting tick values.
 */

#include "scheduler.h"

//*******************************************************************************************
// Static Variables
//*******************************************************************************************
static task_t* taskList;

//*******************************************************************************************
// Creates a dynamically allocated task list for the scheduler to iterate through
//*******************************************************************************************
void initialiseTaskList(void) {
    taskList = malloc(NUMTASKS * sizeof(task_t));
};


//*******************************************************************************************
// Initialise each task for the scheduler
//
// @param functionToRun Pointer to the function the task completes.
//
// @param cycleLength Number of ticks to complete before being ready.
//*******************************************************************************************
void
initialiseTask(void(*functionToRun)(void), size_t cycleLength, size_t index)
{
    task_t* newTask = &taskList[index];
    newTask->taskToComplete = functionToRun;
    newTask->currentTicks = 0;
    newTask->numTicksCycle = cycleLength;
    newTask->completeFunc = false;
}

//*******************************************************************************************
// Starts the while loop to run the scheduler. Complete tasks when the boolean in the struct
// is true.
//*******************************************************************************************
void
startScheduler(void) {
    while(1) {
        size_t index = 0;
        for (index = 0; index < NUMTASKS; index++) {
            task_t* currentTask = &taskList[index];
            if (currentTask->completeFunc) {
                currentTask->completeFunc = false;
                currentTask->taskToComplete();
                break;
            }
        }
    }

}

//*******************************************************************************************
// Updates the system ticks for the scheduler and sets a task to run if the full cycle
// for a task is complete
//*******************************************************************************************
void
updateScheduleTicks(void)
{
    size_t index = 0;
    for (index = 0; index < NUMTASKS; index++) {
        task_t* currentTask = &taskList[index];
        currentTask->currentTicks++;
        if (currentTask->currentTicks >= currentTask->numTicksCycle) {
            currentTask->currentTicks = 0;
            currentTask->completeFunc = true;
        }
    }
}

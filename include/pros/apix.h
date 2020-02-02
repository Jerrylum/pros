/**
 * \file pros/apix.h
 *
 * PROS Extended API header
 *
 * Contains additional declarations for use by advaned users of PROS. These
 * functions do not typically have as much error handling or require deeper
 * knowledge of real time operating systems.
 *
 * Visit https://pros.cs.purdue.edu/v5/extended/api.html to learn more.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2020, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_API_EXTENDED_H_
#define _PROS_API_EXTENDED_H_

#include "api.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lvgl.h"
#pragma GCC diagnostic pop
#include "pros/serial.h"


/******************************************************************************/
/**                            RTOS CONFIGURATION                            **/
/******************************************************************************/

struct xSTATIC_LIST_ITEM {
	uint32_t xDummy1;
	void *pvDummy2[ 4 ];
};
typedef struct xSTATIC_LIST_ITEM StaticListItem_t;

struct xSTATIC_MINI_LIST_ITEM {
	uint32_t xDummy1;
	void *pvDummy2[ 2 ];
};
typedef struct xSTATIC_MINI_LIST_ITEM StaticMiniListItem_t;

typedef struct xSTATIC_LIST {
	uint32_t uxDummy1;
	void *pvDummy2;
	StaticMiniListItem_t xDummy3;
} StaticList_t;

typedef struct xSTATIC_TCB {
	void				*pxDummy1;
	StaticListItem_t	 xDummy3[ 2 ];
	uint32_t			 uxDummy5;
	void				*pxDummy6;
	uint8_t				 ucDummy7[ 32 ];
	uint32_t		     uxDummy10[ 2 ];
	uint32_t		     uxDummy12[ 2 ];
	void			    *pvDummy15[ 2 ];
	uint32_t		     ulDummy16;
	struct	_reent	     xDummy17;
	uint32_t 		     ulDummy18;
	uint8_t 		     ucDummy19;
	uint8_t			     uxDummy20;
	uint8_t              ucDummy21;

} static_task_s_t;

typedef struct xSTATIC_QUEUE {
	void *pvDummy1[ 3 ];

	union {
		void     *pvDummy2;
		uint32_t  uxDummy2;
	} u;

	StaticList_t xDummy3[ 2 ];
	uint32_t     uxDummy4[ 3 ];
	uint8_t      ucDummy5[ 2 ];
	uint8_t      ucDummy6;
	uint32_t     uxDummy8;
	uint8_t      ucDummy9;
} static_queue_s_t;
typedef static_queue_s_t static_sem_s_t;


#ifdef __cplusplus
#include "pros/serial.hpp"
namespace pros::c {
extern "C" {
#endif

/******************************************************************************/
/**                             RTOS FACILITIES                              **/
/**                                                                          **/
/**                                                                          **/
/**See https://pros.cs.purdue.edu/v5/extended/multitasking.html to learn more**/
/******************************************************************************/

typedef void* queue_t;
typedef void* sem_t;

/**
 * Unblocks a task in the Blocked state (e.g. waiting for a delay, on a
 * semaphore, etc.).
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking.html#abort_delay for
 * details.
 */
bool task_abort_delay(task_t task);

/**
 * Notify a task when a target task is being deleted.
 *
 * This function will configure the PROS kernel to call
 * task_notify_ext(task_to_notify, value, action, NULL) when target_task is
 * deleted.
 *
 *
 * \param target_task
 *				The task being watched for deletion
 * \param task_to_notify
 *        The task to notify when target_task is deleted
 * \param value
 *				The value to supply to task_notify_ext
 * \param notify_action
 * 				The action to supply to task_notify_ext
 */
void task_notify_when_deleting(task_t target_task, task_t task_to_notify, uint32_t value,
                               notify_action_e_t notify_action);

/**
 * Creates a recursive mutex which can be locked recursively by the owner.
 *
 * See
 * https://pros.cs.purdue.edu/v5/extended/multitasking.html#recursive_mutexes
 * for details.
 *
 * \return A newly created recursive mutex.
 */
mutex_t mutex_recursive_create(void);

/**
 * Takes a recursive mutex.
 *
 * See
 * https://pros.cs.purdue.edu/v5/extended/multitasking.html#recursive_mutexes
 * for details.
 *
 * \param mutex
 *        A mutex handle created by mutex_recursive_create
 * \param wait_time
 *        Amount of time to wait before timing out
 *
 * \return 1 if the mutex was obtained, 0 otherwise
 */
bool mutex_recursive_take(mutex_t mutex, uint32_t timeout);

/**
 * Gives a recursive mutex.
 *
 * See
 * https://pros.cs.purdue.edu/v5/extended/multitasking.html#recursive_mutexes
 * for details.
 *
 * \param mutex
 *        A mutex handle created by mutex_recursive_create
 *
 * \return 1 if the mutex was obtained, 0 otherwise
 */
bool mutex_recursive_give(mutex_t mutex);

/**
 * Returns a handle to the current owner of a mutex.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking.html#extra for
 * details.
 *
 * \param mutex
 *        A mutex handle
 *
 * \return A handle to the current task that owns the mutex, or NULL if the
 * mutex isn't owned.
 */
task_t mutex_get_owner(mutex_t mutex);

/**
 * Creates a counting sempahore.
 *
 * See https://pros.cs.purdue.edu/v5/tutorials/multitasking.html#semaphores for
 *details.
 *
 * \param max_count
 *        The maximum count value that can be reached.
 * \param init_count
 *        The initial count value assigned to the new semaphore.
 *
 * \return A newly created semaphore. If an error occurred, NULL will be
 * returned and errno can be checked for hints as to why sem_create failed.
 */
sem_t sem_create(uint32_t max_count, uint32_t init_count);

/**
 * Deletes a semaphore (or binary semaphore)
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking.html#semaphores for
 * details.
 *
 * \param sem
 * 			  Semaphore to delete
 */
void sem_delete(sem_t sem);

/**
 * Creates a binary semaphore.
 *
 * See
 * https://pros.cs.purdue.edu/v5/extended/multitasking#.htmlbinary_semaphores
 * for details.
 *
 * \return A newly created semaphore.
 */
sem_t sem_binary_create(void);

/**
 * Waits for the semaphore's value to be greater than 0. If the value is already
 * greater than 0, this function immediately returns.
 *
 * See https://pros.cs.purdue.edu/v5/tutorials/multitasking.html#semaphores for
 * details.
 *
 * \param sem
 *        Semaphore to wait on
 * \param timeout
 *        Time to wait before the semaphore's becomes available. A timeout of 0
 *        can be used to poll the sempahore. TIMEOUT_MAX can be used to block
 *        indefinitely.
 *
 * \return True if the semaphore was successfully take, false otherwise. If
 * false is returned, then errno is set with a hint about why the sempahore
 * couldn't be taken.
 */
bool sem_wait(sem_t sem, uint32_t timeout);

/**
 * Increments a semaphore's value.
 *
 * See https://pros.cs.purdue.edu/v5/tutorials/multitasking.html#semaphores for
 * details.
 *
 * \param sem
 *        Semaphore to post
 *
 * \return True if the value was incremented, false otherwise. If false is
 * returned, then errno is set with a hint about why the semaphore couldn't be
 * taken.
 */
bool sem_post(sem_t sem);

/**
 * Returns the current value of the semaphore.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking.html#extra for
 * details.
 *
 * \param sem
 *        A semaphore handle
 *
 * \return The current value of the semaphore (e.g. the number of resources
 * available)
 */
uint32_t sem_get_count(sem_t sem);

/**
 * Creates a queue.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking.html#queues for
 * details.
 *
 * \param length
 *        The maximum number of items that the queue can contain.
 * \param item_size
 *        The number of bytes each item in the queue will require.
 *
 * \return A handle to a newly created queue, or NULL if the queue cannot be
 * created.
 */
queue_t queue_create(uint32_t length, uint32_t item_size);

/**
 * Posts an item to the front of a queue. The item is queued by copy, not by
 * reference.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking.html#queues for
 * details.
 *
 * \param queue
 *        The queue handle
 * \param item
 *        A pointer to the item that will be placed on the queue.
 * \param timeout
 *        Time to wait for space to become available. A timeout of 0 can be used
 *        to attempt to post without blocking. TIMEOUT_MAX can be used to block
 *        indefinitely.
 *
 * \return True if the item was preprended, false otherwise.
 */
bool queue_prepend(queue_t queue, const void* item, uint32_t timeout);

/**
 * Posts an item to the end of a queue. The item is queued by copy, not by
 * reference.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking.html#queues for
 * details.
 *
 * \param queue
 *        The queue handle
 * \param item
 *        A pointer to the item that will be placed on the queue.
 * \param timeout
 *        Time to wait for space to become available. A timeout of 0 can be used
 *        to attempt to post without blocking. TIMEOUT_MAX can be used to block
 *        indefinitely.
 *
 * \return True if the item was preprended, false otherwise.
 */
bool queue_append(queue_t queue, const void* item, uint32_t timeout);

/**
 * Receive an item from a queue without removing the item from the queue.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking.html#queues for
 * details.
 *
 * \param queue
 *        The queue handle
 * \param buffer
 *        Pointer to a buffer to which the received item will be copied
 * \param timeout
 *        Time to wait for space to become available. A timeout of 0 can be used
 *        to attempt to post without blocking. TIMEOUT_MAX can be used to block
 *        indefinitely.
 *
 * \return True if an item was copied into the buffer, false otherwise.
 */
bool queue_peek(queue_t queue, void* const buffer, uint32_t timeout);

/**
 * Receive an item from the queue.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking.html#queues for
 * details.
 *
 * \param queue
 *        The queue handle
 * \param buffer
 *        Pointer to a buffer to which the received item will be copied
 * \param timeout
 *        Time to wait for space to become available. A timeout of 0 can be used
 *        to attempt to post without blocking. TIMEOUT_MAX can be used to block
 *        indefinitely.
 *
 * \return True if an item was copied into the buffer, false otherwise.
 */
bool queue_recv(queue_t queue, void* const buffer, uint32_t timeout);

/**
 * Return the number of messages stored in a queue.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking.html#queues for
 * details.
 *
 * \param queue
 *        The queue handle.
 *
 * \return The number of messages available in the queue.
 */
uint32_t queue_get_waiting(const queue_t queue);

/**
 * Return the number of spaces left in a queue.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking.html#queues for
 * details.
 *
 * \param queue
 *        The queue handle.
 *
 * \return The number of spaces available in the queue.
 */
uint32_t queue_get_available(const queue_t queue);

/**
 * Delete a queue.
 *
 * See https://pros.cs.purdue.edu/v5/extended/multitasking.html#queues for
 * details.
 *
 * \param queue
 *        Queue handle to delete
 */
void queue_delete(queue_t queue);

/**
 * Resets a queue to an empty state
 *
 * \param queue
 *        Queue handle to reset
 */
void queue_reset(queue_t queue);

/******************************************************************************/
/**                           Device Registration                            **/
/******************************************************************************/

/*
 * List of possible v5 devices
 *
 * This list contains all current V5 Devices, and mirrors V5_DeviceType from the
 * api.
 */
typedef enum v5_device_e {
	E_DEVICE_NONE = 0,
	E_DEVICE_MOTOR = 2,
	E_DEVICE_IMU = 6,
	E_DEVICE_RADIO = 8,
	E_DEVICE_VISION = 11,
	E_DEVICE_ADI = 12,
	E_DEVICE_GENERIC = 129,
	E_DEVICE_UNDEFINED = 255
} v5_device_e_t;

/*
 * Registers a device in the given port
 *
 * Registers a device of the given type in the given port into the registry, if
 * that type of device is detected to be plugged in to that port.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21), or a
 * a different device than specified is plugged in.
 * EADDRINUSE - The port is already registered to another device.
 *
 * \param port
 *        The port number to register the device
 * \param device
 *        The type of device to register
 *
 * \return 1 upon success, PROS_ERR upon failure
 */
int registry_bind_port(uint8_t port, v5_device_e_t device_type);

/*
 * Deregisters a devices from the given port
 *
 * Removes the device registed in the given port, if there is one.
 *
 * This function uses the following values of errno when an error state is
 * reached:
 * ENXIO - The given value is not within the range of V5 ports (1-21).
 *
 * \param port
 *        The port number to deregister
 *
 * \return 1 upon success, PROS_ERR upon failure
 */
int registry_unbind_port(uint8_t port);

/******************************************************************************/
/**                               Filesystem                                 **/
/******************************************************************************/
/**
 * Control settings of the serial driver.
 *
 * \param action
 * 			An action to perform on the serial driver. See the SERCTL_* macros for
 * 			details on the different actions.
 * \param extra_arg
 * 			An argument to pass in based on the action
 */
int32_t serctl(const uint32_t action, void* const extra_arg);

/**
 * Control settings of the microSD card driver.
 *
 * \param action
 * 			An action to perform on the microSD card driver. See the USDCTL_* macros
 *      for details on the different actions.
 * \param extra_arg
 * 		   	An argument to pass in based on the action
 */
// Not yet implemented
// int32_t usdctl(const uint32_t action, void* const extra_arg);

/**
 * Control settings of the way the file's driver treats the file
 *
 * \param file
 * 			A valid file descriptor number
 * \param action
 * 			An action to perform on the file's driver. See the *CTL_* macros for
 * 			details on the different actions. Note that the action passed in must
 *      match the correct driver (e.g. don't perform a SERCTL_* action on a
 *      microSD card file)
 * \param extra_arg
 * 		  	An argument to pass in based on the action
 */
int32_t fdctl(int file, const uint32_t action, void* const extra_arg);

/**
 * Action macro to pass into serctl or fdctl that activates the stream
 * identifier.
 *
 * When used with serctl, the extra argument must be the little endian
 * representation of the stream identifier (e.g. "sout" -> 0x74756f73)
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/filesystem.html#serial
 * to learn more.
 */
#define SERCTL_ACTIVATE 10

/**
 * Action macro to pass into serctl or fdctl that deactivates the stream
 * identifier.
 *
 * When used with serctl, the extra argument must be the little endian
 * representation of the stream identifier (e.g. "sout" -> 0x74756f73)
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/filesystem.html#serial
 * to learn more.
 */
#define SERCTL_DEACTIVATE 11

/**
 * Action macro to pass into fdctl that enables blocking writes for the file
 *
 * The extra argument is not used with this action, provide any value (e.g.
 * NULL) instead
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/filesystem.html#serial
 * to learn more.
 */
#define SERCTL_BLKWRITE 12

/**
 * Action macro to pass into fdctl that makes writes non-blocking for the file
 *
 * The extra argument is not used with this action, provide any value (e.g.
 * NULL) instead
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/filesystem.html#serial
 * to learn more.
 */
#define SERCTL_NOBLKWRITE 13

/**
 * Action macro to pass into serctl that enables advanced stream multiplexing
 * capabilities
 *
 * The extra argument is not used with this action, provide any value (e.g.
 * NULL) instead
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/filesystem.html#serial
 * to learn more.
 */
#define SERCTL_ENABLE_COBS 14

/**
 * Action macro to pass into serctl that disables advanced stream multiplexing
 * capabilities
 *
 * The extra argument is not used with this action, provide any value (e.g.
 * NULL) instead
 *
 * Visit https://pros.cs.purdue.edu/v5/tutorials/topical/filesystem.html#serial
 * to learn more.
 */
#define SERCTL_DISABLE_COBS 15

/**
 * Action macro to check if there is data available from the Generic Serial
 * Device
 *
 * The extra argument is not used with this action, provide any value (e.g.
 * NULL) instead
 */
#define DEVCTL_FIONREAD 16

/**
 * Action macro to check if there is space available in the Generic Serial
 * Device's output buffer
 *
 * The extra argument is not used with this action, provide any value (e.g.
 * NULL) instead
 */
#define DEVCTL_FIONWRITE 18

/**
 * Action macro to set the Generic Serial Device's baudrate.
 *
 * The extra argument is the baudrate.
 */
#define DEVCTL_SET_BAUDRATE 17


/******************************************************************************/
/**                       RTOS STATIC TASK FACILITIES                        **/
/******************************************************************************/

#ifdef __cplusplus
#define task_t pros::task_t
#define task_fn_t pros::task_fn_t
#define mutex_t pros::mutex_t
#define sem_t pros::c::sem_t
#define queue_t pros::c::queue_t
#endif

typedef uint32_t task_stack_t;

/**
 * Creates a task using statically allocated buffers. It allows the RAM to be 
 * statically allocated at compile time.
 * This function uses the following values of errno when an error state is
 * reached:
 * EINVAL - The stack cannot be used as the TCB was not created.
 *
 * \param function
 *        Pointer to the task entry function
 * \param parameters
 *        Pointer to memory that will be used as a parameter for the task being
 *        created. This memory should not typically come from stack, but rather
 *        from dynamically (i.e., malloc'd) or statically allocated memory.
 * \param prio
 *        The priority at which the task should run.
 *        TASK_PRIO_DEFAULT plus/minus 1 or 2 is typically used.
 * \param stack_depth
 *        The number of words (i.e. 4 * stack_depth) available on the task's
 *        stack. TASK_STACK_DEPTH_DEFAULT is typically sufficienct.
 * \param name
 *        A descriptive name for the task.  This is mainly used to facilitate
 *        debugging. The name may be up to 32 characters long.
 * \param stack_buffer
 *        Pointer to memory that will be used as the task’s stack. The pointer
 * 		  must point to a task_stack_t array that has at least stack_depth
 *        indexes. This memory should not typically come from stack, but rather
 *        from dynamically (i.e., malloc'd) or statically allocated memory.
 * \param task_buffer
 *        Pointer to memory that will be used to hold the new task’s data
 *        structures (TCB). The pointer must point to a variable of type
 *        static_task_s_t. This memory should not typically come from stack,
 *        but rather from dynamically (i.e., malloc'd) or statically allocated
 *        memory.
 *
 * \return A handle by which the newly created task can be referenced. If an
 * error occurred, NULL will be returned and errno can be checked for hints as
 * to why task_create failed.
 */
task_t task_create_static(task_fn_t function, void* const parameters, uint32_t prio, const size_t stack_depth,
                          const char* const name, task_stack_t* const stack_buffer, static_task_s_t* const task_buffer);

/**
 * Creates a statically allocated mutex.
 *
 * All FreeRTOS primitives must be created statically if they are required for
 * operation of the kernel.
 *
 * \param mutex_buffer
 *        A buffer to store the mutex in
 *
 * \return A handle to a newly created mutex. If an error occurred, NULL will be
 * returned and errno can be checked for hints as to why mutex_create failed.
 */
mutex_t mutex_create_static(static_sem_s_t* mutex_buffer);

/**
 * Creates a statically allocated semaphore.
 *
 * All FreeRTOS primitives must be created statically if they are required for
 * operation of the kernel.
 *
 * \param max_count
 *        The maximum count value that can be reached.
 * \param init_count
 *        The initial count value assigned to the new semaphore.
 * \param semaphore_buffer
 *        A buffer to store the semaphore in
 *
 * \return A newly created semaphore. If an error occurred, NULL will be
 * returned and errno can be checked for hints as to why sem_create failed.
 */
sem_t sem_create_static(uint32_t max_count, uint32_t init_count, static_sem_s_t* semaphore_buffer);

/**
 * Creates a statically allocated queue.
 *
 * All FreeRTOS primitives must be created statically if they are required for
 * operation of the kernel.
 *
 * \param length
 *        The maximum number of items that the queue can contain.
 * \param item_size
 *        The number of bytes each item in the queue will require.
 * \param storage_buffer
 *        A memory location for data storage
 * \param queue_buffer
 *        A buffer to store the queue in
 *
 * \return A handle to a newly created queue, or NULL if the queue cannot be
 * created.
 */
queue_t queue_create_static(uint32_t length, uint32_t item_size, uint8_t* storage_buffer,
                            static_queue_s_t* queue_buffer);

#ifdef __cplusplus
#undef task_t
#undef task_fn_t
#undef mutex_t
#undef sem_t
#undef queue_t
}
}
#endif

#endif  // _PROS_API_EXTENDED_H_

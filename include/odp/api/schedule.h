/* Copyright (c) 2013, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */


/**
 * @file
 *
 * ODP schedule
 */

#ifndef ODP_API_SCHEDULE_H_
#define ODP_API_SCHEDULE_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <odp/std_types.h>
#include <odp/event.h>
#include <odp/queue.h>
#include <odp/schedule_types.h>
#include <odp/thrmask.h>

/** @defgroup odp_scheduler ODP SCHEDULER
 *  Operations on the scheduler.
 *  @{
 */

/**
 * @def ODP_SCHED_WAIT
 * Wait infinitely
 */

/**
 * @def ODP_SCHED_NO_WAIT
 * Do not wait
 */

/**
 * @def ODP_SCHED_GROUP_NAME_LEN
 * Maximum schedule group name length in chars
 */

/**
 * Schedule wait time
 *
 * Converts nanoseconds to wait values for other schedule functions.
 *
 * @param ns Nanoseconds
 *
 * @return Value for the wait parameter in schedule functions
 */
uint64_t odp_schedule_wait_time(uint64_t ns);

/**
 * Schedule
 *
 * Schedules all queues created with ODP_QUEUE_TYPE_SCHED type. Returns
 * next highest priority event which is available for the calling thread.
 * Outputs the source queue of the event. If there's no event available, waits
 * for an event according to the wait parameter setting. Returns
 * ODP_EVENT_INVALID if reaches end of the wait period.
 *
 * @param from    Output parameter for the source queue (where the event was
 *                dequeued from). Ignored if NULL.
 * @param wait    Minimum time to wait for an event. Waits infinitely, if set to
 *                ODP_SCHED_WAIT. Does not wait, if set to ODP_SCHED_NO_WAIT.
 *                Use odp_schedule_wait_time() to convert time to other wait
 *                values.
 *
 * @return Next highest priority event
 * @retval ODP_EVENT_INVALID on timeout and no events available
 */
odp_event_t odp_schedule(odp_queue_t *from, uint64_t wait);

/**
 * Schedule multiple events
 *
 * Like odp_schedule(), but returns multiple events from a queue.
 *
 * @param from    Output parameter for the source queue (where the event was
 *                dequeued from). Ignored if NULL.
 * @param wait    Minimum time to wait for an event. Waits infinitely, if set to
 *                ODP_SCHED_WAIT. Does not wait, if set to ODP_SCHED_NO_WAIT.
 *                Use odp_schedule_wait_time() to convert time to other wait
 *                values.
 * @param events  Event array for output
 * @param num     Maximum number of events to output
 *
 * @return Number of events outputed (0 ... num)
 */
int odp_schedule_multi(odp_queue_t *from, uint64_t wait, odp_event_t events[],
		       int num);

/**
 * Pause scheduling
 *
 * Pause global scheduling for this thread. After this call, all schedule calls
 * will return only locally pre-scheduled events (if any). User can exit the
 * schedule loop only after the schedule function indicates that there's no more
 * (pre-scheduled) events.
 *
 * Must be used with odp_schedule() and odp_schedule_multi() before exiting (or
 * stalling) the schedule loop.
 */
void odp_schedule_pause(void);

/**
 * Resume scheduling
 *
 * Resume global scheduling for this thread. After this call, all schedule calls
 * will schedule normally (perform global scheduling).
 */
void odp_schedule_resume(void);

/**
 * Release the current atomic context
 *
 * This call is valid only for source queues with atomic synchronisation. It
 * hints the scheduler that the user has completed processing of the critical
 * section (which depends on the atomic synchronisation). The scheduler is now
 * allowed to schedule events from the same queue to some other thread.
 *
 * Early atomic context release may increase parallelism and thus system
 * performance, but user needs to design carefully the split into critical vs.
 * non-critical sections.
 */
void odp_schedule_release_atomic(void);

/**
 * Number of scheduling priorities
 *
 * @return Number of scheduling priorities
 */
int odp_schedule_num_prio(void);

/**
 * Schedule group create
 *
 * Creates a schedule group with the thread mask. Only threads in the
 * mask will receive events from a queue that belongs to the schedule group.
 * Thread masks of various schedule groups may overlap. There are predefined
 * groups such as ODP_SCHED_GROUP_ALL and ODP_SCHED_GROUP_WORKER, which are
 * always present and automatically updated. Group name is optional
 * (may be NULL) and can have ODP_SCHED_GROUP_NAME_LEN characters in maximum.
 *
 * @param name    Schedule group name
 * @param mask    Thread mask
 *
 * @return Schedule group handle
 * @retval ODP_SCHED_GROUP_INVALID on failure
 *
 * @see ODP_SCHED_GROUP_ALL, ODP_SCHED_GROUP_WORKER
 */
odp_schedule_group_t odp_schedule_group_create(const char *name,
					       const odp_thrmask_t *mask);

/**
 * Schedule group destroy
 *
 * Destroys a schedule group. All queues belonging to the schedule group must
 * be destroyed before destroying the group. Other operations on this group
 * must not be invoked in parallel.
 *
 * @param group   Schedule group handle
 *
 * @retval 0 on success
 * @retval <0 on failure
 */
int odp_schedule_group_destroy(odp_schedule_group_t group);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

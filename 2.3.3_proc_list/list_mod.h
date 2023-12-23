#ifndef _LIST_MOD_H
#define _LIST_MOD_H

#include <linux/slab.h>
#include <linux/list.h>
#include <linux/sched/signal.h>

struct task_info {
	pid_t pid;
	unsigned long timestamp;
	struct list_head list;
};

struct task_info *task_info_alloc(int pid);
void task_info_add_to_list(int pid);
void task_info_add_for_current(void);
void task_info_print_list(const char *msg);
void task_info_purge_list(void);

#endif
#include "list_mod.h"

extern struct list_head head;

void task_info_add_to_list(int pid)
{
	struct task_info *ti;

	/* Allocate item and then add to list. */
	ti = task_info_alloc(pid);
	list_add(&ti->list, &head);
}
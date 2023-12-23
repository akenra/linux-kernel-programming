#include "list_mod.h"

void task_info_add_for_current(void)
{
	struct task_struct *t;

	/* Current PID */
	task_info_add_to_list(current->pid);

	/* Parent PID */
	task_info_add_to_list(current->parent->pid);

	/* Next process PID */
	t = next_task(current);
	task_info_add_to_list(t->pid);

	/* Next process of next process PID */
	t = next_task(t);
	task_info_add_to_list(t->pid);
}
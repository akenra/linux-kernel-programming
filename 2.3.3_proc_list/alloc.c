#include "list_mod.h"

struct task_info *task_info_alloc(int pid)
{
	struct task_info *ti;

	ti = kmalloc(sizeof(*ti), GFP_KERNEL);
	ti->pid = pid;
	ti->timestamp = jiffies;

	return ti;
}
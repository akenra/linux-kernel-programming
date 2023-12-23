#include "list_mod.h"

extern struct list_head head;

void task_info_purge_list(void)
{
	struct list_head *i, *tmp;

	list_for_each_safe(i, tmp, &head) {
		list_del(i);
	}
}
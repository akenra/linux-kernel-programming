#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include "list_mod.h"

MODULE_DESCRIPTION("Task info processor");
MODULE_LICENSE("GPL");

struct list_head head;

static int list_init(void)
{
	INIT_LIST_HEAD(&head);

	task_info_add_for_current();

	return 0;
}

static void list_exit(void)
{
	task_info_print_list("before exiting");
	task_info_purge_list();
}

module_init(list_init);
module_exit(list_exit);
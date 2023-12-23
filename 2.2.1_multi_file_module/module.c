#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/init.h>
#include "module.h"

static int __init multifile_module_init(void)
{
    
    pr_info("Initializing multi-file module\n");

	int result = sum(22, 33);

    pr_info("Sum of numbers: %d\n", result);

	return 0;
}

static void __exit multifile_module_exit(void)
{    
    pr_info("Multi-line module unloaded\n");
}

module_init(multifile_module_init);
module_exit(multifile_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Multi-line Linux kernel module");
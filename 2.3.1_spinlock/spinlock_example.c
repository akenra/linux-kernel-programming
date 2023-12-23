#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Spinlock Example");

static DEFINE_SPINLOCK(my_spinlock);

static int __init spinlock_example_init(void)
{
    pr_info("Spinlock Example: Module initialized\n");

    pr_info("Acquiring spinlock\n");
    spin_lock(&my_spinlock);
	set_current_state(TASK_INTERRUPTIBLE);

    pr_info("Holding spinlock. Performing critical section operations...\n");

    /* Simulate some work here by sleeping for 5s */
	schedule_timeout(5 * HZ);
    pr_info("Critical section operations complete\n");

    pr_info("Releasing spinlock\n");
    spin_unlock(&my_spinlock);

    return 0;
}

static void __exit spinlock_example_exit(void)
{
    pr_info("Spinlock Example: Module exiting\n");
}

module_init(spinlock_example_init);
module_exit(spinlock_example_exit);
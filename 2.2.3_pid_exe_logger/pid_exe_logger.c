#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Process Identifier and Executable Logger");

static int __init pid_exe_logger_init(void)
{
    struct task_struct *task_list;

    pr_info("PID and Executable Logger\n");
    pr_info("---------------------------------------\n");

    // Go through each task in the system and print it
    for_each_process(task_list) {
        pr_info("PID: %d, Exe: %s\n", task_list->pid, task_list->comm);
    }

    return 0;
}

static void __exit pid_exe_logger_exit(void)
{
    pr_info("---------------------------------------\n");
    pr_info("Exiting PID  and Executable Logger\n");
}

module_init(pid_exe_logger_init);
module_exit(pid_exe_logger_exit);
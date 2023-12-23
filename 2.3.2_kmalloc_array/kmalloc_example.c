#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Memory Allocation Example");

static int num_elements = 10;
static int* my_array = NULL;

static int __init mem_alloc_init(void)
{
    pr_info("Memory Allocation Example: Module initialized\n");

    pr_info("Allocating memory for array\n");
    my_array = kmalloc_array(num_elements, sizeof(int), GFP_KERNEL);

    if (my_array == NULL) {
        pr_err("Failed to allocate memory\n");
        return -ENOMEM;
    }

    pr_info("Memory allocated successfully\n");

    int i;
    for (i = 0; i < num_elements; ++i) {
        my_array[i] = i;
    }

    pr_info("Array values:");
    for (i = 0; i < num_elements; ++i) {
        pr_info("%d ", my_array[i]);
    }
    pr_info("\n");

    return 0;
}

static void __exit mem_alloc_exit(void)
{
    pr_info("Freeing memory\n");
    
    kfree(my_array);

    pr_info("Memory Allocation Example: Module exiting\n");
}

module_param(num_elements, int, S_IRUGO);
MODULE_PARM_DESC(num_elements, "Number of elements in the array");

module_init(mem_alloc_init);
module_exit(mem_alloc_exit);
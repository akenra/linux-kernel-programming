#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
 
static char *hello_str = "World";
module_param(hello_str, charp, S_IRUGO);

static int __init hello_mod_init(void) 
{ 
    pr_info("Hello, %s\n", hello_str);
    return 0; 
} 
 
static void __exit hello_mod_exit(void) 
{ 
    pr_info("Goodbye, %s\n", hello_str);
} 
 
module_init(hello_mod_init); 
module_exit(hello_mod_exit); 
 
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A sample module with a string parameter");
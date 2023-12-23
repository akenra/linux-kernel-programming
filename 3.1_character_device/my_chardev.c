#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/types.h>

#define MY_CDEV_NAME "my_cdev"
#define BUFFER_SIZE 1024

static dev_t device_number;
static struct cdev my_cdev;
static char device_buffer[BUFFER_SIZE];

/**
 * @brief Read callback function for the character device.
 *
 * @param file - Pointer to the file structure.
 * @param buf - User buffer to store the data read from the device.
 * @param count - Number of bytes to read.
 * @param ppos - Pointer to the file position offset.
 * @return ssize_t - Number of bytes read or an error code on failure.
 */
static ssize_t my_cdev_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    ssize_t bytes_read;

    // If the read position is beyond the end of the data, return EOF condition
    if (*ppos >= BUFFER_SIZE)
        return 0;

    // Calculate the number of bytes to read, taking into account any partial reads
    bytes_read = min_t(size_t, count, (size_t)(BUFFER_SIZE - *ppos));

    // Copy data from the device buffer to the user buffer
    if (copy_to_user(buf, device_buffer + *ppos, bytes_read))
        return -EFAULT;  // Oops! Something went wrong when copying data to user space

    // Update the read position
    *ppos += bytes_read;

    return bytes_read;
}

/**
 * @brief Write callback function for the character device.
 *
 * @param file - Pointer to the file structure.
 * @param buf - User buffer containing the data to be written.
 * @param count - Number of bytes to write.
 * @param ppos - Pointer to the file position offset.
 * @return ssize_t - Number of bytes written or an error code on failure.
 */
static ssize_t my_cdev_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    ssize_t bytes_written;

    // If the write position is beyond the end of the buffer, return "no space left on device"
    if (*ppos >= BUFFER_SIZE)
        return -ENOSPC;

    // Calculate the number of bytes to write, taking into account the buffer size
    bytes_written = min_t(size_t, count, (size_t)(BUFFER_SIZE - *ppos));

    // Copy data from the user buffer to the device buffer
    if (copy_from_user(device_buffer + *ppos, buf, bytes_written))
        return -EFAULT;  // Oops! Something went wrong when copying data from user space

    // Update the write position
    *ppos += bytes_written;

    return bytes_written;
}

static const struct file_operations my_cdev_fops = {
    .owner = THIS_MODULE,
    .read = my_cdev_read,
    .write = my_cdev_write,
};

static int __init my_chardev_init(void)
{
    int ret;

    // Allocate a range of character device numbers, in this task we allocate just 1
    ret = alloc_chrdev_region(&device_number, 0, 1, MY_CDEV_NAME);
    if (ret < 0) {
        pr_err("Failed to allocate character device number\n");
        return ret;
    }

    // Initialize the character device
    cdev_init(&my_cdev, &my_cdev_fops);
    my_cdev.owner = THIS_MODULE;

    // Add the character device to the system
    ret = cdev_add(&my_cdev, device_number, 1);
    if (ret < 0) {
        pr_err("Failed to add character device\n");
        // Release the allocated character device numbers for cleanup
        unregister_chrdev_region(device_number, 1);
        return ret;
    }

    pr_info("Registered %s device with major number %d\n", MY_CDEV_NAME, MAJOR(device_number));

    return 0;
}

static void __exit my_chardev_exit(void)
{
    // Remove the character device from the system
    cdev_del(&my_cdev);

    // Release the allocated character device numbers
    unregister_chrdev_region(device_number, 1);

    pr_info("Unregistered %s device\n", MY_CDEV_NAME);
}

module_init(my_chardev_init);
module_exit(my_chardev_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Example kernel module for my_cdev");
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

MODULE_DESCRIPTION("Simple no-dev filesystem");
MODULE_LICENSE("GPL");

#define MYFS_BLOCKSIZE 4096
#define MYFS_BLOCKSIZE_BITS 12
#define MYFS_MAGIC 0xbeefcafe
#define LOG_LEVEL KERN_ALERT

static int myfs_mknod(struct inode *dir,
					  struct dentry *dentry, umode_t mode, dev_t dev);
static int myfs_create(struct inode *dir, struct dentry *dentry,
					   umode_t mode, bool excl);
static int myfs_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode);

static const struct super_operations myfs_super_ops = {
	.statfs = simple_statfs};

static const struct inode_operations myfs_dir_inode_operations = {
	/* Unimplemented in this task */
};

static const struct file_operations myfs_file_operations = {
	/* Unimplemented in this task */
};

static const struct inode_operations myfs_file_inode_operations = {
	/* Unimplemented in this task */
};

static const struct address_space_operations myfs_aops = {
	/* Unimplemented in this task */
};

/**
 * myfs_get_inode - Create a new inode for the filesystem
 * @sb: The super_block structure for the filesystem
 * @dir: The parent directory inode (unused in this implementation)
 * @mode: The mode of the inode (file type and permissions)
 *
 * This function creates a new inode and initializes its fields based on the
 * provided mode and the current user's UID and GID. The inode's timestamps,
 * inode number, and address space operations are set accordingly.
 *
 * Returns:
 *   - Pointer to the new inode on success
 *   - NULL on failure
 */
struct inode *myfs_get_inode(struct super_block *sb, const struct inode *dir,
							 int mode)
{
	struct inode *inode = new_inode(sb);

	if (!inode)
		return NULL;

	/* fill inode structure */
	inode->i_mode = mode;
	inode->i_uid.val = current_uid().val;
	inode->i_gid.val = current_gid().val;
	inode->i_atime = inode->i_ctime = inode->i_mtime = current_time(inode);
	inode->i_ino = get_next_ino();
	inode->i_blocks = 0;
	inode->i_mapping->a_ops = &myfs_aops;

	if (S_ISDIR(mode))
	{
		/* set inode operations for dir inodes. */
		inode->i_op = &myfs_dir_inode_operations;
		inode->i_fop = &simple_dir_operations;
		inc_nlink(inode);
	}

	if (S_ISREG(mode))
	{
		inode->i_op = &myfs_file_inode_operations;
		inode->i_fop = &myfs_file_operations;
	}

	return inode;
}

/**
 * myfs_mknod - Create a new special file
 * @dir: The parent directory inode
 * @dentry: The dentry for the new special file
 * @mode: The mode of the file (file type and permissions)
 * @dev: The device number for the special file (unused in this implementation)
 *
 * This function is called when creating a new special file (e.g., device file).
 * In this implementation, the function is not implemented, and it returns
 * a permission error.
 *
 * Returns:
 *   - -EPERM indicating a permission error
 */
static int myfs_mknod(struct inode *dir, struct dentry *dentry, umode_t mode, dev_t dev)
{
	/* Unimplemented in this task */
	return -EPERM;
}

/**
 * myfs_create - Create a new regular file
 * @dir: The parent directory inode
 * @dentry: The dentry for the new regular file
 * @mode: The mode of the file (file type and permissions)
 * @excl: Whether the create operation is exclusive or not (unused in this implementation)
 *
 * This function is called when creating a new regular file. In this implementation,
 * the function is not implemented, and it returns a permission error.
 *
 * Returns:
 *   - -EPERM indicating a permission error
 */
static int myfs_create(struct inode *dir, struct dentry *dentry, umode_t mode, bool excl)
{
	/* Unimplemented in this task */
	return -EPERM;
}

/**
 * myfs_mkdir - Create a new directory
 * @dir: The parent directory inode
 * @dentry: The dentry for the new directory
 * @mode: The mode of the directory (file type and permissions)
 *
 * This function is called when creating a new directory. In this implementation,
 * the function is not implemented, and it returns a permission error.
 *
 * Returns:
 *   - -EPERM indicating a permission error
 */
static int myfs_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	/* Unimplemented in this task */
	return -EPERM;
}

/**
 * myfs_fill_super - Fill the super_block structure for the filesystem
 * @sb: The super_block structure
 * @data: Data specific to the filesystem (unused in this implementation)
 * @silent: Flag indicating silent mount or not (unused in this implementation)
 *
 * This function is called during filesystem mount to fill the super_block structure.
 * It sets the block size and magic number for the filesystem, assigns the super_operations
 * structure with file system operations, initializes the root inode, and sets the root dentry
 * for the super_block.
 *
 * Returns:
 *   - 0 on success
 *   - -ENOMEM if memory allocation fails
 */
static int myfs_fill_super(struct super_block *sb, void *data, int silent)
{
	struct inode *root_inode;
	struct dentry *root_dentry;

	/* fill super_block */
	sb->s_blocksize = MYFS_BLOCKSIZE;
	sb->s_blocksize_bits = MYFS_BLOCKSIZE_BITS;
	sb->s_magic = MYFS_MAGIC;
	sb->s_op = &myfs_super_ops;
	sb->s_maxbytes = MAX_LFS_FILESIZE;

	/* mode = directory & access rights (755) */
	root_inode = myfs_get_inode(sb, NULL,
								S_IFDIR | S_IRWXU | S_IRGRP |
									S_IXGRP | S_IROTH | S_IXOTH);

	printk(LOG_LEVEL "root inode has %d link(s)\n", root_inode->i_nlink);

	if (!root_inode)
		return -ENOMEM;

	root_dentry = d_make_root(root_inode);
	if (!root_dentry)
		goto out_no_root;
	sb->s_root = root_dentry;

	return 0;

out_no_root:
	iput(root_inode);
	return -ENOMEM;
}

/**
 * myfs_mount - Mount the filesystem
 * @file_system_type: The file system type structure
 * @flags: The mount flags
 * @dev_name: The device name
 * @data: Data specific to the filesystem (unused in this implementation)
 * This function is called to mount the filesystem on the specified device.
 * In this implementation, it simply calls the generic mount function.
 * Returns:
 * 	- Pointer to the super_block on success
 * 	- NULL on failure
 */
static struct dentry *myfs_mount(struct file_system_type *fs_type,
								 int flags, const char *dev_name, void *data)
{
	return mount_nodev(fs_type, flags, data, myfs_fill_super);
}

/* define file_system_type structure */
static struct file_system_type myfs_fs_type = {
	.owner = THIS_MODULE,
	.name = "myfs",
	.mount = myfs_mount,
	.kill_sb = kill_litter_super,
};

static int __init myfs_init(void)
{
	int err;

	err = register_filesystem(&myfs_fs_type);
	if (err)
	{
		printk(LOG_LEVEL "register_filesystem failed\n");
		return err;
	}

	return 0;
}

static void __exit myfs_exit(void)
{
	unregister_filesystem(&myfs_fs_type);
}

module_init(myfs_init);
module_exit(myfs_exit);
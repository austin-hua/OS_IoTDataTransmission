#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/workqueue.h>
#include <net/sock.h>
#include <net/tcp.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");

static loff_t my_llseek(struct file *filp, loff_t off, int whence);
static ssize_t my_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t my_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);
static long my_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);
static int my_open(struct inode *inode, struct file *file);
static int my_close(struct inode *inode, struct file *file);
static ssize_t miniex_recv(struct socket *csock, char *buf, size_t size);
static ssize_t miniex_send(struct socket *csock, char *buf, size_t size);
static void miniex_work_handler(struct work_struct *work);

static struct file_operations miniex_ops = {
	.owner = THIS_MODULE,
	.llseek = my_llseek,
	.read = my_read,
	.write = my_write,
	.unlocked_ioctl = my_ioctl,
	.open = my_open,
	.release = my_close
};

static dev_t devno;
static struct class *miniex_cl;
static struct cdev miniex_dev;
static struct workqueue_struct *wq;
DECLARE_WORK(miniex_work, miniex_work_handler);
DECLARE_WAIT_QUEUE_HEAD(miniex_wait);
static struct socket *ssock;
static char sockbuf[4096];
static int datalen;

static int __init initialize(void)
{
	int ret;

	if((ret = alloc_chrdev_region(&devno, 0, 1, "miniex")) < 0){
		printk(KERN_ERR "alloc_chrdev_region returned %d\n", ret);
		return ret;
	}

	if((miniex_cl = class_create(THIS_MODULE, "chardrv")) == NULL){
		printk(KERN_ERR "class_create returned NULL\n");
		ret = -ENOMEM;
		goto class_create_failed;
	}

	if(device_create(miniex_cl, NULL, devno, NULL, "miniex") == NULL){
		printk(KERN_ERR "device_create returned NULL\n");
		ret = -ENOMEM;
		goto device_create_failed;
	}

	cdev_init(&miniex_dev, &miniex_ops);
	miniex_dev.owner = THIS_MODULE;

	if((ret = cdev_add(&miniex_dev, devno, 1)) < 0){
		printk(KERN_ERR "cdev_add returned %d\n", ret);
		goto cdev_add_failed;
	}

	if((wq = create_workqueue("miniex_wq")) == NULL){
		printk(KERN_ERR "create_workqueue returned NULL\n");
		ret = -ENOMEM;
		goto create_workqueue_failed;
	}

	queue_work(wq, &miniex_work);

	printk(KERN_INFO "miniex initialized!\n");

	return 0;

create_workqueue_failed:
	cdev_del(&miniex_dev);
cdev_add_failed:
	device_destroy(miniex_cl, devno);
device_create_failed:
	class_destroy(miniex_cl);
class_create_failed:
	unregister_chrdev_region(devno, 1);

	return ret;
}

static void __exit exiting(void)
{
	if(ssock != NULL){	// FIXME: race condition!?
		ssock->ops->shutdown(ssock, SHUT_RDWR);
	}

	if(wq != NULL)	destroy_workqueue(wq);

	cdev_del(&miniex_dev);
	device_destroy(miniex_cl, devno);
	class_destroy(miniex_cl);
	unregister_chrdev_region(devno, 1);

	printk(KERN_INFO "miniex removed!\n");
}

module_init(initialize);
module_exit(exiting);

static ssize_t miniex_recv(struct socket *csock, char *buf, size_t size)
{
	struct msghdr msg;
	struct iovec iov;
	mm_segment_t oldfs;
	int ret;

	iov.iov_base = (void *)buf;
	iov.iov_len = (__kernel_size_t)size;

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	msg.msg_flags = 0;

	oldfs = get_fs();
	set_fs(KERNEL_DS);
	ret = sock_recvmsg(csock, &msg, size, 0);
	set_fs(oldfs);

	return ret;
}

static ssize_t miniex_send(struct socket *csock, char *buf, size_t size)
{
	struct msghdr msg;
	struct iovec iov;
	mm_segment_t old_fs;
	int ret;

	iov.iov_base = (void *)buf;
	iov.iov_len = (__kernel_size_t)size;

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	msg.msg_flags = 0;

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	ret = sock_sendmsg(csock, &msg, size);
	set_fs(old_fs);
	
	return ret;
}


static void miniex_work_handler(struct work_struct *work)
{
	int ret;
	struct sockaddr_in saddr;
	struct socket *csock = NULL;

	if(ssock != NULL){
		printk(KERN_ERR "ssock != NULL\n");
		return;
	}
	
	if((ret = sock_create_kern(AF_INET, SOCK_STREAM, IPPROTO_TCP, &ssock)) < 0){
		printk(KERN_ERR "sock_create returned %d\n", ret);
		ssock = NULL;
		return;
	}

	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8888);
	saddr.sin_addr.s_addr = INADDR_ANY;

	if((ret = ssock->ops->bind(ssock, (struct sockaddr*)&saddr, sizeof(saddr))) < 0){
		printk(KERN_ERR "bind returned %d\n", ret);
		sock_release(ssock);
		ssock = NULL;
		return;
	}

	if((ret = ssock->ops->listen(ssock, 1)) < 0){
		printk(KERN_ERR "listen returned %d\n", ret);
		ssock->ops->release(ssock);
		sock_release(ssock);
		ssock = NULL;
		return;
	}

	while(1){
		if((ret = sock_create_kern(AF_INET, SOCK_STREAM, IPPROTO_TCP, &csock)) < 0){
			printk(KERN_ERR "sock_create_lite returned %d\n", ret);
			break;
		}

		if((ssock->ops->accept(ssock, csock, 0)) < 0){
			printk(KERN_ERR "accept returned %d\n", ret);
			sock_release(csock);
			break;
		}

		// got a connection

		ret = miniex_recv(csock, sockbuf, 4096);
		
		if(ret > 0){
			sockbuf[ret] = 0;
			printk(KERN_INFO "recv: %s", sockbuf);
			miniex_send(csock, sockbuf, ret);	// echo
			datalen = ret;
			wake_up_interruptible(&miniex_wait);
		}

		csock->ops->shutdown(csock, SHUT_RDWR);
		csock->ops->release(csock);
		sock_release(csock);
	}

	ssock->ops->release(ssock);
	sock_release(ssock);
	ssock = NULL;
}

static long my_ioctl(struct file *file,unsigned int ioctl_num, unsigned long ioctl_param)
{
	long ret = -EINVAL;

	switch(ioctl_num){
		default:
			break;
	}

	return ret;
}

static int my_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
	return 0;
}

static loff_t my_llseek(struct file *filp, loff_t off, int whence)
{
	return -1;
}

static ssize_t my_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	int rlen;

	if(wait_event_interruptible(miniex_wait, datalen > 0) != 0)	return -ERESTARTSYS;

	rlen = (count < datalen)?count:datalen;

	if(copy_to_user(buff, sockbuf, rlen))	return -EFAULT;

	datalen = 0;	// FIXME: race condition!?

	return rlen;
}

static ssize_t my_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
	return -1;
}


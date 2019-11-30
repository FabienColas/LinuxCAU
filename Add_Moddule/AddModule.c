#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/sched.h>

enum CONSTANTS{
  //NUM_THREADS = 1,
	NUM_THREADS = 4,
	NUM_ITERS = 100000/NUM_THREADS
};

struct my_node{
	struct list_head list;
	int data;
};

struct params {
  struct list_head list;
  struct my_node *data;
  struct completion comp;
  int segmentNb;
};

int thread_fn(void *args)
{
        struct params *params = (struct params *)args;
	int i;

	/*initialize list*/
	INIT_LIST_HEAD(&params->list);


        for (i = 0; i < NUM_ITERS; i++)
	{
		struct my_node *new = &params->data[i + params->segmentNb * NUM_ITERS];
		new->data = i + params->segmentNb * NUM_ITERS;
        	list_add(&new->list, &params->list);
	}
	complete_and_exit(&params->comp, 0);
        return 0;
}

int __init module_ass6_init(void)
{
	printk("Assignment 6 - Insert Module!\n");
	printk("Beginning");
	
	struct my_node *data = kmalloc(sizeof(struct my_node) * 100000,GFP_KERNEL);
	struct task_struct *threads[NUM_THREADS];
	struct params *params = kmalloc(sizeof(struct params) * NUM_THREADS,GFP_KERNEL);
	int i;

	for (i = 0; i < NUM_THREADS; i++) {
		init_completion(&params[i].comp);
		params[i].data = &data[i * NUM_ITERS];
		params[i].segmentNb = i;
        	threads[i] = kthread_run(&thread_fn, &params[i], "thread_fn");
	}

	for (i = 0; i < NUM_THREADS; i++) {
	  wait_for_completion(&params[i].comp);
	}

	for (i = 1; i < NUM_THREADS; i++) {
		list_splice(&params[i].list, &params[0].list);
	}
	printk("End\n");	
	return 0;
}

void __exit module_ass6_cleanup(void){
	printk("Assignment 6 - Bye Module!\n");
}


module_init(module_ass6_init);
module_exit(module_ass6_cleanup);
MODULE_LICENSE("GPL");

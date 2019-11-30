#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/sched.h>

enum CONSTANTS{
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

int add(void *args)
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

int del(void *args)
{
        struct params *params = (struct params *)args;
        struct my_node *current_node; /* This will point on the actual data structures during the iteration */
        struct list_head *p; /* Temporary variable needed to iterate; */
        int i;

        p = &params->list;
        for (i = 0; i < params->segmentNb; i++) {
                p = p->next;
        }

        for (i = 0; i < NUM_ITERS; i++) {
                current_node = list_entry(p, struct my_node, list);
                if (current_node->data == 99999) {
			kfree(current_node);
                        printk("End Delete\n");
                        break;
                }
                p = p->next->next->next->next;
        }

        return 0;
}

int __init delete_module_init(void){
        printk("Delete Module - Hello Module!\n");

        struct my_node *data = kmalloc(sizeof(struct my_node) * 100000,GFP_KERNEL);
        struct task_struct *threads[NUM_THREADS];
        struct params *params = kmalloc(sizeof(struct params) * NUM_THREADS,GFP_KERNEL);
        int i;

        for (i = 0; i < NUM_THREADS; i++) {
                init_completion(&params[i].comp);
                params[i].data = &data[i * NUM_ITERS];
                params[i].segmentNb = i;
                threads[i] = kthread_run(&add, &params[i], "add");
        }

        for (i = 0; i < NUM_THREADS; i++) {
          wait_for_completion(&params[i].comp);
        }

        for (i = 1; i < NUM_THREADS; i++) {
                list_splice(&params[i].list, &params[0].list);
        }

        printk("Beginning\n");
        printk("Start Delete\n");

        for (i = 0; i < NUM_THREADS; i++) {
                params[i].segmentNb = i;
                params[i].list = params[0].list;
                threads[i] = kthread_run(&del, &params[i], "delete");
        }

        printk("End\n");

        return 0;
}

void __exit delete_module_cleanup(void){
        printk("Delete Module - Bye Module!\n");
}

module_init(delete_module_init);
module_exit(delete_module_cleanup);




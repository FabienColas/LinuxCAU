#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/sched.h>

static struct task_struct *thread1, *thread2, *thread3, *thread4;// *thread5, *thread6, *thread7, *threa$


enum CONSTANTS{
        NUM_THREADS = 4,
        NUM_ITERS = 100000/NUM_THREADS
};

struct my_node{
        struct list_head list;
        int data;
};

int search(void)
{
        struct list_head my_list;

        /*initialize list*/
        INIT_LIST_HEAD(&my_list);

        /*list element add*/
        int i;
        struct my_node *news = kmalloc(sizeof(struct my_node)*NUM_ITERS,GFP_KERNEL);

        for(i=0; i<NUM_ITERS; i++)
        {

                /*if(i==0) {
                        printk("Beginning, i=0");
                }*/
                struct my_node *new = &news[i];
                new->data=i;
                list_add(&new->list,&my_list);
                /*if(i==24999) {
                        printk("End, i=24999");
                }*/
        }

        struct my_node *current_node; /* This will point on the actual data structures during the iteration */
        struct list_head *p; /* Temporary variable needed to iterate; */

        list_for_each_entry(current_node, &my_list, list) {
                if (current_node->data == 99999)
                        printk("End Search\n");
        }

	return 0;
}

int __init search_module_init(void){
        printk("Search Module - Hello Module!\n");
        printk("Beginning");
        thread1 = kthread_create(search(), NULL,"search");
        thread2 = kthread_create(search(), NULL,"search");
        thread3 = kthread_create(search(), NULL,"search");
        thread4 = kthread_create(search(), NULL,"search");
        /*thread5 = kthread_create(thread_fn(), NULL,"thread");
        thread6 = kthread_create(thread_fn(), NULL,"thread");
        thread7 = kthread_create(thread_fn(), NULL,"thread");
        thread8 = kthread_create(thread_fn(), NULL,"thread");
        thread9 = kthread_create(thread_fn(), NULL,"thread");
        thread10 = kthread_create(thread_fn(), NULL,"thread");
*/
        kthread_bind(thread1, NULL);
        kthread_bind(thread2, NULL);
        kthread_bind(thread3, NULL);
        kthread_bind(thread4, NULL);
/*      kthread_bind(thread5, NULL);
        kthread_bind(thread6, NULL);
        kthread_bind(thread7, NULL);
        kthread_bind(thread8, NULL);
        kthread_bind(thread9, NULL);
        kthread_bind(thread10, NULL);
*/

        printk("End");

        kthread_stop(thread1);
        kthread_stop(thread2);
        kthread_stop(thread3);
        kthread_stop(thread4);

        return 0;
}

void __exit search_module_cleanup(void){
        printk("Search Module - Bye Module!\n");
}

module_init(search_module_init);
module_exit(search_module_cleanup);

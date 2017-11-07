#include "mbed.h"
#include "rtos.h"
#include <stdio.h>
#include <string>
#include <string.h>
using namespace std;

typedef struct {
    float    voltage;
    float    current;
    uint32_t counter;
} message_t;

typedef struct {
	char  *subj;
} mail_t;

DigitalOut led1(LED1);

Queue<message_t, sizeof(message_t*)> queue;
MemoryPool<message_t, sizeof(message_t*)> queue_mem_pool;

Mail<mail_t, sizeof(mail_t*)> mail_box;
MemoryPool<mail_t, sizeof(mail_t*)> mail_mem_pool;

int gA=1;
int gB;
const int C = 10;

static void myHeap_Address()
{
	//pc.printf("address gA: %d\n", &gA);
 	//pc.printf("address gB: %d\n", &gB);
 	//pc.printf("address constant C: %d\n", &C);
}

void Th_print_1(uint32_t i) {
	message_t *message = queue_mem_pool.alloc();
	mail_t *mail = mail_mem_pool.alloc();

	message->voltage = (i * 0.1) * 33;
	message->current = (i * 0.1) * 11;
	message->counter = i;
	queue.put(message);


	mail->subj = "U-blox";
    mail_box.put(mail);
}

void Th_print_2(void) {
	//message_t *message_rec = queue_mem_pool.alloc();
	//mail_t *mail_rec = mail_mem_pool.alloc();
	osEvent evt;

    evt = queue.get();

    if (evt.status == osEventMessage) {
    	message_t *message_rec = (message_t*)evt.value.p;

        printf("\nVoltage: %.2f V\n\r", message_rec->voltage);
        printf("Current: %.2f A\n\r"  , message_rec->current);
        printf("Counter: %u\n\r"	  , message_rec->counter);

        queue_mem_pool.free(message_rec);
    }

    evt = mail_box.get();

    if (evt.status == osEventMail) {
    	mail_t *mail_rec = (mail_t*)evt.value.p;

        printf("\nName: %s \n\r"   , mail_rec->subj);

        mail_mem_pool.free(mail_rec);
    }
}

void test_thread_1(void const *args) {
	uint32_t i = 0;
	while(true){
		i++;
		Th_print_1(i);
		Thread::wait(50);
	}
}

void test_thread_2(void const *args) {

	while(true){
		Th_print_2();
		Thread::wait(50);
	}
}


int main (void) {
    Thread t1(osPriorityHigh);
    Thread t2(osPriorityHigh);

    t1.start(callback(test_thread_1, (void *)"Th 1"));
    t2.start(callback(test_thread_2, (void *)"Th 2"));


    while (true) {
    	myHeap_Address();
    }
}


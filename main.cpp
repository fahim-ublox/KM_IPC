
#include "mbed.h"
#include "rtos.h"

Mutex mutex;

void Th_print_1(const char* name, int state) {
	mutex.lock();
    printf("Thread 1\n\r");
    mutex.unlock();
}

void Th_print_2(const char* name, int state) {
	mutex.lock();
    printf("Thread 1\n\r");
    mutex.unlock();
}

void test_thread_1(void const *args) {
    while (true) {
    	Th_print_1((const char*)args, 0);
        Thread::wait(1000);
    }
}

void test_thread_2(void const *args) {
    while (true) {
    	Th_print_2((const char*)args, 0);
        Thread::wait(1000);
    }
}

int main() {
    Thread t1(test_thread_1);
    Thread t2(test_thread_2);

    test_thread_1((void *)"Th 1");
    test_thread_2((void *)"Th 2");
}

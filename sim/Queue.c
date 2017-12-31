#include "Queue.h"

extern iq* iq_arr;

void enqueue(inst* data) {

	if (iq_arr->last == MAX_ITEMS - 1) {
		iq_arr->last = -1;
	}

	iq_arr->inst_arr[++iq_arr->last] = data;
	iq_arr->num_items++;

}

inst* dequeue() {
	inst* data = iq_arr->inst_arr[iq_arr->front++];

	if (iq_arr->front == MAX_ITEMS) {
		iq_arr->front = 0;
	}

	iq_arr->num_items--;
	return data;
}

bool is_queue_full()
{
	return iq_arr->num_items == MAX_ITEMS;
}

inst* peek() {
	inst* res = &iq_arr->inst_arr[iq_arr->front];
	return res;
}
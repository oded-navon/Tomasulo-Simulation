#include "Utils.h"



inst peek(iq iq_arr) {
	return iq_arr.inst_arr[iq_arr.front];
}


void enqueue(iq iq_arr,inst data) {

	if (iq_arr.last == MAX_ITEMS - 1) {
		iq_arr.last = -1;
	}

	iq_arr.inst_arr[++iq_arr.last] = data;
	iq_arr.num_items++;

}
bool is_queue_full(iq iq_arr)
{
	return iq_arr.num_items == MAX_ITEMS;
}

inst dequeue(iq iq_arr) {
	inst data = iq_arr.inst_arr[iq_arr.front++];

	if (iq_arr.front == MAX_ITEMS) {
		iq_arr.front = 0;
	}

	iq_arr.num_items--;
	return data;
}
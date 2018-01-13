#include "Queue.h"

extern inst_queue* _iq_arr;

//Inserting an item to the head of the list
void enqueue(inst* data) 
{
	if (is_queue_full())
	{
		return;
	}
	queue_node_t* new_node = malloc(sizeof(queue_node_t));
	new_node->node_inst = data;

	//In case the queue was empty
	if (is_queue_empty())
	{
		new_node->next = NULL;
		new_node->prev = NULL;
		_iq_arr->head = new_node;
		_iq_arr->tail = new_node;
		_iq_arr->num_items_in_queue++;
		return;
	}

	//In case the queue only had one item
	if (queue_has_single_item())
	{
		new_node->next = _iq_arr->tail;
		_iq_arr->tail->prev = new_node;
		new_node->prev = NULL;
		_iq_arr->head = new_node;
		_iq_arr->num_items_in_queue++;
		return;
	}

	//In case we have more than 1 items in the queue
	new_node->next = _iq_arr->head;
	new_node->prev = NULL;
	_iq_arr->head->prev = new_node;
	_iq_arr->head = new_node;
	_iq_arr->num_items_in_queue++;
}

//Removing an item from the tail of the list
void dequeue() 
{
	//In case the queue was empty
	if (is_queue_empty())
	{
		return;
	}

	//In case the queue only had one item
	if (queue_has_single_item())
	{
		_iq_arr->tail = NULL;
		free(_iq_arr->head->node_inst);
		free(_iq_arr->head);
		_iq_arr->head = NULL;
		_iq_arr->num_items_in_queue = 0;
		return;
	}
	
	//In case the queue has more than 1 items
	queue_node_t* tail_to_delete = _iq_arr->tail;
	//Detach the tail from the list
	_iq_arr->tail->prev->next = NULL;
	//Make the item before the tail, the new tail
	_iq_arr->tail = _iq_arr->tail->prev;
	//Free the deleted item resources
	free(tail_to_delete->node_inst);
	free(tail_to_delete);
	_iq_arr->num_items_in_queue--;
}

bool is_queue_full()
{
	return _iq_arr->num_items_in_queue == _iq_arr->queue_max_size;
}

bool is_queue_empty()
{
	return _iq_arr->num_items_in_queue == 0;
}

bool queue_has_single_item()
{
	return _iq_arr->num_items_in_queue == 1 && _iq_arr->head == _iq_arr->tail;
}

inst* peek_queue_tail() 
{
	inst* res = NULL;
	if (!is_queue_empty())
	{
		res = _iq_arr->tail->node_inst;
	}
	return res;
}


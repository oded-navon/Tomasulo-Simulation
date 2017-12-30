


#define MAX 16

int intArray[MAX];
int front = 0;
int last = -1;
int itemCount = 0;

int peek() {
	return intArray[front];
}


void enqueue(int data) {

	if (last == MAX - 1) {
		last = -1;
	}

	intArray[++last] = data;
	itemCount++;

}

int dequeue() {
	int data = intArray[front++];

	if (front == MAX) {
		front = 0;
	}

	itemCount--;
	return data;
}
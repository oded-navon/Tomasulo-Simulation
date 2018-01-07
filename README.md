#Tomasulo-Simulation

The program's flow (Main function):

1. The processor's arguments are parsed and inserted into the relevant data structures.
2. The data structures are initialized.
3. The program goes into a loop over the input instructions:
	a. Fetch: fetches instructions from the instructions input into the instructions queue.
	b. Issue: takes the first instruction in the queue and inserts it into a free reservatin station, if found.
	c. Dispatch: Goes over all the calculation units to find a free one and if found, goes over the relevant reservation stations to find a ready instruction.
	d. Execute: Goes over all the calculation units and decrements the timer if the unit is taken, the timer counts the instruction's delay down to 0.
	e. Broadcast: Looks for an occupied calculation unit which finished its delay, executes the instruction and broadcasts the results to the relevant reservation stations. 

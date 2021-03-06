#Tomasulo-Simulation
submitters:
Oded Navon 303066674
Itai Shchorry 305584690
Ran Toledo 200024438

Welcome to Tomasulo-Simulation.
the purpose of this code is to demonstrate the processor-logic algorithm of Tomasulo.

To run the program:
simply build the solution and run it.

there are many notes inside the code explaining the different parts's purposes and flows,
so here we'll present the overall flow of the program.
an important note that's related to the entire program - the program simulates the concurrency of the
different processor's parts with a single thread. this means there were several implementation-related-add-ons
to the different data structures which are used for that purpose.


The program's flow (Main function):

1. The processor's arguments are parsed and inserted into the relevant data structures.
2. The data structures are initialized.
3. The program loops over it's 5 pillars:
	a. Fetch: fetches maximum of 2 instructions from the instructions input into the instructions queue.
	   Fetching happens only if there's room in the IQ and there are instructions to be fetched.
	b. Issue: Issues a maximum of 2 instructions from the IQ to the reservation stations and the store buffers,
	   and performs register renaming in case it's needed.
	   Issuing happens only if there are unoccupied load/store buffers or RS's relevant to the instructions opcode.
	c. Dispatch: Dispatches the instructions that are ready for execution to the calculation units and the pipelined memory unit.
	d. Execute: Executes a cycle's worth of work in the memory unit and per each occupied calculation unit
	e. Broadcast: broadcasts over the CDB's the results of the calculation and memory units.
	   Broadcast happens only for finished executions and over available CDB's.

	
the program exists once it's finished completing all the instructions. 
When finished, the different log files memout, regout, tracecdb and traceinst are available for you to read.


We also have a few test cases inside the "sim" project folder available to test the correctness of our implementation.



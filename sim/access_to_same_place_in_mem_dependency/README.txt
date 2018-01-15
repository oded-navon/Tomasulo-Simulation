0100100C
0100304E
00F0004E
the idea is that the first command occupies the only store buffer, and then the load buffer gets issued
before the next store buffer, and both are addressing the same memory.
we want to see that the load buffer "waits" for the store buffer to properly execute thoroughly, and only then
he can start his work in the memory.
expecting to see '3' in Mem[4E],which is 78  

later (after some buffering adding instructions)
02A42000
00A00020
0100B020

we would expect the load operation to wait for the adding to finish to avoid race condition, and
then we'll expect the store to wait for the load
meaning we want to see 11 at Mem[32] and reg[10] = 0


last case (instructions around 24):
02442000
01002040
01007040

we'll want to see the first store waiting for it's value, and the 2nd store waiting for the 1st store to finish
meaning, we'll see a value of 7 at Mem[0x40] 
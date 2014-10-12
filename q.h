/*************************************************************
*
Write a set of Q-ing routines as follows. The routines manipulate a queue and items. Q is a pointer to the first element of the queue (aka head pointer) and the queue is circular, doubly linked. The items are of type  TCB_t (defined later). The routines are:

InitQ (*Q)  //Note that if Q is a head pointer to the queue, then InitQ will have to be passed &Q.
AddQ(*Q, *item)
DelQ(*Q) // will return a pointer to the item deleted.
RotateQ(*Q) // deletes the head and adds it to the tail, by just moving the header pointer to the next item.

Please (please) thoroughly double check the correctness of the Q implementation, by using a Q of integers and many tests. Put the final implementation into a file called q.h. Especially test the situation where the last element gets deleted from a Q, and what happens when you try to delete an item from an empty Q.

Test.
Then re-test. 
The Q routines are the cause on 95% of the problems with this and subsequent projects.
 It is the hardest module to write correctly
...
*/
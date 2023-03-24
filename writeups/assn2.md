Assignment 2 Writeup
=============

My name: Taehyeok Ha

My POVIS ID: hth021002

My student ID (numeric): 20210273

This assignment took me about [20] hours to do (including the time on studying, designing, and writing the code).

If you used any part of best-submission codes, specify all the best-submission numbers that you used (e.g., 1, 2): []

- **Caution**: If you have no idea about above best-submission item, please refer the Assignment PDF for detailed description.

Program Structure and Design of the TCPReceiver and wrap/unwrap routines:
[In TCPReceiver, variables that used to check syn and fin of packet is received are added. And add _isn variable that used to save seqno of header. First receiver should check syn of TCPHeader and save this result. First if state of cpp file, check syn status. When I check existing fin flag, this work can solve that not add if state. So, just use "|" operator. Function segement_received have to push substring of segment string. If syn flag is existed, check str_seqno that indicate index of buffer and push them. ackno function is check existing syn flag and return wrap value. When make complete wrapping function, think about how to compare checkpoint and sequence number. And check bit operation of n - wrap(checkpoint, isn)]

Implementation Challenges:
[When calculate bit operation of n, isn, and checkpoint, I can use static_cast from uint32 to int32.]

Remaining Bugs:
[]

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this assignment better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]

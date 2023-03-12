Assignment 1 Writeup
=============

My name: Taehyeok Ha

My POVIS ID: hth021002

My student ID (numeric): 20210273

This assignment took me about [15] hours to do (including the time on studying, designing, and writing the code).

Program Structure and Design of the StreamReassembler:
[ Overolly, the most above design of reassembler is buffer design. Buffer is designed to deque of boolian, charactor pair. Char section of buffer is used to save string input data and Bool section is used to check buffer-state. For consider each index of total file stream, declare unassembled index, unacceptable index, and check bitween of assemble part. If data are saved in buffer, they would transfer to temporary string and write though bytestream.]

Implementation Challenges:
[The most curious concept is eof that used indicate end of file. There are some error case, if end-index of data is not same from assembled end. Protech program from this error, make some if state and check condition that eof section is not over capacity of belong in already assembled section.]

Remaining Bugs:
[]

- Optional: I had unexpected difficulty with: [What is meaning of each variable like index, data, and eof.]

- Optional: I think you could make this assignment better by: [Already perfect]

- Optional: I was surprised by: []

- Optional: I'm not sure about: [What happend in currenctly collision of two file]

Assignment 3 Writeup
=============

My name: Taehyeok Ha

My POVIS ID: hth021002

My student ID (numeric): 20210273

This assignment took me about [50] hours to do (including the time on studying, designing, and writing the code).

If you used any part of best-submission codes, specify all the best-submission numbers that you used (e.g., 1, 2): []

- **Caution**: If you have no idea about above best-submission item, please refer the Assignment PDF for detailed description.

Program Structure and Design of the TCPSender:
[For check being sent segment and ack, decleare additional private variable which is _abs_ackno, rwnd, _segments_outstanding. TCP sender is implemented ack_received first and then implement fill_window. In first, using ack_received, check how many segment get ack signal by check ackno whick can compared unwrap value from _segments_outstanding. In fill_window function, program check available window size which is almost case smaller than general window size and read stream and then send segment.]

Implementation Challenges:
[It is pretty much troblesome that think about available window size which consider the case where sender send segment before check acknowledge number.]

Remaining Bugs:
[Complete all test case but it require many time for consider exceptional case.]

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this assignment better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]

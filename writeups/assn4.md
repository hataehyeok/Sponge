Assignment 4 Writeup
=============

My name: Taehyeok Ha

My POVIS ID: hth021002

My student ID (numeric): 20210273

This assignment took me about [21] hours to do (including the time on studying, designing, and writing the code).

If you used any part of best-submission codes, specify all the best-submission numbers that you used (e.g., 1, 2): []

- **Caution**: If you have no idea about above best-submission item, please refer the Assignment PDF for detailed description.

Your benchmark results (without reordering, with reordering): [1.54, 1.53]

Program Structure and Design of the TCPConnection:
[The important function is segment_received, tick, and send_segment that is private function for send segment from sender's queue to internet. segment_received get TCPSegment and verify rst flag, keep-alive, and ack flag. Design a function using if statements to meet the given conditions and finally execute send_segment for send segment to internet. A keypoint of segment_received is I use the variable '_error_state' which is storing either inbound, outbound streams is error state. It is used to set RST flag of send packet in segment_send function. tick function check time and inspect abort condition _linger_after_streams_finish. Finally, send_segments take sender's outgoing segment queue and send to _segment_out for satisfy while loop condition. And in this loop, _error_state is used.]

Implementation Challenges:
[Find _linger_after_streams_finish condition is too hard. In particular, I struggled to understand concepts like esactive, prerequisite, and linger.]

Remaining Bugs:
[None.]

- Optional: I had unexpected difficulty with: [After midterm exam, I forgot many basic function of TCP, so I don't save deadline.]

- Optional: I think you could make this assignment better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]

Assignment 5 Writeup
=============

My name: Taehyeok Ha

My POVIS ID: hth021002

My student ID (numeric): 20210273

This assignment took me about [10] hours to do (including the time on studying, designing, and writing the code).

If you used any part of best-submission codes, specify all the best-submission numbers that you used (e.g., 1, 2): []

- **Caution**: If you have no idea about above best-submission item, please refer the Assignment PDF for detailed description.

Program Structure and Design of the NetworkInterface:
[
    I use map data structure for saving ARP table and tracking information of ARP protocol such as time since send ARP protocol and remained internet datagram. When send ethernet frame from sender to other node, NetworkInterface need the processes that fill information of frame's header and push to _frames_out. This process is conducted in send_ethernet_frame function. Similer to this process, whenever ARP protocol is transported, using send_arp function.

]

Implementation Challenges:
[Not difficult to understand the ARP process. However, learing interface of ipv4 and ehternet, and parse is requested many time. ]

Remaining Bugs:
[
    The confuse processes that I don't try to understand is erase element of arp_tabletick when called tick function. I was write this code in recv_frame because, this explain is in here.
]

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this assignment better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]

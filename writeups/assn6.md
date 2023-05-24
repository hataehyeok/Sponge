Assignment 6 Writeup
=============

My name: Taehyeok Ha

My POVIS ID: hth021002

My student ID (numeric): 20210273

This assignment took me about [5] hours to do (including the time on studying, designing, and writing the code).

If you used any part of best-submission codes, specify all the best-submission numbers that you used (e.g., 1, 2): []

- **Caution**: If you have no idea about above best-submission item, please refer the Assignment PDF for detailed description.

Program Structure and Design of the Router:
[
    The crusial functions are add_route and route_one_datagram. In add_route, routing table should include route with satisfying the format of table which is contain _route_prefix, _prefix_length, _next_hop, _interface_num, _subnet_mask.
    In next function, router have to check dgram's ttl and find next path. For find next path, route_one_datagram use linear searching algorithm refering O(n).
]

Implementation Challenges:
[
    There are better way to implement longest prefix match algorithm such as using Trie, RadixTree, and Hash table. After I finish assignment 6 with O(n) algorithm and try to make better. But due to I don't have enough time and energe, cannot make complete RadixTree.
]

Remaining Bugs:
[
    RadixTree algorithm do not implement search because still has some bug.
]

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this lab better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]

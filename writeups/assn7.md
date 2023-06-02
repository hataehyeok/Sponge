Assignment 7 Writeup
=============

My name: Taehyeok Ha

My POVIS ID: hth021002

My student ID (numeric): 20210273

My assignment partner's name: Yoonho Kim

My assignment partner's POVIS ID: hjyang093

My assignment partner's ID (numeric): 20200003

This assignment took me about [5] hours to do (including the time on studying, designing, and writing the code).

If you used any part of best-submission codes, specify all the best-submission numbers that you used (e.g., 1, 2): []

- **Caution**: If you have no idea about above best-submission item, please refer the Assignment PDF for detailed description.

Solo portion:
[
    Successfully fintsh that start and end a conversation with another copy of file. When our tcp protocol transfer the file, we can watch the message of 'has been fully acknowledged'. Firstly I fail to transfer file due to IPv4Datagram::serialize: payload is wrong size. To analysis this error, I use 'ls -al /tmp | grep big' and checking transmitted file size. File is sended but not fully, which mean that sender and receiver implement well but in network interface, there is some bug.
    I consider exception code, and revise exception process.
]

Group portion:
[
    Team mate is Yoonho Kim(hjyang093) and we successfully fintsh that start and end a conversation with another copy of file. When we check file's hash value, we can see same value.
]

Creative portion (optional):
[]

Other remarks:
[
    After implement sha256sum /tmp/big.txt, sha256sum /tmp/big-received.txt command, we get same value like 9b74c4c1c79e0a3967d0d312f67485671b8da5119cb741fab0494050fa9d5a0f.
]

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this assignment better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]

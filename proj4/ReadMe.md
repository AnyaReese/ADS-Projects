# Red-black Tree

There is a kind of binary tree named red-black tree in the data structure. It has the following 5 properties:

(1) Every node is either red or black.</br>
(2) The root is black.</br>
(3) All the leaves are NULL nodes and are colored black.</br>
(4) Each red node must have 2 black descends (maybe NULL).</br>
(5) All simple paths from any node x to a descendant leaf have the same number of black nodes.

We call a **non-NULL** node an **internal** node. From property (5) we can define the black-height of a
red-black tree as the number of nodes on the simple path from the root (excluding the root itself)
to any NULL leaf (including the NULL leaf). And we can derive that a red-black tree with black height H has at least $2^H − 1$ internal nodes.

Here comes the question: given a positive $N$, how many distinct red-black trees are there that consist of exactly $N$ internal nodes?

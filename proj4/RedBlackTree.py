import math

# Constants
NODES_MAX = 505  # Maximum number of nodes
OVERFLOW = 1000000007  # A large prime for modulo operations to prevent integer overflow
HEIGHT_MAX = 30  # Adjusted maximum height based on possible maximum black height

# Global variables for dynamic programming results
BlackDP = [[0] * HEIGHT_MAX for _ in range(NODES_MAX)]  # Count of trees with a black root, classified by number of nodes and black height
RedDP = [[0] * HEIGHT_MAX for _ in range(NODES_MAX)]    # Count of trees with a red root, classified by number of nodes and black height

def main():
    n = int(input("Enter the total number of nodes: "))  # Input total number of nodes

    # Initialize base cases, root node don't count into BH, NIL count into BH
    BlackDP[1][1] = 1  # 1 for node number, 1 for black height, 1 for distinct trees number
    BlackDP[2][1] = 2  # 2 for node number, 1 for black height, 2 for distinct trees number
    RedDP[1][1] = 1    # 1 for node number, 1 for black height, 1 for distinct trees number

    # Dynamic programming to fill the table
    for i in range(3, n + 1):
        for j in range(int(math.log2(i + 1) / 2), int(math.log2(i + 1) * 2) + 1): # Iterate through all possible black heights
            for k in range(1, i - 1):
                # for each Black Root Tree, its subtrees can have 1 black root or 1 red root
                # and the black height of left subtree and right subtree must be the same
                BlackDP[i][j] = (BlackDP[i][j] + (((BlackDP[k][j - 1] + RedDP[k][j])
                                                   * (BlackDP[i - 1 - k][j - 1] + RedDP[i - 1 - k][j])) % OVERFLOW)) % OVERFLOW
                # for each Red Root Tree, its subtrees must have black root
                # and the black height of left subtree and right subtree must be the same
                RedDP[i][j] = (RedDP[i][j] + ((BlackDP[k][j - 1] * BlackDP[i - 1 - k][j - 1]) % OVERFLOW)) % OVERFLOW

    # Calculate the final result for trees with a black root at all possible heights
    totalTrees = 0
    for i in range(HEIGHT_MAX):
        totalTrees = (totalTrees + BlackDP[n][i]) % OVERFLOW

    # Output the final result
    print(totalTrees)

if __name__ == "__main__":
    main()


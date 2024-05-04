#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Constants
const int NODES_MAX = 505;        // Maximum number of nodes
const int OVER = 1000000007;  // A large prime for modulo operations to prevent integer overflow
const int HEIGHT_MAX = 30;        // Adjusted maximum height based on possible maximum black height

// Global variables for dynamic programming results
vector<vector<long long> > BlackDP(NODES_MAX, vector<long long>(HEIGHT_MAX, 0)); // Count of trees with a black root
vector<vector<long long> > RedDP(NODES_MAX, vector<long long>(HEIGHT_MAX, 0));   // Count of trees with a red root

int main() {
    int n = 0;  // Input total number of nodes
    cin >> n;

    // Initialize base cases, root node don't count into BH, NIL count into BH
    BlackDP[1][1] = 1;  // 1 for node number, 1 for black height, 1 for distinct trees number
    BlackDP[2][1] = 2;  // 2 for node number, 1 for black height, 2 for distinct trees number
    RedDP[1][1] = 1;    // 1 for node number, 1 for black height, 1 for distinct trees number

    // Dynamic programming to fill the table
    for (int i = 3; i <= n; i++) {
        for (int j = static_cast<int>(log2(i + 1) / 2); j <= static_cast<int>(log2(i + 1) * 2) + 1; j++) {
            for (int k = 1; k < i; ++k) {
                // For each Black Root Tree, its subtrees can have 1 black root or 1 red root
                // And the black height of left subtree and right subtree must be the same
                BlackDP[i][j] = (BlackDP[i][j] + (((BlackDP[k][j - 1] + RedDP[k][j]) *
                                                   (BlackDP[i - 1 - k][j - 1] + RedDP[i - 1 - k][j])) % OVER)) % OVER;
                // For each Red Root Tree, its subtrees must have black root
                // And the black height of left subtree and right subtree must be the same
                RedDP[i][j] = (RedDP[i][j] + ((BlackDP[k][j - 1] * BlackDP[i - 1 - k][j - 1]) % OVER)) % OVER;
            }
        }
    }

    // Calculate the final result for trees with a black root at all possible heights
    long long totalTrees = 0;
    for (int i = 0; i < HEIGHT_MAX; i++) {
        totalTrees = (totalTrees + BlackDP[n][i]) % OVER;
    }

    // Output the final result
    cout << totalTrees << endl;

    return 0;
}

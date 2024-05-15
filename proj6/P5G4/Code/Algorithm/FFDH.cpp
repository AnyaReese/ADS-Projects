#include <bits/stdc++.h>
using namespace std;
#define MAXN 10000
#define MAX_LEVELS 10000

typedef struct {
    int width;
    int height;
    int index;
} Item;
Item items[MAXN];
typedef struct {
    int x;
    int y;
} Position;
Position positions[MAXN];

// Comparison function used for sorting items in non-increasing order of height
bool compareItems(const Item& a, const Item& b) {
    return a.height > b.height;
}

// First-Fit Decreasing Height (FFDH) algorithm for Strip Packing
void strip_packing_FFDH(Item items[], int n, int strip_width, Position positions[]) {
    // Sort items in non-increasing order of height
    std::sort(items, items + n, compareItems);

    int level_heights[MAX_LEVELS] = { 0 };  // Heights of each level
    int level_widths[MAX_LEVELS] = { 0 };   // Widths of each level

    int levels = 1;  // Current number of levels

    for (int i = 0; i < n; i++) {
        bool placed = false;
        // Try placing the item in existing levels
        for (int j = 0; j < levels; j++) {
            if (items[i].width + level_widths[j] <= strip_width) {
                positions[i].x = level_widths[j];
                positions[i].y = level_heights[j];
                level_widths[j] += items[i].width;
                level_heights[j + 1] = max(level_heights[j + 1], level_heights[j] + items[i].height);
                placed = true;
                break;
            }
        }

        if (!placed) {
            // Create a new level and place the item
            positions[i].x = 0;
            positions[i].y = level_heights[levels];
            level_heights[levels + 1] = level_heights[levels] + items[i].height;
            level_widths[levels] = items[i].width;
            levels++;
        }
    }
}

int main() {
    int n; // Number of items
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d%d", &items[i].width, &items[i].height);
        items[i].index = i;
    }

    int strip_width;
    scanf("%d", &strip_width);

    // Apply the First-Fit Decreasing Height (FFDH) algorithm to determine item positions
    strip_packing_FFDH(items, n, strip_width, positions);

    // Print the positions of each item
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (items[j].index == i) {
                cout << "Item " << i + 1 << ": (" << positions[j].x << ", " << positions[j].y << ")\n";
                break;
            }
        }
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
#define MAXN 10000

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


bool compareItems(const Item& a, const Item& b) {
    return a.height > b.height;
}

// Next-Fit Decreasing Height (NFDH) algorithm for Strip Packing
void strip_packing_NFDH(Item items[], int n, int strip_width, Position positions[]) {
    
    // Sort items in descending order of height
    std::sort(items, items + n, compareItems);

    int current_x = 0;  // Current x-coordinate in the strip
    int current_y = 0;  // Current y-coordinate in the strip
    int current_level_height = 0;  // Height of the current level in the strip

    for (int i = 0; i < n; i++) {
       
        // If the item exceeds the strip width, move to the next line
        if (current_x + items[i].width > strip_width) {
            current_x = 0;
            current_y += current_level_height;
            current_level_height = 0;
        }

        // Set the position of the item
        positions[i].x = current_x;
        positions[i].y = current_y;

        // Update the current x-coordinate and level height
        current_x += items[i].width;
        current_level_height = max(current_level_height, items[i].height);
    }
}

int main() {
    int n; 
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d%d", &items[i].width, &items[i].height);
        items[i].index = i;
    }

    int strip_width;
    scanf("%d", &strip_width);

    // Apply the Next-Fit Decreasing Height (NFDH) algorithm to determine item positions
    strip_packing_NFDH(items, n, strip_width, positions);

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

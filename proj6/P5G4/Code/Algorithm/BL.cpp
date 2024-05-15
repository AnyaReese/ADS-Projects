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

// Comparator function to sort items in descending order of width
bool compareItems(const Item& a, const Item& b) {
    return a.width > b.width;
}

// Bottom-Left (BL) variant of Strip Packing algorithm
void strip_packing_BL(Item items[], int n, int strip_width, Position positions[]) {

    // Sort items in descending order of width
    sort(items, items + n, compareItems);

    // Place each item in the strip
    for (int i = 0; i < n; i++) {
        int x = 0; // X-coordinate of item's bottom-left corner
        int current_y = 0; // Y-coordinate of item's bottom-left corner

        // Find a suitable position for the item
        while (true) {
            bool overlap = false;

            // Check for overlap with previously placed items
            for (int j = 0; j < i; j++) {
                if (x < positions[j].x + items[j].width && x + items[i].width > positions[j].x &&
                    current_y < positions[j].y + items[j].height && current_y + items[i].height > positions[j].y) {
                    overlap = true;
                    break;
                }
            }

            // If no overlap, break the loop
            if (!overlap)
                break;

            x++; // Move to the next position

            // If item exceeds the strip width, move to the next row
            if (x + items[i].width > strip_width) {
                x = 0;

                // Find the minimum height of the items in the current row
                int min_height = MAXN;
                for (int j = 0; j <= i; j++) {
                    if (positions[j].y + items[j].height > current_y && positions[j].y + items[j].height < min_height)
                        min_height = positions[j].y + items[j].height;
                }
                current_y = min_height; // Move to the next row
            }
        }

        positions[i].x = x; // Set the item's x-coordinate
        positions[i].y = current_y; // Set the item's y-coordinate
    }
}

int main() {
    int n;
    scanf("%d", &n);

    // Read the dimensions of the items from input
    for (int i = 0; i < n; i++) {
        scanf("%d%d", &items[i].width, &items[i].height);
        items[i].index = i;
    }

    int strip_width;
    scanf("%d", &strip_width);

    // Apply the Strip Packing algorithm to determine item positions
    strip_packing_BL(items, n, strip_width, positions);

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
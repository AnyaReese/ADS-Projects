#include <iostream>
#include <vector>
#include "algorithms.h"
#include "utils.h"
using namespace std;

int main() {
    cout << "Input the number of rectangles and the width of the large bin:\n";
    cin >> n >> given_width;

    cout << "Input the width and height of each rectangle:" << endl;
    vector<Rectangle> rects; // Input rects
    for (int i = 0; i < n; i++) {
        Rectangle rectangle = {};
        cin >> rectangle.width >> rectangle.height;
        sumupArea += rectangle.height * rectangle.width;
        rects.push_back(rectangle);
    }

    testTime(rects);

    return 0;
}

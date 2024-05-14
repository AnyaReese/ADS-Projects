#include <iostream>
#include <vector>

#define enableLog 0 // Enable log

/**
 * Texture Packing is to pack multiple rectangle shaped textures into one large texture.
 * The resulting texture must have a given width and a minimum height.
 * This project requires you to design an approximation algorithm that runs in polynomial time.
 * You must generate test cases of different sizes (from 10 to 10,000) with different distributions
 * of widths and heights. A thorough analysis on all the factors that might affect the
 * approximation ratio of your proposed algorithm is expected.
*/

using namespace std;

class Rectangle {
public:
    double width;
    double height;
};

int n = 0;  // Input total number of bins
double given_width = 0; // Input width of the large bin

double FFDH(vector<Rectangle>& recs);
double NFDH(vector<Rectangle>& recs);
bool cmpBins(const Rectangle& a, const Rectangle& b) {
    return a.height > b.height;
}

int main() {
    double height = 0; // output
    cout << "Input the number of rectangles and the width of the large bin:\n";
    cin >> n >> given_width;

    cout << "Input the width and height of each rectangle:" << endl;
    vector<Rectangle> rects; // Input rects
    for (int i = 0; i < n; i++) {
        Rectangle rectangle = {};
        cin >> rectangle.width >> rectangle.height;
        rects.push_back(rectangle);
    }

    height = FFDH(rects);

    cout << "Height of the large bin: " << height << endl;

}

/**
 * First Fit Decreasing Height
 * @author AnyaReese
 * @param rects: a vector of rectangles
 * @return height
 */
double FFDH(vector<Rectangle>& rects) {
    double height = 0;
    double current_width[n]; // Current width of each bin
    for (int i = 0; i < n; i++) {
        current_width[i] = 0;
    }

    sort(rects.begin(), rects.end(), cmpBins); // Decreasing Height

    int count = 0;
    // Initialize the first bin
    if (enableLog) {
        cout << "\e[33m[Info]: Using FFDH\e[0m" << endl;
        cout << "\e[32mInserting 0th rectangle" << endl;
        cout << "current_width[" << count << "] = " << current_width[count] << "\e[0m" << endl;
    }
    if (rects[0].width > given_width) {
        cout << "\e[33mThe width of the 0th rectangle is larger than the given width of the large bin.\e[0m" << endl;
        return -1;
    }
    current_width[count] = rects[0].width;
    height = rects[0].height;

    for (int i = 1; i < rects.size(); i++) { // traverse each bin
        if (rects[i].width > given_width) {
            cout << "\e[31m[ERROR]: The width of the " << i << "th rectangle is larger than the given width of the large bin.\e[0m" << endl;
            return -1;
        }
        if (enableLog) {
            cout << "\e[32mInserting " << i << "th rectangle" << endl;
            cout << "current_width[" << count << "] = " << current_width[count] << "\e[0m" << endl;
        }
        bool packed = false;
        for (int j = 0; j <= count; j++) { // Traverse bins, First Fit
            if (current_width[j] + rects[i].width <= given_width) {
                current_width[j] += rects[i].width;
                packed = true;
                if (enableLog) {
                    cout << "Current width[" << j << "] = " << current_width[j] << ", rects[" << i << "].width = " << rects[i].width << endl;
                }
                break;
            }
        }
        if (!packed) {
            count++;
            current_width[count] = rects[i].width;
            height += rects[i].height;
            if (enableLog) {
                cout << "Create another bin, current_width[" << count << "] = " << current_width[count];
                cout << ", height = " << height << endl;
            }
        }
    }

    return height;
}

/**
 * Next Fit Decreasing Height
 * @author AnyaReese
 * @param rects
 * @return height
 */
double NFDH(vector<Rectangle>& rects) {
    double height = 0;
    double current_width[n]; // Current width of each bin
    for (int i = 0; i < n; i++) {
        current_width[i] = 0;
    }

    sort(rects.begin(), rects.end(), cmpBins); // Decreasing Height

    int count = 0;
    // Initialize the first bin
    if (enableLog) {
        cout << "\e[33m[Info]: Using NFDH\e[0m" << endl;
        cout << "\e[32mInserting 0th rectangle" << endl;
        cout << "current_width[" << count << "] = " << current_width[count] << "\e[0m" << endl;
    }
    if (rects[0].width > given_width) {
        cout << "\e[33mThe width of the 0th rectangle is larger than the given width of the large bin.\e[0m" << endl;
        return -1;
    }
    current_width[count] = rects[0].width;
    height = rects[0].height;

    for (int i = 1; i < rects.size(); i++) { // traverse each bin
        if (rects[i].width > given_width) {
            cout << "\e[31m[ERROR]: The width of the " << i << "th rectangle is larger than the given width of the large bin.\e[0m" << endl;
            return -1;
        }
        if (enableLog) {
            cout << "\e[32mInserting " << i << "th rectangle" << endl;
            cout << "current_width[" << count << "] = " << current_width[count] << "\e[0m" << endl;
        }
        bool packed = false;
        if (current_width[count] + rects[i].width <= given_width) { // DO NOT traverse bins, Next Fit
            current_width[count] += rects[i].width;
            packed = true;
            if (enableLog) {
                cout << "Current width[" << count << "] = " << current_width[count] << ", rects[" << i << "].width = " << rects[i].width << endl;
            }
        }
        if (!packed) {
            count++;
            current_width[count] = rects[i].width;
            height += rects[i].height;
            if (enableLog) {
                cout << "Create another bin, current_width[" << count << "] = " << current_width[count];
                cout << ", height = " << height << endl;
            }
        }
    }

    return height;
}


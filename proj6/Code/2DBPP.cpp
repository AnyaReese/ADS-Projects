#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
// #include <time.h>

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

/** Classes **/
class Rectangle {
public:
    double x;
    double y;
    double width;
    double height;
};

/** Global Variables **/
int n = 0;  // Input total number of bins
double given_width = 0; // Input width of the large bin

/** Functions **/
double SP(vector<Rectangle>& recs);
double Sleator(vector<Rectangle>& recs);
double FFDH(vector<Rectangle>& recs);
double NFDH(vector<Rectangle>& recs);
double SAS(vector<Rectangle>& recs);
void PackNarrow(vector<Rectangle>& narrow, vector<Rectangle>& wide, double x1, double y1, double x_limit, double y_limit);
void PackWide(vector<Rectangle>& narrow, vector<Rectangle>& wide, double x1, double y1, double x_limit, double y_limit);
double ad_SAS(vector<Rectangle>& recs);
void ad_PackNarrow(vector<Rectangle>& narrow, vector<Rectangle>& wide, double x1, double y1, double x_limit, double y_limit);
void ad_PackWide(vector<Rectangle>& narrow, vector<Rectangle>& wide, double x1, double y1, double x_limit, double y_limit);
void testTime(vector<Rectangle>& recs);

bool cmpBins(const Rectangle& a, const Rectangle& b) {
    return a.height > b.height;
}
bool cmpBinsWid(const Rectangle& a, const Rectangle& b) {
    return a.width > b.width;
}

int main() {
    cout << "Input the number of rectangles and the width of the large bin:\n";
    cin >> n >> given_width;

    cout << "Input the width and height of each rectangle:" << endl;
    vector<Rectangle> rects; // Input rects
    for (int i = 0; i < n; i++) {
        Rectangle rectangle = {};
        cin >> rectangle.width >> rectangle.height;
        rects.push_back(rectangle);
    }

    testTime(rects);

}

void testTime(vector<Rectangle>& rects) {
    // calculate time
    clock_t start_ad_SAS,end_ad_SAS,start_SAS, end_SAS, start_FFDH, end_FFDH, start_NFDH, end_NFDH, start_Sleator, end_Sleator, start_SP, end_SP;
    
    start_SAS = clock();
    double height_SAS = SAS(rects);
    end_SAS = clock();

    start_ad_SAS = clock();
    double height_ad_SAS = ad_SAS(rects);
    end_ad_SAS = clock();

    start_FFDH = clock();
    double height_FFDH = FFDH(rects);
    end_FFDH = clock();

    start_NFDH = clock();
    double height_NFDH = NFDH(rects);
    end_NFDH = clock();

    start_Sleator = clock();
    double height_Sleator = Sleator(rects);
    end_Sleator = clock();

    start_SP = clock();
    double heigh_SP = SP(rects);
    end_SP = clock();

    cout << "Height of the packing obtained in the strip using improved SAS: " << height_ad_SAS << ", takes " << (double)(end_ad_SAS - start_ad_SAS) / CLOCKS_PER_SEC << " seconds" << endl;
    cout << "Height of the packing obtained in the strip using SAS: " << height_SAS << ", takes " << (double)(end_SAS - start_SAS) / CLOCKS_PER_SEC << " seconds" << endl;
    cout << "Height of the packing obtained in the strip using FFDH: " << height_FFDH << ", takes " << (double)(end_FFDH - start_FFDH) / CLOCKS_PER_SEC << " seconds" << endl;
    cout << "Height of the packing obtained in the strip using NFDH: " << height_NFDH << ", takes " << (double)(end_NFDH - start_NFDH) / CLOCKS_PER_SEC << " seconds" << endl;
    cout << "Height of the packing obtained in the strip using Sleator: " << height_Sleator << ", takes " << (double)(end_Sleator - start_Sleator) / CLOCKS_PER_SEC << " seconds" << endl;
    cout << "Height of the packing obtained in the strip using SP: " << heigh_SP << ", takes " << (double)(end_SP - start_SP) / CLOCKS_PER_SEC << " seconds" << endl;
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
/**
 * Input: The number of rectangles to be packed n, the dimensions of the rectangles ⟨w(Li ), h(Li )⟩
 * and the strip width W .
 * Output: The height H of the packing obtained in the strip.
 * @author AnyaReese
 * @param recs a vector of rectangles
 * @return height
 */
double ad_SAS(vector<Rectangle>& recs) {
    vector<Rectangle> narrow;
    vector<Rectangle> wide;

    if (enableLog) {
        cout << "\e[33m[Info]: Using SAS\e[0m" << endl;
    }

    // Partition rectangles into narrow and wide
    for (const auto& rec : recs) {
        if (rec.width < rec.height) {
            narrow.push_back(rec);
        } else {
            wide.push_back(rec);
        }
    }

    // Sort by height to prioritize taller rectangles first
    auto cmpHeight = [](const Rectangle& a, const Rectangle& b) { return a.height > b.height; };
    auto cmpWide = [](const Rectangle& a, const Rectangle& b) { return a.width > b.width; };
    sort(narrow.begin(), narrow.end(), cmpHeight);
    sort(wide.begin(), wide.end(), cmpWide);

    double current_x = 0, current_y = 0;

    bool init_with_wide = 0;
    double height_limit = 0;
    int count = 0;
    // Packing process
    while (!narrow.empty() || !wide.empty()) {
        count++;
        if (!narrow.empty() && !wide.empty()) {
            if (narrow[0].height > wide[0].height) {
                init_with_wide = false;
                current_x = narrow[0].width;
                current_y = height_limit;
                height_limit += narrow[0].height;
                if (enableLog) {
                    cout << "\e[32mHeight Changed By Narrow Rectangle whose height is: " << narrow[0].height << "\e[0m" << endl;
                }
                narrow.erase(narrow.begin());
            } else { // wide[0].height >= narrow[0].height
                init_with_wide = true;
                current_x = wide[0].width;
                current_y = height_limit;
                height_limit += wide[0].height;
                if (enableLog) {
                    cout << "\e[32mHeight Changed By Wide Rectangle whose height is: " << wide[0].height << "\e[0m" << endl;
                }
                wide.erase(wide.begin());
            }
        } else if (!narrow.empty()) {
            init_with_wide = false;
            current_x = narrow[0].width;
            current_y = height_limit;
            height_limit += narrow[0].height;
            if (enableLog) {
                cout << "\e[32mHeight Changed By Narrow Rectangle whose height is: " << narrow[0].height << "\e[0m" << endl;
            }
            narrow.erase(narrow.begin());
        } else { // wide.size()
            init_with_wide = true;
            current_x = wide[0].width;
            current_y = height_limit;
            height_limit += wide[0].height;
            if (enableLog) {
                cout << "\e[32mHeight Changed By Wide Rectangle whose height is: " << wide[0].height << "\e[0m" << endl;
            }
            wide.erase(wide.begin());
        } /* end of Initialization Narrow or Wide */
        if (init_with_wide) { // Pack Narrow
            cout<<count<<" : ad_PackNarrow"<<endl;
            ad_PackNarrow(narrow, wide, current_x, current_y, given_width, height_limit);
        } else { // Pack Wide
            cout<<count<<" : ad_PackWide"<<endl;
            ad_PackWide(narrow, wide, current_x, current_y, given_width, height_limit);
        }
    }
    return height_limit;
}

/**
 * Improved Pack Narrow
 * Traverse each narrow, if it fits narrow-wise and height-wise, pack it.
 * @return
 */

void ad_PackNarrow(vector<Rectangle>& narrow, vector<Rectangle>& wide, double x1, double y1, double x_limit, double y_limit) {
    double baseWidth;
    double curr_X1 = x1;
    double curr_Y1 = y1;
    if (!narrow.empty()) {
        while (!narrow.empty() && (curr_X1 + narrow[0].width <= x_limit)) {
            baseWidth = narrow[0].width;
            while (!narrow.empty() && (curr_Y1 + narrow[0].height <= y_limit) && (narrow[0].width <= baseWidth)) {
                curr_Y1 += narrow[0].height;
                narrow.erase(narrow.begin());
            }
            
            cout << "Packing narrow at (" << curr_X1 << ", " << curr_Y1 << ")" << endl;
            
            curr_X1 += baseWidth;
            curr_Y1 = y1;
        }

        double min_height = narrow.back().height;
        double min_width = wide.back().width;

    }
}

/**
 * Pack Wide
 * Traverse each wide, if it fits wide-wise and height-wise, pack it.
 * If no wide fits but narrows spare, pack narrow.
 * @return
 */

void ad_PackWide(vector<Rectangle>& narrow, vector<Rectangle>& wide, double x1, double y1, double x_limit, double y_limit) {
    double current_X1 = x1;
    double pre_Y1 = y1;
    double pre_x_limit = x_limit; // 保存刚开始 PackWide 的 y1
    int flag = 1;
    if (!wide.empty() && (x1 + wide[0].width <= x_limit)) {
        for (int i = 0; i < wide.size(); i++) { // traverse each wide
        
            cout<<"i  "<<i<<endl;
            // 放置宽矩形
            if (y1 + wide[i].height <= y_limit) {

                if(flag){
                    current_X1 += wide[i].width;
                    flag = 0;
                }
                // 在形成的间隙中放入窄矩形
                if ((x1 + wide[i].width <= x_limit) && (x_limit != given_width)) {
                    ad_PackNarrow(narrow, wide, x1 + wide[i].width, y1, x_limit, y_limit);
                }
                cout << "Packing wide at (" << x1 << ", " << y1 << ")" << endl;
                x_limit = x1 + wide[i].width;
                y1 += wide[i].height;
                wide.erase(wide.begin() + i);
                i--;
            }
        }
    }

    if (wide.empty() && !narrow.empty()) { // 如果宽矩形放完了，就直接放窄矩形
        ad_PackNarrow(narrow, wide, x1, y1, x_limit, y_limit);
    }

    // 如果在右边的间隙中还能放得下宽矩形，就继续放宽矩形
    if((!wide.empty() && current_X1 + wide[0].width <= pre_x_limit )) {
        double baseWidth = wide[0].width;
        wide.erase(wide.begin());
        cout << "ad_PackWide at " << current_X1 <<"  "<< pre_Y1<<"  "<< pre_x_limit <<"  "<<y_limit<< endl;
        
        ad_PackWide(narrow, wide, current_X1, pre_Y1, pre_x_limit , y_limit);
    }

    // 如果宽矩形没了但是右边还能放窄矩形，就继续放窄矩形
    if (!narrow.empty() && current_X1 + narrow[0].width < pre_x_limit) {
        cout << "ad_PackNarrow at " << current_X1 <<"  "<< pre_Y1<<"  "<< pre_x_limit <<"  "<<y_limit<< endl;
        ad_PackNarrow(narrow, wide, current_X1, pre_Y1, pre_x_limit, y_limit );
    }
     

}


/**
 * Input: The number of rectangles to be packed n, the dimensions of the rectangles ⟨w(Li ), h(Li )⟩
 * and the strip width W .
 * Output: The height H of the packing obtained in the strip.
 * @author AnyaReese
 * @param recs a vector of rectangles
 * @return height
 */
double SAS(vector<Rectangle>& recs) {
    vector<Rectangle> narrow;
    vector<Rectangle> wide;

    if (enableLog) {
        cout << "\e[33m[Info]: Using SAS\e[0m" << endl;
    }

    // Partition rectangles into narrow and wide
    for (const auto& rec : recs) {
        if (rec.width < rec.height) {
            narrow.push_back(rec);
        } else {
            wide.push_back(rec);
        }
    }

    // Sort by height to prioritize taller rectangles first
    auto cmpHeight = [](const Rectangle& a, const Rectangle& b) { return a.height > b.height; };
    auto cmpWide = [](const Rectangle& a, const Rectangle& b) { return a.width > b.width; };
    sort(narrow.begin(), narrow.end(), cmpHeight);
    sort(wide.begin(), wide.end(), cmpWide);

    double current_x = 0, current_y = 0;

    bool init_with_wide = 0;
    double height_limit = 0;
    // Packing process
    while (!narrow.empty() || !wide.empty()) {
        if (!narrow.empty() && !wide.empty()) {
            if (narrow[0].height > wide[0].height) {
                init_with_wide = false;
                current_x = narrow[0].width;
                current_y = height_limit;
                height_limit += narrow[0].height;
                if (enableLog) {
                    cout << "\e[32mHeight Changed By Narrow Rectangle whose height is: " << narrow[0].height << "\e[0m" << endl;
                }
                narrow.erase(narrow.begin());
            } else { // wide[0].height >= narrow[0].height
                init_with_wide = true;
                current_x = wide[0].width;
                current_y = height_limit;
                height_limit += wide[0].height;
                if (enableLog) {
                    cout << "\e[32mHeight Changed By Wide Rectangle whose height is: " << wide[0].height << "\e[0m" << endl;
                }
                wide.erase(wide.begin());
            }
        } else if (!narrow.empty()) {
            init_with_wide = false;
            current_x = narrow[0].width;
            current_y = height_limit;
            height_limit += narrow[0].height;
            if (enableLog) {
                cout << "\e[32mHeight Changed By Narrow Rectangle whose height is: " << narrow[0].height << "\e[0m" << endl;
            }
            narrow.erase(narrow.begin());
        } else { // wide.size()
            init_with_wide = true;
            current_x = wide[0].width;
            current_y = height_limit;
            height_limit += wide[0].height;
            if (enableLog) {
                cout << "\e[32mHeight Changed By Wide Rectangle whose height is: " << wide[0].height << "\e[0m" << endl;
            }
            wide.erase(wide.begin());
        } /* end of Initialization Narrow or Wide */

        if (init_with_wide) { // Pack Narrow
            PackNarrow(narrow, wide, current_x, current_y, given_width, height_limit);
        } else { // Pack Wide
            PackWide(narrow, wide, current_x, current_y, given_width, height_limit);
        }
    }
    return height_limit;
}

/**
 * Pack Narrow
 * Traverse each narrow, if it fits narrow-wise and height-wise, pack it.
 * @return
 */

void PackNarrow(vector<Rectangle>& narrow, vector<Rectangle>& wide, double x1, double y1, double x_limit, double y_limit) {
    if (!narrow.empty()) {
        double baseWidth;
        double curr_X1 = x1;
        double curr_Y1 = y1;
        while (!narrow.empty() && (curr_X1 + narrow[0].width <= x_limit)) {
            baseWidth = narrow[0].width;
            while (!narrow.empty() && (curr_Y1 + narrow[0].height <= y_limit) && (narrow[0].width <= baseWidth)) {
                curr_Y1 += narrow[0].height;
                narrow.erase(narrow.begin());
            }
            if (enableLog) {
                cout << "Packing narrow at (" << curr_X1 << ", " << curr_Y1 << ")" << endl;
            }
            curr_X1 += baseWidth;
            curr_Y1 = y1;
        }
    }
}

/**
 * Pack Wide
 * Traverse each wide, if it fits wide-wise and height-wise, pack it.
 * If no wide fits but narrows spare, pack narrow.
 * @return
 */
void PackWide(vector<Rectangle>& narrow, vector<Rectangle>& wide, double x1, double y1, double x_limit, double y_limit) {
    if (!wide.empty() && (x1 + wide[0].width <= x_limit)) {
        for (int i = 0; i < wide.size(); i++) { // traverse each wide
            // if rectangle fits wide-wise and height-wise
            if (y1 + wide[i].height <= y_limit) {
                // if rectangle fits wide-wise
                if ((x1 + wide[i].width <= x_limit) && (x_limit != given_width)) {
                    PackNarrow(narrow, wide, x1 + wide[i].width, y1, x_limit, y_limit);
                }
                if (enableLog) {
                    cout << "Packing wide at (" << x1 << ", " << y1 << ")" << endl;
                }
                x_limit = x1 + wide[i].width;
                y1 += wide[i].height;
                wide.erase(wide.begin() + i);
                i--;
            } /* end of this wide[i] */
        }
    }
    if (wide.empty() && !narrow.empty()) {
        PackNarrow(narrow, wide, x1, y1, x_limit, y_limit);
    }
}

double Sleator(vector<Rectangle>& recs) {
    vector<Rectangle> groupA;
    vector<Rectangle> groupB;

    // Divide the rectangles into two groups based on their width
    for (const auto& rect : recs) {
        if (2 * rect.width > given_width) {
            groupA.push_back(rect);  // Group A: width is greater than half of the strip width
        } else {
            groupB.push_back(rect);  // Group B: width is less than or equal to half of the strip width
        }
    }


    double currentHeight = 0;

    // Place items from Group A vertically at the left side of the strip
    for (const auto& rect : groupA) {
        currentHeight += rect.height;
    }

    // Sort items in Group B in decreasing order of height
    sort(groupB.begin(), groupB.end(), [](const Rectangle& a, const Rectangle& b) {
        return a.height > b.height;
    });

    double lowerHeight = currentHeight, upperHeight = currentHeight;  // Track heights at lower and upper ends

    // Place items from Group B horizontally at the bottom or top of the strip
    for (size_t i = 0; i < groupB.size();) {
        double currentWidth = groupB[i].width;
        double maxHeight = groupB[i].height;
        size_t j = i; // Index of the last item placed horizontally

        // Find the maximum height that can be achieved by placing items horizontally
        while (j + 1 < groupB.size() && 2 * (groupB[j + 1].width + currentWidth) <= given_width) {
            currentWidth += groupB[j + 1].width;
            maxHeight = max(maxHeight, groupB[j + 1].height);
            j++;
        }

        // Place the items horizontally at the lower or upper end of the strip
        if (lowerHeight <= upperHeight) {
            lowerHeight += maxHeight;
        } else {
            upperHeight += maxHeight;
        }

        i = j + 1;  // Move to the next block of items
    }

    return max(lowerHeight, upperHeight);
}

class Strip {
public:
    double itemWidth;   // Width of the item occupying this strip
    double width;       // Total width of the strip
    double lower;       // Lower bound Y of the unoccupied space in the strip
    double upper;       // Upper bound Y of the occupied space in the strip
};

// Comparator for sorting rectangles by width and height
bool cmp(const Rectangle& a, const Rectangle& b) {
    if (a.width == b.width) return a.height > b.height;
    return a.width > b.width;
}


double SP(vector<Rectangle>& recs) {
    double W = given_width; //

    sort(recs.begin(), recs.end(), cmp);

    vector<Strip> strips;
    strips.emplace_back(Strip{ 0, W, 0, 0}); // Initialize the first strip

    int strip_num = 1;


    for (int i = 0; i < n; i++) { // Traverse each rectangle
        int s_index = 0; // Find the first strip that can accommodate the rectangle

        for (int j = 1; j <= strip_num; j++) { // Traverse each strip
            if (strips[j-1].width - strips[j-1].itemWidth >= recs[i].width) { // Check if the strip can accommodate the rectangle
                if (!s_index) {
                    s_index = j;
                } else if (strips[j-1].width < strips[s_index-1].width) {
                    s_index = j; // Find the strip with the lowest upper bound
                }
            }
        } /* Traverse Strip End */

        if (!s_index) { // No suitable strip found
            // Find the lowest upper bound
            for (int j = 1; j <= strip_num; j++) {
                if (!s_index || (strips[s_index-1].upper < strips[j - 1].upper && strips[j].width >= recs[i].width)) s_index = j;
            }
            strips[s_index - 1].upper += recs[i].height;
            strips[s_index - 1].itemWidth = recs[i].width;
        } else {
            // Suitable strip found, place the item
            Strip s1 = { // itemWidth, width, lower, upper
                    recs[i].width,
                    recs[i].width,
                    strips[s_index - 1].upper,
                    strips[s_index - 1].lower + recs[i].height
            };

            Strip s2 = {
                    0,
                    strips[s_index - 1].width - recs[i].width,
                    strips[s_index - 1].upper,
                    strips[s_index - 1].upper
            };

            // Cut the strip into two substrips
            strips.erase(strips.begin() + s_index - 1);
            strips.push_back(s1);
            strips.push_back(s2);
            strip_num++;
        }
    } /* Traverse Rectangle end */

    // Calculate and return the maximum height used
    double maxHeight = 0;
    for (auto& strip : strips) {
        maxHeight = max(maxHeight, strip.upper);
    }
    return maxHeight;
}


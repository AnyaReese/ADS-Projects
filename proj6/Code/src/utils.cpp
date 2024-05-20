#include "utils.h"
#include "algorithms.h"
using namespace std;
bool cmpBins(const Rectangle& a, const Rectangle& b) {
    if (a.height != b.height) {
        return a.height > b.height;
    } else {
        return a.width > b.width;
    }
}

bool cmpHeight(const Rectangle& a, const Rectangle& b) {
    return a.height > b.height;
}

bool cmpWide(const Rectangle& a, const Rectangle& b) {
    return a.width > b.width;
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

    enableLog = 1;
    start_SP = clock();
    double heigh_SP = SP(rects);
    end_SP = clock();
    enableLog = 0;

    cout << "The sum of area is:" << sumupArea << endl;

    cout << "Height of the packing obtained in the strip using improved SAS: " << height_ad_SAS << fixed << setprecision(6) << " with a ratio of: " << (double)(height_ad_SAS * given_width) / (sumupArea);
    cout << ", takes " << (double)(end_ad_SAS - start_ad_SAS) / CLOCKS_PER_SEC << " seconds" << endl;
    cout << fixed << setprecision(0);
    cout << "Height of the packing obtained in the strip using SAS: " << height_SAS << fixed << setprecision(6) << " with a ratio of: " << (double)(height_SAS * given_width) / (sumupArea);
    cout << ", takes " << (double)(end_SAS - start_SAS) / CLOCKS_PER_SEC << " seconds" << endl;
    cout << fixed << setprecision(0);
    cout << "Height of the packing obtained in the strip using FFDH: " << height_FFDH << fixed << setprecision(6) << " with a ratio of: " << (double)(height_FFDH * given_width) / (sumupArea);
    cout << ", takes " << (double)(end_FFDH - start_FFDH) / CLOCKS_PER_SEC << " seconds" << endl;
    cout << fixed << setprecision(0);
    cout << "Height of the packing obtained in the strip using NFDH: " << height_NFDH << fixed << setprecision(6) << " with a ratio of: " << (double)(height_NFDH * given_width) / (sumupArea);
    cout << ", takes " << (double)(end_NFDH - start_NFDH) / CLOCKS_PER_SEC << " seconds" << endl;
    cout << fixed << setprecision(0);
    cout << "Height of the packing obtained in the strip using Sleator: " << height_Sleator << fixed << setprecision(6) << " with a ratio of: " << (double)(height_Sleator * given_width) / (sumupArea);
    cout << ", takes " << (double)(end_Sleator - start_Sleator) / CLOCKS_PER_SEC << " seconds" << endl;
    cout << fixed << setprecision(0);
    cout << "Height of the packing obtained in the strip using SP: " << heigh_SP << fixed << setprecision(6) << " with a ratio of: " << (double)(heigh_SP * given_width) / (sumupArea);
    cout << ", takes " << (double)(end_SP - start_SP) / CLOCKS_PER_SEC << " seconds" << endl;
}

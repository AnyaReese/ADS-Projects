#ifndef UTILS_H
#define UTILS_H
#include "algorithms.h"
#include <iomanip>
#include <vector>
#include <iostream>
using namespace std;
bool cmpBins(const Rectangle& a, const Rectangle& b);
bool cmpHeight(const Rectangle& a, const Rectangle& b);
bool cmpWide(const Rectangle& a, const Rectangle& b);
void testTime(vector<Rectangle>& rects);
#endif

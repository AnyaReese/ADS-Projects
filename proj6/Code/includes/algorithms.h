#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
using namespace std;
class Rectangle {
public:
    double x;
    double y;
    double width;
    double height;
};


double SP(std::vector<Rectangle>& recs);
double Sleator(std::vector<Rectangle>& recs);
double FFDH(std::vector<Rectangle>& recs);
double NFDH(std::vector<Rectangle>& recs);
double SAS(std::vector<Rectangle>& recs);
double ad_SAS(std::vector<Rectangle>& recs);
void PackNarrow(std::vector<Rectangle>& narrow, std::vector<Rectangle>& wide, double x1, double y1, double x_limit, double y_limit);
void PackWide(std::vector<Rectangle>& narrow, std::vector<Rectangle>& wide, double x1, double y1, double x_limit, double y_limit);
void ad_PackNarrow(std::vector<Rectangle>& narrow, std::vector<Rectangle>& wide, double x1, double y1, double x_limit, double y_limit);
void ad_PackWide(std::vector<Rectangle>& narrow, std::vector<Rectangle>& wide, double x1, double y1, double x_limit, double y_limit);


extern int enableLog;
extern int n;
extern double given_width;
extern long long sumupArea;

#endif // ALGORITHMS_H

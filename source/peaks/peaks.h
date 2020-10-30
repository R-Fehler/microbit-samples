// Util.h
#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

namespace Peaks {
    const int64_t EPS =1;
    void findPeaks(const vector<int64_t> &xInput, vector<int> &peakOutputInds, int SetID);
}

#endif

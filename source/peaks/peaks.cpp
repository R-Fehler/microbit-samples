// Util.cpp
#include <cstdint>
#include "peaks.h"


void diff(const vector<int64_t> &in, vector<int64_t> &out) {
    out = vector<int64_t>(in.size() - 1);

    for (int i = 1; i < in.size(); ++i)
        out[i - 1] = in[i] - in[i - 1];
}

void vectorProduct(const vector<int64_t> &a, const vector<int64_t> &b, vector<int64_t> &out) {
    out = vector<int64_t>(a.size());

    for (int i = 0; i < a.size(); ++i)
        out[i] = a[i] * b[i];
}

void findIndicesLessThan(const vector<int64_t> &in, int64_t threshold, vector<int> &indices) {
    for (int i = 0; i < in.size(); ++i)
        if (in[i] < threshold)
            indices.push_back(i + 1);
}

void selectElements(const vector<int64_t> &in, const vector<int> &indices, vector<int64_t> &out) {
    for (int i = 0; i < indices.size(); ++i)
        out.push_back(in[indices[i]]);
}

void selectElements(const vector<int> &in, const vector<int> &indices, vector<int> &out) {
    for (int i = 0; i < indices.size(); ++i)
        out.push_back(in[indices[i]]);
}

void signVector(const vector<int64_t> &in, vector<int> &out) {
    out = vector<int>(in.size());

    for (int i = 0; i < in.size(); ++i) {
        if (in[i] > 0)
            out[i] = 1;
        else if (in[i] < 0)
            out[i] = -1;
        else
            out[i] = 0;
    }
}


void Peaks::findPeaks(const vector<int64_t> &xInput, vector<int> &peakOutputInds, int SetID) {
    int minIdx = distance(xInput.begin(), min_element(xInput.begin(), xInput.end()));
    int maxIdx = distance(xInput.begin(), max_element(xInput.begin(), xInput.end()));

    // TODO Calibrate this variable with the delta of the first rep.
    const int64_t limit = 280;
    int64_t deltaMax = (xInput[maxIdx] - xInput[minIdx]) / 2;
    static int64_t sel = limit;
    static int calibID = SetID;
    if (calibID == SetID) {
        sel = max(sel, deltaMax);
    } else {
        calibID = SetID; // change ID when new Set / Exercise is started
        sel = limit;
    }
    int len0 = xInput.size();

    vector<int64_t> dx;
    diff(xInput, dx);
    replace(dx.begin(), dx.end(), (int64_t) 0, -Peaks::EPS);
    vector<int64_t> dx0(dx.begin(), dx.end() - 1);
    vector<int64_t> dx1(dx.begin() + 1, dx.end());
    vector<int64_t> adjacentDifferenceOfDX;

    vectorProduct(dx0, dx1, adjacentDifferenceOfDX); // 2*-3=-6 , -3*-3=9 etc , here elementwise multiplication

    vector<int> indexOfAllPointsOfInterest;
    findIndicesLessThan(adjacentDifferenceOfDX, 0,
                        indexOfAllPointsOfInterest); // Find where the derivative changes sign :

    vector<int64_t> PeakValleyVectorX;

    vector<int> indAux(indexOfAllPointsOfInterest.begin(), indexOfAllPointsOfInterest.end());
    selectElements(xInput, indAux, PeakValleyVectorX); // PeakValleyVectorX is vector with only Peaks and valleys
    PeakValleyVectorX.insert(PeakValleyVectorX.begin(), xInput[0]);
    PeakValleyVectorX.insert(PeakValleyVectorX.end(),
                             xInput[xInput.size() - 1]);; // add edge points to PeakValleyVectorX vector (only P and V)


    indexOfAllPointsOfInterest.insert(indexOfAllPointsOfInterest.begin(), 0); //add indices for edge points
    indexOfAllPointsOfInterest.insert(indexOfAllPointsOfInterest.end(), len0);

    int indexOfSmallestElement = distance(PeakValleyVectorX.begin(),
                                          min_element(PeakValleyVectorX.begin(), PeakValleyVectorX.end())); //
    int64_t smallestElementOfX = PeakValleyVectorX[indexOfSmallestElement];
    int64_t leftMinimum = smallestElementOfX;
    int lengthOfPeakValleyVectorX = PeakValleyVectorX.size();

    if (lengthOfPeakValleyVectorX > 2) // if peaks/valleys exist eg not monoton data
    {
        int64_t tempMag = smallestElementOfX;
        bool foundPeak = false;
        int ii;

        // Deal with first point a little differently since tacked it on
        // Calculate the sign of the derivative since we tacked the first
        //  point on it does not neccessarily alternate like the rest.
        // The Rest is always alternating Peak and Valley.
        vector<int64_t> xSub0(PeakValleyVectorX.begin(), PeakValleyVectorX.begin() + 3);//tener cuidado subvector
        vector<int64_t> xDiff;//tener cuidado subvector
        diff(xSub0, xDiff);

        vector<int> signDx;
        signVector(xDiff, signDx);

        if (signDx[0] <= 0) // The first point is larger or equal to the second
        {
            if (signDx[0] == signDx[1]) // Want alternating signs
            {
                PeakValleyVectorX.erase(PeakValleyVectorX.begin() + 1); // erase first real data so they alternate P/V
                indexOfAllPointsOfInterest.erase(indexOfAllPointsOfInterest.begin() + 1);
                lengthOfPeakValleyVectorX = lengthOfPeakValleyVectorX - 1;
            }
        } else // First point is smaller than the second
        {
            if (signDx[0] == signDx[1]) // Want alternating signs
            {
                PeakValleyVectorX.erase(PeakValleyVectorX.begin());
                indexOfAllPointsOfInterest.erase(indexOfAllPointsOfInterest.begin());
                lengthOfPeakValleyVectorX = lengthOfPeakValleyVectorX - 1;
            }
        }

        if (PeakValleyVectorX[0] >= PeakValleyVectorX[1]) // if first is bigger then second (peak at edge)
            ii = 0;
        else
            ii = 1;

        int64_t maxPeaks = ceil((int64_t) lengthOfPeakValleyVectorX / 2);
        vector<int> peakLoc(maxPeaks, 0);
        vector<int64_t> peakMag(maxPeaks, 0);
        int peakVectorsInd = 1; // index of peakLoc and peakMag
        int tempLoc;

        while (ii < lengthOfPeakValleyVectorX) {
            ii = ii + 1;//This is a peak
            //Reset peak finding if we had a peak and the next peak is bigger
            //than the last or the left min was small enough to reset.
            if (foundPeak) {
                tempMag = smallestElementOfX; //reset values
                foundPeak = false;
            }

            //Found new peak that was lager than temp mag and selectivity larger
            //than the minimum to its left.
            // leftmin is at first smallestElementofX
            if (PeakValleyVectorX[ii - 1] > tempMag && PeakValleyVectorX[ii - 1] > leftMinimum + sel) {
                tempLoc = ii - 1;
                tempMag = PeakValleyVectorX[ii - 1];
            }

            //Make sure we don't iterate past the length of our vector
            if (ii == lengthOfPeakValleyVectorX)
                break; //We assign the last point differently out of the loop

            ii = ii + 1; // Move onto the valley

            //Come down at least sel from peak
            if (!foundPeak && tempMag > sel + PeakValleyVectorX[ii - 1]) {
                foundPeak = true; //We have found a peak
                leftMinimum = PeakValleyVectorX[ii - 1];
                peakLoc[peakVectorsInd - 1] = tempLoc; // Add peak to index
                peakMag[peakVectorsInd - 1] = tempMag;
                peakVectorsInd = peakVectorsInd + 1;
            } else if (PeakValleyVectorX[ii - 1] < leftMinimum) // New left minima
                leftMinimum = PeakValleyVectorX[ii - 1];

        }
        /**
         * produces Bug when there is a rising endpoint it is seen as peak (altough the right valley is missing)
         */
//        // Check end point
//        if (PeakValleyVectorX[PeakValleyVectorX.size() - 1] > tempMag && PeakValleyVectorX[PeakValleyVectorX.size() - 1] > leftMinimum + sel )
//        {
//            peakLoc[peakVectorsInd - 1] = lengthOfPeakValleyVectorX - 1;
//            peakMag[peakVectorsInd - 1] = PeakValleyVectorX[PeakValleyVectorX.size() - 1];
//            peakVectorsInd = peakVectorsInd + 1;
//        }
//        else if( !foundPeak && tempMag > smallestElementOfX )// Check if we still need to add the last point
//        {
//            peakLoc[peakVectorsInd - 1] = tempLoc;
//            peakMag[peakVectorsInd - 1] = tempMag;
//            peakVectorsInd = peakVectorsInd + 1;
//        }

        //Create output
        if (peakVectorsInd > 0) // if there are any peaks
        {
            vector<int> peakLocTmp(peakLoc.begin(), peakLoc.begin() + peakVectorsInd - 1);

            //TODO minDistanceOfPeaks is buggy, not working with cutted window in unique set in main
            int minDistanceOfPeaks = 4;
            if (peakLocTmp.size() >= 2) {

                for (int i = peakLocTmp.size() - 1; i >= 1; --i) {
                    if (abs(peakLocTmp.at(i) - peakLocTmp.at(i - 1)) < minDistanceOfPeaks) {
                        peakLocTmp.erase(peakLocTmp.begin() + i);
                    }
                }
            }
            selectElements(indexOfAllPointsOfInterest, peakLocTmp, peakOutputInds);
            //peakMags = vector<int64_t>(peakLoc.begin(), peakLoc.begin()+peakVectorsInd-1);
        }


    }


}

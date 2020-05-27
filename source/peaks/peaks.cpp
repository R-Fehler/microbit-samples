// Util.cpp
#include <cstdint>
#include "peaks.h"


void diff(const vector<int64_t>& in, vector<int64_t>& out)
{
	out = vector<int64_t>(in.size()-1);

	for(int i=1; i<in.size(); ++i)
		out[i-1] = in[i] - in[i-1];
}

void vectorProduct(const vector<int64_t>& a, const vector<int64_t>& b, vector<int64_t>& out)
{
	out = vector<int64_t>(a.size());

	for(int i=0; i<a.size(); ++i)
		out[i] = a[i] * b[i];
}

void findIndicesLessThan(const vector<int64_t>& in, int64_t threshold, vector<int>& indices)
{
	for(int i=0; i<in.size(); ++i)
		if(in[i]<threshold)
			indices.push_back(i+1);
}

void selectElements(const vector<int64_t>& in, const vector<int>& indices, vector<int64_t>& out)
{
	for(int i=0; i<indices.size(); ++i)
		out.push_back(in[indices[i]]);
}

void selectElements(const vector<int>& in, const vector<int>& indices, vector<int>& out)
{
	for(int i=0; i<indices.size(); ++i)
		out.push_back(in[indices[i]]);
}

void signVector(const vector<int64_t>& in, vector<int>& out)
{
	out = vector<int>(in.size());

	for(int i=0; i<in.size(); ++i)
	{
		if(in[i]>0)
			out[i]=1;
		else if(in[i]<0)
			out[i]=-1;
		else
			out[i]=0;
	}
}


void Peaks::findPeaks(const vector<int64_t> &x0, vector<int> &peakInds, int SetID)
{
	int minIdx = distance(x0.begin(), min_element(x0.begin(), x0.end()));
	int maxIdx = distance(x0.begin(), max_element(x0.begin(), x0.end()));

	// TODO Calibrate this variable with the delta of the first rep.
	const int64_t limit=400;
	int64_t deltaMax=(x0[maxIdx]-x0[minIdx])/2;
	static int64_t sel =limit;
	static int calibID=SetID;
	if (calibID==SetID){
    sel=max(sel,deltaMax);
	}
	else{
	    calibID=SetID;
	    sel=limit;
	}
	int len0 = x0.size();

	vector<int64_t> dx;
	diff(x0, dx);
	replace(dx.begin(), dx.end(), (int64_t)0, -Peaks::EPS);
	vector<int64_t> dx0(dx.begin(), dx.end()-1);
	vector<int64_t> dx1(dx.begin()+1, dx.end());
	vector<int64_t> dx2;

	vectorProduct(dx0, dx1, dx2);

	vector<int> ind;
	findIndicesLessThan(dx2, 0, ind); // Find where the derivative changes sign
	
	vector<int64_t> x;

	vector<int> indAux(ind.begin(), ind.end());
	selectElements(x0, indAux, x);
	x.insert(x.begin(), x0[0]);
	x.insert(x.end(), x0[x0.size()-1]);;


	ind.insert(ind.begin(), 0);
	ind.insert(ind.end(), len0);

	int minMagIdx = distance(x.begin(), min_element(x.begin(), x.end()));
	int64_t minMag = x[minMagIdx];
	int64_t leftMin = minMag;
	int len = x.size();

	if(len>2)
	{
		int64_t tempMag = minMag;
    	bool foundPeak = false;
    	int ii;

    	// Deal with first point a little differently since tacked it on
        // Calculate the sign of the derivative since we tacked the first
        //  point on it does not neccessarily alternate like the rest.
    	vector<int64_t> xSub0(x.begin(), x.begin()+3);//tener cuidado subvector
    	vector<int64_t> xDiff;//tener cuidado subvector
    	diff(xSub0, xDiff);

    	vector<int> signDx;
    	signVector(xDiff, signDx);

        if (signDx[0] <= 0) // The first point is larger or equal to the second
        {
            if (signDx[0] == signDx[1]) // Want alternating signs
            {
                x.erase(x.begin()+1);
                ind.erase(ind.begin()+1);
                len = len-1;
            }
        }
        else // First point is smaller than the second
        {
            if (signDx[0] == signDx[1]) // Want alternating signs
            {
            	x.erase(x.begin());
            	ind.erase(ind.begin());
                len = len-1;
            }
        }

    	if ( x[0] >= x[1] )
        	ii = 0;
    	else
        	ii = 1;

    	int64_t maxPeaks = ceil((int64_t)len/2);
    	vector<int> peakLoc(maxPeaks,0);
    	vector<int64_t> peakMag(maxPeaks,0);
    	int cInd = 1;
    	int tempLoc;
    
    	while(ii < len)
    	{
        	ii = ii+1;//This is a peak
        	//Reset peak finding if we had a peak and the next peak is bigger
        	//than the last or the left min was small enough to reset.
        	if(foundPeak)
        	{
            	tempMag = minMag;
            	foundPeak = false;
            }
        
        	//Found new peak that was lager than temp mag and selectivity larger
        	//than the minimum to its left.
        
        	if( x[ii-1] > tempMag && x[ii-1] > leftMin + sel )
        	{
            	tempLoc = ii-1;
            	tempMag = x[ii-1];
        	}

        	//Make sure we don't iterate past the length of our vector
        	if(ii == len)
            	break; //We assign the last point differently out of the loop

        	ii = ii+1; // Move onto the valley
        	
        	//Come down at least sel from peak
        	if(!foundPeak && tempMag > sel + x[ii-1])
            {            	
	            foundPeak = true; //We have found a peak
	            leftMin = x[ii-1];
	            peakLoc[cInd-1] = tempLoc; // Add peak to index
	            peakMag[cInd-1] = tempMag;
	            cInd = cInd+1;
	        }
        	else if(x[ii-1] < leftMin) // New left minima
            	leftMin = x[ii-1];
            
        }

        // Check end point
        if ( x[x.size()-1] > tempMag && x[x.size()-1] > leftMin + sel )
        {
            peakLoc[cInd-1] = len-1;
            peakMag[cInd-1] = x[x.size()-1];
            cInd = cInd + 1;
        }
        else if( !foundPeak && tempMag > minMag )// Check if we still need to add the last point
        {
            peakLoc[cInd-1] = tempLoc;
            peakMag[cInd-1] = tempMag;
            cInd = cInd + 1;
        }

    	//Create output
    	if( cInd > 0 )
    	{        	
        	vector<int> peakLocTmp(peakLoc.begin(), peakLoc.begin()+cInd-1);
			selectElements(ind, peakLocTmp, peakInds);
        	//peakMags = vector<int64_t>(peakLoc.begin(), peakLoc.begin()+cInd-1);
        }
    	


	}


}

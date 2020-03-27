/* ~/Programs/Matrix/ Mat.cc
 
*/

#include "Mat.h"
#include "Matimp.h"

// Mat default constructor, 2D matrix
Mat::Mat() { }	// see #define USE_ABC in Mat.h



//-- STRIDE,  INDEX ----------------
template<>
void stride1<F>(const vector<lint>& siz, vector<lint>& M)
{ 
 M.resize( siz.size());
 int k, ndim = siz.size();
 if (ndim==0) return;
 M[0] = 1;
 for(k=1; k < ndim; ++k) M[k]= siz[k-1]*M[k-1];
}
template<>
void stride1<C>(const vector<lint>& siz, vector<lint>& M)
{
 M.resize(siz.size());
 int k, ndim = siz.size();
 if (ndim==0) return;
 M[ndim-1] = 1;
 for(k=ndim-2; k>=0; --k) M[k] = siz[k+1]*M[k+1];
}
/*
I	M - stride array, from stride<>()
I   inx  array of indices for each axis/dimension
		range NOT checked
O   index index of internal buf[] data array
*/
lint index( const vector<lint>& M, const vector<lint>& inx)
{if( M.size() != inx.size()  )
	 cout<< "ERROR index() wrong number of indices."<<endl;
 lint indx=0;
 for(int k=0; k<M.size(); ++k) indx+= inx[k]*M[k];
 return indx;
} 
/* -- NEXTINDX ---------------
  Iterates thru all possible index vectors.
<F>	left to right,  <C> right to left
I	size_	size of each axis/dimension
I	stride	strides, for <F or C>	// not used
I/O inx		In: inx, Out: ++inx
O   index(++inx)
Function: increment inx, return index(inx)
		  does not return 0, first return
 Usage:
 index = 0; inx={0,0...0};
 while(index < sizebuf_) 
	{ use(inx); ...;
	  nextindex(...,++inx,index);
    }
*/
template<>		// Mat.cc
lint nextindx<F>(const vector<lint>& size_, 
		vector<lint>& inx, lint& index)
{int k=0; int ndim=size_.size();

 for(k=0; k<= ndim-2; ++k)
  { if( inx[k] < size_[k] -1) {++inx[k]; ++index; return index;}
    inx[k]=0;  // reset, check next k
  }
// k=ndim_1 all previous where inx[k]=size_[k]-1, set = 0
// k = ndim-1;
 if( inx[k] < size_[k] -1) {++inx[k]; ++index; return index;}
 ++inx[k];
 ++index;	// = sizebuf_, out of range, inx[k] == 0
 return index;
}

template<>		// Mat.cc  
lint nextindx<C>(const vector<lint>& size_, 
		vector<lint>& inx, lint& index)
{ 
 int k=0, kc; int ndim = size_.size();
 for(kc = 0; kc<=ndim-2; ++kc)
 {k=ndim-1-kc;
  if( inx[k] < size_[k]-1) {++inx[k]; ++index; return index; }
  inx[k] = 0;
  }
 // kc = ndim-1 here.
 k = 0;
 if(inx[k] < size_[k] -1) {++inx[k]; ++index;return index;}
 ++inx[k];
 ++index;
 return index;
}
//========================================================

// UTILITY FUNCTIONS:
// start, stop times: (using <chrono>  )
Timept hires_now()
	{return chrono::high_resolution_clock::now(); }

// time difference in seconds
double timediff(Timept start, Timept stop)
{chrono::duration<double> elapsed = stop-start;
 return elapsed.count();	// time diff. in seconds
}

// Force definition of Matimp.h template classes/functions:
void init_all()
{  
init_Mattimp<float, F>();
init_Mattimp<float, C>();
init_Mattimp<double, F>();
init_Mattimp<complex<double>, F >();
}



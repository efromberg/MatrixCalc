/* ~/Programs/Matrix/	Mat.h   3/22/2020,
 Test case:  tstMatt.cc
*/  
 
#ifndef _MATT_H
#define _MATT_H
 
#include <memory>		// shared_ptr
#include <iostream>
#include <algorithm>
#include <complex>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
  
// long int is not used  
// Convenient abbreviations:  
typedef unsigned int uint;
typedef long long int lint;
typedef unsigned long long int ulint;
typedef long double ldbl;
typedef complex<float> cflt;
typedef complex<double> cdbl;
typedef complex<long double> cldbl;

/*	F=Column major, dense,Fortran
	C=Row major, dense,C
	S=sparse-dictionary, TBI    */
enum Stortype {F, C, S };

#define USE_ABC
	//	compile COTRs 1-4 with  :Mat()	see below
#undef USE_ABC
	//  eliminate use of ABC
#define USE_MATTS
	//	class Matts<,> ref/pointers for slices
	// included below, after Mat{  };
#undef USE_MATTS
	//	default for basic Matrix class



//=== Mat =========================================
// Needed by Interpreter (if any).
class Mat		// ABC, Abstract Base Class
{
public:
virtual ~Mat() {}; // C++Primer 5th,p.622
protected:			// seen by derived classes only
Mat();			// default CTOR
 // need list of virtual functions.
 //virtual void print(string="")=0;
};	// end class  Mat

//=== MATT<T> ===================================
template<typename T, int St = F> 
	class Matt 
#ifdef USE_ABC
  : public Mat
#endif
{
// Basic Members: size_, buf[], stride_
protected:
 vector<lint> size_; // size/dims of each axis
 vector<lint> stride_; 
 string name_;

 Stortype stor_;	// storage type <ST>, enum Stortype{ }
 uint ndim_;		// number of axes/dimensions, = size_.size()
 ulint sizebuf_;	// = size() = buf.size()
 
public:
 lint size(int idim) const 	// size of axis idim
	{if(idim<0) return 0;  				// ERROR
	 if(idim>=size_.size()) return 1;	// ERROR
	 return size_[idim];  };
// or sizev[idim] == size(idim)
 lint size() const 		// size of buf[],
	{	lint sizeb = 1;	
		if(size_.size() == 0) sizeb = 0;
		for (int k=0; k<size_.size(); ++k) sizeb *= size_[k];
		return sizeb; };
 int ndim() const {return size_.size(); };
 const vector<lint>& sizev() const { return size_; };
 const vector<lint>& stride() const {return stride_; };
 Stortype stor() const { return stor_;};
 string& name() { return name_; };			// get name
 void name(string name1) { name_ = name1; }	// set name

 void init0(vector<lint> dim={});
 void init0(lint i1, lint i2);	// Matrix version.

public:	
 vector<T> buf;		// dense arrays <F,C>
 shared_ptr<vector<T>> spt;
 vector<T>& data;	// = *spt;
 Matt();								// CTOR 1
 Matt(lint nrow, lint ncol, string name="");	// CTOR 2
 Matt(vector<lint> dims, string name="");		// CTOR 2B N-Dim
 Matt(const Matt<T,St>& A);				// CTOR 3
 template<typename T2, int St2>			// CTOR 4 below
   Matt(const Matt<T2, St2>& A);
 Matt(T value);							// CTOR 5 1 x 1

virtual ~Matt<T, St>()			// v1
  { }
virtual void print(string="");

//=== OPERATORS ============================================

//=== op()  index ops
T& operator()(lint nr, lint nc);		// on objects  OP()-1
const T& operator()(lint nr, lint nc) const;// on temporaries OP()-2

//=== op=() =========================
Matt<T, St>& operator=(const Matt<T, St>& rhs);
template<class Tr, int Str>
Matt<T,St>& operator=(const Matt<Tr, Str>& rhs);

//=== op<<(), op>>() =======================
Matt<T, St>& operator<<(const vector<T>& data);
void operator>>(vector<T>& data);

//=== op+(), op-() ======================
Matt<T,St> operator+(const Matt<T,St>& rhs);

Matt<T,St> operator-(const Matt<T,St>& rhs);

Matt<T,St> operator*(const Matt<T,St>& rhs);

// FUNCTIONS:
//Matt<T,St> invgjv();

};	// class Matt


//=== MATTS ===========================================
// copied to Matts.h

#ifdef USE_MATTS
#include "Matts.h"
#endif

//=== END MATTS ========================================

//template<class T, int St> X promote <double> to Matt<double>
//Matt<T,St> operator+(const Matt<T,St>& lhs,	const Matt<T,St>& rhs);

//=== UTILITY FUNCTIONS, non-member =======================

/* -- STRIDE1() ----------------------------------------
	Utility function, called by init0() during CTOR, 
    stride1() avoids conflict with .stride()
I	size_	size of each axis/dimension
O	stride_ stride for each axis, used by index()
*/
template<int St>
void stride1(const vector<lint>& size_, vector<lint>& stride_) {};

template<>		//  Mat.cc
void stride1<F>(const vector<lint>& siz, vector<lint>& M);

template<>		// Mat.cc
void stride1<C>(const vector<lint>& siz, vector<lint>& M);


lint index( const vector<lint>& M, const vector<lint>& inx); //Mat.cc

/* -- NEXTINDX ---------------
  Iterates thru all possible index vectors.
<F>	left to right,  <C> right to left
I	size_	size of each axis/dimension
I	stride	strides, for <F or C>
I/O inx		current indices, from [0,0..0] to [N0-1, N1-1...]
*/
template<int St>
lint nextindx(const vector<lint>& size_, 
	vector<lint>& inx, lint& index) { };
template<>		// Mat.cc
lint nextindx<F>(const vector<lint>& size_, 
		vector<lint>& inx, lint& index);
template<>		// Mat.cc
lint nextindx<C>(const vector<lint>& size_, 
		vector<lint>& inx, lint& index);

template<class T>
void init_Mattimp();	// Matimp.h

template<class T, int St>
bool match(const Matt<T,St>& lhs, const Matt<T,St>& rhs);	//utility

void init_all();	// Mat.cc, force library functions

// ===CTORs ====== NOT IN LIBRARY: LIBMTRX1.SO
// clang requires all CTOR's here
// INIT0() Resets: size_, stride_, buf[], in Matimp.h

#ifdef USE_ABC
// CTORs use  :Mat()  ABC
template<class T, int St>		//  CTOR 1,  sizebuf_ = 0
Matt<T,St>::Matt():Mat()	//	ndim_ = 0
	{ init0(); }
template<class T, int St>		// CTOR-2 
Matt<T, St>::Matt(lint nrow, lint ncol, string name):Mat()
	{ init0({nrow, ncol}); name_ = name;}
template<class T, int St>		// CTOR 2-B
Matt<T,St>::Matt(vector<lint> dims, string name):Mat()  //(dims)
	{ init0(dims); name_ = name;}
template<typename T, int St>		// CTOR-3
Matt<T,St>::Matt(const Matt<T,St>& A):Mat()  //(A.size_ )
	{init0(A.sizev()); buf = A.buf;}
template<typename T, int St>		// CTOR-4, from Matimp.h
template<typename T2,int St2>	// limited by T = T2 conversion
Matt<T, St>::Matt(const Matt<T2, St2>& A):Mat()
	{init0(A.sizev());
 	 for(int k=0; k<sizebuf_; ++k) 
		buf[k] = A.buf[k]; //ERROR if St!=St2
	}

#else
// CTORs without  :Mat(), no ABC
template<class T, int St>		//  CTOR 1,  sizebuf_ = 0
Matt<T,St>::Matt():spt(make_shared< vector<T>>(0)),data(*spt)	//	ndim_ = 2
	{ init0({0,0}); }

template<class T, int St>		// CTOR-2 
Matt<T, St>::Matt(lint nrow, lint ncol, string name):spt(make_shared< vector<T>>(0)),data(*spt)
	{cout<<"CTOR 2" << endl; 
	 init0({nrow, ncol}); name_ = name;}

/*template<class T, int St>		// CTOR 2-B
Matt<T,St>::Matt(vector<lint> dims, string name)  //(dims)
	{ init0(dims); name_ = name;}	*/
template<typename T, int St>		// CTOR-3
Matt<T,St>::Matt(const Matt<T,St>& A):spt(make_shared< vector<T>>(0)),data(*spt)
	{init0(A.sizev()); data=A.data; } // buf = A.buf;

template<typename T, int St>		// CTOR-4
template<typename T2,int St2>	// limited by T = T2 conversion
Matt<T, St>::Matt(const Matt<T2, St2>& A):spt(make_shared< vector<T>>(0)),data(*spt)
	{
	 init0(A.sizev());
	 for(int k=0; k<sizebuf_; ++k) 
		data[k]=A.data[k];   //buf[k] = A.buf[k]; //ERROR if St!=St2
	}
template<typename T, int St>		// CTOR-5  1x1
Matt<T,St>::Matt(T value):spt(make_shared< vector<T>>(0)),data(*spt)
	{ init0( {1, 1});
	  data[0]=value;	 } //buf[0] = value;

#endif

// === MIXED TYPE OPERATORS =================================
template<class T, int St>
template<class Tr, int Str>
Matt<T,St>& Matt<T,St>::operator=(const Matt<Tr,Str>& rhs)
{ init0(rhs.sizev());
 for(lint k=0; k<sizebuf_; ++k) data[k]=rhs.data[k];
	 //buf[k] = rhs.buf[k]; //ERROR if St != St2
 return *this;
}
// ======================================================
// UTILITY FUNCTIONS:
// #include <chrono>
typedef chrono::high_resolution_clock::time_point Timept;
Timept hires_now();
double timediff(Timept start, Timept stop);		//secs,  Mat.cc

// ========================================================
// Matrix FUNCTIONS:	(defined in matfuncs.h )
template<typename T, int St>
Matt<T,St> inv(const Matt<T,St>& M);

#include "matfuncs.h"	// add to Matimp.h instead

#endif


/*  Matrix/tstconv.cc
test built-in type conversions
see MatCalc/tst/typeconv.cc

g++ -o tstconv tstconv.cc -std=c++11 -Wall

*/

#ifdef __clang__
#define COMPILER "CLANG "
#define VERSION __clang_major__
#define MINOR __clang_minor__
#elif __GNUC__
#define COMPILER "GNUC "
#define VERSION __GNUC__
#define MINOR __GNUC_MINOR__
#endif

#include <memory>		// shared_ptr<>
#include <algorithm> 
#include<complex>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>

using namespace std;



int main(int argc, char* argv[])
{cout<<"Starting TSTCONV"<<endl;
/*
// char vs int:
char xc1 = 4;  // xc1= 
char xc2 = 'A';
xc2 = xc1*xc1;
complex<char> xcc(29,30);
cout<<"xc1= "<< xc1 <<", xc2= "<< xc2
	<<", xcc= "<< xcc << endl;	// xcc= (,)
int  xi1 = xc1;
cout<<"xi1= "<< xi1 <<", (int)xc2= "<<(int)xc2 << endl;
*/
//============================
int	xi= 4;
float xf1;
xf1 = xi;	// float = int
xf1= 313;

xi = xf1;	// int = float
complex<double> zcd(1.,2.);
zcd= {3.,4.};	// assign to complex
zcd = xf1;	// complex <= real
cout<<"zcd= "<< zcd << endl;
//xf1 = zcd;	ERROR  real <= complex
xf1 = real(zcd);
cout<<"real zcd= "<< real(zcd) <<", imag zcd= "
	<<imag(zcd) << endl;
xf1 = real(xf1);
cout<<"real(xf1)= "<< real(xf1) <<", imag(xf1)= "
	<<imag(xf1) << endl;







}


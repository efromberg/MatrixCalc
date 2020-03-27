/*  tstMatt.cc
 Test Matrix class, Mat.h
  see Makefile
clang++
// compile with gdb, view with nm
$ g++ -c -g   -std=c++11  tstMatt.cc 
$ nm -lCn libmtrx.so > libmtrx-gcc.txt
requires LD_LIBRARY_PATH (set in ~/.bashrc)

*/


#include <typeinfo>
#include <chrono>
#include <ctime>
#include <complex>
using namespace std;

#include "Mat.h"

//#include "Matimp.h"	// precedence over libmtrx1.so



// F is default StorageType
int main( int argc, char* argv[])

{cout<<"Start tstMatt "<< endl;
auto start = chrono::high_resolution_clock::now();
freopen("tstMatt.log", "w", stderr);
time_t now = time(0);		// "Sun Mar 15 12:28:19 2020"
clog<<"Start tstMatt " <<", time: "<< ctime(&now) << endl;


// CTORs
Matt<double> matd2;						// CTOR-1
cout<<"CTOR 1"<< endl;
Matt<double, F> matd3(3,3,"matd3");		// CTOR-2
//Matt<double, F> matda({2,3,2},"matda");	// CTOR-2B
Matt<double, F> matd4(matd3);			// CTOR-3

Matt<double, F> matd1 = matd3;		// same as matd1(matd3)
//Matt<float, C> matf1(matd3);		// CTOR-4

int vsize= 12;
vector<double> vdat; 
vdat.assign(13, 0.);
 vdat.resize(vsize);
for(int k=0; k<vsize; ++k) vdat[k]= k;
cout<<"vdat.size= "<< vdat.size() << endl;


//Matt<float> matf2;
cout<<"tstMatt -1"<< endl;

matd4<<vdat;
cout<<"tstMatt -2"<< endl;
double x = matd4(1,1);		// op() - 1

matd4(1,1)= 17.;
matd4.print("matd4");
cout<<"tstMatt: after matd4.print" << endl;

Matt<float, F> matf3(3,3,"matf3");	// CTOR-2
cout<<"tstMatt: after matf3" << endl;
//matf3(1,2) = 15.;
matf3.print("matf3");

Matt<double, F> matd5(matf3);
matd5.print(" matd5(matf3);"); //ok

Matt<float, F> matf4(matd5);
matf4(0,2)=13.;
matf4.print("matf4");

matd4 = matd5;
matd4.print("matd4:");
matd4  = matf4;
matd4.print("matd4=matf4");
Matt<complex<double>, F> matcd1(4,4,"matcd1");
matcd1 = matd4;			// OK
matcd1.print("matcd1");
//matd4 = matcd1;		// fails
matd4.name("matd4x");
cout<<"name= "<< matd4.name() << endl;
cout<<"type= "<< typeid(matd4).name() << endl;

vector<lint> dim = {2, 3, 2};
//Matt<double,F> matd6(dim);
//matd6.print("matd6");
//Matt<double,F> matd7({2, 3, 3});
//matd7.print("matd7");
vector<double> data = { 0,1,2,3,4,5,6,7,8  };

/*matd7 << data;
matd7.print("matd7");
matd7 >> data; */
cout<<"data.size()= "<< data.size() << endl;
// ERROR below
matd4.print("matd4 before + matd5");
matd5.print("matd5");

matd4 = matd4 + matd5;
(matd4+matd5).print("matd4+matd5");

cout << (matd4+matd5)(1,2) << endl;
matd4 = matd4 - matd5;
matd4 = matd4*matd5;
matd4.print("matd4*matd5");
//Matt<float> matf5({4,4});		"ambiguous"
//Matt<float, F> matf5({4,4, 2});
(matd4*matf4).print("matd4*matf4");
(matf4*matd4).print("matf4*matd4");
(matd4+matd4*matf4).print("matd4+matd4*matf4");

double &dr = data[1];
dr = 3.5;
cout<<"dr= "<< dr <<", data[1]= "<< data[1] <<  endl;


//auto
chrono::high_resolution_clock::time_point stop = chrono::high_resolution_clock::now();
chrono::duration<double> elapsed = stop-start;
cout<< "elapsed= "<< elapsed.count() << endl; // time in sec's
cout<< (stop-start).count() << endl;
/*
Matts<double> matsd(matd7);
matsd.print("matsd");
*matsd.pbuf[3] = 17;
matd7.print("matd7");
*/

matd4<< data;
matd4 = matd4 + 3;	// C++ promote 3.2 to Matt<double>, CTOR 5
matd4.print("matd4=matd4+3.2");
//Matt<double,F> B= matd4.invgjv();
Matt<double,F> B= inv(matd4);
B.print("B=matd4.invgjv()");
(matd4*B).print("matd4*B");
matcd1 = matd4;
(matcd1*inv(matcd1)).print("matcd1*inv(matcd1)");

matd4 = 2.7;
matd4.print("matd4=2.7");


clog<<"tstMatt ending."<< endl;
}	// main()








/* tstnext.cc

 $ g++ -o tstnext tstnext.cc -L. -lmtrx1

*/

#include "Mat.h"

int main( int argc, char* argv[])
{ cout<<"Start tstnext" << endl;

Matt<double> matd(3,4);
vector<double> vect12(12);
for(int k=0; k<12; ++k) vect12[k] = k;

vector<lint> inxf, inxc;

vector<lint> dims;
dims = {3, 4, 3};
lint index;

inxf = {-1, 0, 0}; 
index= -1;
while(index < 3*4*3)
{
 nextindx<F>(dims, inxf, index);
 cout<<index <<", inxf: "<<inxf[0] << ", "<< inxf[1]
	<<", "<< inxf[2] << endl;
}	// while..
//---------------------------------------------
inxc = { 0, 0, -1};
index = -1;
while(index < 3*4*3)
{ nextindx<C>(dims, inxc, index);
  cout<<index <<", inxc: "<<inxc[0] << ", "<< inxc[1]
	<<", "<< inxc[2] << endl;
}	// while ...

}	// main



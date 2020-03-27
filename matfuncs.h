/* matfuncs.h

*/

#ifndef _MATFUNCS_H
#define _MATFUNCS_H

//from MatCalc/Mat2.h,  based on mfuncs.h/invgj()
// E.S.Quintana,..., "Efficient Matrix Inversion..."

template <class T, int St>
//Matt<T,St> Matt<T,St>::invgjv()
Matt<T,St> inv(const Matt<T,St>& M)
{
 // Matt<T,St> A(*this);	// copy, so *this is unchanged
  Matt<T,St> A(M);		// copy, so M is unchanged
  Matt<T,St> B;		// rhs of  A*x = B
  int n, nc, k, m;	// rows & columns
  n = A.size(0); nc = A.size(1);
  if((n != nc)||(n == 0)) {
  	cerr<<"ERROR - inv(A): Matrix is not square "
	<<n <<", "<< nc << endl;
  	return B;}

  B.init0( n,n);
  m = B.size(1);		// future: B n x m
  for( k=0; k< n; ++k) B(k,k) = (T)1.0;  // identity matrix
  ldbl  Amx, Amx2;	// not if T=cmplx
  T Atmp = 0.;
  int ir, jmx, jr, jc;
  
  for(ir = 0; ir<n; ++ir)	// loop thru rows
  { 						// find largest leading value
    Amx = abs(A(ir,ir));
    jmx = ir;
    for(jr = ir; jr<n; ++jr)	// search following rows
    { Amx2 = abs(A(jr,ir));
      if(Amx2 > Amx) 
      { Amx = Amx2;  // new max found
      	 jmx = jr; }
    }	// for(jr = ir; jr<n; ++jr)
 // here have:  jmx, Amx
    for(jc = ir; jc<n; ++jc)	// A:interchange rows ir and jmx
      {Atmp=A(ir,jc); A(ir,jc)=A(jmx,jc); A(jmx,jc)=Atmp;}
    for(jc=0; jc<m; ++jc)		// B: interchange
      {Atmp=B(ir,jc); B(ir,jc)=B(jmx,jc); B(jmx,jc)=Atmp;}
    Atmp = A(ir,ir);	// should check  != 0.
    if(abs(Atmp) == 0.) 
    	{cerr<<"ERROR - INV() max pivot element = 0." << endl;
    	 B.init0(0,0); return B; }
    for(jc=ir; jc < n; ++jc) A(ir,jc) = A(ir,jc)/Atmp; //normalize
    for(jc=0; jc <m; ++jc) B(ir,jc) = B(ir,jc)/Atmp;
    	// now A(ir,ir)=1.0; subtract row ir from all rows
    for(jr = 0; jr<n; ++jr) if(jr != ir) //except row ir
    	{Atmp = A(jr,ir);
    	 for(jc=ir; jc<n; ++jc) A(jr,jc)=A(jr,jc)-Atmp*A(ir,jc);
      	 for(jc=0; jc<m; ++jc)  B(jr,jc)=B(jr,jc)-Atmp*B(ir,jc);
      	}
   }	//  for(ir = 0; ir<n; ++ir)
   return B;
}	//  invgjv()



#endif



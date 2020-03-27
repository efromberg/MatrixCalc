#ifndef _MATTIMP_H
#define _MATTIMP_H

#include <iostream>

// == IMPLEMENTATION Matt<T> ============================
//  CLANG requires all CTORs to be in Mat.h

template<typename T, int St>	// print() 2D only
void Matt<T,St>::print(string str)
{ std::cout<<str<<", "<<name_ 
  <<", "<<size() <<" = ";
  for(int kd=0; kd<size_.size(); ++kd) cout << size_[kd]<<" ";
  cout<<", typeid: "<< typeid(*this).name()
   << ", this: "<< this  << endl;
int kr, kc, k3;
if(size_.size() == 2)
 for(kr=0; kr<size(0); ++kr)
	{std::cout<<kr <<" ";
	 for(kc=0; kc<size(1); ++kc) std::cout<< (*this)(kr,kc) <<", ";
	 std::cout<< '\n';
	}	// for(kr...

 else cout<<"ndim= "<< size_.size() << endl;
}	//	print()


// MATT: INIT0() Resets: size_, stride_, buf[]
template<class T, int St>
void Matt<T,St>::init0(vector<lint> dim)
{ size_ = dim;
  for(int k=0; k<size_.size(); ++k) size_[k] = max(lint(0), size_[k]);
  stride1<St>(size_, stride_);
  buf.clear();
  buf.resize(size(), 0);

  (*spt).resize(size(), 0);
 // data = *spt;

  stor_ = (Stortype)St;
  sizebuf_ = size();
  ndim_ = dim.size();
 
 // name_ unchanged
}
template<class T, int St>
void Matt<T,St>::init0(lint i1, lint i2)
{ init0( {i1, i2}); }

//=== OPERATORS ============================================

//=== op()  index ops

template<typename T, int St>		// OP()-1 on objects
T& Matt<T,St>::operator()(lint nr, lint nc)
  { //return buf[ index(stride_,  {nr,nc} )];
	return data[ index(stride_,  {nr,nc} )];  }	

template<typename T, int St>		// OP()-2
const T& Matt<T,St>::operator()(lint nr, lint nc) const	// on temps
{ //return buf[ index(stride_,  {nr,nc} )  ];
  return data[ index(stride_,  {nr,nc} )];  }		// column major

//=== op=() =========================
template<class T, int St>
Matt<T,St>& Matt<T,St>::operator=(const Matt<T,St>& rhs)
{init0({rhs.size(0),rhs.size(1)});
 lint sizez = size();
 //for(lint k=0; k<sizez; ++k) buf[k] = rhs.buf[k];
 for(lint k=0; k<sizez; ++k) data[k] = rhs.data[k];
 return *this;
}
//=== op<<(), op>>() =========================
template<class T, int St>
Matt<T, St>& Matt<T,St>::operator<<(const vector<T>& rhs)
{ if(size() == 0) init0( {(lint)rhs.size(),(lint)1} );
  lint k;
  lint siz = min( size(), (lint)rhs.size()); // read what fits
  /*for(k=0; k<siz; ++k) buf[k] = rhs[k];	// uses DENSE_COL or DENSE_ROW
  for(k=rhs.size(); k<size(); ++k) buf[k] = 0; // 0 fill remainder */
  for(k=0; k<siz; ++k) data[k] = rhs[k];
  for(k=rhs.size(); k<size(); ++k) data[k] = (T)0;

  return *this;
}
template<class T, int St>
void Matt<T,St >::operator>>(vector<T>& rhs)
{ rhs.resize(size());	// adjust output vector to fit
 for(lint k=0; k<size(); ++k) rhs[k] = data[k];	// buf[k];
}

//=== op+(), op-() ======================

template<class T, int St>
Matt<T, St> Matt<T,St>::operator+(const Matt<T,St>& rhs)
{ Matt<T,St> ret;
 // if( !match(*this, rhs) ) return ret;	// ret 0x0 - error
  ret.init0( size_ );
  if(size_ == rhs.sizev())
  	for(lint k=0; k< size(); ++k) 
		ret.data[k]=data[k] + rhs.data[k];
		//ret.buf[k]=buf[k]+rhs.buf[k];
  else if(rhs.size()==1) 
	for(lint k=0; k< size(); ++k) 
		ret.data[k] = data[k]+rhs.data[0];
		//ret.buf[k]=buf[k]+rhs.buf[0];
  return ret;
} 
/*
template<class T, int St>
Matt<T,St> operator+(const Matt<T,St>& lhs,
					const Matt<T,St>& rhs)
{ Matt<T,St> ret;
  ret.init0( lhs.sizev() );
  if(lhs.sizev() == rhs.sizev()){
	ret.init0(lhs.sizev());
  	for(lint k=0; k< lhs.size(); ++k) 
		ret.buf[k]= lhs.buf[k]+rhs.buf[k];}
  else if(rhs.size()==1) {
	ret.init0(lhs.sizev());
	for(lint k=0; k< lhs.size(); ++k) 
		ret.buf[k]= lhs.buf[k]+rhs.buf[0];}
  else if(lhs.size()==1) {
	ret.init0(rhs.sizev());
	for(lint k=0; k< rhs.size(); ++k) 
		ret.buf[k]= lhs.buf[0]+rhs.buf[k];}
  return ret;
}  */

template<class T, int St>
Matt<T,St> Matt<T,St>::operator-(const Matt<T,St>& rhs)
{ Matt<T,St> ret;
  if( !match(*this, rhs) ) return ret;	// ret 0x0 - error
  ret.init0( size_ );
  for(lint k=0; k< size(); ++k) 
		ret.data[k] = data[k] - rhs.data[k];
		//ret.buf[k]=buf[k]-rhs.buf[k];
  return ret;
}
//=== op*() ============================================
// 2D matrices only
template<class T, int St>
Matt<T,St> Matt<T,St>::operator*(const Matt<T,St>& rhs)
{Matt<T,St> ret;
 if(size(1) != rhs.size(0)) return ret;	// error ret(0x0)
 lint Nr = size(0);		//rows
 lint Nc = rhs.size(1);	//cols
 lint Ns = size(1);		// summation range
 ret.init0({Nr, Nc});
 lint kr,kc,ks;
 T tmp;
 for(kr=0; kr< Nr; ++kr) for(kc=0; kc<Nc; ++kc)
  {tmp = 0.;
   for(ks = 0; ks<Ns; ++ks)
       tmp+= this->operator()(kr, ks) * rhs(ks, kc);
   ret(kr, kc) = tmp;
   }	// for(kr...
 
  return ret;
}
//=== Utility Functions, non-member ==================
// same Dims & same STOR
template<class T, int St>
bool match(const Matt<T,St>& lhs, const Matt<T, St>& rhs)
{bool ret = true;
 ret = (lhs.sizev().size()==rhs.sizev().size());
 if( !ret) return ret;
 for(int k=0; k<lhs.sizev().size(); ++k) 
		ret=(lhs.sizev()[k]==rhs.sizev()[k])&&ret;
 ret = (lhs.stor() == rhs.stor())&&ret;
 return ret;
}

// called by Mat.cc/init_all()
template<class T, int St>
void init_Mattimp()
{
 
 Matt<T, St> mat0; 			// CTOR-1
 Matt<T,St> mat1(1,1);		// CTOR-2
 mat1.init0(1,1);	// compile init0(lint,lint)
 Matt<T,St> mat2(mat1); 	// CTOR-3
 //T x =  mat1(0,0);		// OP()-1
 mat1 = mat2;			// op=()
 vector<lint> dims={ 2,3,2};
		// Matt<T,St> mat6(dims);	X Version 1.0
 vector<T> data = {1,2, 3};
 mat1 << data;
 mat1 >> data;
 match(mat1, mat2);
 mat1+mat1;
 mat1-mat1;
 mat1*mat1;

// Matts<T,St> mats1(mat6);
//mats1.print("mats1");

}


#endif


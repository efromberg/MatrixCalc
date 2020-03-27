/* Matts.h		Slice type array class, ref/ptrs to source
extracted from Mat.h
Included in Mat.h if #define USE_MATTS

*/

#ifndef _MATTS_H
#define _MATTS_H

template<typename T, int St = F> 
	class Matts 

{protected:
 vector<lint> size_; // size/dims of each axis
 vector<lint> stride_; 
 string name_;

 Stortype stor_;	// storage type <ST>, enum Stortype{ }
 uint ndim_;		// number of axes/dimensions, = size_.size()
 ulint sizebuf_;	// = size() = buf.size()
 
public:
 lint size(int idim) const 
	{if(idim<0) return 0;  				// ERROR
	 if(idim>=size_.size()) return 1;	// ERROR
	 return size_[idim];  };
// or sizev[idim] == size(idim)

 lint size() const {		// size of buf[],
		lint sizeb = 1;	
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

public:
// T* buf;		// dense arrays <F,C>
 vector<T*> pbuf;
// Matts();					// CTOR 1
 Matts(const Matt<T,St>& A);	// CTOR 3
 ~Matts<T, St>()			// v1
  { }
 void print(string="");
};  
//  ---  MATTS ------------------------------
template<typename T, int St> 
Matts<T, St>::Matts(const Matt<T,St>& A)	// CTOR 3
{init0(A.sizev());
 cout<<"MATTS(): sizebuf_= "<< sizebuf_ << endl;
for(int k=0; k<sizebuf_; ++k) cout<< A.buf[k]<<", "; cout<< endl;
for(int k=0; k<sizebuf_; ++k) pbuf[k] =(T *)&(A.buf[k]); 
}

// INIT0() Resets: size_, stride_, buf[], in Matimp.h 
// MATTS:  INIT0() Resets: size_, stride_, buf[]
template<class T, int St>
void Matts<T,St>::init0(vector<lint> dim)
{ size_ = dim;
  for(int k=0; k<size_.size(); ++k) size_[k] = max(lint(0), size_[k]);
  //stride1<St>(size_, stride_); 	// in Mat.cc
  pbuf.clear();
  pbuf.resize(size(), 0);
  stor_ = (Stortype)St;
  sizebuf_ = size();
  ndim_ = dim.size();
 // name_ unchanged
} 

template<typename T, int St>	// print() 2D, 3D 
void Matts<T,St>::print(string str)
{ std::cout<<str<<", "<<name_ 
  <<", typeid: "<< typeid(*this).name()
  <<", "<<size() <<" = ";
  for(int kd=0; kd<size_.size(); ++kd) cout << size_[kd]<<", ";
  cout << " this: "<< this  << endl;
  cout<<"*pbuf[]= ";
  for(int k=0; k<size(); ++k)cout<< *pbuf[k] <<", ";
  cout<<endl;
}

#endif


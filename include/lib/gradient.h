#ifndef GRADIENT_H
#define GRADIENT_H

template<class T, float p=0.1f>
class gradient {
public:
	gradient(){} 
	gradient(const T& v): val(v), tar(v) {}
	gradient(const T& v, const T& t): val(v), tar(t) {}
	
	inline void setTar(const T& targ) { tar = targ; }
	inline T getTar() { return tar; }
	inline void setVal(const T& valu) { val = valu; }
	inline T getVal() { return val; }
	inline void upd() { val = val + (tar - val) * p; }
private:
	T val, tar;
};

#endif

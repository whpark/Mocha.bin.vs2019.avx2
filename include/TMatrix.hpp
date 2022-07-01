#pragma once

// TMatrix.hpp: TMatrix class.
//
// PWH
// 2000.05.27 
// 2008.11.01
//
//////////////////////////////////////////////////////////////////////

template <class T> class TMatrix {
protected:
	int m_nM, m_nN;
	T **m_M;
	T *m_body;
public:
	int M(void) const {return m_nM;}
	int N(void) const {return m_nN;}
	TMatrix(void) { m_body = NULL; m_M = NULL; m_nM = m_nN = 0;}
	TMatrix(int m, int n);
	TMatrix& Set(int m, int n);
	TMatrix(int m, int n, T f);
	TMatrix& Set(int m, int n, T f);
	TMatrix(const TMatrix& B);
	TMatrix& Set(const TMatrix& B);
	template < class D >
	TMatrix& SetFrom(int m, int n, const D d[]) {
		Set(m, n);
		for (int i = 0; i < m_nM; i++) {
			int ix = i * n;
			for (int j = 0; j < m_nN; j++) {
				m_M[i][j] = d[ix + j];
			}
		}
		return *this;
	}

	void Attach(T* body, int m, int n) {
		Destroy();
		m_body = body;
		if (m && n) {
			m_nM = m;
			m_nN = n;

			m_M = new T*[m_nM];
			T* pt = m_body;
			int i;
			for (i = 0; i < m_nM; i++, pt += m_nN)
				m_M[i] = pt;
		}
	}
	T* Detach() {
		T* pt = m_body;
		m_body = NULL;
		Destroy();
		return pt;
	}

	template < class Image >
	TMatrix& SetFromImage(Image& img, int eRGB = 0) { return SetFromBitmap(img.GetWidth(), img.GetHeight(), img.GetBits(), img.GetPitch(), img.GetBPP(), eRGB); }
	TMatrix& SetFromBitmap(int nWidth, int nHeight, void* bmp, int nPitch, int nBPP, int eRGB = 0) {
		if ( (m_nM != nWidth) || (m_nN != nHeight) )
			Set(nWidth, nHeight);

		BYTE* buffer = (BYTE*)bmp;

		int nBytePerPixel = nBPP/8;
		if (nBytePerPixel == 0)
			nBytePerPixel = 1;
		if (eRGB == 4) {
			for (int j = 0; j < m_nN; j++, buffer += nPitch) {
				for (int i = 0, x = 0; i < m_nM; i++, x += nBytePerPixel) {
					m_M[i][j] = *(COLORREF*)(buffer+x);
				}
			}
		} else {
			for (int j = 0; j < m_nN; j++, buffer += nPitch) {
				for (int i = 0, x = 0; i < m_nM; i++, x += nBytePerPixel) {
					m_M[i][j] = buffer[x + eRGB];
				}
			}
		}
		return *this;
	}
	template < class Image >
	TMatrix& SetToImage(Image& img, int eRGB = -1) { return SetToBitmap(img.GetWidth(), img.GetHeight(), img.GetBits(), img.GetPitch(), img.GetBPP(), eRGB); }
	TMatrix& SetToBitmap(int nWidth, int nHeight, void* bmp, int nPitch, int nBPP, int eRGB = -1) {
		BYTE* buffer = (BYTE*)bmp;
		int nBytePerPixel = nBPP/8;
		if (nBytePerPixel == 0)
			nBytePerPixel = 1;
		if (eRGB == 4) {
			for (int j = 0; j < m_nN; j++, buffer += nPitch) {
				for (int i = 0, x = 0; i < m_nM; i++, x += nBytePerPixel) {
					*(COLORREF*)(buffer+x) = (COLORREF)m_M[i][j];
				}
			}
		} else if (eRGB == -1) {
			for (int j = 0; j < m_nN; j++, buffer += nPitch) {
				for (int i = 0, x = 0; i < m_nM; i++, x += nBytePerPixel) {
					BYTE v = (BYTE)m_M[i][j];
					BYTE* p = buffer+x;
					for (int iPixel = 0; iPixel < nBytePerPixel; iPixel++, p++) {
						*p = v;
					}
				}
			}
		} else {
			for (int j = 0; j < m_nN; j++, buffer += nPitch) {
				for (int i = 0, x = 0; i < m_nM; i++, x += nBytePerPixel) {
					BYTE v = (BYTE)m_M[i][j];
					BYTE* p = buffer+x+eRGB;
					*p = v;
				}
			}
		}

		return *this;
	}

	void Destroy(void);
	~TMatrix(void);
	T* operator[] (int i) {return m_M[i];}
	T& operator() (int i, int j) {
		ASSERT( (i < m_nM) && (i >= 0) );
		ASSERT( (j < m_nN) && (j >= 0) );
		return m_M[i][j];
	}
	const T& operator() (int i, int j) const {
		ASSERT( (i < m_nM) && (i >= 0) );
		ASSERT( (j < m_nN) && (j >= 0) );
		return m_M[i][j];
	}
	T& GetSafeValue(int i, int j) {
		ASSERT(m_M);
		if (i < 0) i = 0;
		else if (i >= m_nM) i = m_nM-1;
		if (j < 0) j = 0;
		else if (j >= m_nN) j = m_nN-1;
		return m_M[i][j];
	}
	const T& GetSafeValue(int i, int j) const {
		ASSERT(m_M);
		if (i < 0) i = 0;
		else if (i >= m_nM) i = m_nM-1;
		if (j < 0) j = 0;
		else if (j >= m_nN) j = m_nN-1;
		return m_M[i][j];
	}
	T& operator() (int i) {
		ASSERT( (m_nM == 1) || (m_nN == 1) );
		if (m_nM == 1) {						// column vector
			ASSERT( (i < m_nN) && (i >= 0) );
			return m_M[0][i];
		} else {							// row vector
			ASSERT( (i < m_nM) && (i >= 0) );
			return m_M[i][0];
		}
	}
	void SetElement(const T& t) { int i, j; for (i = 0; i < m_nM; i++) for (j = 0; j < m_nN; j++) m_M[i][j] = t; };
	TMatrix& operator = (const TMatrix& B);
	int operator == (const TMatrix& B);
	int operator != (const TMatrix& B);
	TMatrix operator + (const TMatrix& B);
	TMatrix& operator += (const TMatrix& B);
	TMatrix operator - (const TMatrix& B);
	TMatrix& operator -= (const TMatrix& B);
	TMatrix operator * (const TMatrix& B);
	TMatrix& operator *= (const TMatrix& B);
	TMatrix& operator *= (const T& a);
	TMatrix operator ^ (int r);
	TMatrix& operator ^= (int r);
	template <class D>
	TMatrix Convolute(const TMatrix<D>& B, const T& vFactor);
	T Sum(void) const;
	TMatrix trans(void);
	T det(void);
	void eigen(TMatrix& E_value, TMatrix& E_Vector) const;
	TMatrix inv(void);

	TMatrix<T> Partial(int m_nM, int m_nN, int x, int y) {
		TMatrix<T> M;
		M.Set(m_nM, m_nN);

		for (int i = 0; i < m_nM; i++) {
			for (int j = 0; j < m_nN; j++) {
				M(i, j) = (*this)(i+m_nM/2, j+m_nN/2);
			}
		}
		return M;
	}

	template < class Vector >
	friend TMatrix<Vector> operator*(const TMatrix<double>& A, const TMatrix<Vector>& B);
	template < class Vector >
	friend TMatrix<Vector> operator*(const TMatrix<Vector>& A, const TMatrix<double>& B);
	template < class Vector >
	friend Vector operator*(TMatrix<double>& A, Vector& B);
	friend TMatrix operator *(double a, const TMatrix& B);

	friend void ludcmp(TMatrix&, int*);
	friend void lubksb(TMatrix&, TMatrix&, int*);

	friend void jacobi(TMatrix&, TMatrix&, TMatrix&);
	friend void eigsrt(TMatrix&, TMatrix&);
};

template <class T>
int Mcheck(TMatrix<T> A);

template <class T> TMatrix<T>::TMatrix(int m, int n) {
	m_body = NULL;
	m_M = NULL;
	m_nM = m_nN = 0;
	Set(m, n);
}

template <class T> TMatrix<T>& TMatrix<T>::Set(int m, int n) {
	Destroy();

	if (m && n) {
		m_nM = m;
		m_nN = n;

		m_body = new T[m_nM*m_nN];
		m_M = new T*[m_nM];
		T* pt = m_body;
		int i;
		for (i = 0; i < m_nM; i++, pt += m_nN)
			m_M[i] = pt;

	}
	return *this;
}

template <class T> TMatrix<T>::TMatrix(int m, int n, T f) {
	m_body = NULL;
	m_M = NULL;
	m_nM = m_nN = 0;
	Set(m, n, f);
}

template <class T> TMatrix<T>& TMatrix<T>::Set(int m, int n, T f) {
	Destroy();
	int i, j;

	Set(m, n);

	for (i = 0; i < m_nM; i++) {
		for (j = 0; j < m_nN; j++)
			m_M[i][j] = f;
	}

	return *this;
}

template <class T> TMatrix<T>::TMatrix(const TMatrix<T>& B) {
	m_body = NULL;
	m_M = NULL;
	m_nM = m_nN = 0;
	Set(B);
}

template <class T> TMatrix<T>& TMatrix<T>::Set(const TMatrix<T>& B) {
	Destroy();

	Set(B.M(), B.N());
	if (m_body && B.m_body)
		memcpy(m_body, B.m_body, m_nM*m_nN*sizeof(T));

	return *this;
}

template <class T> TMatrix<T>::~TMatrix(void) {
	Destroy();
}

template <class T> void TMatrix<T>::Destroy(void) {
	if (m_M)
		delete [] m_M;
	if (m_body)
		delete [] m_body;

	m_body = NULL;
	m_M = NULL;
	m_nM = m_nN = 0;
}

template <class T> TMatrix<T>& TMatrix<T>::operator = (const TMatrix<T>& B) {
	int i, j;

	if (this == &B)
		return *this;

//	ASSERT( (m_nM == B.m_nM) && (m_nN == B.m_nN) );

	Set(B.m_nM, B.m_nN);

	for (i = 0; i < m_nM; i++) {
		for (j = 0; j < m_nN; j++)
			m_M[i][j] = B.m_M[i][j];//memcpy(m_M[i], B.m_M[i], m_nN*sizeof(T));
	}

	return *this;
}

template <class T> int TMatrix<T>::operator == (const TMatrix<T>& B) {
	int i;
	if ( (m_nM != B.m_nM) || (m_nN != B.m_nN) ) return 0;
	for (i = 0; i < m_nM; i++)
		if (!memcmp(m_M[i], B.m_M[i], m_nN)) return 0;
	return 1;
}

template <class T> int TMatrix<T>::operator != (const TMatrix<T>& B) {
	int i;
	if ( (m_nM == B.m_nM) && (m_nN == B.m_nN) )
		for (i = 0; i < m_nM; i++)
			if (memcmp(m_M[i], B.m_M[i], m_nN)) return 1;
	else return 1;

	return 0;
}

template <class T> TMatrix<T> TMatrix<T>::operator + (const TMatrix<T>& B) {
	int i, j;

	ASSERT( (m_nM == B.m_nM) && (m_nN == B.m_nN) );

	TMatrix<T> C(m_nM, m_nN);
	for (i = 0; i < m_nM; i++)
		for (j = 0; j < m_nN; j++)
			C.m_M[i][j] = m_M[i][j] + B.m_M[i][j];

	return C;
}

template <class T> TMatrix<T>& TMatrix<T>::operator += (const TMatrix<T>& B) {
	int i, j;

	ASSERT( (m_nM == B.m_nM) && (m_nN == B.m_nN) );

	for (i = 0; i < m_nM; i++)
		for (j = 0; j < m_nN; j++)
			m_M[i][j] += B.m_M[i][j];

	return *this;
}

template <class T> TMatrix<T> TMatrix<T>::operator - (const TMatrix<T>& B) {
	int i, j;

	ASSERT( (m_nM == B.m_nM) && (m_nN == B.m_nN) );

	TMatrix<T> C(m_nM, m_nN);
	for (i = 0; i < m_nM; i++)
		for (j = 0; j < m_nN; j++)
			C.m_M[i][j] = m_M[i][j] - B.m_M[i][j];

	return C;
}

template <class T> TMatrix<T>& TMatrix<T>::operator -= (const TMatrix<T>& B) {
	int i, j;

	ASSERT( (m_nM == B.m_nM) && (m_nN == B.m_nN) );

	for (i = 0; i < m_nM; i++)
		for (j = 0; j < m_nN; j++)
			m_M[i][j] -= B.m_M[i][j];

	return *this;
}

template <class T> TMatrix<T> TMatrix<T>::operator * (const TMatrix<T>& B) {
	int i, j, k;

	if ( (B.m_nM == 1) && (B.m_nN == 1) ) {
		TMatrix<T> C(m_nM, m_nN);
		for (i = 0; i < m_nM; i++)
			for (j = 0; j < m_nN; j++)
				C.m_M[i][j] *= B.m_M[0][0];
		return C;
	}

	ASSERT(m_nN == B.m_nM);

	TMatrix<T> C(m_nM, B.m_nN, 0.0);
	for (i = 0; i < C.m_nM; i++)
		for (j = 0; j < C.m_nN; j++)
			for (k = 0; k < m_nN; k++)
				C.m_M[i][j] += m_M[i][k] * B.m_M[k][j];

	return C;
}

template <class T> TMatrix<T>& TMatrix<T>::operator *= (const TMatrix<T>& B) {
	int i, j, k;

	ASSERT( (m_nM == B.m_nM) && (m_nN == B.m_nN) );

	TMatrix<T> C(m_nM, B.m_nN, 0.0);
	for (i = 0; i < C.m_nM; i++)
		for (j = 0; j < C.m_nN; j++)
			for (k = 0; k < m_nN; k++)
				C.m_M[i][j] += m_M[i][k] * B.m_M[k][j];

	*this = C;
	return *this;
}

template <class T> TMatrix<T> TMatrix<T>::operator ^ (int r) {
	int i;

	ASSERT(m_nM == m_nN);

	TMatrix<T> C(m_nM, m_nN, 0.0);

	if (r == 0) {
		for (i = 0; i < m_nM; i++)
			C.m_M[i][i] = 1.0;
		return C;
	}

	if (r < 0) {
		r = -r;
		C = inv();
	}

	for (i = 0; i < r-1; i++)
		C = C * *this;

	return C;
}

template <class T> TMatrix<T>& TMatrix<T>::operator ^= (int r) {
	return *this = (*this)^r;
}

#if (_MSC_VER >= _MSC_VER_VS2005)
template <class T> template < class D>
#else
template <class T, class D>
#endif
TMatrix<T> TMatrix<T>::Convolute(const TMatrix<D>& B, const T& vFactor) {
	TMatrix<T> C(m_nM, m_nN, vFactor-vFactor);

	int sh_i = B.M()/2, sh_j = B.N()/2;
	for (int i = 0; i < C.m_nM; i++) {
		for (int j = 0; j < C.m_nN; j++) {

			for (int k = 0; k < B.M(); k++) {
				for (int l = 0; l < B.N(); l++) {
					C(i, j) += GetSafeValue(i-sh_i+k, j-sh_j+l) * B(k, l);
				}
			}

		}
	}

	C *= vFactor;

	return C;
}

template <class T> T TMatrix<T>::Sum(void) const {
	T v = (*this)(0, 0);
	for (int i = 0; i < m_nM; i++) {
		for (int j = 0; j < m_nN; j++) {
			v += (*this)(i, j);
		}
	}

	v -= (*this)(0, 0);
	return v;
}

template <class T> TMatrix<T> TMatrix<T>::trans(void) {
	int i, j;

	TMatrix<T> C(m_nN, m_nM);
	for (i = 0; i < m_nN; i++)
		for (j = 0; j < m_nM; j++)
			C.m_M[i][j] = m_M[j][i];

	return C;
}

template <class T> T TMatrix<T>::det() {
	int dim;
	if (m_nM != m_nN) {return (T)0.0;}
	dim = m_nM;
	T d;
	d = (T)0.0;
	if (dim <= 3) {
		int i, j;
		for (i = 0; i < dim; i++) {
			T s;
			for (s = 1.0, j = 0; j < dim; j++)
				s *= m_M[j][(i+j)%dim];
			d += s;
			for (s = 1.0, j = 0; j < dim; j++)
				s *= m_M[j][(dim+i-j)%dim];
			d -= s;
		}
	} else {
		int i, j, k;
		TMatrix<T> C(m_nM-1, m_nN-1);
		for (i = 0; i < dim; i++) {
			if (m_M[0][i] == 0.0) continue;
			for (j = 0; j < C.m_nM; j++) {
				for (k = 0; k < C.m_nN; k++) {
					C[j][k] = m_M[j+1][ (k+i+1) % m_nN ];
				}
			}
			d += m_M[0][i] * C.det();
		}
	}
	return d;
}

/*
template <class T> T TMatrix<T>::detNew(void) const {
	int i, j, dim, level;
	T d, s;

	if (m_nM != m_nN) { return 0.0; }

	d = 0.0;
	dim = m_nM;

	if (dim == 1) return m_M[0][0];
	if (dim < 1) return 0.0;

	int sign = 0;
	int* index = new int [dim];
	for (i = 0; i < dim; i++) {
		for (j = i; j < dim; j++) {
			for (k = j; k < dim; k++) {
				for (l = k; l < dim; l++) {
					;
				}
			}
		}
	}

	delete [] index;

	return d;
}
*/
/* The Routine for TMatrix Inversion
 *      By L-U Decomposition And Back-Substitution
 */

// Routine edited at 1996. 3. 19.

template <class T> TMatrix<T> TMatrix<T>::inv(void) {
	int dim, i, j;
	int *index;

//	ASSERT(det() != 0.0);
	dim = m_nN;
	TMatrix<T> A(*this);
	TMatrix<T> B(dim, dim, 0.0);
	TMatrix<T> col(1, dim, 0.0);
	index = new int[dim];
	for (i = 0; i < dim; i++)
		index[i] = 0;

	ludcmp(A, index);

	for (j = 0; j < dim; j++) {
		for(i = 0; i < dim; i++) col.m_M[0][i] = .0;
		col.m_M[0][j] = 1.0;
		lubksb(A, col, index);
		for (i = 0; i < dim; i++) B.m_M[i][j] = col.m_M[0][i];
	}

	delete [] index;
	return B;
}

template <class T> void TMatrix<T>::eigen(TMatrix<T>& E_value, TMatrix<T>& E_Vector) const {

	TMatrix<T> A(*this);

	jacobi(A, E_value, E_Vector);

	eigsrt(E_value, E_Vector);
}

/* Routine for L-U Decomposition
 * ref> Numerical Recipes
 */

// Routine edited at 1996. 3. 19.
#define TINY (1.0e-20)
template <class T> void ludcmp(TMatrix<T>& A, int *index) {
	int imax, i, j, k, dim;
	T big, dum, sum, temp;

	dim = A.m_nN;
	TMatrix<T> vv(1, dim, 0);
	for (i = 0; i < dim; i++) {
		big = .0;
		for (j = 0; j < dim; j++)
			if ( (temp=fabs(A.m_M[i][j])) > big ) big = temp;
		ASSERT(big != 0.0);
		vv.m_M[0][i] = 1.0 / big;
	}
	for (j = 0; j < dim; j++) {
		for (i = 0; i < j; i++) {
			sum = A.m_M[i][j];
			for (k = 0; k < i; k++) sum -= A.m_M[i][k] * A.m_M[k][j];
			A.m_M[i][j] = sum;
		}
		big = .0;
		for (i = j; i < dim; i++) {
			sum = A.m_M[i][j];
			for (k = 0; k < j; k++) sum -= A.m_M[i][k] * A.m_M[k][j];
			A.m_M[i][j] = sum;
			if ( (dum=vv.m_M[0][i]*fabs(sum)) >= big ) {
				big = dum;
				imax = i;
			}
		}
		if (j != imax) {
			for (k = 0; k < dim; k++) {
				dum = A.m_M[imax][k];
				A.m_M[imax][k] = A.m_M[j][k];
				A.m_M[j][k] = dum;
			}
			vv.m_M[0][imax] = vv.m_M[0][j];
		}
		index[j] = imax;
		if ( A.m_M[j][j] == .0) A.m_M[j][j] = TINY;
		if ( j != (dim-1) ) {
			dum = 1.0 / (A.m_M[j][j]);
			for (i = j+1; i < dim; i++) A.m_M[i][j] *= dum;
		}
	}
}
#undef TINY

/* Routine for L-U Back Substitution
 * ref> Numerical Recipes
 */

// Routine edited at 1996. 3. 19.
template <class T> void lubksb(TMatrix<T>& A, TMatrix<T>& Col, int *index) {
	int ii = -1;
	int i, ip, j;
	int dim;
	T sum;

	dim = A.m_nN;
	for (i = 0; i < dim; i++) {
		ip = index[i];
		sum = Col.m_M[0][ip];
		Col.m_M[0][ip] = Col.m_M[0][i];
		if (ii != -1)
			for (j = ii; j < i; j++) sum -= A.m_M[i][j] * Col.m_M[0][j];
		else if (sum) ii = i;
		Col.m_M[0][i] = sum;
	}
	for (i = dim-1; i >= 0; i--) {
		sum = Col.m_M[0][i];
		for (j = i+1; j < dim; j++) sum -= A.m_M[i][j] * Col.m_M[0][j];
		Col.m_M[0][i] = sum / A.m_M[i][i];
	}
}

/* Eigen values & Eigen Vectors of Real Symmetric matrix A[m_nN][m_nN]
 * 1994. 9. 7.
 */

// Routine edited at 1996. 3. 19.

#define Rotate(A, i, j, k, l) \
	{\
		g = A[i][j];\
		h = A[k][l];\
		A[i][j] = g - s*(h + g*tau);\
		A[k][l] = h + s*(g - h*tau);\
	}\


#define Iter 50
template <class T> void jacobi(TMatrix<T>& A, TMatrix<T>& d, TMatrix<T>& V) {
	int iq, ip, i, j, dim;
	T tresh, theta, tau, t, sm, s, h, g, c;

	dim = A.m_nN;
	TMatrix<T> b(1, dim, 0.0);
	TMatrix<T> z(1, dim, 0.0);

	for (ip = 0; ip < dim; ip++) {
		for (iq = 0; iq < dim; iq++) V.m_M[ip][iq] = .0;
		V.m_M[ip][ip] = 1.0;
	}
	for (ip = 0; ip < dim; ip++) {
		b.m_M[0][ip] = d.m_M[0][ip] = A.m_M[ip][ip];
		z.m_M[0][ip] = .0;
	}

	for (i = 0; i < Iter; i++) {
		sm = .0;

		for (ip = 0; ip < dim-1; ip++)
			for (iq = ip+1; iq < dim; iq++)
				sm += fabs(A.m_M[ip][iq]);
		if (sm == .0)
			return;

		if (i < 3) tresh = 0.2 * sm / (dim*dim);
		else tresh = .0;

		for (ip = 0; ip < dim-1; ip++) {
			for (iq = ip+1; iq < dim; iq ++) {
				g = 100.0 * fabs(A.m_M[ip][iq]);
				if ( (i > 3) && (fabs(d.m_M[0][ip])+g == fabs(d.m_M[0][ip]))
					&& (fabs(d.m_M[0][iq])+g == fabs(d.m_M[0][iq])) )
					A.m_M[ip][iq] = .0;
				else
				if (fabs(A.m_M[ip][iq]) > tresh) {
					h = d.m_M[0][iq] - d.m_M[0][ip];
					if ( fabs(h) + g == fabs(h) ) t = A.m_M[ip][iq]/h;
					else {
						theta = 0.5 * h / A.m_M[ip][iq];
						t = 1.0 / ( fabs(theta) + sqrt(1.0 + theta*theta) );
						if (theta < .0) t = -t;
					}
					c = 1.0 / sqrt(1 + t*t);
					s = t*c;
					tau = s / (1.0 + c);
					h = t * A.m_M[ip][iq];
					z.m_M[0][ip] -= h;
					z.m_M[0][iq] += h;
					d.m_M[0][ip] -= h;
					d.m_M[0][iq] += h;
					A.m_M[ip][iq] = .0;

					for (j = 0; j < ip; j++) Rotate(A.m_M, j, ip, j, iq);
					for (j = ip + 1; j < iq; j++) Rotate(A.m_M, ip, j, j, iq);
					for (j = iq + 1; j < dim; j++) Rotate(A.m_M, ip, j, iq, j);
					for (j = 0; j < dim; j++) Rotate(V.m_M, j, ip, j, iq);
				}
			}
		}
		for (ip = 0; ip < dim; ip++) {
			b.m_M[0][ip] += z.m_M[0][ip];
			d.m_M[0][ip] = b.m_M[0][ip];
			z.m_M[0][ip] =.0;
		}
	}
	ASSERT(false);
	// !! Too many iterations in routine JACOBI
}
#undef Iter
#undef Rotate

/* Sorting the Eigen values & Eigen vectors
 *   from Jacobi() in Ascending Order
 * 1994. 9. 7. -> 1995. 3. 28.
 */

template <class T> void eigsrt(TMatrix<T>& d, TMatrix<T>& V) {
	int i, j, k;
	T p;
	int dim;

	dim = V.m_nN;
	for (i = 0; i < dim - 1; i++) {
		p = d.m_M[0][k=i];

		for (j = i + 1; j < dim; j ++)
			if (d.m_M[0][j] >= p) p = d.m_M[0][k=j];

		if (k != i) {
			d.m_M[0][k] = d.m_M[0][i];
			d.m_M[0][i] = p;
			for (j = 0; j < dim; j++) {
				p = V.m_M[j][i];
				V.m_M[j][i] = V.m_M[j][k];
				V.m_M[j][k] = p;
			}
		}
	}
}

template <class T> int Mcheck(TMatrix<T> A) {
	int i, j, dim;
	T d1;

	if (A.m_nN != A.m_nM) {
		return 0;
	}

	dim = A.m_nN;

	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++) {
			if (i == j) d1 = 1;
			else d1 = 0;
			if (A.m_M[i][j] != d1) return 0;
		}

	return 1;
}

template<class T> TMatrix<T> operator * (double a, const TMatrix<T>& B) {
	TMatrix<T> c = B;
	for (int i = 0; i < c.m_nM; i++) {
		for (int j = 0; j < c.m_nN; j++)
			c[i][j] *= a;
	}
	return c;
}

template<class T> TMatrix<T>& TMatrix<T>::operator *= (const T& a) {
	for (int i = 0; i < m_nM; i++) {
		for (int j = 0; j < m_nN; j++)
			m_M[i][j] *= a;
	}
	return *this;
}


#include <math.h>
typedef TMatrix<double> DMatrix;

template < class Vector >
TMatrix<Vector> operator*(const DMatrix& A, const TMatrix<Vector>& B) {
	int i, j, k;

	if ( (A.m_nM == 1) && (A.m_nN == 1) ) {
		TMatrix<Vector> C(B.m_nM, B.m_nN);
		for (i = 0; i < B.m_nM; i++)
			for (j = 0; j < B.m_nN; j++)
				C.m_M[i][j] = A.m_M[0][0] * B.m_M[i][j];
	}
	if ( (B.m_nM == 1) && (B.m_nN == 1) ) {
		TMatrix<Vector> C(A.m_nM, A.m_nN);
		for (i = 0; i < A.m_nM; i++)
			for (j = 0; j < A.m_nN; j++)
				C.m_M[i][j] = A.m_M[i][j] * B.m_M[0][0];
		return C;
	}

	ASSERT(A.m_nN == B.m_nM);

	TMatrix<Vector> C(A.m_nM, B.m_nN, Vector(0.0, 0.0, 0.0));
	for (i = 0; i < C.m_nM; i++)
		for (j = 0; j < C.m_nN; j++)
			for (k = 0; k < A.m_nN; k++)
				C.m_M[i][j] += A.m_M[i][k] * B.m_M[k][j];
	return C;
}

template < class Vector >
TMatrix<Vector> operator*(const TMatrix<Vector>& A, const DMatrix& B) {
	int i, j, k;

	if ( (A.m_nM == 1) && (A.m_nN == 1) ) {
		TMatrix<Vector> C(B.m_nM, B.m_nN);
		for (i = 0; i < B.m_nM; i++)
			for (j = 0; j < B.m_nN; j++)
				C.m_M[i][j] = A.m_M[0][0] * B.m_M[i][j];
	}
	if ( (B.m_nM == 1) && (B.m_nN == 1) ) {
		TMatrix<Vector> C(A.m_nM, A.m_nN);
		for (i = 0; i < A.m_nM; i++)
			for (j = 0; j < A.m_nN; j++)
				C.m_M[i][j] = A.m_M[i][j] * B.m_M[0][0];
		return C;
	}

	ASSERT(A.m_nN == B.m_nM);

	TMatrix<Vector> C(A.m_nM, B.m_nN, Vector(0.0, 0.0, 0.0));
	for (i = 0; i < C.m_nM; i++)
		for (j = 0; j < C.m_nN; j++)
			for (k = 0; k < A.m_nN; k++)
				C.m_M[i][j] += A.m_M[i][k] * B.m_M[k][j];
	return C;
}

/*
template < class Vector >
Vector operator*(DMatrix& A, Vector& B) {
	Vector C(B);
	Vector D(0, 0, 0, 0);
	int t;
	if (A.m_nM == 3) { t = 3; D[3] = 0.0; C.PreSet(); }
	else if (A.m_nM == 4) t = 4;
	for (int i = 0; i < t; i++)
		for (int j = 0; j < t; j++)
			D[i] += A[i][j] * B[j];
	return D;

}
*/

DMatrix MRotate3_x(double);
DMatrix MRotate3_y(double);
DMatrix MRotate3_z(double);

DMatrix MRotate4_x(double);
DMatrix MRotate4_y(double);
DMatrix MRotate4_z(double);

// modified at 1995.3.28.
inline DMatrix MRotate3_x(double theta) {
	DMatrix mat(3, 3, 0.0);
	double c, s;

	c = cos(theta); s = sin(theta);
	mat[1][1] = mat[2][2] = c;
	mat[1][2] = -s;
	mat[2][1] = s;
	mat[0][0] = 1.0;

	return mat;
}

inline DMatrix MRotate3_y(double theta) {
	DMatrix mat(3, 3, 0.0);
	double c, s;

	c = cos(theta); s = sin(theta);
	mat[0][0] = mat[2][2] = c;
	mat[0][2] = s;
	mat[2][0] = -s;
	mat[1][1] = 1.0;

	return mat;
}

inline DMatrix MRotate3_z(double theta) {
	DMatrix mat(3, 3, 0.0);
	double c, s;

	c = cos(theta); s = sin(theta);
	mat[0][0] = mat[1][1] = c;
	mat[0][1] = -s;
	mat[1][0] = s;
	mat[2][2] = 1.0;

	return mat;
}

inline DMatrix MRotate4_x(double theta) {
	DMatrix mat(4, 4, 0.0);
	double c, s;

	c = cos(theta); s = sin(theta);
	mat[1][1] = mat[2][2] = c;
	mat[1][2] = -s;
	mat[2][1] = s;
	mat[0][0] = 1.0;

	mat[3][3] = 1.0;

	return mat;
}

inline DMatrix MRotate4_y(double theta) {
	DMatrix mat(4, 4, 0.0);
	double c, s;

	c = cos(theta); s = sin(theta);
	mat[0][0] = mat[2][2] = c;
	mat[0][2] = s;
	mat[2][0] = -s;
	mat[1][1] = 1.0;

	mat[3][3] = 1.0;

	return mat;
}

inline DMatrix MRotate4_z(double theta) {
	DMatrix mat(4, 4, 0.0);
	double c, s;

	c = cos(theta); s = sin(theta);
	mat[0][0] = mat[1][1] = c;
	mat[0][1] = -s;
	mat[1][0] = s;
	mat[2][2] = 1.0;

	mat[3][3] = 1.0;

	return mat;
}

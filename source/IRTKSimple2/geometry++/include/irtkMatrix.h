/*=========================================================================

  Library   : Image Registration Toolkit (IRTK)
  Module    : $Id$
  Copyright : Imperial College, Department of Computing
              Visual Information Processing (VIP), 2008 onwards
  Date      : $Date$
  Version   : $Revision$
  Changes   : $Author$

Copyright (c) 1999-2014 and onwards, Imperial College London
All rights reserved.
See LICENSE for details

=========================================================================*/

#ifndef _IRTKMATRIX_H

#define _IRTKMATRIX_H

#ifdef USE_VXL
#include <vnl/vnl_matrix.h>
#else
#include <gsl/gsl_matrix.h>
#endif

#include <irtkObject.h>

#include <iostream>
#include <cmath>
#include <memory>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/archive/tmpdir.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/**

  Matrix class.

*/

class irtkMatrix : public irtkObject
{
    class Matrix {
    public:
	Matrix(){
	    rows_ = 0;
	    cols_ = 0;
	    data_ = NULL;
	}
    
    Matrix(int rows, int cols, std::unique_ptr<double[]> data)
	: rows_(rows), cols_(cols), data_(std::move(data)) {}
	
    Matrix(int rows, int cols)
	: rows_(rows), cols_(cols), data_(new double[rows * cols]) {
	    for (auto i = 0; i < (rows_ * cols_); ++i) {
		data_[i] = i;
	    }
	}
	
	void setRow(int r) {rows_ = r;}
	void setCol(int c) {cols_ = c;}
	//void setMat(double *m) {std::make_unique<double*>(m);}
	double* getMat() {return data_.get();}
	double *operator[](int row) const { return &data_[row * cols_]; }
	
    private:
        int rows_;
        int cols_;
	//double *data_;
	std::unique_ptr<double[]> data_;
    };
    
protected:

  /// Number of rows
  int _rows;

  /// Number of colums
  int _cols;

  /// Data
  //double **_matrix;
  Matrix _matrix;
    
  /// Serialization
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
      int i, j;
      ar & _rows & _cols;

      //need to allocate memory
      /*if (_matrix == NULL)
      {
	  _matrix = Allocate(_matrix, _rows, _cols);
      }

      for (i = 0; i < _rows; i++) 
      {
	  for (j = 0; j < _cols; j++) 
	  {
	      ar & _matrix[i][j];
	  }
	  }*/
  }

public:

  /// Default constructor
  irtkMatrix();

  /// Constructor for given number of rows and columns
  irtkMatrix(int, int);
  //irtkMatrix(int, int, double[]);
  irtkMatrix(int, int, std::unique_ptr<double[]>);

  /// Copy constructor
  irtkMatrix(const irtkMatrix &);

  /// Destructor
  ~irtkMatrix();

  /// Initialize matrix with number of rows and columns
  void Initialize(int, int);

  //
  // Matrix access functions
  //

  /// Returns number of rows
  int Rows() const;

  /// Returns number of columns
  int Cols() const;

  double* GetMatrix();
  /// Puts matrix value
  void   Put(int, int, double);

  /// Gets matrix value
  double Get(int, int) const;

  //
  // Operators for matrix access
  //

  /// Puts matrix value
  double& operator()(int, int);

  /// Gets matrix value
  double  operator()(int, int) const;

  /// Access matrix get operator
  irtkMatrix  operator()(int, int, int, int);

  /// Access matrix put operator
  void    operator()(irtkMatrix &, int, int);

  //
  // Matrix operators for doubles
  //

  /// Subtraction of a double
  irtkMatrix& operator-=(const double&);

  /// Addition of a double
  irtkMatrix& operator+=(const double&);

  /// Multiplication with a double
  irtkMatrix& operator*=(const double&);

  /// Division by a double
  irtkMatrix& operator/=(const double&);

  /// Return result of subtraction of a double
  irtkMatrix  operator- (const double&);

  /// Return result of addition of a double
  irtkMatrix  operator+ (const double&);

  /// Return result of multiplication with a double
  irtkMatrix  operator* (const double&);

  /// Return result of division by a double
  irtkMatrix  operator/ (const double&);

  //
  // Matrix operators for matrices
  //

  /// Matrix copy operator
  irtkMatrix& operator =(const irtkMatrix&);

  /// Matrix subtraction operator
  irtkMatrix& operator-=(const irtkMatrix&);

  /// Matrix addition operator
  irtkMatrix& operator+=(const irtkMatrix&);

  /// Matrix multiplication operator
  irtkMatrix& operator*=(const irtkMatrix&);

  /// Return result of matrix subtraction
  irtkMatrix  operator- (const irtkMatrix&);

  /// Return result of matrix addition
  irtkMatrix  operator+ (const irtkMatrix&);

  /// Return result of matrix multiplication
  irtkMatrix  operator* (const irtkMatrix&);

  /// Matrix inversion operator
  irtkMatrix  operator! (void);

  /// Matrix transpose operator
  irtkMatrix  operator~ (void);

  /// Matrix comparison operator =
  int operator==(const irtkMatrix &);

  // Matrix exponential via Pade approximation.
  // See Golub and Van Loan, Matrix Computations, Algorithm 11.3-1.
  friend irtkMatrix expm(irtkMatrix);

  // Matrix logarithm.
  friend irtkMatrix logm(irtkMatrix);

  // Matrix square root.
  friend irtkMatrix sqrtm(irtkMatrix);

  friend irtkMatrix FrechetMean(irtkMatrix *, int, int = 10);

  friend irtkMatrix FrechetMean(irtkMatrix *, double *, int, int = 10);

#ifndef USE_STL
  /// Comparison operator != (if USE_STL is defined, negate == operator)
  int operator!=(const irtkMatrix &);
#endif

  //
  // Matrix functions
  //

  /// Calculate norm of matrix
  double Norm(void) const;

  /// Calculate trace of matrix
  double Trace(void) const;

  // The infinity norm is the maximum of the absolute value row sums.
  double InfinityNorm(void) const;

  /// Calculate determinant of matrix
  double Det() const;

  /* Calculate SVD of matrix
   * now does the same as Eigenvalues (but eigenvalues are in square root)
   * algorithm chosen according to matrix properties (symmetry, diagonizability)
   */
  void   SVD(irtkMatrix &, irtkVector &, irtkMatrix &);

  /// Identity matrix
  void   Ident();

  /// Returns true if the matrix is an identity matrix.
  bool IsIdentity() const;

  /// Invert of matrix
  void   Invert();

  /// Adjugate of matrix and return determine;
  void   Adjugate(double &);

  /// Transpose matrix
  void   Transpose();

  /* Calculate eigenvalues and eigenvectors of matrix
   * now does the same as SVD (but eigenvalues are NOT in square root)
   * algorithm chosen according to matrix properties (symmetry, diagonizability)
   */
  void   Eigenvalues(irtkMatrix &, irtkVector &, irtkMatrix &);

  /// Calculate least square fit via SVD
  void   LeastSquaresFit(const irtkVector &, irtkVector &);

  //
  // Matrix in- and output
  //

  /// Interface to output stream
  friend std::ostream& operator<< (std::ostream&, const irtkMatrix&);

  /// Interface to input stream
  friend std::istream& operator>> (std::istream&, irtkMatrix&);

  /// Print matrix
  void Print();

  /// Read matrix from file
  void Read (char *);

  /// Write matrix to file
  void Write(char *);

  /// Import matrix from text file (requires no. of expected rows and cols)
  void Import (char *, int, int);

  double Sum();
  void setRow(int);
  void setCol(int);
  void setMatrix(double *mat);
  
#ifdef USE_VXL

  /// Conversion to VNL matrix
  template <class T> void Matrix2Vnl(vnl_matrix<T> *m) const;

  /// Conversion from VNL matrix
  template <class T> void Vnl2Matrix(vnl_matrix<T> *m);

#else

  /// Conversion to GSL matrix (memory must be allocated)
  void Matrix2GSL(gsl_matrix *) const;

  /// Conversion from GSL matrix
  void GSL2Matrix(gsl_matrix *);

  //
  // Matrix operators for vectors
  //

  /// Return result of multiplication of matrix and vector
  irtkVector  operator* (const irtkVector&);

#endif
};

//
// Access operators
//

inline int irtkMatrix::Rows() const
{
  return _rows;
}

inline int irtkMatrix::Cols() const
{
  return _cols;
}

inline double* irtkMatrix::GetMatrix()
{
    return _matrix.getMat();
}

inline void irtkMatrix::Put(int rows, int cols, double matrix)
{
#ifdef NO_BOUNDS
  _matrix[cols][rows] = matrix;
#else
  if ((rows >= 0) && (rows < _rows) && (cols >= 0) && (cols < _cols)) {
    _matrix[cols][rows] = matrix;
  } else {
    cout << "irtkMatrix::Put: parameter out of range\n";
  }
#endif
}

inline double irtkMatrix::Get(int rows, int cols) const
{
#ifdef NO_BOUNDS
  return _matrix[cols][rows];
#else
  if ((rows >= 0) && (rows < _rows) && (cols >= 0) && (cols < _cols)) {
    return (double)_matrix[cols][rows];
  } else {
    cout << "irtkMatrix::Get: parameter out of range\n";
    return 0;
  }
#endif
}

inline double &irtkMatrix::operator()(int rows, int cols)
{
#ifdef NO_BOUNDS
  return _matrix[cols][rows];
#else
  if ((rows >= 0) && (rows < _rows) && (cols >= 0) && (cols < _cols)) {
    return _matrix[cols][rows];
  } else {
    cout << "irtkMatrix::operator(): parameter out of range\n";
    return _matrix[0][0];
  }
#endif
}

inline double irtkMatrix::operator()(int rows, int cols) const
{
#ifdef NO_BOUNDS
  return _matrix[cols][rows];
#else
  if ((rows >= 0) && (rows < _rows) && (cols >= 0) && (cols < _cols)) {
    return _matrix[cols][rows];
  } else {
    cout << "irtkMatrix::operator(): parameter out of range\n";
    return 0;
  }
#endif
}

//
// Matrix operators for doubles
//

inline irtkMatrix& irtkMatrix::operator-=(const double &x)
{
  int i, j;

  for (j = 0; j < _cols; j++) {
    for (i = 0; i < _rows; i++) {
      _matrix[j][i] -= x;
    }
  }
  return *this;
}

inline irtkMatrix& irtkMatrix::operator+=(const double &x)
{
  int i, j;

  for (j = 0; j < _cols; j++) {
    for (i = 0; i < _rows; i++) {
      _matrix[j][i] += x;
    }
  }
  return *this;
}

inline irtkMatrix& irtkMatrix::operator*=(const double &x)
{
  int i, j;

  for (j = 0; j < _cols; j++) {
    for (i = 0; i < _rows; i++) {
      _matrix[j][i] *= x;
    }
  }
  return *this;
}

inline irtkMatrix& irtkMatrix::operator/=(const double &x)
{
  int i, j;

  for (j = 0; j < _cols; j++) {
    for (i = 0; i < _rows; i++) {
      _matrix[j][i] /= x;
    }
  }
  return *this;
}

inline irtkMatrix irtkMatrix::operator-(const double &x)
{
  int i, j;
  irtkMatrix m;

  m = *this;
  for (j = 0; j < _cols; j++) {
    for (i = 0; i < m._rows; i++) {
      m._matrix[j][i] = _matrix[j][i] - x;
    }
  }
  return m;
}

inline irtkMatrix irtkMatrix::operator+(const double &x)
{
  int i, j;
  irtkMatrix m;

  m = *this;
  for (j = 0; j < _cols; j++) {
    for (i = 0; i < m._rows; i++) {
      m._matrix[j][i] = _matrix[j][i] + x;
    }
  }
  return m;
}

inline irtkMatrix irtkMatrix::operator*(const double &x)
{
  int i, j;
  irtkMatrix m;

  m = *this;
  for (j = 0; j < _cols; j++) {
    for (i = 0; i < m._rows; i++) {
      m._matrix[j][i] = _matrix[j][i] * x;
    }
  }
  return m;
}

inline irtkMatrix irtkMatrix::operator/(const double &x)
{
  int i, j;
  irtkMatrix m;

  m = *this;
  for (j = 0; j < _cols; j++) {
    for (i = 0; i < m._rows; i++) {
      m._matrix[j][i] = _matrix[j][i] / x;
    }
  }
  return m;
}

//
// Matrix operators for matrices
//

inline int irtkMatrix::operator==(const irtkMatrix& m)
{
  int i, j;

  if ((m._rows != _rows) || (m._cols != _cols)) return 0;
  for (j = 0; j < _cols; j++) {
    for (i = 0; i < m._rows; i++) {
      if (m._matrix[j][i] != _matrix[j][i]) return 0;
    }
  }
  return 1;
}

#ifndef USE_STL
inline int irtkMatrix::operator!=(const irtkMatrix& m)
{
  return !(*this == m);
}
#endif

inline double irtkMatrix::Trace(void) const
{
    int i, j;
    double trace = 0;

    if(_rows == _cols){

        // The trace of a matrix
        for (j = 0; j < _cols; j++) {
            for (i = 0; i < _rows; i++) {
                trace += _matrix[j][i];
            }
        }
        return trace;
    }else{
        std::cerr << "irtkMatrix::Trace() matrix number of col != row" << std::endl;
        return 0;
    }
}

inline double irtkMatrix::Norm(void) const
{
  int i, j;
  double norm = 0;

  // The norm of a matrix M is defined as trace(M * M~)
  for (j = 0; j < _cols; j++) {
    for (i = 0; i < _rows; i++) {
      norm += _matrix[j][i]*_matrix[j][i];
    }
  }
  return std::sqrt(norm);
}

inline double irtkMatrix::InfinityNorm(void) const
{
  int i, j;
  double normInf = -1.0 * DBL_MAX;
  double sum;

  for (i = 0; i < _rows; ++i) {
    sum = 0;
    for (j = 0; j < _cols; ++j) {
      sum += abs(_matrix[j][i]);
    }
    if (sum > normInf)
      normInf = sum;
  }
  return normInf;
}

inline double irtkMatrix::Sum()
{
    int i, j;
    double sum;
    
    sum = 0.0;
    for(i=0;i<_rows;i++)
    {
	for(j=0;j<_cols;j++)
	{
	    sum += _matrix[i][j];
	}
    }
    
    return sum;
}

inline void irtkMatrix::setRow(int r)
{
    _rows = r;
}

inline void irtkMatrix::setCol(int c)
{
    _cols = c;
}

inline void irtkMatrix::setMatrix(double *mat)
{
    //_matrix.setMat(mat);
}

#endif



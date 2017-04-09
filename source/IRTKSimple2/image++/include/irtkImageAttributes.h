#ifndef IRTKIMAGEATTRIBUTES_H

#define IRTKIMAGEATTRIBUTES_H

/**
 * Class which defines the attributes of the imaging geometry
 */

class irtkImageAttributes : public irtkObject
{

public:

  /// Image x-dimension (in voxels)
  int _x;
  
  /// Image y-dimension (in voxels)
  int _y;
  
  /// Image z-dimension (in voxels)
  int _z;
  
  /// Image t-dimension (in voxels)
  int _t;

  /// Voxel x-dimensions (in mm)
  double _dx;
  
  /// Voxel y-dimensions (in mm)
  double _dy;
  
  /// Voxel z-dimensions (in mm)
  double _dz;
  
  /// Voxel t-dimensions (in ms)
  double _dt;

  /// Image x-origin (in mm)
  double _xorigin;

  /// Image y-origin (in mm)
  double _yorigin;
  
  /// Image z-origin (in mm)
  double _zorigin;

  /// Image t-origin (in ms)
  double _torigin;

  /// Direction of x-axis
  double _xaxis[3];

  /// Direction of y-axis
  double _yaxis[3];

  /// Direction of z-axis
  double _zaxis[3];

  /// Serialization
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
      ar & _x & _y & _z & _t & _dx & _dy & _dz & _dt & _xorigin & _yorigin & _zorigin
	  & _torigin & _xaxis[0] & _xaxis[1] & _xaxis[2] & _yaxis[0] & _yaxis[1] & _yaxis[2]
	  & _zaxis[0] & _zaxis[1] & _zaxis[2];
  }

  /// Constructor
  irtkImageAttributes();

  /// Copy constructor
  irtkImageAttributes(const irtkImageAttributes &);

  irtkImageAttributes
      (int x, int y, int z, int t, 
// Default voxel size 
       double dx, double dy, double dz, double dt,
// Default origin
       double xorigin,
       double yorigin,
       double zorigin,
       double torigin,						
       // Default x-axis
       double  xaxis0,
       double  xaxis1,
       double  xaxis2,
       
       // Default y-axis
       double  yaxis0,
       double  yaxis1,
       double  yaxis2,
       
       // Default z-axis
       double  zaxis0,
       double  zaxis1,
       double  zaxis2);

  /// Copy operator
  irtkImageAttributes& operator= (const irtkImageAttributes &);

  /// Comparison operator
  bool operator==(const irtkImageAttributes &attr) const;

  /// Get Index from Lattice
  int LatticeToIndex(int i, int j, int k, int l = 0) const;

  /// Get Index from Lattice
  void IndexToLattice(int index, int *i, int *j, int *k, int *l = NULL);

  /// Print attributes
  void Print();

  void Print2(string s);

  double Sum();

};

inline irtkImageAttributes::irtkImageAttributes()
{
  _x  = 0;
  _y  = 0;
  _z  = 1;
  _t  = 1;

  // Default voxel size
  _dx = 1;
  _dy = 1;
  _dz = 1;
  _dt = 1;

  // Default origin
  _xorigin = 0;
  _yorigin = 0;
  _zorigin = 0;
  _torigin = 0;

  // Default x-axis
  _xaxis[0] = 1;
  _xaxis[1] = 0;
  _xaxis[2] = 0;

  // Default y-axis
  _yaxis[0] = 0;
  _yaxis[1] = 1;
  _yaxis[2] = 0;

  // Default z-axis
  _zaxis[0] = 0;
  _zaxis[1] = 0;
  _zaxis[2] = 1;
}

inline irtkImageAttributes::irtkImageAttributes
(int x, int y, int z, int t, 
// Default voxel size 
 double dx, double dy, double dz, double dt,
// Default origin
double xorigin,
double yorigin,
double zorigin,
double torigin,						
 // Default x-axis
double  xaxis0,
double  xaxis1,
double  xaxis2,
 
 // Default y-axis
double  yaxis0,
double  yaxis1,
double  yaxis2,
 
 // Default z-axis
double  zaxis0,
double  zaxis1,
double  zaxis2)
{
  _x  = x;
  _y  = y;
  _z  = z;
  _t  = t;

  // Default voxel size
  _dx = dx;
  _dy = dy;
  _dz = dz;
  _dt = dt;

  // Default origin
  _xorigin = xorigin;
  _yorigin = yorigin;
  _zorigin = zorigin;
  _torigin = torigin;

  // Default x-axis
  _xaxis[0] = xaxis0;
  _xaxis[1] = xaxis1;
  _xaxis[2] = xaxis2;

  // Default y-axis
  _yaxis[0] = yaxis0;
  _yaxis[1] = yaxis1;
  _yaxis[2] = yaxis2;

  // Default z-axis
  _zaxis[0] = zaxis0;
  _zaxis[1] = zaxis1;
  _zaxis[2] = zaxis2;
}

inline irtkImageAttributes::irtkImageAttributes(const irtkImageAttributes &attr) : irtkObject(attr)
{
  _x  = attr._x;
  _y  = attr._y;
  _z  = attr._z;
  _t  = attr._t;

  // Default voxel size
  _dx = attr._dx;
  _dy = attr._dy;
  _dz = attr._dz;
  _dt = attr._dt;

  // Default origin
  _xorigin = attr._xorigin;
  _yorigin = attr._yorigin;
  _zorigin = attr._zorigin;
  _torigin = attr._torigin;

  // Default x-axis
  _xaxis[0] = attr._xaxis[0];
  _xaxis[1] = attr._xaxis[1];
  _xaxis[2] = attr._xaxis[2];

  // Default y-axis
  _yaxis[0] = attr._yaxis[0];
  _yaxis[1] = attr._yaxis[1];
  _yaxis[2] = attr._yaxis[2];

  // Default z-axis
  _zaxis[0] = attr._zaxis[0];
  _zaxis[1] = attr._zaxis[1];
  _zaxis[2] = attr._zaxis[2];
}

inline irtkImageAttributes& irtkImageAttributes::operator=(const irtkImageAttributes &attr)
{
	
  _x  = attr._x;
  _y  = attr._y;
  _z  = attr._z;
  _t  = attr._t;

  // Default voxel size
  _dx = attr._dx;
  _dy = attr._dy;
  _dz = attr._dz;
  _dt = attr._dt;

  // Default origin
  _xorigin = attr._xorigin;
  _yorigin = attr._yorigin;
  _zorigin = attr._zorigin;
  _torigin = attr._torigin;

  // Default x-axis
  _xaxis[0] = attr._xaxis[0];
  _xaxis[1] = attr._xaxis[1];
  _xaxis[2] = attr._xaxis[2];

  // Default y-axis
  _yaxis[0] = attr._yaxis[0];
  _yaxis[1] = attr._yaxis[1];
  _yaxis[2] = attr._yaxis[2];

  // Default z-axis
  _zaxis[0] = attr._zaxis[0];
  _zaxis[1] = attr._zaxis[1];
  _zaxis[2] = attr._zaxis[2];
  
  return *this;
}

inline bool irtkImageAttributes::operator==(const irtkImageAttributes &attr) const
{
  return ((_x  == attr._x)  && (_y  == attr._y)  && (_z  == attr._z) && (_t  == attr._t) &&
          (_dx == attr._dx) && (_dy == attr._dy) && (_dz == attr._dz) && (_dt == attr._dt) &&
          (_xaxis[0] == attr._xaxis[0]) && (_xaxis[1] == attr._xaxis[1]) && (_xaxis[2] == attr._xaxis[2]) &&
          (_yaxis[0] == attr._yaxis[0]) && (_yaxis[1] == attr._yaxis[1]) && (_yaxis[2] == attr._yaxis[2]) &&
          (_zaxis[0] == attr._zaxis[0]) && (_zaxis[1] == attr._zaxis[1]) && (_zaxis[2] == attr._zaxis[2]) &&
          (_xorigin == attr._xorigin) && (_yorigin == attr._yorigin) && (_zorigin == attr._zorigin) && 
          (_torigin == attr._torigin));
}

inline void irtkImageAttributes::Print()
{
	
  cout<<_x<<" "<<_y<<" "<<_z<<" "<<_t<<endl;
  cout<<_dx<<" "<<_dy<<" "<<_dz<<" "<<_dt<<endl;
  cout<<_xorigin<<" "<<_yorigin<<" "<<_zorigin<<" "<<_torigin<<endl;
  cout<<_xaxis[0]<<" "<<_xaxis[1]<<" "<<_xaxis[2]<<endl;
  cout<<_yaxis[0]<<" "<<_yaxis[1]<<" "<<_yaxis[2]<<endl;
  cout<<_zaxis[0]<<" "<<_zaxis[1]<<" "<<_zaxis[2]<<endl;
}

inline void irtkImageAttributes::Print2(string s)
{
	
  cout<< s << _x<<" "<<_y<<" "<<_z<<" "<<_t<<endl;
  cout<< s << _dx<<" "<<_dy<<" "<<_dz<<" "<<_dt<<endl;
  cout<< s << _xorigin<<" "<<_yorigin<<" "<<_zorigin<<" "<<_torigin<<endl;
  cout<< s << _xaxis[0]<<" "<<_xaxis[1]<<" "<<_xaxis[2]<<endl;
  cout<< s << _yaxis[0]<<" "<<_yaxis[1]<<" "<<_yaxis[2]<<endl;
  cout<< s << _zaxis[0]<<" "<<_zaxis[1]<<" "<<_zaxis[2]<<endl;
}

inline int irtkImageAttributes::LatticeToIndex(int i, int j, int k, int l) const
{
  return l*_z*_y*_x + k*_y*_x + j*_x + i;
}

inline void irtkImageAttributes::IndexToLattice(int index, int *i, int *j, int *k, int *l)
{
    if(l != NULL){
        *l = index/(_x*_y*_z);
    }
	*k = index%(_x*_y*_z)/(_y*_x);
	*j = index%(_x*_y*_z)%(_y*_x)/_x;
	*i = index%(_x*_y*_z)%(_y*_x)%_x;
}

inline double irtkImageAttributes::Sum()
{
    double sum;
    sum = 0.0;

    sum += _x+_y+_z+_t + _dx+_dy+_dz+_dt+
	_xorigin+_yorigin+_zorigin+_torigin+
	_xaxis[0]+_xaxis[1]+_xaxis[2]+
	_yaxis[0]+_yaxis[1]+_yaxis[2]+
	_zaxis[0]+_zaxis[1]+_zaxis[2];
    return sum;
}
#endif

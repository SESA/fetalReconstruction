#ifndef _EBBCROSSCORRELATIONSIMILARITYMETRIC_H

#define _EBBCROSSCORRELATIONSIMILARITYMETRIC_H

/**
 * Class for voxel similarity measure based on cross correlation
 *
 */

class EbbCrossCorrelationSimilarityMetric
{

private:

  /// Number of samples
  double _n;

  /// Internal variables
  double _xy, _x, _y, _x2, _y2;

public:

  /// Constructor
  EbbCrossCorrelationSimilarityMetric();

  /// Destructor
  //~EbbCrossCorrelationSimilarityMetric();
  
  /// Add sample
  void Add(int, int);

  /// Remove sample
  void Delete(int, int);

  /// Add weighted sample
  void AddWeightedSample(int, int, double = 1);

  /// Remove weighted sample
  void DeleteWeightedSample(int, int, double = 1);

  /// Combine similarity metrics
  //virtual void Combine(irtkSimilarityMetric *);

  /// Reset similarity metric
  void Reset();

  /// Reset similarity metric
  //virtual void ResetAndCopy(irtkSimilarityMetric *);

  /// Evaluate similarity measure
  double Evaluate();

};

inline EbbCrossCorrelationSimilarityMetric::EbbCrossCorrelationSimilarityMetric()
{
  _xy = 0;
  _x  = 0;
  _y  = 0;
  _x2 = 0;
  _y2 = 0;
  _n  = 0;
}

inline void EbbCrossCorrelationSimilarityMetric::Add(int x, int y)
{
  _xy += x*y;
  _x  += x;
  _x2 += x*x;
  _y  += y;
  _y2 += y*y;
  _n++;
}

inline void EbbCrossCorrelationSimilarityMetric::Delete(int x, int y)
{
  _xy -= x*y;
  _x  -= x;
  _x2 -= x*x;
  _y  -= y;
  _y2 -= y*y;
  _n--;
}

inline void EbbCrossCorrelationSimilarityMetric::AddWeightedSample(int x, int y, double weight)
{
  _xy += weight*x*y;
  _x  += weight*x;
  _x2 += weight*x*x;
  _y  += weight*y;
  _y2 += weight*y*y;
  _n  += weight;
}

inline void EbbCrossCorrelationSimilarityMetric::DeleteWeightedSample(int x, int y, double weight)
{
  _xy -= weight*x*y;
  _x  -= weight*x;
  _x2 -= weight*x*x;
  _y  -= weight*y;
  _y2 -= weight*y*y;
  _n  -= weight;
}

/*inline void EbbCrossCorrelationSimilarityMetric::Combine(EbbSimilarityMetric *metric)
{
  EbbCrossCorrelationSimilarityMetric *m = dynamic_cast<EbbCrossCorrelationSimilarityMetric *>(metric);

  if (m == NULL) {
    cerr << "EbbCrossCorrelationSimilarityMetric::Combine: Dynamic cast failed" << endl;
    exit(1);
  }

  _xy += m->_xy;
  _x2 += m->_x2;
  _y2 += m->_y2;
  _x  += m->_x;
  _y  += m->_y;
  _n  += m->_n;

  }*/


inline void EbbCrossCorrelationSimilarityMetric::Reset()
{
  _xy = 0;
  _x2 = 0;
  _y2 = 0;
  _x  = 0;
  _y  = 0;
  _n  = 0;
}

/*inline void EbbCrossCorrelationSimilarityMetric::ResetAndCopy(EbbSimilarityMetric *metric)
{
  EbbCrossCorrelationSimilarityMetric *m = dynamic_cast<EbbCrossCorrelationSimilarityMetric *>(metric);

  if (m == NULL) {
    cerr << "EbbCrossCorrelationSimilarityMetric::ResetAndCopy: Dynamic cast failed" << endl;
    exit(1);
  }

  _xy = m->_xy;
  _x  = m->_x;
  _x2 = m->_x2;
  _y  = m->_y;
  _y2 = m->_y2;
  _n  = m->_n;
  }*/

inline double EbbCrossCorrelationSimilarityMetric::Evaluate()
{
  if (_n > 0) {
    return (_xy - (_x * _y) / _n) / (sqrt(_x2 - _x * _x / _n) * sqrt(_y2 - _y *_y / _n));
  } else {
      //cerr << "EbbCrossCorrelationSimilarityMetric::Evaluate: No samples";
    return 0;
  }
}

#endif

/* 
 * File:   routeSet.h
 * Author: MAN
 *
 * Created on September 6, 2013, 7:03 PM
 */

#ifndef _ROUTESET_H
#define	_ROUTESET_H

#include<EO.h>
#include <vector>
#include "route.h"

using namespace std;

template< class FitT>
class RouteSet: public EO<FitT> {
public:
  /** Ctor: you MUST provide a default ctor.
   * though such individuals will generally be processed
   * by some eoInit object
   */
  RouteSet()
  {
    // START Code of default Ctor of an RouteSet object
    // END   Code of default Ctor of an RouteSet object
  }

  virtual ~RouteSet()
  {
    // START Code of Destructor of an eoEASEAGenome object
    // END   Code of Destructor of an eoEASEAGenome object
  }

  virtual string className() const { return "RouteSet"; }

  /** printing... */
    void printOn(ostream& _os) const
    {
      // First write the fitness
      EO<FitT>::printOn(_os);
      _os << ' ';
    // START Code of default output

	/** HINTS
	 * in EO we systematically write the sizes of things before the things
	 * so readFrom is easier to code (see below)
	 */
     // _os << rS.size() << ' ' ;
      for (unsigned i=0; i<rS.size(); i++)
	_os << rS[i] << " , " ;

      _os << endl;
    // END   Code of default output
    }

  /** reading...
   * of course, your readFrom must be able to read what printOn writes!!!
   */
  void readFrom(istream& _is)
  {
    // of course you should read the fitness first!
  //  EO<FitT>::readFrom(_is);
    // START Code of input

    /** HINTS
     * remember the RouteSet object will come from the default ctor
     * this is why having the sizes written out is useful
     */
    unsigned s;
    _is >> s;
    rS.resize(s);
    for (unsigned i=0; i<s; i++)
      {
	 Route<double>  bTmp;
	_is >> bTmp;
	rS[i] = bTmp;
      }
    // END   Code of input
  }

  // accessing and setting values
  void setRs(vector< Route<double> > & _rS)
  {
    rS=_rS;
  }
  const vector< Route<double> > & Rs()
  {
    return rS;
  }
  vector< Route<double> > & mutableRs()
  {
    return rS;
  }
 Route<double>& operator[](int i) {
      return rS[i];
   }
 int size()
 {
     rS.size();
 }
 bool isTrouble()
 {
      for (unsigned i=0; i<rS.size(); i++)
      {
          if(rS[i].size() == 0)
          {
              return true;
          }
      }
      return false;
 }


private:			   // put all data here
    // START Private data of an RouteSet object
  vector< Route<double> > rS;
    // END   Private data of an RouteSet object
public:
    mutable double D[3],Dun,ATT;
};

#endif	/* _ROUTESET_H */


/* 
 * File:   route.h
 * Author: MAN
 *
 * Created on September 5, 2013, 8:21 PM
 */

#ifndef _ROUTE_H
#define	_ROUTE_H

#include<EO.h>
#include<list>


using namespace std;

template< class FitT>
class Route: public EO<FitT> {
public:
  /** Ctor: you MUST provide a default ctor.
   * though such individuals will generally be processed
   * by some eoInit object
   */
  Route()
  {
    // START Code of default Ctor of an Route object
    // END   Code of default Ctor of an Route object
  }

  virtual ~Route()
  {
    // START Code of Destructor of an eoEASEAGenome object
    // END   Code of Destructor of an eoEASEAGenome object
  }

  virtual string className() const { return "Route"; }

  /** printing... */
    void printOn(ostream& _os) const
    {
      // First write the fitness
     // EO<FitT>::printOn(_os);
      _os << ' ';
    // START Code of default output

	/** HINTS
	 * in EO we systematically write the sizes of things before the things
	 * so readFrom is easier to code (see below)
	 */
      _os << r.size() << ':' ;
      for (list<int>::const_iterator it=r.begin(); it!=r.end(); it++)
	_os << *it << '-' ;
    // END   Code of default output
    }

  /** reading...
   * of course, your readFrom must be able to read what printOn writes!!!
   */
  void readFrom(istream& _is)
  {
    // of course you should read the fitness first!
   // EO<FitT>::readFrom(_is);
    // START Code of input

    /** HINTS
     * remember the Route object will come from the default ctor
     * this is why having the sizes written out is useful
     */
    unsigned s;
    _is >> s;
    r.resize(s);
    for (unsigned i=0; i<s; i++)
      {
	int bTmp;
	_is >> bTmp;
	r.push_back(bTmp);
      }
    // END   Code of input
  }

  // accessing and setting values
  void setR(list<int> & _r)
  {
    r=_r;
  }
  const list<int> & R()
  {
    return r;
  }
  list<int> & mutableR()
  {
      return r;
  }
  void insert(list<int>::iterator it, int elem)
  {
      r.insert(it,elem);
  }
  void erase(list<int>::iterator it)
  {
      r.erase(it);
  }
  void setNodeList(vector<int>& _nodeList)
  {
      nodeList=_nodeList;
  }
  int size()
  {
      return r.size();
  }

private:			   // put all data here
    // START Private data of an Route object
  list<int> r;
    // END   Private data of an Route object
public:
    vector<int> nodeList;
};

#endif	/* _ROUTE_H */


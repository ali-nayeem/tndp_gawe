// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// eoGAWE.h
// (c) GeNeura Team, 2000 - EEAAX 1999 - Maarten Keijzer 2000
/*
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact: todos@geneura.ugr.es, http://geneura.ugr.es
             Marc.Schoenauer@polytechnique.fr
             mak@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef _eoGAWE_h
#define _eoGAWE_h

#include <eoInvalidateOps.h>
#include <eoContinue.h>
#include <eoPop.h>
#include <eoSelectOne.h>
#include <eoSelectPerc.h>
#include <eoEvalFunc.h>
#include <eoAlgo.h>
#include <apply.h>
#include <iostream>

/** The Simple Genetic Algorithm,
 *
 * Needs a selector (class eoSelectOne) a crossover (eoQuad, i.e. a
 * 2->2 operator) and a mutation with their respective rates, of
 * course an evaluation function (eoEvalFunc) and a continuator
 * (eoContinue) which gives the stopping criterion. Performs full
 * generational replacement.
 *
 * @ingroup Algorithms
 */
template <class EOT>
class eoGAWE : public eoAlgo<EOT>
{
public:

    // added this second ctor as I didn't like the ordering of the parameters
    // in the one above. Any objection :-) MS

    eoGAWE(
           eoSelectOne<EOT>& _select,
           eoQuadOp<EOT>& _cross, float _crate,
           eoMonOp<EOT>& _mutate, float _mrate,
           eoEvalFunc<EOT>& _eval,
           eoContinue<EOT>& _cont)
    : cont(_cont),
    mutate(_mutate),
    mutationRate(_mrate),
    cross(_cross),
    crossoverRate(_crate),
    select(_select),
    eval(_eval)
    {
    }

    eoGAWE(
           eoSelectPerc<EOT>& _select,
           eoQuadOp<EOT>& _cross,
           eoMonOp<EOT>& _mutate,
           eoEvalFunc<EOT>& _eval,
           eoContinue<EOT>& _cont,
           unsigned _elite)
    : cont(_cont),
    mutate(_mutate),
    cross(_cross),
    select(_select),
    elite(_elite),
    eval(_eval)
    {
        //best = NULL;
    }

    void operator()(eoPop<EOT>& _pop)
    {
        eoPop<EOT> Q;
        int gen = 0;
        do
        {
            gen++;
            select(_pop, Q); //select parents from _pop and put in Q
            //std::cout<<"Print Q" <<Q;

            for (unsigned i = 0; i < (_pop.size() - elite) / 2; i++)
            {
                // this crossover generates 2 Q from two parents
                if (cross(Q[2 * i], Q[2 * i + 1]))
                {
                    Q[2 * i].invalidate();
                    Q[2 * i + 1].invalidate();
                }

                if (mutate(Q[2 * i]))
                {
                    Q[2 * i].invalidate();
                }
                if (mutate(Q[2 * i + 1]))
                {
                    Q[2 * i + 1].invalidate();
                }

            }
            // std::cout <<"Q after breed"<<Q;
            //choose best individuals from _pop
            typename eoPop<EOT>::iterator it;
            for (unsigned i = 0; i < elite; i++)
            {
                it = _pop.it_best_element();
                Q.push_back(*it);
                _pop.erase(it);
            }
            //std::cout <<"Q after add best"<<Q;
            // std::cout <<"Best indi:"<<Q[Q.size() - elite]<<std::endl;
            //std::cout<<"Gen:"<<gen<<std::endl;
            /* if(gen == 100)
             break;*/
            EOT competitor = Q[Q.size() - elite]; //the best in _pop

            if (best < competitor)
            {
                best = competitor;
            }
            std::cout << best.D[0] << ',' << best.D[1] << ',' << best.D[2] << ',' << best.Dun << ',' << best.ATT << std::endl;
            _pop.swap(Q);
            apply<EOT > (eval, _pop); //Assess Fitness
        }
        while (cont(_pop));
    }

private:

    eoContinue<EOT>& cont;
    /// eoInvalidateMonOp invalidates the embedded operator
    eoInvalidateMonOp<EOT> mutate;
    float mutationRate;
    // eoInvalidateQuadOp invalidates the embedded operator
    eoInvalidateQuadOp<EOT> cross;
    float crossoverRate;
    eoSelectPerc<EOT> select;
    unsigned elite;
    eoEvalFunc<EOT>& eval;

public:
    EOT best;

};

#endif

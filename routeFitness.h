/* 
 * File:   routeFitness.h
 * Author: MAN
 *
 * Created on September 7, 2013, 7:41 AM
 */

#ifndef _ROUTEFITNESS_H
#define	_ROUTEFITNESS_H

#include "route.h"


double routeFitness(const Route<double>& EO) 
{
    int sum = 0;
    Route<double> dummy = EO;
    const std::list<int>& _chrom = dummy.R();
    std::list<int>::const_iterator outer = _chrom.end();
    for (outer--; outer != _chrom.begin(); outer--)
    {
        std::list<int>::const_iterator inner;
        for (inner = _chrom.begin(); inner != outer; inner++)
        {
            sum += d[*inner][*outer];
        }
    }
    return 1.0/sum;
}

#endif	/* _ROUTEFITNESS_H */


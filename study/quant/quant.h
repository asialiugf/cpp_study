#ifndef _SEE_QUANT
#define _SEE_QUANT 1

#pragma GCC system_header

#include <bits/c++config.h>
#include <ostream>
#include <istream>

namespace SEE(default)
{
class BAR
{
public:
    double h;
    double o;
    double c;
    double l;
    double v;
    char date[8];
    char time[8];

}
} /* end of namespace SEE */


#endif /* _SEE_QUANT */

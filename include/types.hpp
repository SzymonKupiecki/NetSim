#ifndef TYPES_H
#define TYPES_H

#include <functional>

using ElementID = unsigned int;
using ProbabilityGenerator = std::function<double()>; // nie weim czy powinno to sie tu znajdowac (uzywane w helpers)
#endif
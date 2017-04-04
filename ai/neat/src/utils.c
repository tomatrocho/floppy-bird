#include "utils.h"

/*!
* \brief Return a random number between 0 and the specified parameter
* \param[in] limit the upper limit
* \return Return a random number in [0; limit]
*/
int randomAtMost(int limit) {
  int r;
  int d;

  if (limit == 0) return 0;

  d = RAND_MAX / limit;
  limit *= d;

  do {
    r = rand();
  } while (r >= limit);

  return r / d;
}
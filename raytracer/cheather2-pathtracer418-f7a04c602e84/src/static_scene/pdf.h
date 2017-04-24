#ifndef CMU462_STATICSCENE_ENVIRONMENTLIGHT_H
#define CMU462_STATICSCENE_ENVIRONMENTLIGHT_H

#include "../sampler.h"
#include "../image.h"
#include "scene.h"

namespace CMU462 { namespace StaticScene {
class PDF {
 public:
   PDF();
   double prob;
   int index;
   bool pdf_compare(pdf * p1, pdf * p2)
   {
       return p1->prob < p2->prob;
   }

}; // class PDF

} // namespace StaticScene
} // namespace CMU462

#endif //CMU462_STATICSCENE_ENVIRONMENTLIGHT_H

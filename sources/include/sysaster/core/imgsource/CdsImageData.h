#ifndef _CDS_IMG_DET_
#define _CDS_IMG_DET_

#include "opencv2/core/mat.hpp"
#include "cds/intrusive/basket_queue.h"
#include "cds/gc/hp.h"

namespace ci = cds::intrusive;
typedef cds::gc::HP hp_gc;

// Basket queue with Hazard Pointer garbage collector, base hook + item disposer:
struct ImageNode: public ci::basket_queue::node< hp_gc >
{
    cv::Mat data;

    ImageNode(cv::Mat data) : data {data} {}

    inline decltype(data) get_data() { return data; }
};

// Disposer for Foo struct just deletes the object passed in
struct imageDisposer {
    void operator()( ImageNode * p )
    {
        delete p;
    }
};

struct imageTraits: public ci::basket_queue::traits {
    typedef ci::basket_queue::base_hook< ci::opt::gc<hp_gc> > hook;
    typedef imageDisposer disposer;
};

typedef ci::BasketQueue< hp_gc, ImageNode, imageTraits > ImageQueueT;

#endif

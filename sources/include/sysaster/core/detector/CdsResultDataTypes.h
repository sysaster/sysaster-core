#ifndef _CDS_RES_DET_
#define _CDS_RES_DET_

#include "sysaster/core/detector/DetectionResultData.h"
#include "cds/intrusive/basket_queue.h"
#include "cds/gc/hp.h"

namespace ci = cds::intrusive;
typedef cds::gc::HP hp_gc;

// Basket queue with Hazard Pointer garbage collector, base hook + item disposer:
struct DetectionResultDataNode: public ci::basket_queue::node< hp_gc >
{
    DetectionResultData data;

    DetectionResultDataNode() {}
    DetectionResultDataNode(const DetectionResultData& data) : data {data} {}

    inline decltype(data) get_data() { return data; }
};
// Disposer for Foo struct just deletes the object passed in
struct detectionDataDisposer {
    void operator()( DetectionResultDataNode * p )
    {
        delete p;
    }
};
struct detectionDataTraits: public ci::basket_queue::traits {
    typedef ci::basket_queue::base_hook< ci::opt::gc<hp_gc> > hook;
    typedef detectionDataDisposer disposer;
};

typedef ci::BasketQueue< hp_gc, DetectionResultDataNode, detectionDataTraits > ResultDataQueueT;

#endif

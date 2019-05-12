#ifndef _CDS_CLIENT_INFO_
#define _CDS_CLIENT_INFO_

#include "ClientInfo.h"
#include "cds/intrusive/basket_queue.h"
#include "cds/gc/hp.h"

namespace ci = cds::intrusive;
typedef cds::gc::HP hp_gc;

// Basket queue with Hazard Pointer garbage collector, base hook + item disposer:
struct ClientInfoNode: public ci::basket_queue::node< hp_gc >
{
    ClientInfo data;

    ClientInfoNode() {}
    ClientInfoNode(const ClientInfo& data) : data {data} {}

    inline decltype(data) get_data() { return data; }
};
// Disposer for Foo struct just deletes the object passed in
struct clientInfoDisposer {
    void operator()( ClientInfoNode * p )
    {
        delete p;
    }
};
struct clientInfoTraits: public ci::basket_queue::traits {
    typedef ci::basket_queue::base_hook< ci::opt::gc<hp_gc> > hook;
    typedef clientInfoDisposer disposer;
};

typedef ci::BasketQueue< hp_gc, ClientInfoNode, clientInfoTraits > ClientInfoQueueT;

#endif

#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_message.h"
#include "ceammc_object.h"

#include <list>

#define OBJ_NAME "data.fifo"
#define MSG_PREFIX "[" OBJ_NAME "] "

using namespace ceammc;

typedef std::list<Message> MessageFifo;

static size_t MAX_SIZE = 1024;

class DataFifo : public BaseObject {
    MessageFifo fifo_;
    size_t size_;

public:
    DataFifo(const PdArgs& args)
        : BaseObject(args)
        , size_(MAX_SIZE)
    {
        if (args.args.size() > 0)
            size_ = args.args.first()->asSizeT(MAX_SIZE);

        createOutlet();
        createCbProperty("@size", &DataFifo::p_size);
        createCbProperty("@maxsize", &DataFifo::p_max_size);
        createCbProperty("@free", &DataFifo::p_free);
    }

    void onBang() { flush(); }
    void onFloat(float v) { add(Atom(v)); }
    void onSymbol(t_symbol* s) { add(Atom(s)); }
    void onList(const AtomList& lst) { add(lst); }
    void onAny(t_symbol* s, const AtomList& lst) { add(Message(s, lst)); }

    void m_flush(t_symbol*, const AtomList&) { flush(); }
    void m_clear(t_symbol*, const AtomList&) { clear(); }
    void m_pop(t_symbol*, const AtomList&) { pop(); }
    void m_resize(t_symbol*, const AtomList& l)
    {
        size_t sz = l.asSizeT(0);
        if (sz < 1) {
            pd_error(owner(), MSG_PREFIX "invalid size value %lu. Using default size: %lu", sz, MAX_SIZE);
            return;
        }

        resize(static_cast<size_t>(sz));
    }

    AtomList p_size() { return listFrom(fifo_.size()); }
    AtomList p_max_size() { return listFrom(size_); }
    AtomList p_free() { return listFrom(size_ - fifo_.size()); }

    void add(const Message& msg)
    {
        if (fifo_.size() >= size_)
            pop();

        fifo_.push_back(msg);
    }

    // remove all, no output
    void clear()
    {
        fifo_.clear();
    }

    // output all, remove all
    void flush()
    {
        for (MessageFifo::iterator it = fifo_.begin(); it != fifo_.end(); it++)
            messageTo(0, *it);

        fifo_.clear();
    }

    // output and remove first in queue
    void pop()
    {
        if (fifo_.empty())
            return;

        messageTo(0, fifo_.front());
        fifo_.pop_front();
    }

    void resize(size_t sz)
    {
        clear();
        size_ = std::min(sz, MAX_SIZE);
    }

    void dump()
    {
        BaseObject::dump();

        post("values:");
        for (MessageFifo::reverse_iterator it = fifo_.rbegin(); it != fifo_.rend(); it++) {
            post("    %s", to_string(*it).c_str());
        }
    }
};

extern "C" void setup_data0x2efifo()
{
    ObjectFactory<DataFifo> obj("data.fifo");
    obj.addMethod("flush", &DataFifo::m_flush);
    obj.addMethod("clear", &DataFifo::m_clear);
    obj.addMethod("pop", &DataFifo::m_pop);
    obj.addMethod("resize", &DataFifo::m_resize);
}

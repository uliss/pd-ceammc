#include "ceammc_factory.h"
#include "ceammc_message.h"
#include "ceammc_object.h"

#include <queue>

#define OBJ_NAME "data.fifo"
#define MSG_PREFIX "[" OBJ_NAME "] "

using namespace ceammc;

typedef std::queue<Message> MessageFifo;

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

    AtomList
    p_size()
    {
        return listFrom(size_);
    }

    void add(const Message& msg)
    {
        if (fifo_.size() >= size_)
            pop();

        fifo_.push(msg);
    }

    // remove all, no output
    void clear()
    {
        for (size_t i = 0; i < fifo_.size(); i++)
            fifo_.pop();
    }

    // output all, remove all
    void flush()
    {
        for (size_t i = 0; i < fifo_.size(); i++)
            pop();
    }

    // output and remove first in queue
    void pop()
    {
        if (fifo_.empty())
            return;

        messageTo(0, fifo_.front());
        fifo_.pop();
    }

    void resize(size_t sz)
    {
        clear();
        size_ = std::min(sz, MAX_SIZE);
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

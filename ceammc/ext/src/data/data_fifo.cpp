#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "ceammc_message.h"
#include "ceammc_object.h"

#include <list>

using namespace ceammc;

using MessageFifo = std::list<Message>;

constexpr size_t DEFAULT_SIZE = 32;
constexpr size_t MIN_SIZE = 1;
constexpr size_t MAX_SIZE = 1024;

class DataFifo : public BaseObject {
    MessageFifo fifo_;
    size_t size_;

public:
    DataFifo(const PdArgs& args)
        : BaseObject(args)
        , size_(positionalConstant<DEFAULT_SIZE, MIN_SIZE, MAX_SIZE>(0))
    {
        createCbBoolProperty("@empty", [this]() -> bool { return fifo_.empty(); });
        createCbIntProperty("@filled", [this]() -> int { return fifo_.size(); })
            ->checkNonNegative();
        createCbIntProperty("@size", [this]() -> int { return fifo_.max_size(); })
            ->checkNonNegative();
        createCbIntProperty("@free", [this]() -> int { return size_ - fifo_.size(); })
            ->checkNonNegative();

        createOutlet();
    }

    void onBang() override { flush(); }
    void onFloat(float v) override { add(Atom(v)); }
    void onSymbol(t_symbol* s) override { add(Atom(s)); }
    void onList(const AtomList& lst) override { add(lst); }
    void onAny(t_symbol* s, const AtomList& lst) override { add(Message(s, lst)); }

    void m_flush(t_symbol*, const AtomList&) { flush(); }
    void m_clear(t_symbol*, const AtomList&) { clear(); }
    void m_pop(t_symbol*, const AtomList&) { pop(); }
    void m_resize(t_symbol*, const AtomList& l)
    {
        size_t sz = l.asSizeT(0);
        if (sz < 1) {
            OBJ_ERR << "invalid size value: " << sz << ". Using default size: " << DEFAULT_SIZE;
            return;
        }

        resize(static_cast<size_t>(sz));
    }

    AtomList p_free() const { return listFrom(size_ - fifo_.size()); }

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
        for (auto& m : fifo_)
            messageTo(0, m);

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
        size_ = std::min(sz, DEFAULT_SIZE);
    }

    void dump() const override
    {
        BaseObject::dump();

        post("values:");
        for (auto it = fifo_.rbegin(); it != fifo_.rend(); ++it) {
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

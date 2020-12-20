#include "data_fifo.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_log.h"

constexpr size_t DEFAULT_SIZE = 32;
constexpr size_t MIN_SIZE = 1;
constexpr size_t MAX_SIZE = 1024;

DataFifo::DataFifo(const PdArgs& args)
    : BaseObject(args)
    , size_(positionalConstant<DEFAULT_SIZE, MIN_SIZE, MAX_SIZE>(0))
{
    createCbBoolProperty("@empty", [this]() -> bool { return fifo_.empty(); });
    createCbIntProperty("@filled", [this]() -> int { return fifo_.size(); })
        ->checkNonNegative();
    createCbIntProperty(
        "@size", [this]() -> int { return size_; },
        [this](int v) -> bool { clear(); size_ = v; return true; })
        ->setIntCheck(PropValueConstraints::CLOSED_RANGE, MIN_SIZE, MAX_SIZE);
    createCbIntProperty("@free", [this]() -> int { return size_ - fifo_.size(); })
        ->checkNonNegative();

    createOutlet();
}

void DataFifo::onBang() { flush(); }

void DataFifo::onFloat(t_float v) { add(Atom(v)); }

void DataFifo::onSymbol(t_symbol* s) { add(Atom(s)); }

void DataFifo::onList(const AtomList& lst) { add(lst); }

void DataFifo::onAny(t_symbol* s, const AtomListView& lst) { add(Message(s, lst)); }

void DataFifo::m_flush(t_symbol*, const AtomListView&) { flush(); }

void DataFifo::m_clear(t_symbol*, const AtomListView&) { clear(); }

void DataFifo::m_pop(t_symbol*, const AtomListView&) { pop(); }

void DataFifo::m_resize(t_symbol*, const AtomListView& l)
{
    size_t sz = l.toT<size_t>(0);
    if (sz < 1) {
        OBJ_ERR << "invalid size value: " << sz << ". Using default size: " << DEFAULT_SIZE;
        return;
    }

    resize(static_cast<size_t>(sz));
}

void DataFifo::add(const Message& msg)
{
    if (fifo_.size() >= size_)
        pop();

    fifo_.push_back(msg);
}

void DataFifo::clear()
{
    fifo_.clear();
}

void DataFifo::flush()
{
    for (auto& m : fifo_)
        messageTo(0, m);

    fifo_.clear();
}

void DataFifo::pop()
{
    if (fifo_.empty())
        return;

    messageTo(0, fifo_.front());
    fifo_.pop_front();
}

void DataFifo::resize(size_t sz)
{
    clear();
    size_ = std::min<size_t>(sz, MAX_SIZE);
}

void DataFifo::dump() const
{
    BaseObject::dump();

    post("values:");
    for (auto it = fifo_.rbegin(); it != fifo_.rend(); ++it) {
        post("    %s", to_string(*it).c_str());
    }
}

void setup_data_fifo()
{
    ObjectFactory<DataFifo> obj("data.fifo");
    obj.addMethod("flush", &DataFifo::m_flush);
    obj.addMethod("clear", &DataFifo::m_clear);
    obj.addMethod("pop", &DataFifo::m_pop);
    obj.addMethod("resize", &DataFifo::m_resize);
}

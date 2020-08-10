#include <algorithm>
#include <cmath>

#include "ceammc_factory.h"
#include "list_seq.h"

constexpr t_float DEFAULT_FROM = 0;
constexpr t_float DEFAULT_TO = 1;
constexpr t_float DEFAULT_STEP = 1;

ListSeq::ListSeq(const PdArgs& a)
    : BaseObject(a)
    , from_(0)
    , to_(0)
    , step_(0)
    , closed_range_(0)
{
    createOutlet();

    from_ = new FloatProperty("@from", DEFAULT_FROM);
    addProperty(from_);

    to_ = new FloatProperty("@to", DEFAULT_TO);
    addProperty(to_);

    step_ = new FloatProperty("@step", DEFAULT_STEP);
    step_->checkNonZero();
    addProperty(step_);

    closed_range_ = new FlagProperty("@closed");
    addProperty(closed_range_);

    switch (parsedPosArgs().size()) {
    case 0:
        break;
    case 1:
        to_->setArgIndex(0);
        break;
    case 2:
        from_->setArgIndex(0);
        to_->setArgIndex(1);
        break;
    case 3:
        from_->setArgIndex(0);
        to_->setArgIndex(1);
        step_->setArgIndex(2);
        break;
    default:
        OBJ_ERR << "too many positional args: " << parsedPosArgs();
        break;
    }
}

void ListSeq::onBang()
{
    AtomList res;
    const t_float from = from_->value();
    const t_float to = to_->value();
    const t_float step = std::fabs(step_->value());

    assert(step != 0);

    if (from < to) {
        if (closed_range_->value()) {
            for (t_float i = from; i <= to; i += step)
                res.append(i);
        } else {
            for (t_float i = from; i < to; i += step)
                res.append(i);
        }
    } else if (from > to) {
        if (closed_range_->value()) {
            for (t_float i = from; i >= to; i -= step)
                res.append(i);
        } else {
            for (t_float i = from; i > to; i -= step)
                res.append(i);
        }
    } else {
        OBJ_ERR << "invalid sequence args: @from " << from << ", @to " << to << ", @step " << step;
    }

    listTo(0, res);
}

void ListSeq::onFloat(t_float f)
{
    from_->setValue(0);
    to_->setValue(f);
    step_->setValue(1);
    onBang();
}

void ListSeq::onList(const AtomList& lst)
{
    switch (lst.size()) {
    case 1: {
        from_->setValue(0);
        to_->setValue(lst[0].asFloat(0));
        step_->setValue(1);
        onBang();
    } break;
    case 2: {
        from_->setValue(lst[0].asFloat(0));
        to_->setValue(lst[1].asFloat(0));
        step_->setValue(1);
        onBang();
    } break;
    case 3: {
        from_->setValue(lst[0].asFloat(0));
        to_->setValue(lst[1].asFloat(0));
        step_->setValue(lst[2].asFloat(0));
        onBang();
    } break;
    default:
        OBJ_ERR << "wrong arguments. FROM, TO, STEP expected";
        break;
    }
}

void setup_list_seq()
{
    ObjectFactory<ListSeq> obj("list.seq");

    obj.setDescription("numeric sequence list generator");
    obj.addAuthor("Alex Nadzharov");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "sequence", "generate" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 1);

    ListSeq::setInletsInfo(obj.classPointer(), { "bang:         generate sequence with specified parameters\n"
                                                 "float:        sequence FROM=0 TO=float STEP=1\n"
                                                 "list (len=1): same as float\n"
                                                 "list (len=2): sequence FROM=l0 TO=l1 STEP=1\n"
                                                 "list (len=3): sequence FROM=l0 TO=l1 STEP=l2" });
    ListSeq::setOutletsInfo(obj.classPointer(), { "list" });
}

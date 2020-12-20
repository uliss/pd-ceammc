#include "env_to_array.h"
#include "ceammc_factory.h"

static t_symbol* SYM_FIT;
static t_symbol* SYM_RAW;
static t_symbol* SYM_RESIZE;

Env2Array::Env2Array(const PdArgs& args)
    : ArrayMod(args)
    , mode_(nullptr)
{
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", { SYM_FIT, SYM_RAW, SYM_RESIZE });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@fit", mode_, SYM_FIT));
    addProperty(new SymbolEnumAlias("@raw", mode_, SYM_RAW));
    addProperty(new SymbolEnumAlias("@resize", mode_, SYM_RESIZE));
}

void Env2Array::onDataT(const DataAtom<DataTypeEnv>& env)
{
    if (!checkArray())
        return;

    if (env->empty()) {
        OBJ_ERR << "empty envelope";
        return;
    }

    // resize if needed
    if (mode_->value() == SYM_RESIZE) {
        size_t n = (sys_getsr() / 1000) * (env->back().timeMs()) + 1;
        if (!array_.resize(n)) {
            OBJ_ERR << "can't resize array to " << n;
            return;
        }
    }

    // if shifted envelope
    if (env->numPoints() > 0 && env->pointAt(0).utime > 0) {
        DataTypeEnv env_fixed(*env);
        // insert start point with STEP segment
        env_fixed.insertPoint(EnvelopePoint(0, 0, false, CURVE_STEP));
        render(env_fixed);
    } else {
        render(*env);
    }

    if (shouldRedraw())
        array_.redraw();

    floatTo(0, array_.size());
}

void Env2Array::render(const DataTypeEnv& env)
{
    if (mode_->value() == SYM_FIT || mode_->value() == SYM_RESIZE) {
        env.render(array_.begin(), array_.end());
    } else if (mode_->value() == SYM_RAW) {
        size_t n = array_.size();
        double slen = 1000000.0 / double(sys_getsr());

        for (size_t i = 0; i < n; i++)
            array_[i] = env.valueAtTime(i * slen);
    }
}

void setup_env_to_array()
{
    SYM_FIT = gensym("fit");
    SYM_RAW = gensym("raw");
    SYM_RESIZE = gensym("resize");

    ObjectFactory<Env2Array> obj("env2array");
    obj.addAlias("env->array");
    obj.processData<DataTypeEnv>();
}

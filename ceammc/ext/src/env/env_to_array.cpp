#include "env_to_array.h"
#include "ceammc_factory.h"

t_symbol* SYM_FIT = gensym("fit");
t_symbol* SYM_RAW = gensym("raw");
t_symbol* SYM_RESIZE = gensym("resize");

Env2Array::Env2Array(const PdArgs& args)
    : ArrayMod(args)
    , mode_(0)
    , mode_alias_(0)
{
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", SYM_FIT);
    mode_->appendEnum(SYM_RAW);
    mode_->appendEnum(SYM_RESIZE);
    createProperty(mode_);

    createProperty(new SymbolEnumAlias("@fit", mode_, SYM_FIT));
    createProperty(new SymbolEnumAlias("@raw", mode_, SYM_RAW));
    createProperty(new SymbolEnumAlias("@resize", mode_, SYM_RESIZE));
}

void Env2Array::onDataT(const DataTypeEnv& env)
{
    if (!checkArray())
        return;

    if (env.empty()) {
        OBJ_ERR << "empty envelope";
        return;
    }

    // resize if needed
    if (mode_->value() == SYM_RESIZE) {
        size_t n = (sys_getsr() / 1000) * (env.back().timeMs()) + 1;
        if (!array_.resize(n)) {
            OBJ_ERR << "can't resize array to " << n;
            return;
        }
    }

    // if shifted envelope
    if (env.numPoints() > 0 && env.pointAt(0).utime > 0) {
        DataTypeEnv env_fixed(env);
        // insert start point with STEP segment
        env_fixed.insertPoint(EnvelopePoint(0, 0, false, CURVE_STEP));
        render(env_fixed);
    } else {
        render(env);
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
    ObjectFactory<Env2Array> obj("env2array");
    obj.addAlias("env->array");
    obj.processData<DataTypeEnv>();
}

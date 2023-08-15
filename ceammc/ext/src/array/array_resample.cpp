#include "array_resample.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

#include "soxr.h"

#include <cmath>

constexpr size_t MAX_ARRAY_SIZE = 44100 * 600;

static int symbol2quality(t_symbol* s)
{
    static t_symbol* names[] = { gensym("fast"), gensym("low"), gensym("medium"), gensym("high"), gensym("best") };
    static int q[] = { SOXR_QQ, SOXR_LQ, SOXR_MQ, SOXR_HQ, SOXR_VHQ };

    static_assert(sizeof(names) / sizeof(names[0]) == sizeof(q) / sizeof(q[0]), "");

    for (size_t i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
        if (names[i] == s)
            return q[i];
    }

    return SOXR_LQ;
}

ArrayResample::ArrayResample(const PdArgs& a)
    : BaseObject(a)
    , src_name_(nullptr)
    , dest_name_(nullptr)
    , ratio_(nullptr)
    , quality_(nullptr)
{
    src_name_ = new SymbolProperty("@src", &s_);
    src_name_->setArgIndex(0);
    src_name_->setSuccessFn(
        [this](Property*) {
            if (!src_array_.open(src_name_->value()))
                OBJ_DBG << fmt::format("array not found: '{}'", src_name_->value()->s_name);
        });
    addProperty(src_name_);

    dest_name_ = new SymbolProperty("@dest", &s_);
    dest_name_->setArgIndex(1);
    addProperty(dest_name_);

    ratio_ = new FloatProperty("@ratio", 0);
    ratio_->checkMinEq(0);
    addProperty(ratio_);

    quality_ = new SymbolEnumProperty("@quality", { "high", "fast", "low", "medium", "best" });
    addProperty(quality_);

    addProperty(new SymbolEnumAlias("@fast", quality_, gensym("fast")));
    addProperty(new SymbolEnumAlias("@low", quality_, gensym("low")));
    addProperty(new SymbolEnumAlias("@medium", quality_, gensym("medium")));
    addProperty(new SymbolEnumAlias("@high", quality_, gensym("high")));
    addProperty(new SymbolEnumAlias("@best", quality_, gensym("best")));

    createOutlet();
}

void ArrayResample::onBang()
{
    if (!src_array_.open(src_name_->value())) {
        OBJ_ERR << fmt::format("can't open source array: '{}'", src_name_->value()->s_name);
        return;
    }

    if (src_array_.size() == 0) {
        OBJ_ERR << fmt::format("empty input array: '{}'", src_array_.name()->s_name);
        return;
    }

    if (dest_name_->value() == &s_) {
        resampleSingle();
    } else {
        if (!dest_array_.open(dest_name_->value())) {
            OBJ_ERR << fmt::format("can't open destination array: '{}'", dest_name_->value()->s_name);
            return;
        }

        resampleCopy();
    }
}

void ArrayResample::resampleCopy()
{
    // assert(src_array_.isValid())
    // assert(dest_array_.isValid())

    const double IN_RATE = 1;
    double orate = 1;

    if (ratio_->value() == 0) {
        if (dest_array_.size() == 0) {
            OBJ_ERR << fmt::format(
                "resample ratio is not specified and destination array '{}' has zero length",
                dest_name_->value()->s_name);
            return;
        }

        orate = dest_array_.size() / static_cast<double>(src_array_.size());
    } else {
        orate = ratio_->value();
    }

    const auto DEST_LEN = static_cast<size_t>(std::round(src_array_.size() * orate));
    if (DEST_LEN < 1 || DEST_LEN > MAX_ARRAY_SIZE) {
        OBJ_ERR << fmt::format("invalid destination size: {}", DEST_LEN);
        return;
    }

    if (!dest_array_.resize(DEST_LEN)) {
        OBJ_ERR << fmt::format(
            "can't resize output array '{}' to {} samples",
            src_name_->value()->s_name, DEST_LEN);

        return;
    }

    if (orate == 1) {
        std::copy(src_array_.begin(), src_array_.end(), dest_array_.begin());
        dest_array_.redraw();
        floatTo(0, dest_array_.size());
        return;
    }

    constexpr soxr_datatype_t samptype = (sizeof(t_sample) == sizeof(float)) ? SOXR_FLOAT32_I : SOXR_FLOAT64_I;
    const auto io_spec = soxr_io_spec(samptype, samptype);
    const soxr_quality_spec_t q_spec = soxr_quality_spec(symbol2quality(quality_->value()), 0);
    soxr_error_t error;

    soxr_t soxr = soxr_create(
        IN_RATE, orate, 1, /* Input rate, output rate, # of channels. */
        &error, /* To report any error during creation. */
        &io_spec, &q_spec, nullptr); /* Use configuration defaults.*/

    if (error) {
        OBJ_ERR << fmt::format("soxr_create: {}", error);
        soxr_delete(soxr);
        return;
    }

    constexpr size_t BUF_TOTAL_LEN = 10000;
    const size_t OUT_LEN = (size_t)(orate * BUF_TOTAL_LEN / (IN_RATE + orate) + .5);
    const size_t IN_LEN = BUF_TOTAL_LEN - OUT_LEN;
    std::unique_ptr<t_sample[]> out(new t_sample[OUT_LEN]);
    std::unique_ptr<t_sample[]> in(new t_sample[IN_LEN]);

    bool need_input = true;
    size_t read_pos = 0;
    size_t write_pos = 0;

    do {
        size_t ilen1 = 0;

        if (need_input) {
            ilen1 = std::min<size_t>(IN_LEN, src_array_.size() - read_pos);

            if (ilen1 == 0) { /* If the is no (more) input data available, */
                in.reset();
            } else {
                auto from = src_array_.begin() + read_pos;
                std::copy(from, from + ilen1, in.get());
                read_pos += ilen1;
            }
        }

        /*
         * Copy data from the input buffer into the resampler, and resample
         * to produce as much output as is possible to the given output buffer:
         */
        size_t odone;
        error = soxr_process(soxr, in.get(), ilen1, nullptr, out.get(), OUT_LEN, &odone);

        if (error) {
            OBJ_ERR << fmt::format("soxr_process: {}", error);
            break;
        }

        std::copy(out.get(), out.get() + odone, dest_array_.begin() + write_pos);
        write_pos += odone;

        /*
         * If the actual amount of data output is less than that requested, and
         * we have not already reached the end of the input data, then supply some
         * more input next time round the loop:
         */
        need_input = (odone < OUT_LEN) && in;

    } while (need_input);

    soxr_delete(soxr);

    // redraw
    dest_array_.redraw();
    floatTo(0, dest_array_.size());
}

void ArrayResample::resampleSingle()
{
    const double IN_RATE = 1;
    const double OUT_RATE = ratio_->value();

    if (OUT_RATE == 0) {
        OBJ_ERR << fmt::format("resample ratio is not specified");
        return;
    }

    const auto DEST_LEN = static_cast<size_t>(std::round(src_array_.size() * OUT_RATE / IN_RATE));
    if (DEST_LEN < 1 || DEST_LEN > MAX_ARRAY_SIZE) {
        OBJ_ERR << fmt::format("invalid destination size: {}", DEST_LEN);
        return;
    }

    std::vector<t_sample> tmp(DEST_LEN);

    constexpr soxr_datatype_t samptype = (sizeof(t_sample) == sizeof(float)) ? SOXR_FLOAT32_I : SOXR_FLOAT64_I;
    const auto io_spec = soxr_io_spec(samptype, samptype);
    soxr_quality_spec_t q_spec = soxr_quality_spec(symbol2quality(quality_->value()), 0);
    soxr_error_t error;

    soxr_t soxr = soxr_create(
        IN_RATE, OUT_RATE, 1, /* Input rate, output rate, # of channels. */
        &error, /* To report any error during creation. */
        &io_spec, &q_spec, nullptr); /* Use configuration defaults.*/

    if (error) {
        OBJ_ERR << fmt::format("soxr_create: {}", error);
        soxr_delete(soxr);
        return;
    }

    const size_t IN_LEN = 4096;
    std::unique_ptr<t_sample[]> in(new t_sample[IN_LEN]);

    bool need_input = true;
    size_t read_pos = 0;
    size_t write_pos = 0;

    do {
        size_t ilen1 = 0;

        if (need_input) {
            ilen1 = std::min<size_t>(IN_LEN, src_array_.size() - read_pos);

            if (ilen1 == 0) { /* If the is no (more) input data available, */
                in.reset();
            } else {
                auto from = src_array_.begin() + read_pos;
                std::copy(from, from + ilen1, in.get());
                read_pos += ilen1;
            }
        }

        /*
         * Copy data from the input buffer into the resampler, and resample
         * to produce as much output as is possible to the given output buffer:
         */
        size_t odone;
        auto out_it = tmp.begin() + write_pos;
        error = soxr_process(soxr, in.get(), ilen1, nullptr, &(*out_it), DEST_LEN - write_pos, &odone);

        if (error) {
            OBJ_ERR << fmt::format("soxr_process: {}", error);
            break;
        }

        write_pos += odone;

        /*
         * If the actual amount of data output is less than that requested, and
         * we have not already reached the end of the input data, then supply some
         * more input next time round the loop:
         */
        need_input = (odone < DEST_LEN) && in;

    } while (need_input);

    soxr_delete(soxr);

    if (!src_array_.resize(DEST_LEN)) {
        OBJ_ERR << fmt::format("can't resize array '{}' to {} samples", src_array_.name()->s_name, DEST_LEN);
        return;
    }

    std::copy(tmp.begin(), tmp.end(), src_array_.begin());
    src_array_.redraw();
    floatTo(0, src_array_.size());
}

void setup_array_resample()
{
    ObjectFactory<ArrayResample> obj("array.resample");
    obj.addAlias("array.r");
    LIB_DBG << "soxr version: " << SOXR_THIS_VERSION_STR;
}

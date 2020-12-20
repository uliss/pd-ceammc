#include "synth_glitch.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

#include <chrono>
#include <fstream>
#include <sstream>
#include <stdexcept>

constexpr size_t MAX_GLITCH_FILE_SIZE = 4 * 1024;

static std::future<std::string> readFile(const std::string& fullPath)
{
    auto do_read = [](const std::string& path) -> std::string {
        std::ifstream ifs(path);
        if (!ifs) {
            std::string msg = "can't read from file: " + path;
            throw std::runtime_error(msg);
        }

        ifs.seekg(0, std::ios::end); // go to the end
        const auto len = ifs.tellg(); // length in bytes
        if (len > 1024) {
            std::ostringstream os;
            os << "file is too big: " << len << " bytes, max size is " << MAX_GLITCH_FILE_SIZE;
            throw std::runtime_error(os.str());
        }

        ifs.seekg(0, std::ios::beg); // go back to the beginning
        return std::string(
            std::istreambuf_iterator<char>(ifs),
            std::istreambuf_iterator<char>());
    };

    std::packaged_task<std::string(const std::string&)> task(do_read);
    auto future = task.get_future();
    std::thread thread(std::move(task), fullPath);
    thread.detach();
    return future;
}

static t_symbol* SYM_BYTE_BEGIN;
static t_symbol* SYM_BYTE_END;
static t_symbol* SYM_FILE;

SynthGlitch::SynthGlitch(const PdArgs& args)
    : SoundExternal(args)
    , t_(0)
    , expr_(nullptr)
    , clip_(nullptr)
    , read_clock_([this]() {
        if (read_content_.valid()) {
            auto st = read_content_.wait_for(std::chrono::milliseconds(20));

            if (st == std::future_status::ready) {
                try {
                    const auto str = read_content_.get();
                    if (!glitch_.setExpr(str)) {
                        OBJ_ERR << "can't set expression: " << str;
                    } else {
                        char buf[MAXPDSTRING];
                        snprintf(buf, MAXPDSTRING - 1, "file(\"%s\")", file_path_.c_str());

                        expr_->value().resizeClip(1);
                        expr_->value().at(0).setSymbol(gensym(buf), true);
                    }

                } catch (std::exception& e) {
                    OBJ_ERR << e.what();
                }
            } else
                read_clock_.delay(20);
        }
    })
{
    createSignalOutlet();
    glitch_.setSamplerate(sys_getsr());

    expr_ = new ListProperty("@expr");
    if (!expr_->setListCheckFn(
            [this](const AtomList& l) -> bool {
                const auto str = to_string(l);
                return glitch_.setExpr(str);
            })) {
        OBJ_ERR << "can't set list check...";
    }
    expr_->value().reserve(32);
    addProperty(expr_);
    if (!expr_->setList(unparsedPosArgs()))
        OBJ_ERR << "invalid expression: " << unparsedPosArgs();

    clip_ = new BoolProperty("@clip", true);
    addProperty(clip_);

    for (auto& p : args.args.properties()) {
        if (p[0].asT<t_symbol*>() == clip_->name())
            clip_->setList(p.view(1));
    }

    file_path_.reserve(256);
}

void SynthGlitch::processBlock(const t_sample** /*in*/, t_sample** out)
{
    const auto bs = blockSize();
    const auto do_clip = clip_->value();


#if PD_FLOATSIZE == 32
    glitch_.fill(out[0], bs, 1);

    if (do_clip) {
        for (size_t i = 0; i < bs; i++)
            out[0][i] = clip<t_float, -1, 1>(out[0][i]);
    }
#elif PD_FLOATSIZE == 64
    float buf[bs];
    glitch_.fill(buf, bs, 1);

    for (size_t i = 0; i < bs; i++)
        out[0][i] = clip<t_float, -1, 1>(buf[i]);
#else
#error "unknown float size"
#endif
}

void SynthGlitch::samplerateChanged(size_t sr)
{
    glitch_.setSamplerate(sr);
}

void SynthGlitch::m_byte(t_symbol* /*s*/, const AtomListView& lv)
{
    const size_t n = lv.size() + 2;

    // using stack allocation
    Atom atoms[n];
    atoms[0].setSymbol(SYM_BYTE_BEGIN, true);
    atoms[n - 1].setSymbol(SYM_BYTE_END, true);
    for (size_t i = 0; i < lv.size(); i++)
        atoms[i + 1] = lv[i];

    expr_->setList(AtomListView(&(atoms->atom()), n));
}

void SynthGlitch::m_reset(t_symbol* s, const AtomListView& lv)
{
    glitch_.reset();
}

void SynthGlitch::m_read(t_symbol* s, const AtomListView& lv)
{
    file_path_ = to_string(lv);
    if (file_path_.empty()) {
        METHOD_ERR(s) << "empty path";
        return;
    }

    const auto full_path = findInStdPaths(file_path_.c_str());
    if (full_path.empty()) {
        METHOD_ERR(s) << "file not found in standard search paths: " << file_path_;
        return;
    }

    OBJ_DBG << "loading glitch file: " << full_path;

    if (read_content_.valid()) {
        OBJ_ERR << "already reading";
        return;
    }

    read_content_ = readFile(full_path);
    read_clock_.delay(20);
}

void setup_synth_glitch()
{
    SYM_BYTE_BEGIN = gensym("byte(");
    SYM_BYTE_END = gensym(")");
    SYM_FILE = gensym("file");

    SoundExternalFactory<SynthGlitch> obj("synth.glitch~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("byte", &SynthGlitch::m_byte);
    obj.addMethod("reset", &SynthGlitch::m_reset);
    obj.addMethod("read", &SynthGlitch::m_read);
    obj.useDefaultPdFloatFn();
    obj.noArgsDataParsing();
}

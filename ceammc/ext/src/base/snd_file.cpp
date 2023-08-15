#include "snd_file.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_platform.h"
#include "ceammc_sound.h"
#include "fmt/format.h"
#include "lex/array_loader.h"
#include "lex/array_saver.h"
#include "lex/parser_strings.h"

#include "config.h"

#include <cctype>

using namespace ceammc;
using namespace ceammc::sound;

SndFile::SndFile(const PdArgs& a)
    : BaseObject(a)
    , verbose_(nullptr)
    , debug_(nullptr)
    , smpte_framerate_(nullptr)
{
    createOutlet();

    verbose_ = new FlagProperty("@verbose");
    addProperty(verbose_);

    debug_ = new FlagProperty("@.debug");
    debug_->setInternal();
    addProperty(debug_);

    smpte_framerate_ = new FloatProperty("@smpte_fr", 30);
    smpte_framerate_->checkClosedRange(1, 99);
    addProperty(smpte_framerate_);

    createCbListProperty("@sr", [this]() -> AtomList { return samplerates_; });
    createCbListProperty("@filename", [this]() -> AtomList { return filenames_; });
    createCbListProperty("@samples", [this]() -> AtomList { return samplecount_; });
    createCbListProperty("@channels", [this]() -> AtomList { return channels_; });

    createCbListProperty("@formats",
        []() -> AtomList {
            AtomList res;
            FormatList lst = SoundFileFactory::supportedReadFormats();
            res.reserve(lst.size());
            for (size_t i = 0; i < lst.size(); i++)
                res.append(gensym(lst[i].first.c_str()));

            return res;
        });
}

void SndFile::m_load(t_symbol* s, const AtomListView& lv)
{
    std::string fname, array_opts;

    // clear previous info
    filenames_.clear();
    samplerates_.clear();
    samplecount_.clear();

    if (!extractLoadArgs(lv, fname, array_opts)) {
        METHOD_ERR(s) << "not enough arguments: " << lv;
        return postLoadUsage();
    }

    // check if file exists and if true obtain full path
    auto mfull_path = fullLoadPath(fname);
    if (!mfull_path)
        return;

    Error err(this);
    Log log(this);

    ArrayLoader loader;
    loader.setDebug(debug_->value());
    loader.setSmpteFramerate(smpte_framerate_->value());

    loader.setErr(&err.stream());
    loader.setLog(&log.stream());

    auto file = loader.openFile(mfull_path.value().c_str());
    if (!file) {
        OBJ_ERR << fmt::format("can't open file: {}", *mfull_path);
        return;
    }

    // src samplerate set from this moment
    // set destination samplerate before parse -
    // after parse resampleRatio can be set
    loader.setDestSamplerate(sys_getsr());

    if (!loader.parse(array_opts)) {
        OBJ_ERR << "can't parse options: " << array_opts;
        return postLoadUsage();
    }

    loader.validateArrays();
    loader.removeInvalidArrays();
    loader.fixArrayChannelPairs();

    if (verbose_->value())
        loader.dump();

    err.flush();
    log.flush();

    if (loader.arrays().empty()) {
        OBJ_ERR << "no valid arrays...";
        return;
    }

    // set gain after parse, but before loading
    file->setGain(loader.gain());
    // set resample after parse, but before loading
    if (loader.resampleRatio() > 0)
        file->setResampleRatio(loader.resampleRatio());

    if (!loader.loadArrays(file, true)) {
        OBJ_ERR << fmt::format("can't load file '{}' to arrays: {}",
            *mfull_path, fmt::join(loader.arrays(), ", "));

        return;
    }

    if (verbose_->value()) {
        OBJ_POST << fmt::format("loaded file '{}' to arrays: {}",
            *mfull_path, fmt::join(loader.arrays(), ", "));
        OBJ_POST << fmt::format("loaded samples: {}", fmt::join(loader.loadedSamples(), " "));
    }

    filenames_.append(gensym((*mfull_path).c_str()));
    samplerates_.append(loader.srcSampleRate());

    for (auto& s : loader.loadedSamples())
        samplecount_.append(atomFrom(s));

    for (auto c : loader.loadedChannels())
        channels_.append(c);

    listTo(0, samplecount_);
}

void SndFile::m_save(t_symbol* s, const AtomListView& lv)
{
    ArraySaver saver;
    auto n = saver.saveTo(lv, this);
    if (n >= 0)
        floatTo(0, n);
}

void SndFile::postLoadUsage()
{
    OBJ_DBG << fmt::format(
        "usage: load <FILENAME> (to | @to) <ARRAY>... \n\t{}\n",
        fmt::join(ArrayLoader::optionsList(), "\n\t"));
}

MaybeString SndFile::fullLoadPath(const std::string& fname) const
{
    if (isAbsolutePath(fname.c_str())) {
        if (platform::path_exists(fname.c_str())) {
            return { fname };
        } else {
            OBJ_ERR << fmt::format("file not exists: '{}'", fname);
            return {};
        }
    } else {
        auto full_path = findInStdPaths(fname.c_str());
        if (full_path.empty()) {
            OBJ_ERR << fmt::format("can't find file '{}' in standart paths", fname);
            return {};
        } else {
            return full_path;
        }
    }
}

bool SndFile::extractLoadArgs(const AtomListView& lv, std::string& fname, std::string& array_opts)
{
    CEAMMC_DEFINE_HASH(to);

    if (lv.size() < 3)
        return false;

    auto it = std::find_if(lv.begin(), lv.end(),
        [](const Atom& a) {
            return a.isProperty()
                || (a.isSymbol() && crc32_hash(a.asT<t_symbol*>()) == hash_to);
        });

    if (it == lv.end())
        return false;

    auto array_opt_pos = std::distance(lv.begin(), it);

    fname = to_string(lv.subView(0, array_opt_pos), " ");
    // parse end escapes quotes
    // check for "quoted path with spaces"
    string::MediumString buf;
    if (string::unquote_and_unescape(fname.c_str(), buf))
        fname.assign(buf.data(), buf.size());

    array_opts = to_string(lv.subView(array_opt_pos), " ");
    return true;
}

void setup_snd_file()
{
    ObjectFactory<SndFile> obj("snd.file");
    obj.addMethod("load", &SndFile::m_load);
    obj.addMethod("save", &SndFile::m_save);

    obj.setDescription("Sound file loader on steroids");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "soundfiler" });
    obj.setCategory("snd");
    obj.setSinceVersion(0, 1);
}

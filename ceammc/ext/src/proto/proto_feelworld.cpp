#include "proto_feelworld.h"
#include "ceammc_factory.h"

#include "fmt/core.h"

#include <numeric>

constexpr int INPUT_MIN = 0;
constexpr int INPUT_MAX = 3;

namespace {

uint8_t nibbleToAscii(uint8_t n)
{
    if (n >= 10)
        return 'a' + (n - 10);
    else
        return '0' + n;
}

std::pair<uint8_t, uint8_t> byteToAscii(uint8_t b)
{
    return { nibbleToAscii(b >> 4), nibbleToAscii(b & 0x0f) };
}
}

ProtoFeelworld::ProtoFeelworld(const PdArgs& args)
    : BaseObject(args)
    , msg_id_(0)
{
    createOutlet();
}

void ProtoFeelworld::m_input(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_BYTE))
        return;

    int idx = lv[0].asT<int>();
    if (idx < INPUT_MIN || idx > INPUT_MAX) {
        METHOD_ERR(s) << fmt::format("invalid input: {}", idx);
        return;
    }

    auto id = ++msg_id_;

    std::array<std::uint8_t, 8> bin_msg = {
        0,
        static_cast<unsigned char>(id),
        0x75,
        0x02,
        0x00,
        static_cast<unsigned char>(idx),
        0x00,
        0x00,
    };
    // calc check sum
    bin_msg.back() = std::accumulate(bin_msg.begin(), bin_msg.end(), 0);

    std::array<Atom, 3 + bin_msg.size() * 2> atom_msg;
    atom_msg[0] = Atom('<');
    atom_msg[1] = Atom('T');
    for (size_t i = 0, j = 2; i < bin_msg.size(); i++, j += 2) {
        auto ascii = byteToAscii(bin_msg[i]);
        atom_msg[j] = ascii.first;
        atom_msg[j + 1] = ascii.second;
    }
    atom_msg.back() = Atom('>');

    listTo(0, AtomListView(atom_msg.data(), atom_msg.size()));

    std::string str_msg;
    str_msg.reserve(atom_msg.size());
    for (auto& a : atom_msg)
        str_msg += a.asT<int>();

    OBJ_LOG << str_msg;
}

void setup_proto_feelworld()
{
    ObjectFactory<ProtoFeelworld> obj("proto.feelworld");
    obj.addMethod("input", &ProtoFeelworld::m_input);
}

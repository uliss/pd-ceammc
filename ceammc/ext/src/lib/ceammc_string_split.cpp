#include "ceammc_string_split.h"
#include "ceammc_string.h"

namespace ceammc {
namespace string {

    void split_by_sep(StringSplitResult& vec, const StringView& sv, const char* sep)
    {
        size_t first = 0;

        while (first < sv.size()) {
            const auto second = sv.find_first_of(sep, first);

            if (first != second)
                vec.emplace_back(sv.substr(first, second - first));

            if (second == StringView::npos)
                break;

            first = second + 1;
        }
    }

    void split_every_char(StringSplitResult& vec, const char* str)
    {
        size_t len = utf8_strlen(str);
        if (len == 0)
            return;

        vec.reserve(len);

        const char* cur = str;

        while (len-- > 0) {
            auto next = utf8_next(cur, strlen(cur));
            vec.push_back(StringView(cur, next - cur));
            cur = next;
        }
    }

}
}

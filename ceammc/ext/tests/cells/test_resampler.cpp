/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "catch.hpp"
#include "ceammc_soxr_resampler.h"
#include "test_base.h"

#include <algorithm>

using namespace ceammc;

TEST_CASE("resampler", "[ceammc]")
{
    test::pdPrintToStdError();

    SECTION("init")
    {
        SoxrResampler r(100, 200, 2);
        REQUIRE(r.ratio() == 0.5);
        REQUIRE(r.numChannels() == 2);
    }

    SECTION("process 1=>1")
    {
        std::array<t_sample, 64> data;
        data.fill(1);

        SoxrResampler r(100, 100, 2);
        REQUIRE(r.delay() == 0);

        std::vector<t_sample> res;
        r.setOutputCallback([&res](const t_sample* data, size_t len, bool done) -> bool {
            std::copy(data, data + len * 2, std::back_inserter(res));
            return true;
        });

        REQUIRE(r.process(data.data(), data.size() / 2) == SoxrResampler::Ok);
        REQUIRE(r.processDone() == SoxrResampler::Ok);
        REQUIRE(res.size() == 64);
        REQUIRE(std::all_of(res.begin(), res.end(), [](t_sample x) { return x == 1; }));
    }

    SECTION("process 1=>2")
    {
        std::array<t_sample, 64> data;
        data.fill(1);
        SoxrResampler r(100, 200, 2);
        REQUIRE(r.delay() == 0);

        std::vector<t_sample> res;
        r.setOutputCallback([&res](const t_sample* data, size_t len, bool done) -> bool {
            std::copy(data, data + len * 2, std::back_inserter(res));
            return true;
        });

        REQUIRE(r.process(data.data(), data.size() / 2) == SoxrResampler::Ok);
        REQUIRE(r.processDone() == SoxrResampler::Ok);
        REQUIRE(res.size() == 128);
        REQUIRE(std::all_of(res.begin() + 8, res.end() - 8, [](t_sample x) { return x == Approx(1).margin(0.001); }));
    }

    SECTION("process 2=>1")
    {
        std::array<t_sample, 12> data { 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3 };
        SoxrResampler r(200, 100, 3);
        REQUIRE(r.delay() == 0);

        std::vector<t_sample> res;
        r.setOutputCallback([&res](const t_sample* data, size_t len, bool done) -> bool {
            std::copy(data, data + len * 3, std::back_inserter(res));
            return true;
        });

        REQUIRE(r.process(data.data(), data.size() / 3) == SoxrResampler::Ok);
        REQUIRE(r.processDone() == SoxrResampler::Ok);
        REQUIRE(res.size() == 6);
    }

    SECTION("variable")
    {
        std::array<t_sample, 64> data;
        data.fill(1);

        SoxrResampler r(100, 100, 2, SoxrResampler::QUICK, SoxrResamplerOptions(true));
        REQUIRE(r.delay() == 100);

        std::vector<t_sample> res;
        r.setOutputCallback([&res](const t_sample* data, size_t len, bool done) -> bool {
            std::copy(data, data + len * 2, std::back_inserter(res));
            return true;
        });

        REQUIRE(r.setResampleRatio(1));

        REQUIRE(r.process(data.data(), data.size() / 2) == SoxrResampler::Ok);
        REQUIRE(r.processDone() == SoxrResampler::Ok);
        REQUIRE(res.size() == 64);
    }

    SECTION("formats")
    {
        SECTION("interleave")
        {
            SECTION("float -> float")
            {
                std::array<float, 64> data;
                data.fill(1);
                SoxrResamplerOptions opts {
                    false,
                    SoxrResamplerFormat::FLOAT_I,
                    SoxrResamplerFormat::FLOAT_I,
                };
                SoxrResampler r(100, 200, 2, SoxrResampler::QUICK, opts);
                REQUIRE(r.delay() == 0);

                std::vector<float> res;
                r.setOutputCallback([&res](const float* data, size_t len, bool done) -> bool {
                    std::copy(data, data + len * 2, std::back_inserter(res));
                    return true;
                });

                REQUIRE(r.process(data.data(), data.size() / 2) == SoxrResampler::Ok);
                REQUIRE(r.processDone() == SoxrResampler::Ok);
                REQUIRE(res.size() == 128);
                REQUIRE(std::all_of(res.begin() + 8, res.end() - 8, [](t_sample x) { return x == Approx(1).margin(0.001); }));
            }

            SECTION("double -> float")
            {
                std::array<double, 64> data;
                data.fill(1);
                SoxrResamplerOptions opts {
                    false,
                    SoxrResamplerFormat::DOUBLE_I,
                    SoxrResamplerFormat::FLOAT_I,
                };
                SoxrResampler r(100, 200, 2, SoxrResampler::QUICK, opts);
                REQUIRE(r.delay() == 0);

                std::vector<float> res;
                r.setOutputCallback([&res](const float* data, size_t len, bool done) -> bool {
                    std::copy(data, data + len * 2, std::back_inserter(res));
                    return true;
                });

                REQUIRE(r.process(data.data(), data.size() / 2) == SoxrResampler::Ok);
                REQUIRE(r.processDone() == SoxrResampler::Ok);
                REQUIRE(res.size() == 128);
                REQUIRE(std::all_of(res.begin() + 8, res.end() - 8, [](t_sample x) { return x == Approx(1).margin(0.001); }));
            }

            SECTION("double -> double")
            {
                std::array<double, 64> data;
                data.fill(1);
                SoxrResamplerOptions opts {
                    false,
                    SoxrResamplerFormat::DOUBLE_I,
                    SoxrResamplerFormat::DOUBLE_I,
                };
                SoxrResampler r(100, 200, 2, SoxrResampler::QUICK, opts);
                REQUIRE(r.delay() == 0);

                std::vector<double> res;
                r.setOutputCallback([&res](const double* data, size_t len, bool done) -> bool {
                    std::copy(data, data + len * 2, std::back_inserter(res));
                    return true;
                });

                REQUIRE(r.process(data.data(), data.size() / 2) == SoxrResampler::Ok);
                REQUIRE(r.processDone() == SoxrResampler::Ok);
                REQUIRE(res.size() == 128);
                REQUIRE(std::all_of(res.begin() + 8, res.end() - 8, [](t_sample x) { return x == Approx(1).margin(0.001); }));
            }
        }

        SECTION("split")
        {
            SECTION("float -> float")
            {
                std::array<float, 64> data;
                data.fill(1);
                SoxrResamplerOptions opts {
                    false,
                    SoxrResamplerFormat::FLOAT_I,
                    SoxrResamplerFormat::FLOAT_S,
                };
                SoxrResampler r(100, 200, 2, SoxrResampler::QUICK, opts);
                REQUIRE(r.delay() == 0);

                std::vector<float> r0;
                std::vector<float> r1;
                r.setOutputCallback([&r1, &r0](const float* const* data, size_t len, bool done) -> bool {
                    std::copy(data[0], data[0] + len, std::back_inserter(r0));
                    std::copy(data[1], data[1] + len, std::back_inserter(r1));
                    return true;
                });

                REQUIRE(r.process(data.data(), data.size() / 2) == SoxrResampler::Ok);
                REQUIRE(r.processDone() == SoxrResampler::Ok);
                REQUIRE(r0.size() == 64);
                REQUIRE(std::all_of(r0.begin() + 8, r0.end() - 8, [](t_sample x) { return x == Approx(1).margin(0.001); }));
                REQUIRE(r1.size() == 64);
                REQUIRE(std::all_of(r1.begin() + 8, r1.end() - 8, [](t_sample x) { return x == Approx(1).margin(0.001); }));
            }
        }
    }
}

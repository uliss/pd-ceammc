/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "conv_bits2bang.h"
#include "test_conv_base.h"

PD_COMPLETE_TEST_SETUP(ConvBits2Bang, conv, bits2bang)

TEST_CASE("conv.bits2bang", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    const Message b = Message::makeBang();
    using ML = std::vector<Message>;

    SECTION("create")
    {
        SECTION("default")
        {
            TObj t("conv.bits2bang");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 8);
            REQUIRE_PROPERTY(t, @rev, 0);
            REQUIRE_PROPERTY(t, @poly, 0);
            REQUIRE_PROPERTY(t, @n, 8);
        }

        SECTION("args")
        {
            TObj t("conv.bits2bang", LF(4));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 4);
            REQUIRE_PROPERTY(t, @rev, 0);
            REQUIRE_PROPERTY(t, @poly, 0);
            REQUIRE_PROPERTY(t, @n, 4);
        }

        SECTION("args @poly")
        {
            TObj t("conv.bits2bang", LA(5, "@poly"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @rev, 0);
            REQUIRE_PROPERTY(t, @poly, 1);
            REQUIRE_PROPERTY(t, @n, 5);
        }

        SECTION("ext")
        {
            TExt t("conv.bits2bang");
        }

        SECTION("alias")
        {
            TExt t("bits->bang");
        }
    }

    SECTION("do fixed")
    {
        SECTION("normal")
        {
            TExt t("bits->bang", 3);
            t << LF(1, 1, 1);
            REQUIRE(t.messagesAt(0) == ML { b });
            REQUIRE(t.messagesAt(1) == ML { b });
            REQUIRE(t.messagesAt(2) == ML { b });
            t << LF(1, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { b });
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML { b });
            t << LF(1, 0, 0);
            REQUIRE(t.messagesAt(0) == ML { b });
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML {});
            t << LF(0, 0, 0);
            REQUIRE(t.messagesAt(0) == ML {});
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML {});

            t << 1;
            REQUIRE(t.messagesAt(0) == ML {});
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML {});
        }

        SECTION("@rev 1")
        {
            TExt t("bits->bang", LA(3, "@rev", 1));
            t << LF(1, 1, 1);
            REQUIRE(t.messagesAt(0) == ML { b });
            REQUIRE(t.messagesAt(1) == ML { b });
            REQUIRE(t.messagesAt(2) == ML { b });

            t << LF(1, 1, 0);
            REQUIRE(t.messagesAt(0) == ML {});
            REQUIRE(t.messagesAt(1) == ML { b });
            REQUIRE(t.messagesAt(2) == ML { b });

            t << LF(1, 0, 0);
            REQUIRE(t.messagesAt(0) == ML {});
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML { b });

            t << LF(0, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { b });
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML {});
        }
    }

    SECTION("@poly")
    {
        auto ml = [](int i) { return Message(AtomList({ (t_float)i, &s_bang })); };

        SECTION("stright")
        {
            TExt t("bits->bang", 3, "@poly");
            t << LF(1, 1, 1);
            REQUIRE(t.messagesAt(0) == ML { ml(0), ml(1), ml(2) });

            t << LF(1, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { ml(0), ml(2) });

            t << LF(0, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { ml(2) });

            t << LF(0, 0, 0, 0, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { ml(5) });
        }

        SECTION("@rev 1")
        {
            TExt t("bits->bang", 3, "@poly", "@rev", 1);
            t << LF(1, 1, 1);
            REQUIRE(t.messagesAt(0) == ML { ml(0), ml(1), ml(2) });

            t << LF(0, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { ml(0) });

            t << LF(0, 0, 0, 1, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { ml(0), ml(2) });
        }
    }
}

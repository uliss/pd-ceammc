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
#include "conv_bits2note.h"
#include "test_conv_base.h"

PD_COMPLETE_TEST_SETUP(ConvBits2Note, conv, bits2note)

TEST_CASE("conv.bits2note", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    using ML = std::vector<Message>;

    SECTION("create")
    {
        SECTION("default")
        {
            TObj t("conv.bits2note");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 0);
            REQUIRE_PROPERTY(t, @vel, 90);
        }

        SECTION("args")
        {
            TObj t("conv.bits2note", LF(60));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @keys, LF(60));
        }

        SECTION("args")
        {
            TObj t("conv.bits2note", LF(60, 72));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @keys, LF(60, 72));
        }

        SECTION("ext")
        {
            TExt t("conv.bits2note");
        }

        SECTION("alias")
        {
            TExt t("bits->note");
        }
    }

    SECTION("do mono")
    {
        SECTION("off")
        {
            TExt t("bits->note", LA(60, 62, 64, "@vel", 92, "@mode", "off"));
            t << LF(1, 0, 0);
            REQUIRE(t.messagesAt(0) == ML { LF(60, 92) });
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML {});

            t << LF(1, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { LF(60, 0), LF(60, 92) });
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML { LF(64, 92) });

            t << LF(1, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { LF(60, 0), LF(60, 92) });
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML { LF(64, 0), LF(64, 92) });

            t << LF(0, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { LF(60, 0) });
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML { LF(64, 0), LF(64, 92) });

            t << LF(0, 0, 1);
            REQUIRE(t.messagesAt(0) == ML {});
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML { LF(64, 0), LF(64, 92) });

            t << LF(1, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { LF(60, 92) });
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML { LF(64, 0), LF(64, 92) });
        }

        SECTION("tie")
        {
            TExt t("bits->note", LA(60, 62, 64, "@vel", 92, "@mode", "tie"));
            t << LF(1, 0, 0);
            REQUIRE(t.messagesAt(0) == ML { LF(60, 92) });
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML {});

            t << LF(1, 0, 1);
            REQUIRE(t.messagesAt(0) == ML {});
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML { LF(64, 92) });

            t << LF(1, 0, 1);
            REQUIRE(t.messagesAt(0) == ML {});
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML {});

            t << LF(0, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { LF(60, 0) });
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML {});

            t << LF(0, 0, 1);
            REQUIRE(t.messagesAt(0) == ML {});
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML {});

            t << LF(1, 0, 0);
            REQUIRE(t.messagesAt(0) == ML { LF(60, 92) });
            REQUIRE(t.messagesAt(1) == ML {});
            REQUIRE(t.messagesAt(2) == ML { LF(64, 0) });
        }

        SECTION("raw")
        {
            TExt t("bits->note", LA(60, 62, 64, "@vel", 92, "@mode", "raw"));
            t << LF(1, 0, 0);
            REQUIRE(t.messagesAt(0) == ML { LF(60, 92) });
            REQUIRE(t.messagesAt(1) == ML { LF(62, 0) });
            REQUIRE(t.messagesAt(2) == ML { LF(64, 0) });

            t << LF(1, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { LF(60, 92) });
            REQUIRE(t.messagesAt(1) == ML { LF(62, 0) });
            REQUIRE(t.messagesAt(2) == ML { LF(64, 92) });

            t << LF(1, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { LF(60, 92) });
            REQUIRE(t.messagesAt(1) == ML { LF(62, 0) });
            REQUIRE(t.messagesAt(2) == ML { LF(64, 92) });

            t << LF(0, 0, 1);
            REQUIRE(t.messagesAt(0) == ML { LF(60, 0) });
            REQUIRE(t.messagesAt(1) == ML { LF(62, 0) });
            REQUIRE(t.messagesAt(2) == ML { LF(64, 92) });
        }
    }
}

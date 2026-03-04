/*****************************************************************************
 * Copyright 2026 Serge Poltavsky. All rights reserved.
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
#ifndef HELP_BOX_VIEW_H
#define HELP_BOX_VIEW_H

#include "nui/helpbox_model.h"
#include "nui/view.h"

namespace ceammc {
namespace ui {

    class HelpboxView : public ModelView<HelpboxData> {
    public:
        HelpboxView(ModelBase<HelpboxData>* model, ViewImplPtr&& impl, const PointF& pos);

        EventAcceptStatus onEvent(EventType t, const PointF& pos, const EventContext& ctx) override;
    };

    class TclHelpboxImpl : public ViewImpl<HelpboxData> {
    public:
        void create(const RectF& bbox, const HelpboxData& data) final;
        void erase() final;
        void update(const RectF& bbox, const HelpboxData& data) final;
        void updateCoords(const RectF& bbox) final;
    };

    void tcl_help_box_init();
} // namespace ui
} // namespace ceammc

#endif // HELP_BOX_VIEW_H

#include "ui_env.h"
#include "ceammc_crc32.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"
#include "cicm/Sources/egraphics.h"
#include "lex/parser_units.h"

#include <boost/range.hpp>
#include <limits>

static const int ENV_MIN_WIDTH = 40;
static const int ENV_MIN_HEIGHT = 30;
static t_rgba DELETE_COLOR = hex_to_rgba("#F00030");
static t_rgba LINE_SELECTION_COLOR = hex_to_rgba("#AAFFFF");
static const char* DEFAULT_LINE_COLOR = "0.1 0.1 0.1 1.0";
static const char* PROP_LENGTH = "length";
constexpr const char* PROP_NORM = "norm";

using namespace ui;

static float lin2lin(float v, float x0, float x1, float y0, float y1)
{
    return convert::lin2lin(clip(v, x0, x1), x0, x1, y0, y1);
}

CEAMMC_DEFINE_SYM(length)
// keep in sync with @output_mode property!
CEAMMC_DEFINE_SYM(mouse_up)
CEAMMC_DEFINE_SYM(drag)

UIEnv::UIEnv()
    : draw_cursor_pos_(false)
    , draw_cursor_cross_(false)
    , delete_mode_(false)
    , envelope_layer_(asEBox(), gensym("envelope_layer"))
    , cursor_layer_(asEBox(), gensym("cursor_layer"))
    , font_(gensym(FONT_FAMILY), FONT_SIZE_SMALL)
    , cursor_txt_pos_(font_.font(), ColorRGBA::black(), ETEXT_UP, ETEXT_JCENTER)
    , max_env_value_(1)
    , env_last_time_(0)
    , txt_font(gensym(FONT_FAMILY), FONT_SIZE_SMALL)
    , txt_value0(txt_font.font(), ColorRGBA::black(), ETEXT_UP_LEFT, ETEXT_JLEFT)
    , txt_value1(txt_font.font(), ColorRGBA::black(), ETEXT_DOWN_LEFT, ETEXT_JLEFT)
    , txt_value2(txt_font.font(), ColorRGBA::black(), ETEXT_DOWN_LEFT, ETEXT_JLEFT)
    , output_mode_(sym_mouse_up())
{
    cursor_pos_.x = 0;
    cursor_pos_.y = 0;
    cursor_txt_pos_.setWidth(100);
    cursor_txt_pos_.setY(FONT_SIZE_SMALL);

    appendToLayerList(&envelope_layer_);
    appendToLayerList(&cursor_layer_);

    createOutlet();
    createOutlet();

    env_.setADSR(40 * 1000, 60 * 1000, 0.3, 400 * 1000);
    updateNodes();

    initPopupMenu("env",
        { { "ADSR(10 20 30 500)", [this](const t_pt&) { setNamedEnvelope(str_adsr, AtomList { 10, 20, 30, 500 }); } },
            { "ASR (500 500)", [this](const t_pt&) { setNamedEnvelope(str_asr, AtomList { 500, 500 }); } },
            { "AR (500 500)", [this](const t_pt&) { setNamedEnvelope(str_ar, AtomList { 500, 500 }); } } });

    initPopupMenu("point",
        { { "toggle stop", [this](const t_pt& /*pt*/) {
               auto idx = findSelectedNodeIdx();
               // ignore first node too
               if (idx < 1)
                   return;

               nodes_[idx].is_stop = !nodes_[idx].is_stop;
               redrawLayer(envelope_layer_);
           } },
            { "toggle fixed Y", [this](const t_pt& /*pt*/) {
                 auto idx = findSelectedNodeIdx();
                 // ignore first node too
                 if (idx < 1)
                     return;

                 nodes_[idx].fixed_y = !nodes_[idx].fixed_y;
                 env_.pointAt(idx).toggleFixValue();
                 redrawLayer(envelope_layer_);
             } } });

    initPopupMenu("line",
        { { "linear", [this](const t_pt& pt) {
               auto idx = findNodeLine(pt);
               if (idx < 0)
                   return;

               nodes_[idx].type = CURVE_LINE;
               nodes_[idx].select = SelectType::NONE;
               redrawLayer(envelope_layer_);
           } },
            { "exp", [this](const t_pt& pt) {
                 auto idx = findNodeLine(pt);
                 if (idx < 0)
                     return;

                 nodes_[idx].type = CURVE_EXP;
                 nodes_[idx].select = SelectType::NONE;
                 redrawLayer(envelope_layer_);
             } },
            { "sin2", [this](const t_pt& pt) {
                 auto idx = findNodeLine(pt);
                 if (idx < 0)
                     return;

                 nodes_[idx].type = CURVE_SIN2;
                 nodes_[idx].select = SelectType::NONE;
                 redrawLayer(envelope_layer_);
             } },
            { "sigmoid", [this](const t_pt& pt) {
                 auto idx = findNodeLine(pt);
                 if (idx < 0)
                     return;

                 nodes_[idx].type = CURVE_SIGMOID;
                 nodes_[idx].select = SelectType::NONE;
                 redrawLayer(envelope_layer_);
             } },
            { "step", [this](const t_pt& pt) {
                 auto idx = findNodeLine(pt);
                 if (idx < 0)
                     return;

                 nodes_[idx].type = CURVE_STEP;
                 nodes_[idx].select = SelectType::NONE;
                 redrawLayer(envelope_layer_);
             } } });
}

void UIEnv::onBang()
{
    atomTo(0, EnvAtom(env_));
}

void UIEnv::onData(const Atom& data)
{
    if (!data.isA<DataTypeEnv>()) {
        UI_ERR << "not Envelope received: " << data;
        return;
    }

    const DataTypeEnv* env = data.asD<DataTypeEnv>();

    if (env->numPoints() < 2) {
        UI_ERR << "invalid (for editor) envelope : " << *env;
        return;
    }

    if (prop_normalize)
        env_ = env->normalize();
    else
        env_ = *env;

    updateNodes();
    redrawAll();
    onBang();
}

void UIEnv::drawCursor(const t_rect& r)
{
    UIPainter cp = cursor_layer_.painter(r);

    if (cp) {
        // draw add point cross
        if (draw_cursor_cross_) {
            cp.setColor(prop_active_color);
            cp.drawLine(0, cursor_pos_.y, width(), cursor_pos_.y);
            cp.drawLine(cursor_pos_.x, 0, cursor_pos_.x, height());
        }

        // draw cursor position text
        if (draw_cursor_pos_) {
            char buf[100];

            snprintf(buf, sizeof(buf) - 1, "%d(ms) : %0.2f",
                (int)std::round(lin2lin(cursor_pos_.x, 0, width(), 0, prop_length)),
                lin2lin(cursor_pos_.y, 0, height(), max_env_value_, 0));

            cursor_txt_pos_.setColor(prop_color_border);
            cursor_txt_pos_.set(buf, r.w / 2, 3, r.w, 20);
            cp.drawText(cursor_txt_pos_);
        }
    }
}

void UIEnv::drawBackground(const t_rect& r)
{
    UIPainter bp = bg_layer_.painter(r);

    if (bp) {
        // grid: horizontal
        bp.setColor(rgba_addContrast(prop_color_background, 0.05));

        bp.drawLine(-1, height() * 0.25, width(), height() * 0.25);
        bp.drawLine(-1, height() * 0.5, width(), height() * 0.5);
        bp.drawLine(-1, height() * 0.75, width(), height() * 0.75);

        // grid: vertical
        const float len_ms = env_.totalLength() / 1000;
        float p = log10f(len_ms);
        float np = 0;
        if (modff(p, &np) < 0.35)
            np -= 1;

        float step = ((powf(10, np)) / len_ms) * width();
        if (step > 3) {
            for (float x = 0; x < width(); x += step) {
                bp.drawLine(x, 0, x, height());
            }
        }

        // labels
        // label top
        txt_value0.setColor(prop_color_border);
        txt_value0.setPos(3, 3);
        txt_value0.set("1.0");
        bp.drawText(txt_value0);

        // draw middle label if has enough space
        if ((height() / zoom()) > 50) {
            txt_value1.setColor(prop_color_border);
            txt_value1.setPos(3, height() * 0.5 - 2);
            txt_value1.set("0.5");
            bp.drawText(txt_value1);
        }

        // label bottom
        txt_value2.setColor(prop_color_border);
        txt_value2.setPos(3, height() - 2);
        txt_value2.set("0.0");
        bp.drawText(txt_value2);
    }
}

void UIEnv::drawEnvelope(const t_rect& r)
{
    UIPainter ep = envelope_layer_.painter(r);

    if (ep) {
        const size_t total = nodes_.size();
        const float z = zoom();

        // draw nodes
        for (size_t i = 0; i < total; i++) {
            const Node& n = nodes_[i];

            if (n.select == SelectType::LINE)
                ep.setColor(LINE_SELECTION_COLOR);
            else
                ep.setColor(prop_line_color);

            // draw segments
            if (i != (total - 1)) { // skip last point
                const Node& next = nodes_[i + 1];

                switch (n.type) {
                case CURVE_LINE:
                    ep.drawLine(n.x * z, n.y * z, next.x * z, next.y * z);
                    break;
                case CURVE_STEP:
                    ep.drawLine(n.x * z, n.y * z, next.x * z, n.y * z);
                    ep.drawLine(next.x * z, n.y * z, next.x * z, next.y * z);
                    break;
                case CURVE_EXP: {
                    ep.moveTo(n.x * z, n.y * z);

                    int i = n.x * z;
                    for (; i < next.x * z; i += 4 * z) {
                        float y = convert::lin2curve(float(i), n.x * z, next.x * z, n.y * z, next.y * z, n.curve);
                        ep.drawLineTo(i, y);
                    }

                    ep.drawLineTo(next.x * z, next.y * z);
                    ep.stroke();

                } break;
                case CURVE_SIN2: {

                    ep.moveTo(n.x * z, n.y * z);

                    int i = n.x * z;
                    for (; i < next.x * z; i += 4 * z) {
                        float y = convert::lin2sin2(i, n.x * z, next.x * z, n.y * z, next.y * z);
                        ep.drawLineTo(i, y);
                    }

                    ep.drawLineTo(next.x * z, next.y * z);
                    ep.stroke();

                } break;
                case CURVE_SIGMOID: {

                    ep.moveTo(n.x * z, n.y * z);

                    int i = n.x * z;
                    for (; i < next.x * z; i += 4 * z) {
                        float y = convert::lin2sigmoid(i, n.x * z, next.x * z,
                            n.y * z, next.y * z, n.sigmoid_skew);
                        ep.drawLineTo(i, y);
                    }

                    ep.drawLineTo(next.x * z, next.y * z);
                    ep.stroke();

                } break;
                default:
                    break;
                }
            }

            // draw selection rectangle
            if (n.select == SelectType::POINT) {
                bool is_inner_node = ((i != 0) && (i != (total - 1)));
                if (delete_mode_ && is_inner_node)
                    ep.setColor(DELETE_COLOR);

                ep.drawRect((n.x - 6) * z, (n.y - 6) * z, 12 * z, 12 * z);
                ep.stroke();
            }

            // draw point itself
            ep.setColor(prop_line_color);
            ep.drawRect((n.x - 3) * z, (n.y - 3) * z, 6 * z, 6 * z);
            ep.fill();

            // draw vertical dash line for stop point
            if (n.is_stop) {
                ep.setDashStyle(EDASHSTYLE_24);
                ep.drawLine(n.x * z, 0, n.x * z, height());
                ep.setDashStyle(EDASHSTYLE_NONE);
            }
        }
    }
}

void UIEnv::addNode(const t_pt& pt)
{
    const float z = zoom();
    const float x_norm = pt.x / z;
    const float y_norm = pt.y / z;

    long insert_idx = 0;
    for (size_t i = 1; i < nodes_.size(); i++) {
        // prevent to close insertion
        if (fabsf(nodes_[i].x - x_norm) < 2)
            return;

        // find node index to insert before
        if (nodes_[i].x > x_norm) {
            insert_idx = i;
            break;
        }
    }

    Node n;
    n.x = x_norm;
    n.y = y_norm;
    n.select = SelectType::POINT;

    // insert new selected node
    nodes_.insert(nodes_.begin() + insert_idx, n);
    redrawLayer(envelope_layer_);
}

long UIEnv::findSelectedNodeIdx() const
{
    auto it = std::find_if(nodes_.begin(), nodes_.end(), [](const Node& n) { return n.select == SelectType::POINT; });
    return (it == nodes_.end()) ? -1 : std::distance(nodes_.begin(), it);
}

void UIEnv::paint()
{
    const t_rect r = rect();

    drawBackground(r);
    drawEnvelope(r);
    drawCursor(r);
}

void UIEnv::okSize(t_rect* newrect)
{
    newrect->w = pd_clip_min(newrect->w, ENV_MIN_WIDTH);
    newrect->h = pd_clip_min(newrect->h, ENV_MIN_HEIGHT);

    updateNodes();
}

void UIEnv::onMouseMove(t_object*, const t_pt& pt, long modifiers)
{
    const float z = zoom();
    const float x_norm = pt.x / z;
    const float y_norm = pt.y / z;

    int node_idx = findNearestNode(x_norm, y_norm);
    if (node_idx >= 0) {
        if (selectNode(node_idx))
            envelope_layer_.invalidate();
    } else {
        if (!hasSelectedEdge()) {
            deselectAll();
            envelope_layer_.invalidate();
        }
    }

    // draw cursor position
    if (modifiers == EMOD_SHIFT) {
        draw_cursor_pos_ = true;
        draw_cursor_cross_ = true;
        cursor_pos_ = pt;
        cursor_layer_.invalidate();
    } else if (modifiers == EMOD_ALT) {
        delete_mode_ = true;
        envelope_layer_.invalidate();
    } else {
        delete_mode_ = false;
        draw_cursor_cross_ = false;
        draw_cursor_pos_ = (node_idx >= 0);
        cursor_layer_.invalidate();

        if (node_idx >= 0) {
            cursor_pos_.x = nodes_[node_idx].x * z;
            cursor_pos_.y = nodes_[node_idx].y * z;
        }
    }

    redraw();
}

void UIEnv::onMouseDrag(t_object*, const t_pt& pt, long mod)
{
    const float z = zoom();
    const float x_norm = pt.x / z;
    const float y_norm = pt.y / z;

    long idx = findSelectedNodeIdx();
    // no selected node
    if (idx < 0)
        return;

    Node& n = nodes_[idx];

    // update coordinates
    if (!n.fixed_x) {
        const auto w_norm = width() / z;
        if (nodes_.isLastNode(n) && x_norm > w_norm) {
            env_.back().utime = env_last_time_ * x_norm / w_norm;
            updateNodes();
            nodes_.back().select = SelectType::POINT;
            bg_layer_.invalidate();
        } else
            n.x = clip<float>(x_norm, 0, w_norm);
    }

    if (!n.fixed_y)
        n.y = clip<float>(y_norm, 0, height() / z);

    // x-coord for any except first and last
    if (nodes_.size() > 1 && idx > 0 && idx < (nodes_.size() - 1)) {
        Node& prev = nodes_[idx - 1];
        Node& next = nodes_[idx + 1];

        if (prev.x > n.x)
            std::swap(n, prev);
        else if (next.x < n.x)
            std::swap(n, next);
    }

    draw_cursor_cross_ = false;
    draw_cursor_pos_ = true;
    cursor_pos_ = pt;

    // no background redraw needed
    envelope_layer_.invalidate();
    cursor_layer_.invalidate();

    redraw();

    if (output_mode_ == sym_drag())
        outputEnvelope();
}

void UIEnv::removeSelectedNode()
{
    auto idx = findSelectedNodeIdx();

    long n = nodes_.size();
    if (idx < 1 || idx >= n - 1)
        return;

    nodes_.erase(nodes_.begin() + idx);
    delete_mode_ = false;
    redrawLayer(envelope_layer_);
}

long UIEnv::findNodeLine(const t_pt& pt)
{
    const float z = zoom();
    const float x_norm = pt.x / z;
    const float y_norm = pt.y / z;

    auto in_between = [](float y, float y0, float y1) {
        auto mm = std::minmax(y0, y1);
        return mm.first <= y && y < mm.second;
    };

    // find selected node line
    for (size_t i = 1; i < nodes_.size(); i++) {
        auto& n0 = nodes_[i - 1];
        auto& n1 = nodes_[i];
        if (in_between(x_norm, n0.x, n1.x) && in_between(y_norm, n0.y, n1.y))
            return i - 1;
    }

    return -1;
}

void UIEnv::deselectAll()
{
    for (auto& n : nodes_) {
        n.select = SelectType::NONE;
    }
}

bool UIEnv::hasSelectedEdge() const
{
    return std::find_if(nodes_.begin(), nodes_.end(), [](const Node& n) { return n.select == SelectType::LINE; }) != nodes_.end();
}

void UIEnv::outputEnvelope()
{
    updateEnvelope();
    atomTo(0, EnvAtom(env_));
}

bool UIEnv::isCmdPressed(long mod)
{
#ifndef __APPLE__
    return (mod & EMOD_CTRL);
#else
    return (mod & EMOD_CMD);
#endif
}

void UIEnv::onMouseDown(t_object*, const t_pt& pt, const t_pt& abs_pt, long mod)
{
    if (mod & EMOD_SHIFT) {
        addNode(pt);
    } else if (mod & EMOD_ALT) {
        removeSelectedNode();
    } else {
        const float z = zoom();
        const float x_norm = pt.x / z;
        const float y_norm = pt.y / z;
        env_last_time_ = env_.back().utime;

        // not a node click
        if (findNearestNode(x_norm, y_norm) < 0) {
            long idx = findNodeLine(pt);

            // reset all others nodes
            if (idx >= 0) {
                // toggle selection
                if (nodes_[idx].select != SelectType::NONE)
                    nodes_[idx].select = SelectType::NONE;
                else
                    nodes_[idx].select = SelectType::LINE;

                for (size_t i = 0; i < nodes_.size(); i++) {
                    if ((long)i == idx)
                        continue;

                    nodes_[i].select = SelectType::NONE;
                }

                redrawLayer(envelope_layer_);
            }
        }
    }

    if (isCmdPressed(mod))
        outputEnvelope();
}

void UIEnv::onMouseLeave(t_object*, const t_pt& pt, long)
{
    draw_cursor_cross_ = false;
    draw_cursor_pos_ = false;
    redrawLayer(cursor_layer_);
}

void UIEnv::onMouseUp(t_object* view, const t_pt& pt, long mod)
{
    if (output_mode_ == sym_mouse_up() && !isCmdPressed(mod))
        outputEnvelope();

    const auto idx = findSelectedNodeIdx();
    if (nodes_.isLastIdx(idx)) {
        updateNodes();
        redrawAll();
    }
}

void UIEnv::onMouseWheel(const t_pt& pt, long mod, float delta)
{
    const float z = zoom();
    long idx = findSelectedNodeIdx();

    // move node
    if (idx >= 0) {
        auto& n = nodes_[idx];
        float k = 0.01 * delta;

        // update coordinates
        if (!n.fixed_y)
            n.y = clip<float>(n.y * (1 + k), 0, height() / z);

        redrawLayer(envelope_layer_);
        return;
    } else {
        for (size_t i = 1; i < nodes_.size(); i++) {
            if (pt.x < nodes_[i].x) {
                idx = (i - 1);
                break;
            }
        }

        if (idx < 0) {
            return;
        }
    }

    switch (nodes_[idx].type) {
    case CURVE_EXP:
        nodes_[idx].curve += (delta < 0) ? -0.2 : 0.2;
        break;
    case CURVE_SIGMOID:
        nodes_[idx].sigmoid_skew += (delta < 0) ? -0.2 : 0.2;
        break;
    default:
        return;
    }

    redrawLayer(envelope_layer_);
}

void UIEnv::showPopup(const t_pt& pt, const t_pt& abs_pt)
{
    auto idx = findSelectedNodeIdx();
    if (idx >= 0) {
        showPopupMenu("point", pt, abs_pt);
        return;
    }

    auto it = std::find_if(nodes_.begin(), nodes_.end(), [](const Node& n) { return n.select == SelectType::LINE; });
    if (it != nodes_.end()) {
        showPopupMenu("line", pt, abs_pt);
        it->select = SelectType::NONE;
    } else {
        showPopupMenu("env", pt, abs_pt);
    }
}

void UIEnv::updateNodes()
{
    nodes_.clear();

    size_t total_us = env_.totalLength();
    size_t n = env_.numPoints();
    const float z = zoom();

    for (size_t i = 0; i < n; i++) {
        // using normalized width() and height()
        nodes_.push_back(Node::fromEnvelope(env_.pointAt(i), total_us, width() / z, height() / z));
    }

    prop_length = total_us / 1000.0;
}

void UIEnv::updateEnvelope()
{
    env_.clear();
    const float z = zoom();

    for (size_t i = 0; i < nodes_.size(); i++) {
        const Node& n = nodes_[i];

        EnvelopePoint pt(((n.x * z) / width()) * prop_length * 1000,
            1 - ((n.y * z) / height()),
            n.is_stop,
            n.type,
            n.curve);

        pt.sigmoid_skew = n.sigmoid_skew;

        if (n.fixed_x)
            pt.setFixTime();
        if (n.fixed_y)
            pt.setFixValue();

        env_.insertPoint(pt);
    }
}

int UIEnv::findNearestNode(float x, float y) const
{
    float min_dist = std::numeric_limits<float>::max();
    int min_idx = -1;

    // find minimal distance
    for (size_t i = 0; i < nodes_.size(); i++) {
        const Node& n = nodes_[i];
        float dist = std::min(min_dist, (n.x - x) * (n.x - x) + (n.y - y) * (n.y - y));

        if (dist < min_dist) {
            min_dist = dist;
            min_idx = i;
        }
    }

    if (min_dist < 36)
        return min_idx;

    return -1;
}

bool UIEnv::selectNode(size_t idx)
{
    int num_changes = 0;

    for (size_t i = 0; i < nodes_.size(); i++) {
        if (i == idx) { // select specified node
            if (nodes_[i].select != SelectType::POINT) {
                nodes_[i].select = SelectType::POINT;
                num_changes++;
            }
        } else if (nodes_[i].select != SelectType::NONE) { // deselect others
            nodes_[i].select = SelectType::NONE;
            num_changes++;
        }
    }

    return num_changes > 0;
}

void UIEnv::m_at(const AtomListView& lv)
{
    using namespace ceammc::parser;

    t_float time_ms = 0;
    UnitTypeFullMatch p;

    if (lv.size() == 1 && lv[0].isFloat()) {
        time_ms = env_.valueAtTime(lv[0].asT<t_float>() * 1000.0);
    } else if (lv.size() == 2 && lv[0].isFloat() && p.parse(lv[1])) {
        const double length = env_.totalLength();
        const double val = lv[0].asT<t_float>();

        switch (p.type()) {
        case TYPE_PHASE:
            time_ms = env_.valueAtTime(val * length);
            break;
        case TYPE_PERCENT:
            time_ms = env_.valueAtTime(val * (length / 100));
            break;
        case TYPE_MSEC:
            time_ms = env_.valueAtTime(val * 1000.0);
            break;
        case TYPE_SEC:
            time_ms = env_.valueAtTime(val * 1000.0 * 1000.0);
            break;
        default:
            UI_ERR << "supported units are: sec, ms(ec), % and *, unknown got: " << lv[1];
            return;
        }
    }

    floatTo(1, time_ms);
}

void UIEnv::setNamedEnvelope(const char* env, const AtomListView& lv)
{
    if (!env_.setNamedEnvelope(env, lv)) {
        UI_ERR << "unknown envelope: " << env << lv;
        return;
    }

    updateNodes();

    redrawAll();
}

void UIEnv::loadPreset(size_t idx)
{
    auto lv = PresetStorage::instance().listValueAt(presetId(), idx);
    DataTypeEnv env;
    if (!env.setFromDataList(lv))
        return;

    env_ = env;
    updateNodes();
    redrawAll();
}

void UIEnv::storePreset(size_t idx)
{
    PresetStorage::instance().setListValueAt(presetId(), idx, AtomList::parseString(env_.toDictString().c_str()));
}

void UIEnv::onPropChange(t_symbol* prop_name)
{
    if (prop_name == sym_length()) {
        if (env_.totalLength() != prop_length * 1000) {
            env_.resizeTime(prop_length * 1000);
            updateNodes();
            redrawAll();
        }
    }

    UIObject::onPropChange(prop_name);
}

void UIEnv::redrawAll()
{
    bg_layer_.invalidate();
    cursor_layer_.invalidate();
    envelope_layer_.invalidate();
    redraw();
}

void UIEnv::setup()
{
    UIObjectFactory<UIEnv> obj("ui.env");
    obj.setDefaultSize(200, 150);

    obj.usePresets();
    obj.useBang();
    obj.usePopup();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_DRAG
        | UI_MOUSE_MOVE | UI_MOUSE_LEAVE
        | UI_MOUSE_WHEEL | UI_MOUSE_UP);
    obj.useData();
    obj.outputMouseEvents(MouseEventsOutput::DEFAULT_OFF);

    obj.addProperty(sym::props::name_active_color, _("Active Color"), DEFAULT_ACTIVE_COLOR, &UIEnv::prop_active_color);
    obj.addProperty("line_color", _("Line Color"), DEFAULT_LINE_COLOR, &UIEnv::prop_line_color);
    obj.addFloatProperty(PROP_LENGTH, _("Length (ms)"), 400, &UIEnv::prop_length, _("Main"));
    obj.setPropertyMin(PROP_LENGTH, 10);
    obj.setPropertyUnits(gensym(PROP_LENGTH), gensym("msec"));
    obj.addBoolProperty(PROP_NORM, _("Normalize input"), true, &UIEnv::prop_normalize, _("Main"));

    obj.addMenuProperty("output_mode", _("Output Mode"), "mouse_up", &UIEnv::output_mode_, "mouse_up drag", _("Main"));

#define ADD_METHOD(name) obj.addMethod(str_##name, &UIEnv::m_##name)

    ADD_METHOD(adsr);
    ADD_METHOD(ar);
    ADD_METHOD(asr);
    ADD_METHOD(eadsr);
    ADD_METHOD(ear);
    ADD_METHOD(easr);
    ADD_METHOD(exp);
    ADD_METHOD(line);
    ADD_METHOD(sigmoid);
    ADD_METHOD(sin2);
    ADD_METHOD(step);

    obj.addMethod(gensym("at"), &UIEnv::m_at);
}

void setup_ui_env()
{
    UIEnv::setup();
}

Node Node::fromEnvelope(const EnvelopePoint& pt, size_t total_us, float w, float h)
{
    Node n;

    n.x = (pt.utime * w) / total_us;
    n.y = h * (1 - pt.value);
    n.curve = pt.data;
    n.sigmoid_skew = pt.sigmoid_skew;
    n.type = pt.type;
    n.select = SelectType::NONE;
    n.is_stop = pt.stop;
    n.fixed_x = pt.fix_pos & EnvelopePoint::FIX_TIME;
    n.fixed_y = pt.fix_pos & EnvelopePoint::FIX_VALUE;

    return n;
}

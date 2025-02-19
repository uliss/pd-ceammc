/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#ifndef BASE_CLONE_H
#define BASE_CLONE_H

#include <cstdint>

#include "ceammc_object.h"
#include "ceammc_proxy.h"
#include "lex/parser_clone.h"
using namespace ceammc;

class CloneInstance {
    t_canvas* canvas_ { nullptr };
    uint16_t idx_;
    bool dsp_on_ { true };

    CloneInstance(const CloneInstance&) = delete;
    CloneInstance& operator=(const CloneInstance&) = delete;

public:
    /**
     * Create new instance
     * @param idx - instance index
     * @param owner - owner canvas
     */
    CloneInstance(uint16_t idx, t_canvas* owner);
    CloneInstance(CloneInstance&& ci);
    ~CloneInstance();

    /**
     * fill instance with given pattern
     * @param pattern - content
     * @param num - total number of instances
     */
    void fillWithPattern(const t_binbuf* pattern, int num);
    void loadbang();

    bool isDspOn() const { return dsp_on_; }
    void dspSet(bool value) { dsp_on_ = value; }
    void dspToggle() { dsp_on_ = !dsp_on_; }

    /**
     * clear instance content
     */
    void clear();

    /**
     * open editor window
     */
    void open();

    /**
     * Instance index
     */
    uint16_t index() const { return idx_; }
    t_canvas* canvas() { return canvas_; }
    const t_canvas* canvas() const { return canvas_; }
};

class BaseClone : public BaseObject {
public:
    enum XletType : uint8_t {
        XLET_CONTROL,
        XLET_AUDIO
    };

    using Proxy = DataProxy<BaseClone, int>;

private:
    IntProperty* num_;
    t_canvas* wrapper_;
    t_canvas* pattern_;
    std::vector<CloneInstance> instances_;
    std::vector<Proxy> proxy_;
    std::uint16_t n_sig_in_;
    std::uint16_t n_sig_out_;
    std::uint16_t n_instance_in_;
    std::uint16_t n_instance_out_;
    size_t pattern_hash_;
    std::vector<std::string> inlet_help_, outlet_help_;
    bool renaming_;

public:
    BaseClone(const PdArgs& args);
    ~BaseClone();

    void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt) override;
    void onAny(t_symbol* s, const AtomListView& lv) final;

    void initDone() override;

    void setupDSP(t_signal** sp);

    void m_open(t_symbol*, const AtomListView& lv);
    void m_menu_open(t_symbol*, const AtomListView& lv);
    void m_send(t_symbol* s, const AtomListView& lv);
    void m_send_spread(t_symbol* s, const AtomListView& lv);
    void m_dsp_set(t_symbol* s, const AtomListView& lv);
    void m_dsp_toggle(t_symbol* s, const AtomListView& lv);
    void m_dsp_spread(t_symbol* s, const AtomListView& lv);

    void data_proxy_bang(const int& i) { bangTo(i); }
    void data_proxy_float(const int& i, t_float f) { floatTo(i, f); }
    void data_proxy_symbol(const int& i, t_symbol* s) { symbolTo(i, s); }
    void data_proxy_list(const int& i, const AtomListView& lv) { listTo(i, lv); }
    void data_proxy_any(const int& i, t_symbol* s, const AtomListView& lv) { anyTo(i, s, lv); }

    void storeContent() const;
    void updateInstances();

    bool changed() const;

    const char* annotateInlet(size_t n) const final;
    const char* annotateOutlet(size_t n) const final;

public:
    virtual void onSave(t_binbuf* b) const;
    virtual void onRestore(const AtomListView& lv);

private:
    bool initInstances();
    bool initInstance(uint16_t idx);

    void updateInlets();
    void updateOutlets();
    void updateXletsInfo();

    void signalInit(t_signal** sp);

    void send(const parser::TargetMessage& msg, const AtomListView& lv);
    static void sendToInlet(t_inlet* inlet, const AtomListView& lv);
    void sendToInstance(uint16_t inst, uint16_t inlet, const AtomListView& lv);
    bool sendToInstanceInlets(int16_t inst, int16_t inlet, const AtomListView& lv);

    void sendSpread(const parser::TargetMessage& msg, const AtomListView& lv);

    void dspSet(const parser::TargetMessage& msg, const AtomListView& lv);
    void dspSetInstance(int16_t idx, bool value);

    void dspToggle(const parser::TargetMessage& msg);
    void dspToggleInstance(int16_t idx);

    void dspSpread(const parser::TargetMessage& msg, const AtomListView& lv);

    uint16_t genRandomInstanceIndex() const;

    /** checks if instance is in [0..NINST) range **/
    bool isValidInstance(int16_t inst) const;
    bool isValidInstanceRange(int16_t a, int16_t b) const;
    bool isValidInlet(int16_t inlet) const;

    size_t numInstances() const { return instances_.size(); }

    struct InstanceRange {
        std::uint16_t a { 0 }, b { 0 }, step { 1 };
        InstanceRange(std::uint16_t a_, std::uint16_t b_, std::uint16_t step_ = 1)
            : a(a_)
            , b(b_)
            , step(step_)
        {
        }
        bool empty() const { return a >= b; }
        bool valid() const { return step > 0; }
    };

    InstanceRange instanceSpreadRange(const parser::TargetMessage& msg, const AtomListView& lv) const;
    InstanceRange instanceRange(const parser::TargetMessage& msg) const;

private:
    // object renaming in Pd is the delete, then create sequence
    // to support object renaming and do not lose the object pattern content
    // we have to store the previous content before destroying the object
    // into this static buffer, restoring it in the object constructor
    static t_binbuf* old_content_;
};

void setup_base_clone();

#endif // BASE_CLONE_H

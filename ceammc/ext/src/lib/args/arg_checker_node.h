/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef ARG_CHECKER_NODE_H
#define ARG_CHECKER_NODE_H

#include <memory>
#include <string>

namespace ceammc {

class AtomListView;
class ArgCheckerNode;

struct CheckResult {
    bool ok;
    size_t matched;
};

using ArgCheckerNodePtr = std::shared_ptr<ArgCheckerNode>;

struct CheckerContext {
    std::string error;
    int error_atom_idx;
};

class ArgCheckerNode {
    size_t min_ = { 0 };
    size_t max_ = { 0 };
    std::string name_;

public:
    ArgCheckerNode();
    ArgCheckerNode(const ArgCheckerNode& c);
    virtual ~ArgCheckerNode() = default;

    virtual bool isLeaf() const = 0;
    virtual bool insertChild(const ArgCheckerNodePtr& node);

    // repeats
    size_t minRepeats() const { return min_; }
    size_t maxRepeats() const { return max_; }
    void setMinRepeats(size_t n) { min_ = n; }
    void setMaxRepeats(size_t n) { max_ = n; }
    void setRepeats(size_t n);
    void setInfinitePattern();

    bool infinitePattern() const;
    bool constRepeat() const { return min_ == max_; }
    bool singleRepeat() const { return min_ == 1 && constRepeat(); }
    bool varRepeat() const { return !constRepeat(); }

    // names
    virtual std::string name() const;
    void setName(const std::string& name) { name_ = name; }
    std::string repeats() const;
    std::string fullName() const;

    // checks
    virtual CheckResult check(const AtomListView& v, CheckerContext& ctx) const = 0;
    virtual CheckResult checkSingle(const AtomListView& v, CheckerContext& ctx) const = 0;
    CheckResult checkRepeats(const AtomListView& v, CheckerContext& ctx) const;
};
}

#endif // ARG_CHECKER_NODE_H

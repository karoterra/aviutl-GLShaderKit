#include "release.hpp"

#include <algorithm>

namespace glshaderkit {

size_t ReleaseContainer::Size() const {
    return targets_.size();
}

bool ReleaseContainer::Contains(IRelease* x) const {
    return std::ranges::find(targets_, x) != targets_.end();
}

void ReleaseContainer::Add(IRelease* x) {
    targets_.push_back(x);
}

void ReleaseContainer::Remove(IRelease* x) {
    std::erase(targets_, x);
}

void ReleaseContainer::ReleaseAll() {
    for (auto target : targets_) {
        target->Release();
    }
    targets_.clear();
}

} // namespace glshaderkit


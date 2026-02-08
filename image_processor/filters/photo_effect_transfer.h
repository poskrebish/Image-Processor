#pragma once
#include "filters.h"

namespace filter {
class PhotoEffectTransfer : public Filter {
public:
    Image Apply(Image& image) override;
};
}  // namespace filter

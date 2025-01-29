#include "adt-flags.hpp"
#include <memory>

namespace adt {
    Flags::Flags() {
        finterface = std::make_unique<FInterface>();
        faudio = std::make_unique<FAudio>();
    }
}
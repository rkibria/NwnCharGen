#include <nwnrace.hpp>

namespace Nwn {

Race::Race(const std::string& n, const std::string& c) :
    name{n},
    classification{c}
{
}

Race::~Race()
{
}

} // namespace Nwn

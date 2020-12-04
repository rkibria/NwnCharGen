#include <Nwn/feat.hpp>

namespace Nwn {

Feat::Feat( int id, const std::string& n ) :
    id{ id },
    name{ n }
{
}

Feat::~Feat()
{
}

} // namespace Nwn

#include <nwnrace.hpp>
#include <nwnablblock.hpp>

namespace Nwn {

Race::Race( const std::string& n, const std::string& c ) :
    name{ n },
    classification{ c },
    ablAdjusts{ std::make_unique<AblBlock>() }
{
}

Race::~Race()
{
}

Race::Race( const Race& rhs ) :
    name{ rhs.name },
    classification{ rhs.classification },
    ablAdjusts{ std::make_unique<AblBlock>() }
{
    *ablAdjusts = *( rhs.ablAdjusts );
}

} // namespace Nwn

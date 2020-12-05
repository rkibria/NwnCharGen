#include <Nwn/race.hpp>
#include <Nwn/ablblock.hpp>

namespace Nwn {

Race::Race( const std::string& n, const std::string& c ) :
    name{ n },
    classification{ c },
    ablAdjusts{ std::make_unique<AblBlock>() },
    feats{ std::make_unique< std::set< int > >() }
{
}

Race::~Race()
{
}

Race::Race( const Race& rhs ) :
    name{ rhs.name },
    classification{ rhs.classification },
    description{ rhs.description },
    ablAdjusts{ std::make_unique<AblBlock>() }
{
    *ablAdjusts = *( rhs.ablAdjusts );
}

AblBlock& Race::getAblAdjusts()
{
    return *ablAdjusts;
}

const AblBlock& Race::getAblAdjusts() const
{
    return *ablAdjusts;
}

} // namespace Nwn

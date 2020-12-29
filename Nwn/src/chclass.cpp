#include <Nwn/chclass.hpp>

namespace Nwn {

ChClass::ChClass( const std::string& n ) :
    name{ n },
    featsPerLvl{ std::make_unique< FeatsPerLevelMap >() }
{
}

ChClass::~ChClass()
{
}

void ChClass::setFeatsPerLvl( std::unique_ptr< FeatsPerLevelMap > fpl )
{
    featsPerLvl = std::move( fpl );
}

const FeatsPerLevelMap& ChClass::getFeatsPerLvl() const
{
    return *featsPerLvl;
}

void ChClass::setBonusFeats( std::unique_ptr< std::vector< bool > > bf )
{
    bonusFeats = std::move( bf );
}

const std::vector< bool >& ChClass::getBonusFeats() const
{
    return *bonusFeats;
}

} // namespace Nwn

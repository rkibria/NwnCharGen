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

bool Feat::hasColumn(const std::string& col) const
{
    return (columns.find(col) != columns.end());
}

int Feat::getColumn(const std::string& col) const
{
    return columns.at(col);
}

void Feat::setColumn(const std::string& col, int val)
{
    columns[col] = val;
}

void Feat::addEffect(FeatEffectType type, int val)
{
    effects[type] = val;
}

bool Feat::hasEffect(FeatEffectType type) const
{
    return (effects.find( type ) != effects.end());
}

int Feat::getEffect(FeatEffectType type) const
{
    return effects.at( type );
}

} // namespace Nwn

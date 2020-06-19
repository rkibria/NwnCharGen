#include <Nwn/ablblock.hpp>
#include <algorithm>

namespace Nwn {

AblBlock::AblBlock( int init ) :
    abls{ init, init, init, init, init, init }
{
}

AblBlock::~AblBlock()
{
}

void AblBlock::setAbl( AblScore abl, int value )
{
    abls[ static_cast<int>( abl ) ] = value;
}

int AblBlock::getAbl( AblScore abl ) const
{
    return abls[ static_cast<int>( abl ) ];
}

int AblBlock::getAblMod( AblScore abl ) const
{
    const auto score = getAbl( abl );
    auto mod = ( score / 2 ) - 5;
    return mod;
}

AblBlock& AblBlock::add( const AblBlock& rhs )
{
    std::transform( abls.begin(), abls.end(), rhs.abls.begin(), abls.begin(), std::plus<int>() );
    return *this;
}

} // namespace Nwn

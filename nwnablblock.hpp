#ifndef NWNABLBLOCK_H
#define NWNABLBLOCK_H

#include <nwnbase.hpp>
#include <array>

namespace Nwn {

class AblBlock
{
public:
    explicit AblBlock( int init = 0 );
    ~AblBlock();

    void setAbl( AblScore, int );
    int getAbl( AblScore ) const;
    int getAblMod( AblScore ) const;

private:
    std::array< int, 6 > abls;
};

} // namespace Nwn

#endif // NWNABLBLOCK_H

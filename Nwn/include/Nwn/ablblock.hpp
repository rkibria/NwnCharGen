#ifndef NWNABLBLOCK_H
#define NWNABLBLOCK_H

#include <Nwn/base.hpp>
#include <array>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/array.hpp>

namespace Nwn {

class AblBlock
{
public:
    explicit AblBlock( int init = 0 );
    ~AblBlock();

    void setAbl( AblScore, int );
    int getAbl( AblScore ) const;
    int getAblMod( AblScore ) const;
    AblBlock& add( const AblBlock& rhs );

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ )
    {
        ar & boost::serialization::make_nvp( "ablblock", abls );
    }

    std::array< int, 6 > abls;
};

} // namespace Nwn

#endif // NWNABLBLOCK_H

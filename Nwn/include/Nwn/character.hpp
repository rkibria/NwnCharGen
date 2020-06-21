#ifndef NWNCHAR_H
#define NWNCHAR_H

#include <Nwn/base.hpp>

#include <array>
#include <string>
#include <memory>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/unique_ptr.hpp>

namespace Nwn {

class AblBlock;

class Character
{
public:
    explicit Character();
    ~Character();

    void setName( const std::string n ) { name = n; }
    const std::string& getName() const { return name; }

    void setDescription( const std::string d ) { description = d; }
    const std::string& getDescription() const { return description; }

    void setAlignment( const Alignment a ) { alignment = a; }
    Alignment getAlignment() const { return alignment; }

    /* Abilities  */
    AblBlock& getAbls() { return *abls; }
    const AblBlock& getAbls() const { return *abls; }

    int getPointsRemain() const { return ablPointsRemain; }
    void decAbl( AblScore );
    void incAbl( AblScore );

    /* Race */
    std::string getRace() const { return race; }
    void setRace( const std::string& r ) { race = r; }

    /* Serialization */
    void save( const char* fileName ) const;
    void restore( const char* fileName );

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ )
    {
        ar & boost::serialization::make_nvp( "name", name )
           & boost::serialization::make_nvp( "description", description )
           & boost::serialization::make_nvp( "race", race )
           & boost::serialization::make_nvp( "alignment", alignment )
           & boost::serialization::make_nvp( "ablPointsRemain", ablPointsRemain )
           & boost::serialization::make_nvp( "abls", abls );
    }

    int incCost( int curScore ) const;

    static const int ablPointBuy;
    static const int minAblScore;
    static const int maxAblScore;

    std::string name;
    std::string description;
    std::string race;
    Alignment alignment;

    int ablPointsRemain;
    std::unique_ptr< AblBlock > abls;
};

} // namespace Nwn

#endif // NWNCHAR_H

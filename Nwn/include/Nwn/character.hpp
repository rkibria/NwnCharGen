#ifndef NWNCHAR_H
#define NWNCHAR_H

#include <Nwn/base.hpp>

#include <array>
#include <vector>
#include <string>
#include <memory>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

namespace Nwn {

class AblBlock;

class Character
{
public:
    explicit Character();
    ~Character() noexcept;

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

    AblScore getAblInc( int n ) const;
    void setAblInc( int n, AblScore abl );

    /* Race */
    std::string getRace() const { return race; }
    void setRace( const std::string& r ) { race = r; }

    /* Serialization */
    void save( const char* fileName ) const;
    void restore( const char* fileName );

    /* Levels */
    constexpr static const int maxLevel = 30;

    int getNumLevels() const { return static_cast<int>( levels.size() ); }
    void pushLevel( const std::string& chclass );
    void popLevel();
    const std::string& getLevel( int lvl ) const;
    void setLevel( int lvl, const std::string& chclass );

    std::map< std::string, int > getChClassCountsAtLvl( int lvl ) const;

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
           & boost::serialization::make_nvp( "abls", abls )
           & boost::serialization::make_nvp( "levels", levels )
           & boost::serialization::make_nvp( "ablIncs", ablIncs );
    }

    int incCost( int curScore ) const;

    constexpr static const int ablPointBuy = 32;
    constexpr static const int minAblScore = 8;
    constexpr static const int maxAblScore = 18;

    std::string name;
    std::string description;
    std::string race;
    Alignment alignment = Alignment::LawfulGood;
    int ablPointsRemain = ablPointBuy;
    std::unique_ptr< AblBlock > abls;
    std::vector< std::string > levels;
    std::array< AblScore, 7 > ablIncs;
};

} // namespace Nwn

#endif // NWNCHAR_H

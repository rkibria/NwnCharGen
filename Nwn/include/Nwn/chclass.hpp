#ifndef NWNCHCLASS_H
#define NWNCHCLASS_H

#include <Nwn/base.hpp>
#include <string>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>

namespace Nwn {

class ChClass
{
public:
    explicit ChClass( const std::string& n = "" );
    ~ChClass();
    ChClass( const ChClass& );

    const std::string& getName() const { return name; }
    void setName( const std::string& n ) { name = n; }

    const std::string& getDescription() const { return description; }
    void setDescription( const std::string& d ) { description = d; }

    Dice getHitDie() const { return hitDie; }
    void setHitDie( Dice d ) { hitDie = d; }

    BabProgression getBabProgression() const { return babProgression; }
    void setBabProgression( BabProgression prg ) { babProgression = prg; }

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ ) {
        ar & boost::serialization::make_nvp( "name", name )
           & boost::serialization::make_nvp( "description", description )
           & boost::serialization::make_nvp( "hitDie", hitDie )
           & boost::serialization::make_nvp( "babProgression", babProgression );
    }

    std::string name;
    std::string description;
    Dice hitDie{ Dice::d4 };
    BabProgression babProgression{ BabProgression::low };
};

} // namespace Nwn

#endif // NWNCHCLASS_H

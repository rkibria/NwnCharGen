#ifndef NWNRACE_H
#define NWNRACE_H

#include <Nwn/base.hpp>
#include <string>
#include <memory>
#include <set>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/set.hpp>

namespace Nwn {

class AblBlock;

class Race
{
public:
    explicit Race( const std::string& n = "", const std::string& c = "" );
    ~Race();
    Race( const Race& );

    const std::string& getName() const { return name; }
    void setName( const std::string& n ) { name = n; }

    const std::string& getClassification() const { return classification; }
    void setClassification( const std::string& c ) { classification = c; }

    const std::string& getDescription() const { return description; }
    void setDescription( const std::string& d ) { description = d; }

    AblBlock& getAblAdjusts();
    const AblBlock& getAblAdjusts() const;

    const std::set<int>& getFeats() const { return *feats; }
    void setFeats( std::unique_ptr< std::set<int> > f ) { feats = std::move( f ); }

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ )
    {
        ar & boost::serialization::make_nvp( "name", name )
           & boost::serialization::make_nvp( "classification", classification )
           & boost::serialization::make_nvp( "description", description )
           & boost::serialization::make_nvp( "ablAdjusts", ablAdjusts )
           & boost::serialization::make_nvp( "feats", feats );
    }

    std::string name;
    std::string classification;
    std::string description;

    std::unique_ptr< AblBlock > ablAdjusts;

    std::unique_ptr< std::set<int> > feats;
};

} // namespace Nwn

#endif // NWNRACE_H

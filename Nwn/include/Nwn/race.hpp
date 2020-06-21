#ifndef NWNRACE_H
#define NWNRACE_H

#include <Nwn/base.hpp>
#include <string>
#include <memory>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/unique_ptr.hpp>

namespace Nwn {

class AblBlock;

class Race
{
public:
    explicit Race( const std::string& n = "", const std::string& c = "" );
    ~Race();
    Race( const Race& );

    const std::string& getName() const { return name; }
    const std::string& getClassification() const { return classification; }
    const std::string& getDescription() const { return description; }

    void setDescription( const std::string& d ) { description = d; }

    AblBlock& getAblAdjusts();
    const AblBlock& getAblAdjusts() const;

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ )
    {
        ar & boost::serialization::make_nvp( "name", name )
           & boost::serialization::make_nvp( "classification", classification )
           & boost::serialization::make_nvp( "description", description )
           & boost::serialization::make_nvp( "ablAdjusts", ablAdjusts );
    }

    std::string name;
    std::string classification;
    std::string description;

    std::unique_ptr<AblBlock> ablAdjusts;
};

} // namespace Nwn

#endif // NWNRACE_H

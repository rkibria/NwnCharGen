#ifndef NWNFEAT_H
#define NWNFEAT_H

#include <Nwn/base.hpp>
#include <string>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/unique_ptr.hpp>

namespace Nwn {

class AblBlock;

class Feat
{
public:
    explicit Feat( int id = -1, const std::string& n = "" );
    ~Feat();

    int getId() const { return id; }
    void setId( int id ) { this->id = id; }

    const std::string& getName() const { return name; }
    void setName( const std::string& n ) { name = n; }

    const std::string& getDescription() const { return description; }
    void setDescription( const std::string& d ) { description = d; }

    void setAllClassesCanUse(bool e) { allClassesCanUse = e; }
    const bool getAllClassesCanUse() const { return allClassesCanUse; }

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ )
    {
        ar & boost::serialization::make_nvp( "id", id )
           & boost::serialization::make_nvp( "name", name )
           & boost::serialization::make_nvp( "description", description )
           & boost::serialization::make_nvp( "allClassesCanUse", allClassesCanUse );
    }

    int id = -1;
    std::string name;
    std::string description;
    bool allClassesCanUse = false;
};

} // namespace Nwn

#endif // NWNFEAT_H

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
    explicit Feat( int id = 0, const std::string& n = "" );
    ~Feat();

    int getId() const { return id; }
    void setId( int id ) { this->id = id; }

    const std::string& getName() const { return name; }
    void setName( const std::string& n ) { name = n; }

    const std::string& getDescription() const { return description; }
    void setDescription( const std::string& d ) { description = d; }

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ )
    {
        ar & boost::serialization::make_nvp( "id", id )
           & boost::serialization::make_nvp( "name", name )
           & boost::serialization::make_nvp( "description", description );
    }

    int id;
    std::string name;
    std::string description;
};

} // namespace Nwn

#endif // NWNFEAT_H

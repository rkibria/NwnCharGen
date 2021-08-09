#ifndef NWNFEAT_H
#define NWNFEAT_H

#include <Nwn/base.hpp>
#include <string>
#include <unordered_map>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/unordered_map.hpp>

namespace Nwn {

static constexpr int INVALID_FEAT_ID = -1;

/// Gives bonus feat at lvl 1, apparently hardcoded in the game engine
static constexpr int FEAT_ID_QUICK_TO_MASTER = 258;

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

    bool hasColumn(const std::string& col) const;
    int getColumn(const std::string& col) const;
    void setColumn(const std::string& col, int val);

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int /* file_version */ )
    {
        ar & boost::serialization::make_nvp( "id", id )
           & boost::serialization::make_nvp( "name", name )
           & boost::serialization::make_nvp( "description", description )
           & boost::serialization::make_nvp( "columns", columns );
    }

    int id = INVALID_FEAT_ID;
    std::string name;
    std::string description;
    std::unordered_map<std::string, int> columns;
};

} // namespace Nwn

#endif // NWNFEAT_H

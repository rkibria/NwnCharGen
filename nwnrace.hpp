#ifndef NWNRACE_H
#define NWNRACE_H

#include <nwnbase.hpp>
#include <string>
#include <memory>

namespace Nwn {

class AblBlock;

class Race
{
public:
    explicit Race( const std::string& n, const std::string& c );
    ~Race();
    Race( const Race& );

    const std::string& getName() const { return name; }
    const std::string& getClassification() const { return classification; }
    const std::string& getDescription() const { return description; }

    void setDescription( const std::string& d ) { description = d; }

private:
    std::string name;
    std::string classification;
    std::string description;

    std::unique_ptr<AblBlock> ablAdjusts;
};

} // namespace Nwn

#endif // NWNRACE_H

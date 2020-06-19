#ifndef NWNRACE_H
#define NWNRACE_H

#include <nwnbase.hpp>
#include <string>

namespace Nwn {

class Race
{
public:
    explicit Race(const std::string& n, const std::string& c);
    ~Race();

    const std::string& getName() const {return name;}
    const std::string& getClassification() const {return classification;}
    const std::string& getDescription() const {return description;}

    void setDescription(const std::string& d) { description = d; }

private:
    std::string name;
    std::string classification;
    std::string description;
};

} // namespace Nwn

#endif // NWNRACE_H

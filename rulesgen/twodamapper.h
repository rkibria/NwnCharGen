#ifndef TWODAMAPPER_H
#define TWODAMAPPER_H

#include <vector>
#include <string>
#include <unordered_map>

struct TwoDAMapValue
{
    /// Containing zip file
    std::string zipFile;
    /// Relative path inside containing zip file
    std::string zipPath;
    /// Path where extracted, empty if not yet extracted
    std::string extractedPath;
};

class TwoDAMapper
{
public:
    TwoDAMapper();
    ~TwoDAMapper();

    void initialize( const std::string& nwn2Path );

private:
    std::unordered_map< std::string, TwoDAMapValue > extractMap;
};

#endif // TWODAMAPPER_H

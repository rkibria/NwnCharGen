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

/// Provides automatically unpacking access to the base game 2da files
class TwoDAMapper
{
public:
    TwoDAMapper();
    ~TwoDAMapper();

    void initialize( const std::string& nwn2Path );

    /// Returns full path to 2da file of that name to load
    const std::string& getFile( const std::string& twoDAname );

private:
    /// 2da file name without .2da extension -> map value
    std::unordered_map< std::string, TwoDAMapValue > extractMap;
};

#endif // TWODAMAPPER_H

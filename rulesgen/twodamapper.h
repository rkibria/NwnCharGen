#ifndef TWODAMAPPER_H
#define TWODAMAPPER_H

#include <vector>
#include <string>
#include <unordered_map>

enum class ExtractionMethod
{
    unzip,
    stored
};

struct TwoDAMapValue
{
    /// How the data can be retrieved
    ExtractionMethod extMtd{ ExtractionMethod::unzip };
    /// Containing zip file
    std::string zipFile;
    /// Relative path inside containing zip file
    std::string zipPath;
    /// Path where extracted, empty if not yet extracted
    std::string extractedPath;
    /// Storage for 2da file contents
    std::vector<char> text;
};

/// Provides automatically unpacking access to the base game 2da files
class TwoDAMapper
{
public:
    TwoDAMapper( const std::string& nwn2Path, const std::string& outputPath );
    ~TwoDAMapper();

    /// Returns full path to 2da file of that name to load
    const std::string& getFile( const std::string& twoDAname );

    /// Load all .2da files from a hak file
    void readHak( const std::string& hakPath );

private:
    void initialize();

    std::string nwn2Path;
    std::string outputPath;

    /// 2da file name without .2da extension -> map value
    std::unordered_map< std::string, TwoDAMapValue > extractMap;
};

#endif // TWODAMAPPER_H

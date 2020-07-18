#include "twodamapper.h"

#include <iostream>
#include <array>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <unziphelper/unziphelper.h>

TwoDAMapper::TwoDAMapper()
{

}

TwoDAMapper::~TwoDAMapper()
{
}

void TwoDAMapper::initialize( const std::string& nwn2Path )
{
    const std::array< std::string, 3 > twoDAzips = {
        nwn2Path + "\\Data\\2DA.zip",
        nwn2Path + "\\Data\\2DA_X1.zip",
        nwn2Path + "\\Data\\2DA_X2.zip"
    };

    for( const auto& zipPath : twoDAzips ) {
        const auto twoDAfiles = UnzipHelper::getFileList( zipPath.c_str() );
        for( const auto& path : twoDAfiles ) {
            if( path.empty() || path.back() == '/' ) {
                continue;
            }

            const auto lowerPath = boost::to_lower_copy( path );
            if( ! boost::algorithm::ends_with( lowerPath, ".2da" ) ) {
                continue;
            }

            std::vector<std::string> tokens;
            boost::split( tokens, lowerPath, boost::is_any_of("/.") );
            assert( tokens.size() == 3 );
            const auto& twoDAkey = tokens[ 1 ];
            extractMap[ twoDAkey ] = TwoDAMapValue{ zipPath, path, "" };
        }
    }
}

const std::string& TwoDAMapper::getFile( const std::string& twoDAname )
{
    assert( extractMap.find( twoDAname ) != extractMap.end() );
    auto& mapEntry = extractMap.at( twoDAname );
    if( mapEntry.extractedPath.empty() ) {

    }

    return mapEntry.extractedPath;
}
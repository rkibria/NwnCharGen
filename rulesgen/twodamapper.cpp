#include "twodamapper.h"

#include <iostream>
#include <array>
#include <vector>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <unziphelper/unziphelper.h>

#include <Precomp.h>
#include <ErfFileReader.h>

using ErfFileReader32 = ErfFileReader< NWN::ResRef32 >;

TwoDAMapper::TwoDAMapper( const std::string& nwn2Path_, const std::string& outputPath_ )
    : nwn2Path{ nwn2Path_ },
      outputPath{ outputPath_ }
{
    initialize();
}

TwoDAMapper::~TwoDAMapper()
{
}

void TwoDAMapper::initialize()
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
            extractMap[ twoDAkey ] = TwoDAMapValue{ ExtractionMethod::unzip, zipPath, path, "", {} };
        }
    }
}

const std::string& TwoDAMapper::getFile( const std::string& twoDAname )
{
    assert( extractMap.find( twoDAname ) != extractMap.end() );
    auto& mapEntry = extractMap.at( twoDAname );
    if( mapEntry.extractedPath.empty() ) {
        switch (mapEntry.extMtd) {
        case ExtractionMethod::unzip:
            UnzipHelper::extract( mapEntry.zipFile.c_str(), mapEntry.zipPath.c_str(), outputPath );
            mapEntry.extractedPath = outputPath + "\\" + twoDAname + ".2da";
            break;
        case ExtractionMethod::stored: {
                mapEntry.extractedPath = outputPath + "\\" + twoDAname + ".2da";
                std::ofstream outFile( mapEntry.extractedPath );
                outFile.write( mapEntry.text.data(), mapEntry.text.size() );
            }
            break;
        default:
            break;
        }
    }

    return mapEntry.extractedPath;
}

void TwoDAMapper::readHak( const std::string& hakPath )
{
    ErfFileReader32 hakFile( hakPath.c_str() );

    const auto count = hakFile.GetEncapsulatedFileCount();
    NWN::ResRef32 resRef;
    ResType  resType;

    for( FileId i = 0; i < count; ++i ) {
        const auto ok = hakFile.GetEncapsulatedFileEntry( i, resRef, resType );
        assert(ok);

        if( resType != NWN::Res2DA )
            continue;

        const auto hndl = hakFile.OpenFile( resRef, resType );
        const auto fSize = hakFile.GetEncapsulatedFileSize( hndl );
        std::vector<char> buf( fSize );
        size_t bytesRead;
        const auto readOk = hakFile.ReadEncapsulatedFile( hndl, 0, fSize, &bytesRead, buf.data() );
        assert( readOk && bytesRead == fSize );
        const auto closeOk = hakFile.CloseFile( hndl );
        assert( closeOk );

        const std::string twoDAkey = boost::to_lower_copy( std::string( resRef.RefStr ) );
        extractMap[ twoDAkey ] = TwoDAMapValue{ ExtractionMethod::stored, "", "", "", std::move( buf ) };

        std::cout << "read 2da in hak: " << twoDAkey << "\n";
    }

}

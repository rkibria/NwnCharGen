#include "twodamapper.h"

#include <iostream>
#include <unziphelper/unziphelper.h>

TwoDAMapper::TwoDAMapper()
{

}

TwoDAMapper::~TwoDAMapper()
{
}

void TwoDAMapper::initialize( const std::string& nwn2Path )
{
    const auto twoDAfiles = UnzipHelper::getFileList( ( nwn2Path + "\\Data\\2DA_X2.zip" ).c_str() );
    for( const auto& path : twoDAfiles ) {
        if( !path.empty() && path.back() != '/' ) {
            std::cout << path << std::endl;
        }
    }


}

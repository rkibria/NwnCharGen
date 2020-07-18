#ifndef UNZIPHELPER_H
#define UNZIPHELPER_H

#include <vector>
#include <string>

namespace UnzipHelper {

int extract( const char *zipfilename, const char *filename_to_extract );

std::vector<std::string> getFileList( const char *zipfilename );

}

#endif // UNZIPHELPER_H

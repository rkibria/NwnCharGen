#ifndef TWODAMAPPER_H
#define TWODAMAPPER_H

#include <vector>
#include <string>

class TwoDAMapper
{
public:
    TwoDAMapper();
    ~TwoDAMapper();

    void initialize( const std::string& nwn2Path );
};

#endif // TWODAMAPPER_H

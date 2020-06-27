#ifndef CONSTMAPLOOPER_H
#define CONSTMAPLOOPER_H

/**
 * Adapter for looping over unique_ptr values of maps
 * */

template< class VALUE_TYPE, class CONTAINER_TYPE, class ITR_TYPE >
class ConstMapLoopItr
{
public:
    explicit ConstMapLoopItr( ITR_TYPE i ) :
      itr{ i }
    {}
    const VALUE_TYPE& operator*() const { return *( (*itr).second ); }
    bool operator!=( const ConstMapLoopItr &b ) const { return itr != b.itr; }
    ConstMapLoopItr& operator++() {
        ++itr;
        return *this;
    }
private:
    ITR_TYPE itr;
};

template< class VALUE_TYPE, class CONTAINER_TYPE >
class ConstMapLooper
{
public:
    explicit ConstMapLooper( const CONTAINER_TYPE& cntnr ) :
      _container{ cntnr }
    {}
    ConstMapLoopItr< VALUE_TYPE, CONTAINER_TYPE, typename CONTAINER_TYPE::const_iterator > begin() {
        return ConstMapLoopItr< VALUE_TYPE, CONTAINER_TYPE, typename CONTAINER_TYPE::const_iterator >( _container.cbegin() );
    }
    ConstMapLoopItr< VALUE_TYPE, CONTAINER_TYPE, typename CONTAINER_TYPE::const_iterator > end() {
        return ConstMapLoopItr< VALUE_TYPE, CONTAINER_TYPE, typename CONTAINER_TYPE::const_iterator >( _container.cend() );
    }
private:
    const CONTAINER_TYPE& _container;
};

#endif // CONSTMAPLOOPER_H

#if !defined(CONCEPT__POINTER__CCT__)
#define CONCEPT__POINTER__CCT__

#include <type_traits>
#include <utility>

namespace cct {

template<typename T>
class ConceptPointer {
    T data_;
    typedef typename std::remove_reference<T>::type type__;
    typedef typename std::remove_const<type__>::type type_;
public:
    ConceptPointer() =default;
    ConceptPointer( const type_ & v ):data_( const_cast<type_ &>(v) ) {}
    ConceptPointer( type_ && v ):data_( std::move(v) ) {}
    ConceptPointer(const ConceptPointer & )=default;
    ConceptPointer(ConceptPointer &&)=default;
    template<typename T0,typename ... Args >
    ConceptPointer(T0 && t0,Args && ... args):data_( std::forward<T0>(t0),std::forward<Args>(args)... ) {}
    ConceptPointer&operator=(const ConceptPointer &)=default;
    ConceptPointer&operator=(ConceptPointer &&)=default;
    auto * operator->() { return &data_; }
    const type_ * operator->() const{ return &data_; }
    auto & operator*() { return data_; }
    const auto & operator*()const { return data_; }
    const void * hash_pointer() const { return &data_; }
    const volatile void * hash_pointer() const volatile { return &data_; }
};

template<typename T>
class ConceptPointer<T &> {
    T & data_;
    typedef typename std::remove_reference<T>::type type__;
    typedef typename std::remove_const<type__>::type type_;
public:
    ConceptPointer( const type_ & v ):data_( const_cast<type_ &>(v) ) {}
    ConceptPointer(type_ && v ):data_( v ) {}
    ConceptPointer(const ConceptPointer & )=default;
    ConceptPointer(ConceptPointer &&)=default;
    ConceptPointer&operator=(const ConceptPointer &)=delete;
    ConceptPointer&operator=(ConceptPointer &&)=delete;
    auto * operator->() { return &data_; }
    const type_ * operator->() const{ return &data_; }
    auto & operator*() { return data_; }
    const auto & operator*()const { return data_; }
    const void * hash_pointer() const { return &data_; }
    const volatile void * hash_pointer() const volatile { return &data_; }
};

template<typename T>
class ConceptPointer<T *> {
    T * data_ ;
    typedef typename std::remove_reference< T >::type type__;
    typedef typename std::remove_const<type__>::type type_;
public:
    ConceptPointer(  ):data_(nullptr) {}
    ConceptPointer( type_ * const & d):data_(d) {}
    ConceptPointer( const type_ * const & d):data_( const_cast<type_ * const &>( d ) ) {}
    ConceptPointer( const type_ & v ):data_( &(const_cast<type_ &>(v)) ) {}
    ConceptPointer( type_ && v ):data_( &v ) {}
    ConceptPointer( const ConceptPointer & )=default;
    ConceptPointer( ConceptPointer &&)=default;
    ConceptPointer&operator=(const ConceptPointer &)=default;
    ConceptPointer&operator=(ConceptPointer &&)=default;
    auto * operator->() { return data_; }
    const type_ * operator->() const{ return data_; }
    auto & operator*() { return *data_; }
    const auto & operator*()const { return *data_; }
    const void * hash_pointer() const { return data_; }
    const volatile void * hash_pointer() const volatile { return data_; }
};

template<typename T>
class ConceptPointer<T * &> : public ConceptPointer<T *> {
    typedef ConceptPointer<T *> U;
public:
    using U::U;
};

template<typename T>
class ConceptPointer<T * &&> : public ConceptPointer<T *> {
    typedef ConceptPointer<T *> U;
public:
    using U::U;
};

template<typename T>
class ConceptPointer<T &&> {
    T && data_;
    typedef typename std::remove_reference<T>::type type__;
    typedef typename std::remove_const<type__>::type type_;
public:
    ConceptPointer(type_ && v ):data_( v ) {}
    ConceptPointer(const type_ && v ):data_( const_cast<type_ &&>( v ) ) {}
    ConceptPointer(const ConceptPointer & )=delete;
    ConceptPointer(ConceptPointer &&)=default;
    ConceptPointer&operator=(const ConceptPointer &)=delete;
    ConceptPointer&operator=(ConceptPointer &&)=default;
    auto * operator->() { return &data_; }
    const type_ * operator->() const{ return &data_; }
    auto && operator*() { return std::move( data_ ); }
    const auto && operator*()const { return std::move(data_); }
    const void * hash_pointer() const { return &data_; }
    const volatile void * hash_pointer() const volatile { return &data_; }
};

template<typename T,typename U>
bool operator<(const ConceptPointer<T> & l,const ConceptPointer<U> & r) { return l.hash_pointer()<r.hash_pointer(); }
template<typename T,typename U>
bool operator==(const ConceptPointer<T> & l,const ConceptPointer<U> & r) { return l.hash_pointer()==r.hash_pointer(); }
template<typename T,typename U>
bool operator!=(const ConceptPointer<T> & l,const ConceptPointer<U> & r) { return l.hash_pointer()!=r.hash_pointer(); }
template<typename T,typename U>
bool operator>(const ConceptPointer<T> & l,const ConceptPointer<U> & r) { return l.hash_pointer()>r.hash_pointer(); }
template<typename T,typename U>
bool operator<=(const ConceptPointer<T> & l,const ConceptPointer<U> & r) { return l.hash_pointer()<=r.hash_pointer(); }
template<typename T,typename U>
bool operator>=(const ConceptPointer<T> & l,const ConceptPointer<U> & r) { return l.hash_pointer()>=r.hash_pointer(); }

}

#endif

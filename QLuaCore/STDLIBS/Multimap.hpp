
#if !defined(MULTIMAP__HPP__CCT)
#define MULTIMAP__HPP__CCT

#include <map>
#include <memory>

namespace cct{

    template<typename T,typename U>
    class Multimap : public std::shared_ptr< std::multimap<T,U> > {
    private:
        typedef std::shared_ptr< std::multimap<T,U> > Super;
    public:

        Multimap( decltype(nullptr) ) {}
        Multimap() :Super(new std::multimap<T,U> ){}
        Multimap(const Multimap &)=default;
        Multimap(Multimap &&)=default;
        Multimap(Super && o):Super( std::move(o) ) {}
        Multimap(const Super & o):Super( o ) {}
        Multimap(const std::multimap<T,U> & o):Super( new std::multimap<T,U>( o ) ) {}
        Multimap(std::multimap<T,U> && o):Super( new std::multimap<T,U>( std::move(o) ) ) {}

        Multimap&operator=(const Multimap&)=default;
        Multimap&operator=(Multimap&&)=default;

        Multimap copy() const { return Multimap( *(*this) ); }
        Multimap unique_copy() const { if (this->use_count()<2) { return *this; }return copy(); }
    private:
        typedef std::multimap<T,U> AssociativeContainer;
        AssociativeContainer * data_() { return this->get(); }
        const AssociativeContainer * data_()const { return this->get(); }
    public:
        typedef typename AssociativeContainer::key_type key_type;
        typedef typename AssociativeContainer::mapped_type mapped_type;
        typedef typename AssociativeContainer::const_iterator const_iterator;
        auto begin()const { return data_()->begin(); }
        auto end()const { return data_()->end(); }
        auto begin() { return data_()->begin(); }
        auto end() { return data_()->end(); }
        auto find(const key_type & v) const { return data_()->find(v); }
    };

}

#endif



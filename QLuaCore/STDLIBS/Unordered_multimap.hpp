
#if !defined(UNORDERED_MULTIMAP__HPP__CCT)
#define UNORDERED_MULTIMAP__HPP__CCT

#include <unordered_map>
#include <memory>

namespace cct{

    template<typename T,typename U>
    class Unordered_multimap : public std::shared_ptr< std::unordered_multimap<T,U> > {
    private:
        typedef std::shared_ptr< std::unordered_multimap<T,U> > Super;
    public:

        Unordered_multimap( decltype(nullptr) ) {}
        Unordered_multimap() :Super(new std::unordered_multimap<T,U> ){}
        Unordered_multimap(const Unordered_multimap &)=default;
        Unordered_multimap(Unordered_multimap &&)=default;
        Unordered_multimap(Super && o):Super( std::move(o) ) {}
        Unordered_multimap(const Super & o):Super( o ) {}
        Unordered_multimap(const std::unordered_multimap<T,U> & o):Super( new std::unordered_multimap<T,U>( o ) ) {}
        Unordered_multimap(std::unordered_multimap<T,U> && o):Super( new std::unordered_multimap<T,U>( std::move(o) ) ) {}

        Unordered_multimap&operator=(const Unordered_multimap&)=default;
        Unordered_multimap&operator=(Unordered_multimap&&)=default;

        Unordered_multimap copy() const { return Unordered_multimap( *(*this) ); }
        Unordered_multimap unique_copy() const { if (this->use_count()<2) { return *this; }return copy(); }
    private:
        typedef std::unordered_multimap<T,U> AssociativeContainer;
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



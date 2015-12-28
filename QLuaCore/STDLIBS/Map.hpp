
#if !defined(MAP__HPP__CCT)
#define MAP__HPP__CCT

#include <map>
#include <memory>

namespace cct{

    template<typename T,typename U>
    class Map : public std::shared_ptr< std::map<T,U> > {
    private:
        typedef std::shared_ptr< std::map<T,U> > Super;
    public:

        Map( decltype(nullptr) ) {}
        Map() :Super(new std::map<T,U> ){}
        Map(const Map &)=default;
        Map(Map &&)=default;
        Map(Super && o):Super( std::move(o) ) {}
        Map(const Super & o):Super( o ) {}
        Map(const std::map<T,U> & o):Super( new std::map<T,U>( o ) ) {}
        Map(std::map<T,U> && o):Super( new std::map<T,U>( std::move(o) ) ) {}

        Map&operator=(const Map&)=default;
        Map&operator=(Map&&)=default;

        Map copy() const { return Map( *(*this) ); }
        Map unique_copy() const { if (this->use_count()<2) { return *this; }return copy(); }
    private:
        typedef std::map<T,U> AssociativeContainer;
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



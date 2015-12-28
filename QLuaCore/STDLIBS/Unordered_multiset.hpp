
#if !defined(UNORDERED_MULTISET__HPP__CCT)
#define UNORDERED_MULTISET__HPP__CCT

#include <unordered_set>
#include <memory>

namespace cct{

    template<typename T>
    class Unordered_multiset : public std::shared_ptr< std::unordered_multiset<T> > {
    private:
        typedef std::shared_ptr< std::unordered_multiset<T> > Super;
    public:

        Unordered_multiset( decltype(nullptr) ) {}
        Unordered_multiset() :Super(new std::unordered_multiset<T> ){}
        Unordered_multiset(const Unordered_multiset &)=default;
        Unordered_multiset(Unordered_multiset &&)=default;
        Unordered_multiset(Super && o):Super( std::move(o) ) {}
        Unordered_multiset(const Super & o):Super( o ) {}
        Unordered_multiset(const std::unordered_multiset<T> & o):Super( new std::unordered_multiset<T>( o ) ) {}
        Unordered_multiset(std::unordered_multiset<T> && o):Super( new std::unordered_multiset<T>( std::move(o) ) ) {}

        Unordered_multiset&operator=(const Unordered_multiset&)=default;
        Unordered_multiset&operator=(Unordered_multiset&&)=default;

        Unordered_multiset copy() const { return Unordered_multiset( *(*this) ); }
        Unordered_multiset unique_copy() const { if (this->use_count()<2) { return *this; }return copy(); }
    private:
        typedef std::unordered_multiset<T> Container;
        Container * data_() { return this->get(); }
        const Container * data_()const { return this->get(); }
    public:
        typedef typename Container::value_type value_type;
        typedef typename Container::const_iterator const_iterator;
        auto begin()const { return data_()->begin(); }
        auto end() const { return data_()->end(); }
        auto begin() { return data_()->begin(); }
        auto end() { return data_()->end(); }
    };

}

#endif




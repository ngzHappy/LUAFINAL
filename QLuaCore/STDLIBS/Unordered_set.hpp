
#if !defined(UNORDERED_SET__HPP__CCT)
#define UNORDERED_SET__HPP__CCT

#include <unordered_set>
#include <memory>

namespace cct{

    template<typename T>
    class Unordered_set : public std::shared_ptr< std::unordered_set<T> > {
    private:
        typedef std::shared_ptr< std::unordered_set<T> > Super;
    public:

        Unordered_set( decltype(nullptr) ) {}
        Unordered_set() :Super(new std::unordered_set<T> ){}
        Unordered_set(const Unordered_set &)=default;
        Unordered_set(Unordered_set &&)=default;
        Unordered_set(Super && o):Super( std::move(o) ) {}
        Unordered_set(const Super & o):Super( o ) {}
        Unordered_set(const std::unordered_set<T> & o):Super( new std::unordered_set<T>( o ) ) {}
        Unordered_set(std::unordered_set<T> && o):Super( new std::unordered_set<T>( std::move(o) ) ) {}

        Unordered_set&operator=(const Unordered_set&)=default;
        Unordered_set&operator=(Unordered_set&&)=default;

        Unordered_set copy() const { return Unordered_set( *(*this) ); }
        Unordered_set unique_copy() const { if (this->use_count()<2) { return *this; }return copy(); }
    private:
        typedef std::unordered_set<T> Container;
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




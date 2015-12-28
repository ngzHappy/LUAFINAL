
#if !defined(LIST__HPP__CCT)
#define LIST__HPP__CCT

#include <list>
#include <memory>

namespace cct{

    template<typename T>
    class List : public std::shared_ptr< std::list<T> > {
    private:
        typedef std::shared_ptr< std::list<T> > Super;
    public:

        List( decltype(nullptr) ) {}
        List() :Super(new std::list<T> ){}
        List(const List &)=default;
        List(List &&)=default;
        List(Super && o):Super( std::move(o) ) {}
        List(const Super & o):Super( o ) {}
        List(const std::list<T> & o):Super( new std::list<T>( o ) ) {}
        List(std::list<T> && o):Super( new std::list<T>( std::move(o) ) ) {}

        List&operator=(const List&)=default;
        List&operator=(List&&)=default;

        List copy() const { return List( *(*this) ); }
        List unique_copy() const { if (this->use_count()<2) { return *this; }return copy(); }
    private:
        typedef std::list<T> Container;
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




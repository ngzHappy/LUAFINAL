#if !defined(_cct_FTYPE__HPP__FUNCTION_TYPE_)
#define _cct_FTYPE__HPP__FUNCTION_TYPE_

#include <memory>
#include <type_traits>
namespace cct {

template<
    typename FunctionType_,
    FunctionType_ Function
> class FType;

template<
    typename ReturnType,
    typename ... Args,
    ReturnType(*Function)(Args...)
>
class FType< ReturnType(*)(Args...),Function >
    :public std::integral_constant<bool,false> {
public:
    typedef ReturnType(*FunctionType)(Args ...);
    static FunctionType function() { return Function; }
    template<typename ... Args_>
    static ReturnType call(Args_ && ... args) { return Function(std::forward<Args_>(args) ...); }
};

template<
    typename ReturnType,
    typename ClassType,
    typename ... Args,
    ReturnType(ClassType::* Function)(Args...)
>
class FType< ReturnType(ClassType::*)(Args...),Function >
    :public std::integral_constant<bool,true> {
public:
    typedef ReturnType(ClassType::*FunctionType)(Args ...);
    static FunctionType function() { return Function; }
    template<typename ... Args_>
    static ReturnType call(ClassType * const & c,Args_ && ... args) { return (c->*Function)(std::forward<Args_>(args) ...); }
    template<typename ... Args_>
    static ReturnType call(ClassType& c,Args_ && ... args) { return (c.*Function)(std::forward<Args_>(args) ...); }
};

template<
    typename ReturnType,
    typename ClassType,
    typename ... Args,
    ReturnType(ClassType::* Function)(Args...) &
>
class FType< ReturnType(ClassType::*)(Args...) &,Function >
    :public std::integral_constant<bool,true> {
public:
    typedef ReturnType(ClassType::*FunctionType)(Args ...) &;
    static FunctionType function() { return Function; }
    template<typename ... Args_>
    static ReturnType call(ClassType * const & c,Args_ && ... args) { return (c->*Function)(std::forward<Args_>(args) ...); }
    template<typename ... Args_>
    static ReturnType call(ClassType & c,Args_ && ... args) { return (c.*Function)(std::forward<Args_>(args) ...); }
};

template<
    typename ReturnType,
    typename ClassType,
    typename ... Args,
    ReturnType(ClassType::* Function)(Args...)&&
>
class FType< ReturnType(ClassType::*)(Args...)&&,Function >
    :public std::integral_constant<bool,true> {
public:
    typedef ReturnType(ClassType::*FunctionType)(Args ...)&&;
    static FunctionType function() { return Function; }
    template<typename ... Args_>
    static ReturnType call(ClassType * const & c,Args_ && ... args) { return ((std::move(*c)).*Function)(std::forward<Args_>(args) ...); }
    template<typename ... Args_>
    static ReturnType call(ClassType && c,Args_ && ... args) { return ((std::move(c)).*Function)(std::forward<Args_>(args) ...); }
};

template<
    typename ReturnType,
    typename ClassType,
    typename ... Args,
    ReturnType(ClassType::* Function)(Args...) const
>
class FType< ReturnType(ClassType::*)(Args...) const,Function >
    :public std::integral_constant<bool,true> {
public:
    typedef ReturnType(ClassType::*FunctionType)(Args ...) const;
    static FunctionType function() { return Function; }
    template<typename ... Args_>
    static ReturnType call(const ClassType * const & c,Args_ && ... args) { return (c->*Function)(std::forward<Args_>(args) ...); }
    template<typename ... Args_>
    static ReturnType call(const ClassType & c,Args_ && ... args) { return (c.*Function)(std::forward<Args_>(args) ...); }
};

template<
    typename ReturnType,
    typename ClassType,
    typename ... Args,
    ReturnType(ClassType::* Function)(Args...) const &
>
class FType< ReturnType(ClassType::*)(Args...) const &,Function >
    :public std::integral_constant<bool,true> {
public:
    typedef ReturnType(ClassType::*FunctionType)(Args ...) const &;
    static FunctionType function() { return Function; }
    template<typename ... Args_>
    static ReturnType call(const ClassType * const & c,Args_ && ... args) { return (c->*Function)(std::forward<Args_>(args) ...); }
    template<typename ... Args_>
    static ReturnType call(const ClassType & c,Args_ && ... args) { return (c.*Function)(std::forward<Args_>(args) ...); }
};

template<
    typename ReturnType,
    typename ClassType,
    typename ... Args,
    ReturnType(ClassType::* Function)(Args...) const &&
>
class FType< ReturnType(ClassType::*)(Args...) const &&,Function >
    :public std::integral_constant<bool,true> {
public:
    typedef ReturnType(ClassType::*FunctionType)(Args ...) const &&;
    static FunctionType function() { return Function; }
    template<typename ... Args_>
    static ReturnType call(const ClassType * const & c,Args_ && ... args) { return (std::move(*c).*Function)(std::forward<Args_>(args) ...); }
    template<typename ... Args_>
    static ReturnType call(const ClassType && c,Args_ && ... args) { return (std::move(c).*Function)(std::forward<Args_>(args) ...); }
};

template<
    typename ReturnType,
    typename ClassType,
    typename ... Args,
    ReturnType(ClassType::* Function)(Args...) const volatile
>
class FType< ReturnType(ClassType::*)(Args...) const volatile,Function >
    :public std::integral_constant<bool,true> {
public:
    typedef ReturnType(ClassType::*FunctionType)(Args ...) const volatile;
    static FunctionType function() { return Function; }
    template<typename ... Args_>
    static ReturnType call(const volatile ClassType * const & c,Args_ && ... args) { return (c->*Function)(std::forward<Args_>(args) ...); }
    template<typename ... Args_>
    static ReturnType call(const volatile ClassType & c,Args_ && ... args) { return (c.*Function)(std::forward<Args_>(args) ...); }
};

template<
    typename ReturnType,
    typename ClassType,
    typename ... Args,
    ReturnType(ClassType::* Function)(Args...) const volatile &
>
class FType< ReturnType(ClassType::*)(Args...) const volatile &,Function >
    :public std::integral_constant<bool,true> {
public:
    typedef ReturnType(ClassType::*FunctionType)(Args ...) const volatile &;
    static FunctionType function() { return Function; }
    template<typename ... Args_>
    static ReturnType call(const volatile ClassType * const &c,Args_ && ... args) { return (c->*Function)(std::forward<Args_>(args) ...); }
    template<typename ... Args_>
    static ReturnType call(const volatile ClassType & c,Args_ && ... args) { return (c.*Function)(std::forward<Args_>(args) ...); }
};

template<
    typename ReturnType,
    typename ClassType,
    typename ... Args,
    ReturnType(ClassType::* Function)(Args...) const volatile &&
>
class FType< ReturnType(ClassType::*)(Args...) const volatile &&,Function >
    :public std::integral_constant<bool,true> {
public:
    typedef ReturnType(ClassType::*FunctionType)(Args ...) const volatile &&;
    static FunctionType function() { return Function; }
    template<typename ... Args_>
    static ReturnType call(const volatile ClassType * const & c,Args_ && ... args) { return (std::move(*c).*Function)(std::forward<Args_>(args) ...); }
    template<typename ... Args_>
    static ReturnType call(const volatile ClassType && c,Args_ && ... args) { return (std::move(c).*Function)(std::forward<Args_>(args) ...); }
};

template<
    typename ReturnType,
    typename ClassType,
    typename ... Args,
    ReturnType(ClassType::* Function)(Args...) volatile
>
class FType< ReturnType(ClassType::*)(Args...) volatile,Function >
    :public std::integral_constant<bool,true> {
public:
    typedef ReturnType(ClassType::*FunctionType)(Args ...) volatile;
    static FunctionType function() { return Function; }
    template<typename ... Args_ >
    static ReturnType call(volatile ClassType * const & c,Args_ && ... args) { return (c->*Function)(std::forward<Args_>(args) ...); }
    template<typename ... Args_ >
    static ReturnType call(volatile ClassType & c,Args_ && ... args) { return (c.*Function)(std::forward<Args_>(args) ...); }
};

template<
    typename ReturnType,
    typename ClassType,
    typename ... Args,
    ReturnType(ClassType::* Function)(Args...) volatile &
>
class FType< ReturnType(ClassType::*)(Args...) volatile &,Function >
    :public std::integral_constant<bool,true> {
public:
    typedef ReturnType(ClassType::*FunctionType)(Args ...) volatile &;
    static FunctionType function() { return Function; }
    template<typename ... Args_ >
    static ReturnType call(volatile ClassType * const & c,Args_ && ... args) { return (c->*Function)(std::forward<Args_>(args) ...); }
    template<typename ... Args_ >
    static ReturnType call(volatile ClassType & c,Args_ && ... args) { return (c.*Function)(std::forward<Args_>(args) ...); }
};

template<
    typename ReturnType,
    typename ClassType,
    typename ... Args,
    ReturnType(ClassType::* Function)(Args...) volatile &&
>
class FType< ReturnType(ClassType::*)(Args...) volatile &&,Function >
    :public std::integral_constant<bool,true> {
public:
    typedef ReturnType(ClassType::*FunctionType)(Args ...) volatile &&;
    static FunctionType function() { return Function; }
    template<typename ... Args_ >
    static ReturnType call(volatile ClassType * const & c,Args_ && ... args) { return (std::move(*c).*Function)(std::forward<Args_>(args) ...); }
    template<typename ... Args_ >
    static ReturnType call(volatile ClassType && c,Args_ && ... args) { return (std::move(c).*Function)(std::forward<Args_>(args) ...); }
};

}/*~cct*/

namespace cct{
template<typename T,T F,const char * (*Name)() >
class FTypeName : public FType<T,F> {
public:
    static const char * functionName(){ return Name(); }
};
}

#if !defined( FTYPE )
#define FTYPE( _x_ ) ::cct::FType< decltype(_x_),_x_ >
#endif

#endif

/*
*/


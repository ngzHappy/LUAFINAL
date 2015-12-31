#if !defined(__PURE__LUA__CORE__CCT)
#define __PURE__LUA__CORE__CCT

#include <QLuaCore.hpp>
namespace cct {
class QLUACORESHARED_EXPORT PureLuaCore {
public:
    enum class LUAType : int {
        NONE=LUA_TNONE,
        NIL=LUA_TNIL,
        BOOLEAN=LUA_TBOOLEAN,
        LIGHTUSERDATA=LUA_TLIGHTUSERDATA,
        NUMBER=LUA_TNUMBER,
        STRING=LUA_TSTRING,
        TABLE=LUA_TTABLE,
        FUNCTION=LUA_TFUNCTION,
        USERDATA=LUA_TUSERDATA,
        THREAD=LUA_TTHREAD,
    };
    typedef std::size_t size_t;
protected:

    template<typename T,typename U = void >
    class __StringCallIndex {public:enum {value=0};/* this is not char[N] or char * */};

    template<typename T>
    class __StringCallIndex<T,typename std::enable_if<
        std::is_array< typename std::remove_reference<T>::type >::value ,
        void>::type > {public:enum {value=1};/* if it is char[N] */};

    template<typename T>
    class __StringCallIndex<T,typename std::enable_if<
        (std::is_same<const char *,typename std::remove_reference<T>::type>::value)||
        (std::is_same<char *,typename std::remove_reference<T>::type>::value)||
        (std::is_same<char * const,typename std::remove_reference<T>::type>::value)||
        (std::is_same<const char * const,typename std::remove_reference<T>::type>::value),
        void>::type > {public:enum {value=2};/* if it is char * */};

    class __PureLuaCorePrivate;
    std::shared_ptr< __PureLuaCorePrivate > dataCore;
    void __PureLuaCore(std::shared_ptr<lua_State>,void * xpointer=reinterpret_cast<void *>(1));

    template<typename IT_,typename IU_,typename ValueType >
    static bool __setGlobal(const lua_State * const & L_,IT_ b,IU_ e,const ValueType & v,std::string &);

    template<typename IT_,typename IU_   >
    static LUAType __getGlobal(const lua_State * const & L_,IT_ b,IU_ e,std::string &);

    template<typename IT_,typename IU_,typename ValueType >
    static bool __setTable(const lua_State * const & L_,int table,IT_ b,IU_ e,const ValueType & v,std::string & errorCode);

    template<typename IT_,typename IU_  >
    static LUAType __getTable(const lua_State * const & L_,int table,IT_ b,IU_ e,std::string & errorCode);

    void setError(std::string &&) const;

    static void __pushKey(const lua_State * const & L_,const char * const & value);
    static void __pushKey(const lua_State * const & L_,const char * const & value,size_t l);
    static void _i_pushKey(const lua_State * const & L_,const lua_Integer);
    static void _v_pushKey(const lua_State * const & L_,const QVariant &);

    static void _b_pushValue(const lua_State * const & L_,const bool);
    static void _n_pushValue(const lua_State * const & L_,const lua_Number);
    static void _f_pushValue(const lua_State * const & L_,lua_CFunction);
    static void _v_pushValue(const lua_State * const & L_,const QVariant &);

    static void __setglobal(const lua_State * const & L_,const char * const&);
    static void _v_setglobal(const lua_State * const & L_,const QVariant &);

    static LUAType ___getGlobal(const lua_State * const & L_,const char * const&);
    static LUAType _v__getGlobal(const lua_State * const & L_,const QVariant&);

    lua_State * __state()const;
private:

    template<typename T,unsigned int N = __StringCallIndex<T>::value >
    class __PublishKey {public:/*0*/
        static void pushKey(const lua_State * const & L_,const std::string & value) { __pushKey(L_,value.c_str(),static_cast<size_t>(value.size())); }
        static void pushKey(const lua_State * const & L_,const QByteArray & value) { __pushKey(L_,value.constData(),static_cast<size_t>(value.size())); }
        static void pushKey(const lua_State * const & L_,const QString & value_) { const auto value=value_.toUtf8(); __pushKey(L_,value.constData(),static_cast<size_t>(value.size())); }
        static void pushKey(const lua_State * const & L_,const lua_Integer value) { _i_pushKey(L_,value); }
        static void pushKey(const lua_State * const & L_,const lua_Number value) { _i_pushKey(L_,static_cast<lua_Integer>(value)); }
        static void pushKey(const lua_State * const & L_,const QVariant &value) { _v_pushKey(L_,value); }
    };

    template<typename T >
    class __PublishKey<T,1> {public:/*1*/
        template<typename CharType_,size_t NNN> 
        static void pushKey(const lua_State * const & L_,const  CharType_(&value)[NNN]) { __pushKey(L_,value,static_cast<size_t>(NNN-1)); }
    };

    template<typename T >
    class __PublishKey<T,2> {public:/*2*/
        static void pushKey(const lua_State * const & L_,const char * const &value) { __pushKey(L_,value); }
    };

    template<typename T,unsigned int N = __StringCallIndex<T>::value >
    class __PushValue {public:/*0*/
        static void pushValue(const lua_State * const & L_,const std::string & value) { __pushKey(L_,value.c_str(),static_cast<size_t>(value.size())); }
        static void pushValue(const lua_State * const & L_,const QByteArray & value) { __pushKey(L_,value.constData(),static_cast<size_t>(value.size())); }
        static void pushValue(const lua_State * const & L_,const QString & value_) { const auto value=value_.toUtf8(); __pushKey(L_,value.constData(),static_cast<size_t>(value.size())); }
        static void pushValue(const lua_State * const & L_,const lua_Integer value) { _i_pushKey(L_,value); }
        static void pushValue(const lua_State * const & L_,const lua_Number value) { _n_pushValue(L_,value); }
        static void pushValue(const lua_State * const & L_,const bool value) { _b_pushValue(L_,value); }
        static void pushValue(const lua_State * const & L_,lua_CFunction value) { _f_pushValue(L_,value); }
        static void pushValue(const lua_State * const & L_,const QVariant &value) { _v_pushValue(L_,value); }
    };

    template<typename T>
    class __PushValue<T,1> {public:/*1*/
        template<typename CharType_,size_t NNN> 
        static void pushValue(const lua_State * const & L_,const CharType_(&value)[NNN]) { __pushKey(L_,value,static_cast<size_t>(NNN-1)); }
    };

    template<typename T>
    class __PushValue<T,2> { public:/*2*/ 
        static void pushValue(const lua_State * const & L_,const char * const & value) { __pushKey(L_,value); }
    };

    template<typename T,unsigned int N = __StringCallIndex<T>::value>
    class __SetGlobal {public:/*0*/
        static void setGlobal(const lua_State * const & L_,const std::string & name) { __setglobal(L_,name.c_str()); }
        static void setGlobal(const lua_State * const & L_,const QByteArray & name) { __setglobal(L_,name.constData()); }
        static void setGlobal(const lua_State * const & L_,const QString & name_) { const auto name=name_.toUtf8();  __setglobal(L_,name.constData()); }
        static void setGlobal(const lua_State * const & L_,const QVariant & name_) { _v_setglobal(L_,name_); }
    };

    template<typename T>
    class __SetGlobal<T,1> { public:/*1*/
        template<typename CharType_,size_t NNN> 
        static void setGlobal(const lua_State * const & L_,const CharType_(&name)[NNN]) { __setglobal(L_,name); }
    };

    template<typename T>
    class __SetGlobal<T,2> { public:/*2*/
        static void setGlobal(const lua_State * const & L_,const char * const& name) { __setglobal(L_,name); }
    };

    template<typename T,unsigned int N = __StringCallIndex<T>::value>
    class __GetGlobal {public:/*0*/
        static LUAType getGlobal(const lua_State * const & L_,const std::string & key) { return ___getGlobal(L_,key.c_str()); }
        static LUAType getGlobal(const lua_State * const & L_,const QByteArray & key) { return ___getGlobal(L_,key.constData()); }
        static LUAType getGlobal(const lua_State * const & L_,const QString & key) { const auto v__=key.toUtf8(); return ___getGlobal(L_,v__.constData()); }
        static LUAType getGlobal(const lua_State * const & L_,const QVariant & key) { return _v__getGlobal(L_,key); }
    };

    template<typename T>
    class __GetGlobal<T,1> {public:/*1*/
        template<typename CharType_,size_t NNN>
        static LUAType getGlobal(const lua_State * const & L_,const CharType_(&key)[NNN]) { return ___getGlobal(L_,key); }
    };

    template<typename T>
    class __GetGlobal<T,2> {public:/*2*/
        static LUAType getGlobal(const lua_State * const & L_,const char * const & key) { return ___getGlobal(L_,key); }
    };
public:

    PureLuaCore() { __PureLuaCore(nullptr,nullptr); }
    PureLuaCore(decltype(nullptr)) { __PureLuaCore(nullptr); }
    PureLuaCore(const std::shared_ptr<lua_State> &v) { __PureLuaCore(v); }
    PureLuaCore(std::shared_ptr<lua_State> &&v) { __PureLuaCore(std::move(v)); }
    PureLuaCore(const PureLuaCore &)=default;
    PureLuaCore(PureLuaCore &&)=default;
    ~PureLuaCore();

public:

    template<typename SizeType>
    static void pushKey(const lua_State * const & L_,const char * const & value,SizeType l) { __pushKey(L_,value,static_cast<size_t>(l)); }
    template<typename t__T>
    static void pushKey(const lua_State * const & L_,const t__T & v) { __PublishKey<t__T>::pushKey(L_,v); }

    template<typename Size_Type_>
    static void pushValue(const lua_State * const & L_,const char * const & value,Size_Type_ l) { __pushKey(L_,value,static_cast<size_t>(l)); }
    template<typename t__T>
    static void pushValue(const lua_State * const & L_,const t__T &value) { __PushValue<t__T>::pushValue(L_,value); }

public:
    template<typename Size_Type_> 
    void pushKey(const char * const & value,Size_Type_ l) { auto L_=__state(); if (L_==nullptr) { return; } pushKey(L_,value,static_cast<size_t>(l) ); }
    template<typename t__T>
    void pushKey(const t__T &value) { auto L_=__state(); if (L_==nullptr) { return; } pushKey(L_,value); }

    template<typename Size_Type_>
    void pushValue(const char * const & value,Size_Type_ l) { auto L_=__state(); if (L_==nullptr) { return; } __pushKey(L_,value,static_cast<size_t>(l)); }
    template<typename t__T> 
    void pushValue(const t__T & v) { auto L_=__state(); if (L_==nullptr) { return; }pushValue(L_,v); }
public:
    template<typename Size_Type_>
    static void setGlobal(const lua_State * const & L_,const char * const& name,Size_Type_ ) { __setglobal(L_,name); }
    template<typename t__T>
    static void setGlobal(const lua_State * const & L_,const t__T & value) { __SetGlobal<t__T>::setGlobal(L_,value); }

    template<typename Size_Type_>
    void setGlobal(const char * const& name,Size_Type_ ) { auto L_=__state(); if (L_==nullptr) { return; }  __setglobal(L_,name); }
    template<typename t__T>
    static void setGlobal(const t__T & value) { auto L_=__state(); if (L_==nullptr) { return; } __SetGlobal<t__T>::setGlobal(L_,value); }

public:
    template<typename NameType,typename ValueType>
    static void setGlobalValue(const lua_State * const & L_,const NameType & key,const ValueType & value) { pushValue(L_,value); setGlobal(L_,key); }
    template<typename ValueType>
    static void setGlobalValue(const lua_State * const & L_,const char * const & key,size_t,const ValueType & value) { pushValue(L_,value); setGlobal(L_,key); }
    template<typename NameType>
    static void setGlobalValue(const lua_State * const & L_,const NameType & key,const char * const & value,size_t l) { pushValue(L_,value,l); setGlobal(L_,key); }
    static void setGlobalValue(const lua_State * const & L_,const char * const & key,size_t,const char * const & value,size_t l) { pushValue(L_,value,l); setGlobal(L_,key); }

    template<typename NameType,typename ValueType>
    void setGlobalValue(const NameType & key,const ValueType & value) { auto L_=__state(); if (L_==nullptr) { return; }setGlobalValue(L_,key,value); }
    template<typename ValueType>
    void setGlobalValue(const char * const & key,size_t,const ValueType & value) { auto L_=__state(); if (L_==nullptr) { return; }setGlobalValue(L_,key,value); }
    template<typename NameType>
    void setGlobalValue(const NameType & key,const char * const & value,size_t l) { auto L_=__state(); if (L_==nullptr) { return; }setGlobalValue(L_,key,value,l); }
    void setGlobalValue(const char * const & key,size_t,const char * const & value,size_t l) { auto L_=__state(); if (L_==nullptr) { return; }setGlobalValue(L_,key,value,l); }
public:
    template<typename  SizeType>
    static LUAType getGlobal(const lua_State * const & L_,const char * const & key,SizeType ) { return ___getGlobal(L_,key); }
    template<typename t__T>
    static LUAType getGlobal(const lua_State * const & L_,const t__T & value) { return __GetGlobal<t__T>::getGlobal(L_,value); }
   
    template<typename  SizeType>
    LUAType getGlobal(const char * const & key,SizeType )const { auto L_=__state(); if (L_==nullptr) { return LUAType::NONE; }return ___getGlobal(L_,key); }
    template<typename t__T>
    LUAType getGlobal(const t__T & key)const {auto L_=__state(); if (L_==nullptr) { return LUAType::NONE; }return __GetGlobal<t__T>::getGlobal(L_,value);}
public:
    static LUAType getTable(const lua_State * const & L_,const int table);
    template<typename KeyType>
    static LUAType getTable(const lua_State * const & L_,const int table,const KeyType &key) { pushKey(L_,key); return getTable(L_,table); }
    template<typename SizeType>
    static LUAType getTable(const lua_State * const & L_,const int table,const char *const &key,const SizeType & l) { pushKey(L_,key,static_cast<size_t>(l)); return getTable(L_,table); }
   
    LUAType getTable(const int table)const { auto L_=__state(); if (L_==nullptr) { return LUAType::NONE; }return getTable(L_,table); }
    template<typename KeyType>
    LUAType getTable(const int table,const KeyType &key) { auto L_=__state(); if (L_==nullptr) { return LUAType::NONE; } pushKey(L_,key); return getTable(L_,table); }
    template<typename SizeType>
    LUAType getTable(const int table,const char *const &key,const SizeType & l) { auto L_=__state(); if (L_==nullptr) { return LUAType::NONE; } pushKey(L_,key,static_cast<size_t>(l)); return getTable(L_,table); }
   
public:
    static void setTable(const lua_State * const & L_,const int table);
    void setTable(const int table) { auto L_=__state(); if (L_==nullptr) { return; }setTable(L_,table); }
public:
    template<typename KeyType,typename ValueType>
    static void setTableValue(const lua_State * const & L_,const int table,const KeyType & key,const ValueType & value) { pushKey(L_,key); pushValue(L_,value); setTable(L_,table); }
    template<typename ValueType>
    static void setTableValue(const lua_State * const & L_,const int table,const char * const & key,const size_t l,const ValueType & value) { pushKey(L_,key,l); pushValue(L_,value); setTable(L_,table); }
    template<typename KeyType>
    static void setTableValue(const lua_State * const & L_,const int table,const KeyType & key,const char * const & value,const size_t l) { pushKey(L_,key); pushValue(L_,value,l); setTable(L_,table); }
    static void setTableValue(const lua_State * const & L_,const int table,const char * & key,const size_t kl,const char * const & value,const size_t vl) { pushKey(L_,key,kl); pushValue(L_,value,vl); setTable(L_,table); }

    template<typename KeyType,typename ValueType>
    void setTableValue(const int table,const KeyType & key,const ValueType & value) {auto L_=__state(); if (L_==nullptr) { return; } pushKey(L_,key); pushValue(L_,value); setTable(L_,table); }
    template<typename ValueType>
    void setTableValue(const int table,const char * const & key,const size_t l,const ValueType & value) {auto L_=__state(); if (L_==nullptr) { return; } pushKey(L_,key,l); pushValue(L_,value); setTable(L_,table); }
    template<typename KeyType>
    void setTableValue(const int table,const KeyType & key,const char * const & value,const size_t l) {auto L_=__state(); if (L_==nullptr) { return; } pushKey(L_,key); pushValue(L_,value,l); setTable(L_,table); }
    void setTableValue(const int table,const char * & key,const size_t kl,const char * const & value,const size_t vl) {auto L_=__state(); if (L_==nullptr) { return; } pushKey(L_,key,kl); pushValue(L_,value,vl); setTable(L_,table); }

public:
    std::shared_ptr<lua_State> getLuaState()const;
    void setLuaState(std::shared_ptr<lua_State>);

    const std::string & getError()const;
    void clearError();

    LUAType type(int) const;
    static LUAType type(const lua_State * const &,int);

    static QVariant toQVariant(const lua_State * const &L_,int v=-1);
    QVariant toQVariant(int v=-1)const;

    static int getTop(const lua_State * const &L_);
    static void setTop(const lua_State * const &L_,int v=0);

    int getTop() const;
    void setTop(int v=0);

    static void newTable(const lua_State * const &L_);
    void newTable();

    static void push(const lua_State * const &L_);
    void push();

    static void push(const lua_State * const &L_,int index);
    void push(int index);

    static void pop(const lua_State * const &L_);
    void pop();

    static void pop(const lua_State * const &L_,int);
    void pop(int);

    static int absIndex(const lua_State * const &L_,int);
    int absIndex(int) const;

    static void pushNIL(const lua_State * const &L_);
    void pushNIL();

    static void copy(const lua_State * const &L_,int from,int to);
    void copy(int from,int to);

    template<typename t__BI,typename t__EI,typename t__V>
    bool setTables(int table,t__BI b,t__EI e,const t__V & v) {
        auto L__=getLuaState(); if (L__) {
            std::string last_error;
            bool ans=__setTable(L__.get(),table,b,e,v,last_error);
            setError(std::move(last_error)); return ans;
        }return false;
    }

    template<typename t__BI,typename t__EI >
    LUAType getTables(int table_,t__BI b,t__EI e)const {
        auto L__=getLuaState(); if (L__) {
            std::string last_error;
            auto ans=__getTable(L__.get(),table_,b,e,last_error);
            setError(std::move(last_error)); return ans;
        }return LUAType::NONE;
    }

    template<typename t__BI,typename t__EI >
    LUAType getGlobals(t__BI b,t__EI e)const {
        auto L__=getLuaState(); if (L__) {
            std::string last_error;
            auto ans=__getGlobal(L__.get(),b,e,last_error);
            setError(std::move(last_error)); return ans;
        }return LUAType::NONE;
    }

    template<typename t__BI,typename t__EI,typename t__V>
    bool setGlobals(t__BI b,t__EI e,const t__V & v) {
        auto L__=getLuaState(); if (L__) {
            std::string last_error;
            bool ans=__setGlobal(L__.get(),b,e,v,last_error);
            setError(std::move(last_error)); return ans;
        }return false;
    }

    LUAType getGlobals(const std::initializer_list<QVariant> & keys) const { return getGlobals(keys.begin(),keys.end()); }
    LUAType getTables(int table,const std::initializer_list<QVariant> & keys) const { return getTables(table,keys.begin(),keys.end()); }
    template<typename t__V>bool setTables(int table,const std::initializer_list<QVariant> & keys,const t__V & v) { return setTables(table,keys.begin(),keys.end(),v); }
    template<typename t__V>bool setGlobals(const std::initializer_list<QVariant> & keys,const t__V & v) { return setGlobals(keys.begin(),keys.end(),v); }

    template<typename t__U>LUAType getGlobals(const t__U & keys) const { return getGlobals(keys.begin(),keys.end()); }
    template<typename t__U>LUAType getTables(int table,const t__U & keys) const { return getTables(table,keys.begin(),keys.end()); }
    template<typename t__U,typename t__V>bool setTables(int table,const t__U & keys,const t__V & v) { return setTables(table,keys.begin(),keys.end(),v); }
    template<typename t__U,typename t__V>bool setGlobals(const t__U & keys,const t__V & v) { return setGlobals(keys.begin(),keys.end(),v); }

public:
    class StateLock {
        lua_State * L; int n;
    public:
        StateLock(const lua_State * const & L_):L(const_cast<lua_State * const &>(L_)) { if (L) { n=getTop(L); } }
        ~StateLock() { if (L) { setTop(L,n); } }
    };
};

template<typename IT_,typename IU_,typename ValueType  >
bool PureLuaCore::__setTable(
    const lua_State * const & L_,int table,
    IT_ b,IU_ e,
    const ValueType & v,
    std::string & errorCode)try {
    table=absIndex(L_,table);
    /*null data*/
    if (b==e) { errorCode="null data"; return false; }
    /*check the table*/
    if (type(L_,table)!=LUAType::TABLE) {
        errorCode="input is not a table ";
        return false;
    }
    /*lock the state*/
    StateLock lock(L_);
    /* ob old pointer */
    auto ob=b++;
    /*just one table*/
    if (b==e) { setTableValue(L_,table,*ob,v); return true; }
    /*get the last table*/
    for (; b!=e; ob=b++) {
        if (getTable(L_,table,*ob)==LUAType::TABLE) { table=getTop(L_); }
        else {
            pop(L_); pushKey(L_,*ob);  newTable(L_); setTable(L_,table);
            if (getTable(L_,table,*ob)==LUAType::TABLE) { table=getTop(L_); }
            else { errorCode="it is not a table"; return false; }
        }
    }
    setTableValue(L_,table,*ob,v);
    return true;
}
catch (std::string & xerr) { errorCode=xerr; return false; }
catch (...) { errorCode="unknow error"; return false; }

template<typename IT_,typename IU_,typename ValueType >
bool PureLuaCore::__setGlobal(
    const lua_State * const & L,
    IT_ b,IU_ e,
    const ValueType & v,std::string & errorCode) try {
    /*null data*/
    if (b==e) { errorCode="null data"; return false; }
    int table=0;
    auto ob=b++;
    /*lock the state*/
    StateLock lock(L);
    if (b==e) {/*only one data*/ pushValue(L,v); setGlobal(L,*ob); return true; }
    if (getGlobal(L,*ob)!=LUAType::TABLE) {
        pop(L); newTable(L); setGlobal(L,*ob);
        if (getGlobal(L,*ob)!=LUAType::TABLE) { errorCode="it can not be table"; return false; }
    }
    table=getTop(L); return __setTable(L,table,b,e,v,errorCode);
}
catch (std::string & xerr) { errorCode=xerr; return false; }
catch (...) { errorCode="unknow error"; return false; }

template<typename IT_,typename IU_  >
PureLuaCore::LUAType PureLuaCore::__getTable(
    const lua_State * const & L_,int table,
    IT_ b,IU_ e,std::string & errorCode) try {
    const auto & L=L_;
    table=absIndex(L_,table);
    if (b==e) { errorCode="null data";  pushNIL(L_); return LUAType::NIL; }
    if (type(L_,table)!=LUAType::TABLE) { errorCode="not a table"; pushNIL(L_); return  LUAType::NIL; }
    auto ob=b++;
    if (b==e) {/*only one data*/return getTable(L_,table,*ob); }
    pushNIL(L_); StateLock lock(L_);
    const auto ans_=getTop(L_);
    for (; b!=e; ob=b++) {
        if (getTable(L_,table,*ob)==LUAType::TABLE) { table=getTop(L_); }
        else {
            errorCode="it is not a table  ";
            return LUAType::NIL;;
        }
    }
    auto ans_type=getTable(L,table,*ob);
    copy(L,-1,ans_);
    return ans_type;
}
catch (std::string & xerr) { errorCode=xerr; return LUAType::NIL; }
catch (...) { errorCode="unknow error"; return LUAType::NIL; }

template<typename IT_,typename IU_   >
PureLuaCore::LUAType PureLuaCore::__getGlobal(const lua_State * const & L_,IT_ b,IU_ e,std::string & errorCode) try {
    if (b==e) { errorCode="null data";  pushNIL(L_); return LUAType::NIL; }
    auto ob=b++;
    pushNIL(L_); const auto ans_=getTop(L_);
    StateLock lock(L_);
    if (b==e) {/**only one*/
        auto ans_type_=getGlobal(L_,*ob);
        copy(L_,-1,ans_); return ans_type_;
    }
    if (getGlobal(L_,*ob)==LUAType::TABLE) {
        auto table=getTop(L_);
        auto ans_type_=__getTable(L_,table,b,e,errorCode);
        copy(L_,-1,ans_); return ans_type_;
    }
    else {
        errorCode="it is not a table"; return LUAType::NIL;
    }
}
catch (std::string & xerr) { errorCode=xerr; return LUAType::NIL; }
catch (...) { errorCode="unknow error"; return LUAType::NIL; }

}/*cct*/

#endif


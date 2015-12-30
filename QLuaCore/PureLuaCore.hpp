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
private:
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
public:

    PureLuaCore() { __PureLuaCore(nullptr,nullptr); }
    PureLuaCore(decltype(nullptr)) { __PureLuaCore(nullptr); }
    PureLuaCore(const std::shared_ptr<lua_State> &v) { __PureLuaCore(v); }
    PureLuaCore(std::shared_ptr<lua_State> &&v) { __PureLuaCore(std::move(v)); }
    PureLuaCore(const PureLuaCore &)=default;
    PureLuaCore(PureLuaCore &&)=default;
    ~PureLuaCore();

    std::shared_ptr<lua_State> getLuaState()const;
    void setLuaState(std::shared_ptr<lua_State>);

    const std::string & getError()const;
    void clearError();

    static void pushKey(const lua_State * const & L_,const QVariant &value);
    static void pushKey(const lua_State * const & L_,const char * value);
    static void pushKey(const lua_State * const & L_,const char * value,lua_Integer l);
    static void pushKey(const lua_State * const & L_,lua_Integer);
    static void pushKey(const lua_State * const & L_,lua_Number);
    static void pushKey(const lua_State * const & L_,const std::string & v) { pushKey(L_,v.c_str(),v.size()); }
    static void pushKey(const lua_State * const & L_,const QByteArray & v) { pushKey(L_,v.constData(),v.size()); }
    static void pushKey(const lua_State * const & L_,const QString & v) { pushKey(L_,v.toUtf8()); }

    void pushKey(const QVariant &value);
    void pushKey(const char * value);
    void pushKey(const char * value,lua_Integer l);
    void pushKey(lua_Integer);
    void pushKey(lua_Number);
    void pushKey(const std::string & v) { pushKey(v.c_str(),v.size()); }
    void pushKey(const QByteArray & v) { pushKey(v.constData(),v.size()); }
    void pushKey(const QString & v) { pushKey(v.toUtf8()); }

    static void pushValue(const lua_State * const & L_,const QVariant &value);
    static void pushValue(const lua_State * const & L_,const char * value);
    static void pushValue(const lua_State * const & L_,const char * value,lua_Integer l);
    static void pushValue(const lua_State * const & L_,lua_Integer);
    static void pushValue(const lua_State * const & L_,lua_Number);
    static void pushValue(const lua_State * const & L_,bool);
    static void pushValue(const lua_State * const & L_,lua_CFunction);
    static void pushValue(const lua_State * const & L_,const std::string &value) { pushValue(L_,value.c_str(),value.size()); }
    static void pushValue(const lua_State * const & L_,const QByteArray &value) { pushValue(L_,value.constData(),value.size()); }
    static void pushValue(const lua_State * const & L_,const QString &value) { const auto v_=value.toUtf8(); pushValue(L_,v_.constData(),v_.size()); }

    void pushValue(const QVariant &value);
    void pushValue(const char * value);
    void pushValue(const char * value,lua_Integer l);
    void pushValue(lua_Integer);
    void pushValue(lua_Number);
    void pushValue(bool);
    void pushValue(lua_CFunction);
    void pushValue(const std::string &value) { pushValue(value.c_str(),value.size()); }
    void pushValue(const QByteArray &value) { pushValue(value.constData(),value.size()); }
    void pushValue(const QString &value) { const auto v_=value.toUtf8(); pushValue(v_.constData(),v_.size()); }

    LUAType getGlobal(const char * const &,lua_Integer=-1)const;
    LUAType getGlobal(const QString & d) const { return getGlobal(d.toUtf8()); }
    LUAType getGlobal(const QByteArray & d) const { return getGlobal(d.constData(),d.size()); }
    LUAType getGlobal(const std::string & d) const { return getGlobal(d.c_str(),d.size()); }
    LUAType getGlobal(const QVariant &) const;

    static LUAType getGlobal(const lua_State * const &,const char * const &,lua_Integer=-1);
    static LUAType getGlobal(const lua_State * const & L,const QString & d) { return getGlobal(L,d.toUtf8()); }
    static LUAType getGlobal(const lua_State * const & L,const QByteArray & d) { return getGlobal(L,d.constData(),d.size()); }
    static LUAType getGlobal(const lua_State * const & L,const std::string & d) { return getGlobal(L,d.c_str(),d.size()); }
    static LUAType getGlobal(const lua_State * const & L,const QVariant &);

    void setGlobal(const char * const &,const char *);
    void setGlobal(const std::string &,const char *);
    void setGlobal(const QByteArray &,const char *);
    void setGlobal(const QString &,const char *);

    static void setGlobal(const lua_State * const &,const char * const &,const char *);
    static void setGlobal(const lua_State * const &,const std::string &,const char *);
    static void setGlobal(const lua_State * const &,const QByteArray &,const char *);
    static void setGlobal(const lua_State * const &,const QString &,const char *);

    static void setGlobal(const lua_State * const &L,const char * const &k,const std::string &v) { setGlobal(L,k,v.c_str(),v.size()); }
    static void setGlobal(const lua_State * const &L,const std::string &k,const std::string &v) {setGlobal(L,k,v.c_str(),v.size());}
    static void setGlobal(const lua_State * const &L,const QByteArray &k,const std::string &v) {setGlobal(L,k,v.c_str(),v.size());}
    static void setGlobal(const lua_State * const &L,const QString &k,const std::string &v) {setGlobal(L,k,v.c_str(),v.size());}

    static void setGlobal(const lua_State * const &L,const char * const &k,const QByteArray &v) { setGlobal(L,k,v.constData(),v.size()); }
    static void setGlobal(const lua_State * const &L,const std::string &k,const QByteArray &v) {setGlobal(L,k,v.constData(),v.size());}
    static void setGlobal(const lua_State * const &L,const QByteArray &k,const QByteArray &v) {setGlobal(L,k,v.constData(),v.size());}
    static void setGlobal(const lua_State * const &L,const QString &k,const QByteArray &v) {setGlobal(L,k,v.constData(),v.size());}

    static void setGlobal(const lua_State * const &L,const char * const &k,const QString &v_) { const auto v=v_.toUtf8(); setGlobal(L,k,v.constData(),v.size()); }
    static void setGlobal(const lua_State * const &L,const std::string &k,const QString &v_) {const auto v=v_.toUtf8();setGlobal(L,k,v.constData(),v.size());}
    static void setGlobal(const lua_State * const &L,const QByteArray &k,const QString &v_) {const auto v=v_.toUtf8();setGlobal(L,k,v.constData(),v.size());}
    static void setGlobal(const lua_State * const &L,const QString &k,const QString &v_) {const auto v=v_.toUtf8();setGlobal(L,k,v.constData(),v.size());}

    void setGlobal( const char * const &k,const std::string &v) { setGlobal(k,v.c_str(),v.size()); }
    void setGlobal( const std::string &k,const std::string &v) {setGlobal(k,v.c_str(),v.size());}
    void setGlobal( const QByteArray &k,const std::string &v) {setGlobal(k,v.c_str(),v.size());}
    void setGlobal( const QString &k,const std::string &v) {setGlobal(k,v.c_str(),v.size());}

    void setGlobal( const char * const &k,const QByteArray &v) { setGlobal(k,v.constData(),v.size()); }
    void setGlobal( const std::string &k,const QByteArray &v) {setGlobal(k,v.constData(),v.size());}
    void setGlobal( const QByteArray &k,const QByteArray &v) {setGlobal(k,v.constData(),v.size());}
    void setGlobal( const QString &k,const QByteArray &v) {setGlobal(k,v.constData(),v.size());}

    void setGlobal( const char * const &k,const QString &v_) { const auto v=v_.toUtf8(); setGlobal(k,v.constData(),v.size()); }
    void setGlobal( const std::string &k,const QString &v_) {const auto v=v_.toUtf8();setGlobal(k,v.constData(),v.size());}
    void setGlobal( const QByteArray &k,const QString &v_) {const auto v=v_.toUtf8();setGlobal(k,v.constData(),v.size());}
    void setGlobal( const QString &k,const QString &v_) {const auto v=v_.toUtf8();setGlobal(k,v.constData(),v.size());}

    void setGlobal(const char * const &);
    void setGlobal(const char * const &,const char *,lua_Integer);
    void setGlobal(const char * const &,lua_Integer);
    void setGlobal(const char * const &,lua_Number);
    void setGlobal(const char * const &,bool);
    void setGlobal(const char * const &,lua_CFunction);
    void setGlobal(const char * const &,const QVariant &);

    void setGlobal(const std::string & v) { return setGlobal(v.c_str()); }
    void setGlobal(const std::string & v,const char * a,lua_Integer b) { return setGlobal(v.c_str(),a,b); }
    void setGlobal(const std::string & v,lua_Integer a) { return setGlobal(v.c_str(),a); }
    void setGlobal(const std::string & v,lua_Number a) { return setGlobal(v.c_str(),a); }
    void setGlobal(const std::string & v,bool a) { return setGlobal(v.c_str(),a); }
    void setGlobal(const std::string & v,lua_CFunction  a) { return setGlobal(v.c_str(),a); }
    void setGlobal(const std::string & v,const QVariant & a) { return setGlobal(v.c_str(),a); }

    void setGlobal(const QByteArray & v) { return setGlobal(v.constData()); }
    void setGlobal(const QByteArray & v,const char * a,lua_Integer b) { return setGlobal(v.constData(),a,b); }
    void setGlobal(const QByteArray & v,lua_Integer a) { return setGlobal(v.constData(),a); }
    void setGlobal(const QByteArray & v,lua_Number a) { return setGlobal(v.constData(),a); }
    void setGlobal(const QByteArray & v,bool a) { return setGlobal(v.constData(),a); }
    void setGlobal(const QByteArray & v,lua_CFunction  a) { return setGlobal(v.constData(),a); }
    void setGlobal(const QByteArray & v,const QVariant & a) { return setGlobal(v.constData(),a); }

    void setGlobal(const QString & v) { return setGlobal(v.toUtf8().constData()); }
    void setGlobal(const QString & v,const char * a,lua_Integer b) { return setGlobal(v.toUtf8().constData(),a,b); }
    void setGlobal(const QString & v,lua_Integer a) { return setGlobal(v.toUtf8().constData(),a); }
    void setGlobal(const QString & v,lua_Number a) { return setGlobal(v.toUtf8().constData(),a); }
    void setGlobal(const QString & v,bool a) { return setGlobal(v.toUtf8().constData(),a); }
    void setGlobal(const QString & v,lua_CFunction  a) { return setGlobal(v.toUtf8().constData(),a); }
    void setGlobal(const QString & v,const QVariant & a) { return setGlobal(v.toUtf8().constData(),a); }

    void setGlobal(const QVariant & v,const QVariant & a);

    static void setGlobal(const lua_State * const &,const char * const &);
    static void setGlobal(const lua_State * const &,const char * const &,const char *,lua_Integer);
    static void setGlobal(const lua_State * const &,const char * const &,lua_Integer);
    static void setGlobal(const lua_State * const &,const char * const &,lua_Number);
    static void setGlobal(const lua_State * const &,const char * const &,lua_CFunction);
    static void setGlobal(const lua_State * const &,const char * const &,bool);
    static void setGlobal(const lua_State * const &,const char * const &,const QVariant &);

    static void setGlobal(const lua_State * const &L,const std::string & v) { return setGlobal(L,v.c_str()); }
    static void setGlobal(const lua_State * const &L,const std::string & v,const char * a,lua_Integer b) { return setGlobal(L,v.c_str(),a,b); }
    static void setGlobal(const lua_State * const &L,const std::string & v,lua_Integer a) { return setGlobal(L,v.c_str(),a); }
    static void setGlobal(const lua_State * const &L,const std::string & v,lua_Number a) { return setGlobal(L,v.c_str(),a); }
    static void setGlobal(const lua_State * const &L,const std::string & v,bool a) { return setGlobal(L,v.c_str(),a); }
    static void setGlobal(const lua_State * const &L,const std::string & v,lua_CFunction  a) { return setGlobal(L,v.c_str(),a); }
    static void setGlobal(const lua_State * const &L,const std::string & v,const QVariant & a) { return setGlobal(L,v.c_str(),a); }

    static void setGlobal(const lua_State * const &L,const QByteArray & v) { return setGlobal(L,v.constData()); }
    static void setGlobal(const lua_State * const &L,const QByteArray & v,const char * a,lua_Integer b) { return setGlobal(L,v.constData(),a,b); }
    static void setGlobal(const lua_State * const &L,const QByteArray & v,lua_Integer a) { return setGlobal(L,v.constData(),a); }
    static void setGlobal(const lua_State * const &L,const QByteArray & v,lua_Number a) { return setGlobal(L,v.constData(),a); }
    static void setGlobal(const lua_State * const &L,const QByteArray & v,bool a) { return setGlobal(L,v.constData(),a); }
    static void setGlobal(const lua_State * const &L,const QByteArray & v,lua_CFunction  a) { return setGlobal(L,v.constData(),a); }
    static void setGlobal(const lua_State * const &L,const QByteArray & v,const QVariant & a) { return setGlobal(L,v.constData(),a); }

    static void setGlobal(const lua_State * const &L,const QString & v) { return setGlobal(L,v.toUtf8().constData()); }
    static void setGlobal(const lua_State * const &L,const QString & v,const char * a,lua_Integer b) { return setGlobal(L,v.toUtf8().constData(),a,b); }
    static void setGlobal(const lua_State * const &L,const QString & v,lua_Integer a) { return setGlobal(L,v.toUtf8().constData(),a); }
    static void setGlobal(const lua_State * const &L,const QString & v,lua_Number a) { return setGlobal(L,v.toUtf8().constData(),a); }
    static void setGlobal(const lua_State * const &L,const QString & v,bool a) { return setGlobal(L,v.toUtf8().constData(),a); }
    static void setGlobal(const lua_State * const &L,const QString & v,lua_CFunction  a) { return setGlobal(L,v.toUtf8().constData(),a); }
    static void setGlobal(const lua_State * const &L,const QString & v,const QVariant & a) { return setGlobal(L,v.toUtf8().constData(),a); }

    static void setGlobal(const lua_State * const &L,const QVariant & v);
    static void setGlobal(const lua_State * const &L,const QVariant & v,const QVariant & a);

    LUAType type(int) const;
    static LUAType type(const lua_State * const &,int);

    static LUAType getTable(const lua_State * const &,int table);
    static LUAType getTable(const lua_State * const &,int table,const QVariant &);
    static LUAType getTable(const lua_State * const &,int table,lua_Integer key);
    static LUAType getTable(const lua_State * const &,int table,const char * key);
    static LUAType getTable(const lua_State * const &,int table,const char * key,lua_Integer l);
    static LUAType getTable(const lua_State * const &L,int table,const std::string & key) { return getTable(L,table,key.c_str(),key.size()); }
    static LUAType getTable(const lua_State * const &L,int table,const QByteArray & key) { return getTable(L,table,key.constData(),key.size()); }
    static LUAType getTable(const lua_State * const &L,int table,const QString & key) { const auto key_=key.toUtf8(); return getTable(L,table,key_.constData(),key_.size()); }

    LUAType getTable(int table)const;
    LUAType getTable(int table,const QVariant &)const;
    LUAType getTable(int table,lua_Integer key)const;
    LUAType getTable(int table,const char * key)const;
    LUAType getTable(int table,const char * key,lua_Integer l)const;
    LUAType getTable(int table,const std::string & key) const { return getTable(table,key.c_str(),key.size()); }
    LUAType getTable(int table,const QByteArray & key) const { return getTable(table,key.constData(),key.size()); }
    LUAType getTable(int table,const QString & key) const { return getTable(table,key.toUtf8(),key.size()); }

    static void setTable(const lua_State * const &L_,int table);
    void setTable(int table);

    static void setTable(const lua_State * const &L_,int table,lua_Integer key,const QVariant &value);
    static void setTable(const lua_State * const &L_,int table,lua_Integer key,const char * value);
    static void setTable(const lua_State * const &L_,int table,lua_Integer key,const char * value,lua_Integer);
    static void setTable(const lua_State * const &L_,int table,lua_Integer key,bool value);
    static void setTable(const lua_State * const &L_,int table,lua_Integer key,lua_Integer value);
    static void setTable(const lua_State * const &L_,int table,lua_Integer key,lua_Number value);
    static void setTable(const lua_State * const &L_,int table,lua_Integer key,lua_CFunction);
    static void setTable(const lua_State * const &L_,int table,lua_Integer key,const std::string & v) { setTable(L_,table,key,v.c_str(),v.size()); }
    static void setTable(const lua_State * const &L_,int table,lua_Integer key,const QByteArray &v) { setTable(L_,table,key,v.constData(),v.size()); }
    static void setTable(const lua_State * const &L_,int table,lua_Integer key,const QString &v) { setTable(L_,table,key,v.toUtf8().constData()); }

    void setTable(int table,lua_Integer key,const QVariant &value);
    void setTable(int table,lua_Integer key,const char * value);
    void setTable(int table,lua_Integer key,const char * value,lua_Integer l);
    void setTable(int table,lua_Integer key,bool value);
    void setTable(int table,lua_Integer key,lua_Integer value);
    void setTable(int table,lua_Integer key,lua_Number value);
    void setTable(int table,lua_Integer key,lua_CFunction value);
    void setTable(int table,lua_Integer key,const std::string & v) { setTable(table,key,v.c_str(),v.size()); }
    void setTable(int table,lua_Integer key,const QByteArray &v) { setTable(table,key,v.constData(),v.size()); }
    void setTable(int table,lua_Integer key,const QString &v) { setTable(table,key,v.toUtf8().constData()); }

    static void setTable(const lua_State * const &L_,int table,const char * key,const QVariant & value);
    static void setTable(const lua_State * const &L_,int table,const char * key,const char * value);
    static void setTable(const lua_State * const &L_,int table,const char * key,const char * value,lua_Integer);
    static void setTable(const lua_State * const &L_,int table,const char * key,bool value);
    static void setTable(const lua_State * const &L_,int table,const char * key,lua_Integer value);
    static void setTable(const lua_State * const &L_,int table,const char * key,lua_Number value);
    static void setTable(const lua_State * const &L_,int table,const char * key,lua_CFunction);
    static void setTable(const lua_State * const &L_,int table,const char * key,const std::string & v) { setTable(L_,table,key,v.c_str(),v.size()); }
    static void setTable(const lua_State * const &L_,int table,const char * key,const QByteArray &v) { setTable(L_,table,key,v.constData(),v.size()); }
    static void setTable(const lua_State * const &L_,int table,const char * key,const QString &v) { setTable(L_,table,key,v.toUtf8().constData()); }

    void setTable(int table,const char * key,const QVariant & value);
    void setTable(int table,const char * key,const char * value);
    void setTable(int table,const char * key,const char * value,lua_Integer);
    void setTable(int table,const char * key,bool value);
    void setTable(int table,const char * key,lua_Integer value);
    void setTable(int table,const char * key,lua_Number value);
    void setTable(int table,const char * key,lua_CFunction);
    void setTable(int table,const char * key,const std::string & v) { setTable(table,key,v.c_str(),v.size()); }
    void setTable(int table,const char * key,const QByteArray &v) { setTable(table,key,v.constData(),v.size()); }
    void setTable(int table,const char * key,const QString &v) { setTable(table,key,v.toUtf8().constData()); }

    static void setTable(const lua_State * const &L_,int table,const char * key,lua_Integer,const QVariant & value);
    static void setTable(const lua_State * const &L_,int table,const char * key,lua_Integer,const char * value);
    static void setTable(const lua_State * const &L_,int table,const char * key,lua_Integer,const char * value,lua_Integer);
    static void setTable(const lua_State * const &L_,int table,const char * key,lua_Integer,bool value);
    static void setTable(const lua_State * const &L_,int table,const char * key,lua_Integer,lua_Integer value);
    static void setTable(const lua_State * const &L_,int table,const char * key,lua_Integer,lua_Number value);
    static void setTable(const lua_State * const &L_,int table,const char * key,lua_Integer,lua_CFunction);
    static void setTable(const lua_State * const &L_,int table,const char * key,lua_Integer l,const std::string & v) { setTable(L_,table,key,l,v.c_str(),v.size()); }
    static void setTable(const lua_State * const &L_,int table,const char * key,lua_Integer l,const QByteArray &v) { setTable(L_,table,key,l,v.constData(),v.size()); }
    static void setTable(const lua_State * const &L_,int table,const char * key,lua_Integer l,const QString &v) { setTable(L_,table,key,l,v.toUtf8().constData()); }

    void setTable(int table,const char * key,lua_Integer,const QVariant & value);
    void setTable(int table,const char * key,lua_Integer,const char * value);
    void setTable(int table,const char * key,lua_Integer,const char * value,lua_Integer);
    void setTable(int table,const char * key,lua_Integer,bool value);
    void setTable(int table,const char * key,lua_Integer,lua_Integer value);
    void setTable(int table,const char * key,lua_Integer,lua_Number value);
    void setTable(int table,const char * key,lua_Integer,lua_CFunction);
    void setTable(int table,const char * key,lua_Integer l,const std::string & v) { setTable(table,key,l,v.c_str(),v.size()); }
    void setTable(int table,const char * key,lua_Integer l,const QByteArray &v) { setTable(table,key,l,v.constData(),v.size()); }
    void setTable(int table,const char * key,lua_Integer l,const QString &v) { setTable(table,key,l,v.toUtf8().constData()); }

    static void setTable(const lua_State * const &L_,int table,const std::string & key,const QVariant & value) { setTable(L_,table,key.c_str(),value); }
    static void setTable(const lua_State * const &L_,int table,const std::string & key,const char * value) { setTable(L_,table,key.c_str(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const std::string & key,const char * value,lua_Integer l) { setTable(L_,table,key.c_str(),key.size(),value,l); }
    static void setTable(const lua_State * const &L_,int table,const std::string & key,bool value) { setTable(L_,table,key.c_str(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const std::string & key,lua_Integer value) { setTable(L_,table,key.c_str(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const std::string & key,lua_Number value) { setTable(L_,table,key.c_str(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const std::string & key,lua_CFunction value) { setTable(L_,table,key.c_str(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const std::string & key,const std::string & value) { setTable(L_,table,key.c_str(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const std::string & key,const QByteArray &value) { setTable(L_,table,key.c_str(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const std::string & key,const QString &value_) { const auto value=value_.toUtf8();  setTable(L_,table,key.c_str(),key.size(),value); }

    void setTable(int table,const std::string & key,const QVariant & value) { setTable(table,key.c_str(),key.size(),value); }
    void setTable(int table,const std::string & key,const char * value) { setTable(table,key.c_str(),key.size(),value); }
    void setTable(int table,const std::string & key,const char * value,lua_Integer l) { setTable(table,key.c_str(),key.size(),value,l); }
    void setTable(int table,const std::string & key,bool value) { setTable(table,key.c_str(),key.size(),value); }
    void setTable(int table,const std::string & key,lua_Integer value) { setTable(table,key.c_str(),key.size(),value); }
    void setTable(int table,const std::string & key,lua_Number value) { setTable(table,key.c_str(),key.size(),value); }
    void setTable(int table,const std::string & key,lua_CFunction value) { setTable(table,key.c_str(),key.size(),value); }
    void setTable(int table,const std::string & key,const std::string & value) { setTable(table,key.c_str(),key.size(),value); }
    void setTable(int table,const std::string & key,const QByteArray &value) { setTable(table,key.c_str(),key.size(),value); }
    void setTable(int table,const std::string & key,const QString &value) { setTable(table,key.c_str(),key.size(),value); }

    static void setTable(const lua_State * const &L_,int table,const QByteArray & key,const QVariant & value) { setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QByteArray & key,const char * value) { setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QByteArray & key,const char * value,lua_Integer l) { setTable(L_,table,key.constData(),key.size(),value,l); }
    static void setTable(const lua_State * const &L_,int table,const QByteArray & key,bool value) { setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QByteArray & key,lua_Integer value) { setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QByteArray & key,lua_Number value) { setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QByteArray & key,lua_CFunction value) { setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QByteArray & key,const std::string & value) { setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QByteArray & key,const QByteArray &value) { setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QByteArray & key,const QString &value) { setTable(L_,table,key.constData(),key.size(),value); }

    void setTable(int table,const QByteArray & key,const QVariant & value) { setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QByteArray & key,const char * value) { setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QByteArray & key,const char * value,lua_Integer l) { setTable(table,key.constData(),key.size(),value,l); }
    void setTable(int table,const QByteArray & key,bool value) { setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QByteArray & key,lua_Integer value) { setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QByteArray & key,lua_Number value) { setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QByteArray & key,lua_CFunction value) { setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QByteArray & key,const std::string & value) { setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QByteArray & key,const QByteArray &value) { setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QByteArray & key,const QString &value) { setTable(table,key.constData(),key.size(),value); }

    static void setTable(const lua_State * const &L_,int table,const QString & key_,const QVariant & value) { const auto key=key_.toUtf8(); setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QString & key_,const char * value) { const auto key=key_.toUtf8(); setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QString & key_,const char * value,lua_Integer l) { const auto key=key_.toUtf8(); setTable(L_,table,key.constData(),key.size(),value,l); }
    static void setTable(const lua_State * const &L_,int table,const QString & key_,bool value) { const auto key=key_.toUtf8(); setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QString & key_,lua_Integer value) { const auto key=key_.toUtf8(); setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QString & key_,lua_Number value) { const auto key=key_.toUtf8(); setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QString & key_,lua_CFunction value) { const auto key=key_.toUtf8(); setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QString & key_,const std::string & value) { const auto key=key_.toUtf8(); setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QString & key_,const QByteArray &value) { const auto key=key_.toUtf8(); setTable(L_,table,key.constData(),key.size(),value); }
    static void setTable(const lua_State * const &L_,int table,const QString & key_,const QString &value) { const auto key=key_.toUtf8(); setTable(L_,table,key.constData(),key.size(),value); }

    static void setTable(const lua_State * const &L_,int table,const QVariant & key,const QVariant & value);
    void setTable(int table,const QVariant & key,const QVariant & value);

    void setTable(int table,const QString & key_,const QVariant & value) { const auto key=key_.toUtf8();  setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QString & key_,const char * value) { const auto key=key_.toUtf8();  setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QString & key_,const char * value,lua_Integer l) { const auto key=key_.toUtf8();  setTable(table,key.constData(),key.size(),value,l); }
    void setTable(int table,const QString & key_,bool value) { const auto key=key_.toUtf8();  setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QString & key_,lua_Integer value) { const auto key=key_.toUtf8();  setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QString & key_,lua_Number value) { const auto key=key_.toUtf8();  setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QString & key_,lua_CFunction value) { const auto key=key_.toUtf8();  setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QString & key_,const std::string & value) { const auto key=key_.toUtf8();  setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QString & key_,const QByteArray &value) { const auto key=key_.toUtf8();  setTable(table,key.constData(),key.size(),value); }
    void setTable(int table,const QString & key_,const QString &value) { const auto key=key_.toUtf8();  setTable(table,key.constData(),key.size(),value); }

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
    if (b==e) { setTable(L_,table,*ob,v); return true; }
    /*get the last table*/
    for (; b!=e; ob=b++) {
        if (getTable(L_,table,*ob)==LUAType::TABLE) { table=getTop(L_); }
        else {
            pop(L_); pushKey(L_,*ob);  newTable(L_); setTable(L_,table);
            if (getTable(L_,table,*ob)==LUAType::TABLE) { table=getTop(L_); }
            else { errorCode="it is not a table"; return false; }
        }
    }
    setTable(L_,table,*ob,v);
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


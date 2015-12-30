#include "PureLuaCore.hpp"

namespace cct {
class PureLuaCore::__PureLuaCorePrivate {
public:
    std::shared_ptr< lua_State > L;
    std::string lastError;
};

/* */
void PureLuaCore::__PureLuaCore(std::shared_ptr<lua_State> L__,void * d) {

    dataCore=std::make_shared<__PureLuaCorePrivate>();

    if (d) {
        dataCore->L=std::move(L__);
    }
    else {
        auto * L_=luaL_newstate();
        if (L_) {
            luaL_openlibs(L_);
            dataCore->L=std::shared_ptr<lua_State>(L_,[](lua_State * _L) { lua_close(_L); });
        }
    }

}

/* */
PureLuaCore::~PureLuaCore() {
}

/* */
std::shared_ptr<lua_State> PureLuaCore::getLuaState()const {
    return dataCore->L;
}

/* */
void PureLuaCore::setLuaState(std::shared_ptr<lua_State> v) {
    dataCore->L=std::move(v);
}

/* */
const std::string & PureLuaCore::getError()const {
    return dataCore->lastError;
}

/* */
void PureLuaCore::clearError() {
    dataCore->lastError.clear();
}

/* */
auto PureLuaCore::getGlobal(const char * const & v,lua_Integer) const->LUAType {
    auto * L=dataCore->L.get();
    if (L) { return static_cast<LUAType>(lua_getglobal(L,v)); }
    return LUAType::NONE;
}

/* */
void PureLuaCore::push(const lua_State * const &L_) { lua_pushvalue(const_cast<lua_State * const &>(L_),-1); }
void PureLuaCore::push() { auto * L=dataCore->L.get(); if (L) { push(L); } }

/* */
void PureLuaCore::push(const lua_State * const &L_,int i) { lua_pushvalue(const_cast<lua_State * const &>(L_),i); }
void PureLuaCore::push(int i) { auto * L=dataCore->L.get(); if (L) { push(L,i); } }

/* */
void PureLuaCore::pop(const lua_State * const &L_,int n) { lua_pop(const_cast<lua_State * const &>(L_),n); }
void PureLuaCore::pop(int n) { auto * L=dataCore->L.get(); if (L) { pop(L,n); } }

/* */
void PureLuaCore::pop(const lua_State * const &L_) { lua_pop(const_cast<lua_State * const &>(L_),1); }
void PureLuaCore::pop() { auto * L=dataCore->L.get(); if (L) { pop(L); } }

/* */
void PureLuaCore::newTable(const lua_State * const &L_) { lua_newtable(const_cast<lua_State * const &>(L_)); }
void PureLuaCore::newTable() { auto * L=dataCore->L.get(); if (L) { newTable(L); } }

/* */
auto PureLuaCore::getGlobal(const QVariant & v)const->LUAType {
    auto * L=dataCore->L.get();
    if (v.isValid()) {
        if (L) {

            switch (v.userType()) {
            case QVariant::String: return getGlobal(v.toString()); break;
            case QVariant::ByteArray: return getGlobal(v.toByteArray()); break;
            }

            if (v.canConvert(qMetaTypeId<QString>())) {
                return getGlobal(v.value<QString>());
            }

            if (v.canConvert(qMetaTypeId<QByteArray>())) {
                return getGlobal(v.value<QByteArray>());
            }

        }
    }
    if (L) { lua_pushnil(L); return LUAType::NIL; }
    return LUAType::NONE;
}

/* */
PureLuaCore::LUAType PureLuaCore::type(int i) const {
    auto * L=dataCore->L.get();
    if (L) { return type(L,i); }
    return LUAType::NONE;
}

/* */
PureLuaCore::LUAType PureLuaCore::type(const lua_State * const & v,int i) {
    return static_cast<LUAType>(lua_type(const_cast<lua_State * const &>(v),i));
}

/* */
void PureLuaCore::setGlobal(const char * const & n,const char * s,lua_Integer l) {
    auto * L=dataCore->L.get();
    if (L) {
        lua_pushlstring(L,s,l);
        lua_setglobal(L,n);
    }
}

/* */
void PureLuaCore::setGlobal(const char * const & n,lua_Integer i) {
    auto * L=dataCore->L.get();
    if (L) {
        lua_pushinteger(L,i);
        lua_setglobal(L,n);
    }
}

/* */
void PureLuaCore::setGlobal(const char * const & n,lua_Number v) {
    auto * L=dataCore->L.get();
    if (L) {
        lua_pushnumber(L,v);
        lua_setglobal(L,n);
    }
}

/* */
void PureLuaCore::setGlobal(const char * const & n,lua_CFunction f) {
    auto * L=dataCore->L.get();
    if (L) {
        lua_pushcfunction(L,f);
        lua_setglobal(L,n);
    }
}

/* */
void PureLuaCore::setGlobal(const lua_State * const & L_,const char * const & a,const char * b,lua_Integer c) {
    auto & L=const_cast<lua_State * const &>(L_);
    lua_pushlstring(L,b,c);
    lua_setglobal(L,a);
}

/* */
void PureLuaCore::setGlobal(const lua_State * const & L_,const char * const & a,lua_Integer b) {
    auto & L=const_cast<lua_State * const &>(L_);
    lua_pushinteger(L,b);
    lua_setglobal(L,a);
}

/* */
void PureLuaCore::setGlobal(const lua_State * const & L_,const char * const & a,lua_Number b) {
    auto & L=const_cast<lua_State * const &>(L_);
    lua_pushnumber(L,b);
    lua_setglobal(L,a);
}

/* */
void PureLuaCore::setGlobal(const lua_State * const & L_,const char * const & a,lua_CFunction b) {
    auto & L=const_cast<lua_State * const &>(L_);
    lua_pushcfunction(L,b);
    lua_setglobal(L,a);
}

/* */
void PureLuaCore::setGlobal(const char * const & n,bool b) {
    auto * L=dataCore->L.get();
    if (L) {
        lua_pushboolean(L,b);
        lua_setglobal(L,n);
    }
}

/* */
void PureLuaCore::setGlobal(const lua_State * const & L_,const char * const & n,bool b) {
    auto & L=const_cast<lua_State * const &>(L_);
    lua_pushboolean(L,b);
    lua_setglobal(L,n);
}

/* */
void PureLuaCore::setGlobal(const char * const & n,const QVariant & v) {
    auto * L=dataCore->L.get();
    if (L) { setGlobal(L,n,v); }
}

/* */
void PureLuaCore::setGlobal(const lua_State * const & L_,const char * const & n) {
    auto & L=const_cast<lua_State * const &>(L_);
    return lua_setglobal(L,n);
}

/* */
void  PureLuaCore::setGlobal(const char * const & n) {
    auto * L=dataCore->L.get();
    if (L) { return lua_setglobal(L,n); }
}

/* */
void PureLuaCore::setGlobal(const lua_State * const & L_,const char * const & n,const QVariant & v) {

    if (v.userType()==qMetaTypeId<lua_CFunction>()) {
        return setGlobal(L_,n,v.value<lua_CFunction>());
    }

    switch (v.userType()) {
    case QVariant::String:return setGlobal(L_,n,v.toString().toUtf8().constData());
    case QVariant::ByteArray:return setGlobal(L_,n,v.toByteArray().constData());
    case QVariant::Bool:return setGlobal(L_,n,v.toBool());
    case QVariant::Double:return setGlobal(L_,n,v.toDouble());
    case QVariant::LongLong:return setGlobal(L_,n,static_cast<lua_Integer>(v.toLongLong()));
    case QVariant::ULongLong:return setGlobal(L_,n,static_cast<lua_Integer>(v.toULongLong()));
    case QVariant::Int:return setGlobal(L_,n,static_cast<lua_Integer>(v.toInt()));
    case QVariant::UInt:return setGlobal(L_,n,static_cast<lua_Integer>(v.toUInt()));

    }

    if (v.canConvert(qMetaTypeId<QString>())) {
        return setGlobal(L_,n,v.toString());
    }

}

/* */
PureLuaCore::LUAType PureLuaCore::getGlobal(const lua_State * const & L_,const QVariant & v) {
    auto & L=const_cast<lua_State * const &>(L_);

    if (v.isValid()) {

        switch (v.userType()) {
        case QVariant::String: return getGlobal(L,v.toString()); break;
        case QVariant::ByteArray: return getGlobal(L,v.toByteArray()); break;
        }

        if (v.canConvert(qMetaTypeId<QString>())) {
            return getGlobal(L,v.value<QString>());
        }

        if (v.canConvert(qMetaTypeId<QByteArray>())) {
            return getGlobal(L,v.value<QByteArray>());
        }

    }

    lua_pushnil(L);
    return LUAType::NIL;

}

/* */
PureLuaCore::LUAType PureLuaCore::getGlobal(
        const lua_State * const & L,
        const char * const & n,
        lua_Integer) {
    return static_cast<LUAType>(lua_getglobal(const_cast<lua_State * const &>(L),n));
}

/* */
void PureLuaCore::setGlobal(const lua_State * const &L_,const QVariant & v) {
    auto & L=const_cast<lua_State * const &>(L_);

    if (v.isValid()) {

        switch (v.userType()) {
        case QVariant::String:   setGlobal(L,v.toString()); return; break;
        case QVariant::ByteArray:setGlobal(L,v.toByteArray()); return; break;
        }

        if (v.canConvert(qMetaTypeId<QString>())) {
            return setGlobal(L,v.value<QString>());
        }

        if (v.canConvert(qMetaTypeId<QByteArray>())) {
            return setGlobal(L,v.value<QByteArray>());
        }

    }
}

/* */
void PureLuaCore::setGlobal(const lua_State * const &L_,const QVariant & v,const QVariant & a) {
    auto & L=const_cast<lua_State * const &>(L_);

    if (v.isValid()) {

        switch (v.userType()) {
        case QVariant::String:   setGlobal(L,v.toString(),a); return; break;
        case QVariant::ByteArray:setGlobal(L,v.toByteArray(),a); return; break;
        }

        if (v.canConvert(qMetaTypeId<QString>())) {
            return setGlobal(L,v.value<QString>(),a);
        }

        if (v.canConvert(qMetaTypeId<QByteArray>())) {
            return setGlobal(L,v.value<QByteArray>(),a);
        }

    }
}

/* */
void PureLuaCore::setGlobal(const QVariant & v,const QVariant & a) {
    auto * L=dataCore->L.get();

    if (L==nullptr) { return; }

    if (v.isValid()) {

        switch (v.userType()) {
        case QVariant::String:   setGlobal(L,v.toString(),a); return; break;
        case QVariant::ByteArray:setGlobal(L,v.toByteArray(),a); return; break;
        }

        if (v.canConvert(qMetaTypeId<QString>())) {
            return setGlobal(L,v.value<QString>(),a);
        }

        if (v.canConvert(qMetaTypeId<QByteArray>())) {
            return setGlobal(L,v.value<QByteArray>(),a);
        }

    }
}

PureLuaCore::LUAType PureLuaCore::getTable(const lua_State * const & L,int index) {
    return static_cast<LUAType>(lua_gettable(const_cast<lua_State * const &>(L),index));
}

PureLuaCore::LUAType PureLuaCore::getTable(const lua_State * const & L_,int table,lua_Integer key_) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushinteger(L,key_);
    return getTable(L_,table);
}

PureLuaCore::LUAType PureLuaCore::getTable(const lua_State * const & L_,int table,const char * key,lua_Integer l) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushlstring(L,key,l);
    return getTable(L_,table);
}

PureLuaCore::LUAType PureLuaCore::getTable(const lua_State * const &L_,int table,const char * key) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushstring(L,key);
    return getTable(L_,table);
}

PureLuaCore::LUAType PureLuaCore::getTable(int a) const {
    auto * L=dataCore->L.get();
    if (L) { return getTable(L,a); }
    return LUAType::NIL;
}
PureLuaCore::LUAType PureLuaCore::getTable(int a,lua_Integer b)const {
    auto * L=dataCore->L.get();
    if (L) { return getTable(L,a,b); }
    return LUAType::NIL;
}

PureLuaCore::LUAType PureLuaCore::getTable(int a,const char * b) const {
    auto * L=dataCore->L.get();
    if (L) { return getTable(L,a,b); }
    return LUAType::NIL;
}
PureLuaCore::LUAType PureLuaCore::getTable(int a,const char * b,lua_Integer c) const {
    auto * L=dataCore->L.get();
    if (L) { return getTable(L,a,b,c); }
    return LUAType::NIL;
}

PureLuaCore::LUAType PureLuaCore::getTable(int table,const QVariant & key_)const {
    auto * L=dataCore->L.get();
    if (L) { return getTable(L,table,key_); }
    return LUAType::NIL;
}

PureLuaCore::LUAType PureLuaCore::getTable(const lua_State * const & L_,int table,const QVariant & key_) {
    pushKey(L_,key_);
    return getTable(L_,table);
}

void PureLuaCore::setTable(const lua_State * const &L_,int table) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_settable(L,table);
}

void PureLuaCore::setTable(const lua_State * const &L_,int table,const char * key,const char * value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushstring(L,key);
    lua_pushstring(L,value);
    setTable(L_,table);
}

void PureLuaCore::setTable(const lua_State * const &L_,int table,const char * key,lua_Integer a,const char * value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushlstring(L,key,a);
    lua_pushstring(L,value);
    setTable(L_,table);
}

void PureLuaCore::setTable(const lua_State * const &L_,int table,const char * key,const char * value,lua_Integer b) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushstring(L,key);
    lua_pushlstring(L,value,b);
    setTable(L_,table);
}

void PureLuaCore::setTable(const lua_State * const &L_,int table,const char * key,lua_Integer a,const char * value,lua_Integer b) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushlstring(L,key,a);
    lua_pushlstring(L,value,b);
    setTable(L_,table);
}

void PureLuaCore::setTable(const lua_State * const &L_,int table,lua_Integer key,const char * value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushinteger(L,key);
    lua_pushstring(L,value);
    setTable(L_,table);
}
void PureLuaCore::setTable(const lua_State * const &L_,int table,lua_Integer key,const char * value,lua_Integer a) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushinteger(L,key);
    lua_pushlstring(L,value,a);
    setTable(L_,table);
}

void PureLuaCore::setTable(const lua_State * const &L_,int table,lua_Integer key,bool value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushinteger(L,key);
    lua_pushboolean(L,value);
    setTable(L_,table);
}
void PureLuaCore::setTable(const lua_State * const &L_,int table,lua_Integer key,lua_Integer value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushinteger(L,key);
    lua_pushinteger(L,value);
    setTable(L_,table);
}
void PureLuaCore::setTable(const lua_State * const &L_,int table,lua_Integer key,lua_Number value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushinteger(L,key);
    lua_pushnumber(L,value);
    setTable(L_,table);
}
void PureLuaCore::setTable(const lua_State * const &L_,int table,lua_Integer key,lua_CFunction value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushinteger(L,key);
    lua_pushcfunction(L,value);
    setTable(L_,table);
}

void PureLuaCore::setTable(const lua_State * const &L_,int table,const char * key,bool value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushstring(L,key);
    lua_pushboolean(L,value);
    setTable(L_,table);
}
void PureLuaCore::setTable(const lua_State * const &L_,int table,const char * key,lua_Integer value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushstring(L,key);
    lua_pushinteger(L,value);
    setTable(L_,table);
}
void PureLuaCore::setTable(const lua_State * const &L_,int table,const char * key,lua_Number value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushstring(L,key);
    lua_pushnumber(L,value);
    setTable(L_,table);
}
void PureLuaCore::setTable(const lua_State * const &L_,int table,const char * key,lua_CFunction value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushstring(L,key);
    lua_pushcfunction(L,value);
    setTable(L_,table);
}

void PureLuaCore::setTable(const lua_State * const &L_,int table,const char * key,lua_Integer a,bool value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushlstring(L,key,a);
    lua_pushboolean(L,value);
    setTable(L_,table);
}
void PureLuaCore::setTable(const lua_State * const &L_,int table,const char * key,lua_Integer a,lua_Integer value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushlstring(L,key,a);
    lua_pushinteger(L,value);
    setTable(L_,table);
}
void PureLuaCore::setTable(const lua_State * const &L_,int table,const char * key,lua_Integer a,lua_Number value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushlstring(L,key,a);
    lua_pushnumber(L,value);
    setTable(L_,table);
}
void PureLuaCore::setTable(const lua_State * const &L_,int table,const char * key,lua_Integer a,lua_CFunction value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushlstring(L,key,a);
    lua_pushcfunction(L,value);
    setTable(L_,table);
}

void PureLuaCore::setTable(int table) {
    auto * L=dataCore->L.get();
    if (L) { setTable(L,table); }
}

void PureLuaCore::setTable(int table,lua_Integer key,const char * value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,value); }
void PureLuaCore::setTable(int table,lua_Integer key,const char * value,lua_Integer l) { auto * L=dataCore->L.get(); if (L==nullptr) { return; }  setTable(L,table,key,value,l); }
void PureLuaCore::setTable(int table,lua_Integer key,bool value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,value); }
void PureLuaCore::setTable(int table,lua_Integer key,lua_Integer value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,value); }
void PureLuaCore::setTable(int table,lua_Integer key,lua_Number value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,value); }
void PureLuaCore::setTable(int table,lua_Integer key,lua_CFunction value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,value); }

void PureLuaCore::setTable(int table,const char * key,const char * value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,value); }
void PureLuaCore::setTable(int table,const char * key,const char * value,lua_Integer l) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,value,l); }
void PureLuaCore::setTable(int table,const char * key,bool value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,value); }
void PureLuaCore::setTable(int table,const char * key,lua_Integer value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; }  setTable(L,table,key,value); }
void PureLuaCore::setTable(int table,const char * key,lua_Number value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,value); }
void PureLuaCore::setTable(int table,const char * key,lua_CFunction value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,value); }

void PureLuaCore::setTable(int table,const char * key,lua_Integer l,const char * value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,l,value); }
void PureLuaCore::setTable(int table,const char * key,lua_Integer l,const char * value,lua_Integer ll) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,l,value,ll); }
void PureLuaCore::setTable(int table,const char * key,lua_Integer l,bool value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,l,value); }
void PureLuaCore::setTable(int table,const char * key,lua_Integer l,lua_Integer value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,l,value); }
void PureLuaCore::setTable(int table,const char * key,lua_Integer l,lua_Number value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,l,value); }
void PureLuaCore::setTable(int table,const char * key,lua_Integer l,lua_CFunction value) { auto * L=dataCore->L.get(); if (L==nullptr) { return; } setTable(L,table,key,l,value); }

void PureLuaCore::pushValue(const lua_State * const & L_,const char * value) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushstring(L,value);
}
void PureLuaCore::pushValue(const lua_State * const & L_,const char * value,lua_Integer l) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushlstring(L,value,l);
}
void PureLuaCore::pushValue(const lua_State * const & L_,lua_Integer v) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushinteger(L,v);
}
void PureLuaCore::pushValue(const lua_State * const & L_,lua_Number v) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushnumber(L,v);
}
void PureLuaCore::pushValue(const lua_State * const & L_,bool v) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushboolean(L,v);
}
void PureLuaCore::pushValue(const lua_State * const & L_,lua_CFunction v) {
    auto * L=const_cast<lua_State * const &>(L_);
    lua_pushcfunction(L,v);
}

void PureLuaCore::pushValue(const lua_State * const & L_,const QVariant &v) {

    if (false==v.isValid()) { return; }

    if (v.userType()==qMetaTypeId<lua_CFunction>()) {
        return pushValue(L_,v.value<lua_CFunction>());
    }

    switch (v.userType()) {
    case QVariant::String:return pushValue(L_,v.toString());
    case QVariant::ByteArray:return pushValue(L_,v.toByteArray());
    case QVariant::Bool:return pushValue(L_,v.toBool());
    case QVariant::Double:return pushValue(L_,v.toDouble());
    case QVariant::LongLong:return pushValue(L_,static_cast<lua_Integer>(v.toLongLong()));
    case QVariant::ULongLong:return pushValue(L_,static_cast<lua_Integer>(v.toULongLong()));
    case QVariant::Int:return pushValue(L_,static_cast<lua_Integer>(v.toInt()));
    case QVariant::UInt:return pushValue(L_,static_cast<lua_Integer>(v.toUInt()));

    }

    if (v.canConvert(qMetaTypeId<lua_Number>())) {
        return pushValue(L_,v.value<lua_Number>());
    }

    if (v.canConvert(qMetaTypeId<lua_Integer>())) {
        return pushValue(L_,v.value<lua_Integer>());
    }

    if (v.canConvert(qMetaTypeId<QString>())) {
        return pushValue(L_,v.toString());
    }

    if (v.canConvert(qMetaTypeId<QByteArray>())) {
        return pushValue(L_,v.toByteArray());
    }

}

void PureLuaCore::setTable(const lua_State * const &L_,int table,const char * key,lua_Integer l,const QVariant & value) {
    pushKey(L_,key,l);
    pushValue(L_,value);
    setTable(L_,table);
}

void PureLuaCore::pushKey(const lua_State * const & L_,const QVariant &v) {

    if (false==v.isValid()) { return; }

    switch (v.userType()) {
    case QVariant::String:return pushValue(L_,v.toString());
    case QVariant::ByteArray:return pushValue(L_,v.toByteArray());
    case QVariant::Bool:return pushValue(L_,v.toBool());
    case QVariant::Double:return pushKey(L_,v.toDouble());
    case QVariant::LongLong:return pushValue(L_,static_cast<lua_Integer>(v.toLongLong()));
    case QVariant::ULongLong:return pushValue(L_,static_cast<lua_Integer>(v.toULongLong()));
    case QVariant::Int:return pushValue(L_,static_cast<lua_Integer>(v.toInt()));
    case QVariant::UInt:return pushValue(L_,static_cast<lua_Integer>(v.toUInt()));

    }

    if (v.canConvert(qMetaTypeId<lua_Number>())) {
        return pushKey(L_,v.value<lua_Number>());
    }

    if (v.canConvert(qMetaTypeId<lua_Integer>())) {
        return pushValue(L_,v.value<lua_Integer>());
    }

    if (v.canConvert(qMetaTypeId<QString>())) {
        return pushValue(L_,v.toString());
    }

    if (v.canConvert(qMetaTypeId<QByteArray>())) {
        return pushValue(L_,v.toByteArray());
    }

}
void PureLuaCore::pushKey(const lua_State * const & L_,const char * value) { pushValue(L_,value); }
void PureLuaCore::pushKey(const lua_State * const & L_,const char * value,lua_Integer l) { pushValue(L_,value,l); }
void PureLuaCore::pushKey(const lua_State * const & L_,lua_Integer v) { pushValue(L_,v); }
void PureLuaCore::pushKey(const lua_State * const & L_,lua_Number v) { pushValue(L_,static_cast<lua_Integer>(v)); }

void PureLuaCore::setTable(int table,const char * key,lua_Integer l,const QVariant & value) {
    auto * L_=dataCore->L.get(); if (L_==nullptr) { return; }
    pushKey(L_,key,l);
    pushValue(L_,value);
    setTable(L_,table);
}

void PureLuaCore::setTable(int table,lua_Integer key,const QVariant &value) {
    auto * L_=dataCore->L.get(); if (L_==nullptr) { return; }
    pushKey(L_,key);
    pushValue(L_,value);
    setTable(L_,table);
}
void PureLuaCore::setTable(int table,const char * key,const QVariant & value) {
    auto * L_=dataCore->L.get(); if (L_==nullptr) { return; }
    pushKey(L_,key);
    pushValue(L_,value);
    setTable(L_,table);
}

void PureLuaCore::setTable(const lua_State * const &L_,int table,const char * key,const QVariant & value) {
    pushKey(L_,key);
    pushValue(L_,value);
    setTable(L_,table);
}
void PureLuaCore::setTable(const lua_State * const &L_,int table,lua_Integer key,const QVariant &value) {
    pushKey(L_,key);
    pushValue(L_,value);
    setTable(L_,table);
}

void PureLuaCore::pushValue(const QVariant &value) { auto * L=dataCore->L.get(); if (L) { pushValue(L,value); } }
void PureLuaCore::pushValue(const char * value) { auto * L=dataCore->L.get(); if (L) { pushValue(L,value); } }
void PureLuaCore::pushValue(const char * value,lua_Integer l) { auto * L=dataCore->L.get(); if (L) { pushValue(L,value,l); } }
void PureLuaCore::pushValue(lua_Integer value) { auto * L=dataCore->L.get(); if (L) { pushValue(L,value); } }
void PureLuaCore::pushValue(lua_Number value) { auto * L=dataCore->L.get(); if (L) { pushValue(L,value); } }
void PureLuaCore::pushValue(bool value) { auto * L=dataCore->L.get(); if (L) { pushValue(L,value); } }
void PureLuaCore::pushValue(lua_CFunction value) { auto * L=dataCore->L.get(); if (L) { pushValue(L,value); } }

void PureLuaCore::pushKey(const QVariant &value) { auto * L=dataCore->L.get(); if (L) { pushValue(L,value); } }
void PureLuaCore::pushKey(const char * value) { pushValue(value); }
void PureLuaCore::pushKey(const char * value,lua_Integer l) { pushValue(value,l); }
void PureLuaCore::pushKey(lua_Integer v) { pushValue(v); }
void PureLuaCore::pushKey(lua_Number value) { auto * L=dataCore->L.get(); if (L) { pushValue(L,value); } }

void PureLuaCore::setTable(const lua_State * const &L_,int table,const QVariant & key,const QVariant & value) {
    pushKey(L_,key);
    pushValue(L_,value);
    setTable(L_,table);
}

void PureLuaCore::setTable(int table,const QVariant & key,const QVariant & value) {
    auto * L=dataCore->L.get();
    if (L) { setTable(L,table,key,value); }
}

int PureLuaCore::getTop(const lua_State * const &L_) {
    return lua_gettop(const_cast<lua_State * const &>(L_));
}

void PureLuaCore::setTop(const lua_State * const &L_,int v) {
    lua_settop(const_cast<lua_State * const &>(L_),v);
}

int PureLuaCore::getTop()const {
    auto * L=dataCore->L.get();
    if (L) { return getTop(L); }
    return 0;
}

void PureLuaCore::setTop(int v) {
    auto * L=dataCore->L.get();
    if (L) { setTop(L,v); }
}

void PureLuaCore::setError(std::string && v) const {
    dataCore->lastError=std::move(v);
}

int PureLuaCore::absIndex(const lua_State * const &L_,int i) { return lua_absindex(const_cast<lua_State * const &>(L_),i); }
int PureLuaCore::absIndex(int i)const { auto * L=dataCore->L.get(); if (L) { return absIndex(L,i); }return i; }

void PureLuaCore::pushNIL(const lua_State * const &L_) { lua_pushnil(const_cast<lua_State * const &>(L_)); }
void PureLuaCore::pushNIL() { auto * L=dataCore->L.get(); if (L) { pushNIL(L); } }

void PureLuaCore::copy(const lua_State * const &L_,int from,int to) { lua_copy(const_cast<lua_State * const &>(L_),from,to); }
void PureLuaCore::copy(int from,int to) { auto * L=dataCore->L.get(); if (L) { copy(L,from,to); } }

QVariant PureLuaCore::toQVariant(const lua_State * const &L_,int n) {
    auto & L=const_cast<lua_State * const &>(L_);
    LUAType type_=type(L_,n);
    switch (type_) {
    case PureLuaCore::LUAType::NONE:break;
    case PureLuaCore::LUAType::NIL:break;
    case PureLuaCore::LUAType::BOOLEAN:return QVariant(static_cast<bool>(lua_toboolean(L,n))); break;
    case PureLuaCore::LUAType::LIGHTUSERDATA:break;
    case PureLuaCore::LUAType::NUMBER:
        if (lua_isinteger(L,n)) { return QVariant(lua_tointeger(L,n)); }
        else { return QVariant(lua_tonumber(L,n)); }
        break;
    case PureLuaCore::LUAType::STRING: {
        size_t n_; const char * data_=lua_tolstring(L,n,&n_); return QString::fromUtf8(data_,n_);
    }break;
    case PureLuaCore::LUAType::TABLE:break;
    case PureLuaCore::LUAType::FUNCTION: {
        return QVariant::fromValue(lua_tocfunction(L,n));
    }break;
    case PureLuaCore::LUAType::USERDATA:break;
    case PureLuaCore::LUAType::THREAD:break;
    default:return QVariant(); break;
    }
    return QVariant();
}

QVariant PureLuaCore::toQVariant(int n)const {
    auto * L=dataCore->L.get();
    if (L) { return toQVariant(L,n); }
    return QVariant();
}

void PureLuaCore::setGlobal(const char * const & k,const char * v) {
    auto * L=dataCore->L.get();
    if (L) { lua_pushstring(L,v); lua_setglobal(L,k); }
}
void PureLuaCore::setGlobal(const std::string & k,const char * v) {
    auto * L=dataCore->L.get();
    if (L) {lua_pushstring(L,v); lua_setglobal(L,k.c_str());}
}
void PureLuaCore::setGlobal(const QByteArray & k,const char * v) {
    auto * L=dataCore->L.get();
    if (L) {lua_pushstring(L,v); lua_setglobal(L,k.constData());}
}
void PureLuaCore::setGlobal(const QString & k,const char * v) {
    auto * L=dataCore->L.get();
    if (L) {lua_pushstring(L,v); lua_setglobal(L,k.toUtf8().constData());}
}

void PureLuaCore::setGlobal(const lua_State * const & L_,const char * const &k,const char *v) {
    auto & L=const_cast<lua_State * const &>(L_);
    lua_pushstring(L,v); lua_setglobal(L,k);
}
void PureLuaCore::setGlobal(const lua_State * const & L_,const std::string &k,const char *v) {
    auto & L=const_cast<lua_State * const &>(L_);
    lua_pushstring(L,v);lua_setglobal(L,k.c_str());
}
void PureLuaCore::setGlobal(const lua_State * const & L_,const QByteArray &k,const char *v) {
    auto & L=const_cast<lua_State * const &>(L_);
    lua_pushstring(L,v);lua_setglobal(L,k.constData());
}
void PureLuaCore::setGlobal(const lua_State * const & L_,const QString &k,const char *v) {
    auto & L=const_cast<lua_State * const &>(L_);
    lua_pushstring(L,v);lua_setglobal(L,k.toUtf8().constData());
}

}/*cct*/
/*************************************************************************/

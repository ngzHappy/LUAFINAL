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
            lua_add_state_counter(L_);
            dataCore->L=std::shared_ptr<lua_State>(L_,[](lua_State * _L) {
                if (lua_sub_state_counter(_L)) { return; }
                lua_close(_L); 
            });
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
std::string & PureLuaCore::getError() {
    return dataCore->lastError;
}

/* */
void PureLuaCore::clearError() {
    dataCore->lastError.clear();
}

/* */
void PureLuaCore::rotate(const lua_State * const &L_,const int f,const int n) {
    lua_rotate(const_cast<lua_State * const &>(L_),f,n);
}

/* */
void PureLuaCore::push(const lua_State * const &L_) { lua_pushvalue(const_cast<lua_State * const &>(L_),-1); }
void PureLuaCore::push() { auto * L=dataCore->L.get(); if (L) { push(L); } }

/* */
void PureLuaCore::push(const lua_State * const &L_,int i) { lua_pushvalue(const_cast<lua_State * const &>(L_),i); }
void PureLuaCore::push(int i) { auto * L=dataCore->L.get(); if (L) { push(L,i); } }

/* */
void PureLuaCore::pop(const lua_State * const &L_,int n) { lua_pop(const_cast<lua_State * const &>(L_),n); }
void PureLuaCore::pop(int n) { auto * L=dataCore->L.get(); if (L) {return pop(L,n); }setError("null lua"); }

/* */
void PureLuaCore::pop(const lua_State * const &L_) { lua_pop(const_cast<lua_State * const &>(L_),1); }
void PureLuaCore::pop() { auto * L=dataCore->L.get(); if (L) { return pop(L); }setError("null lua"); }

/* */
void PureLuaCore::newTable(const lua_State * const &L_) { lua_newtable(const_cast<lua_State * const &>(L_)); }
void PureLuaCore::newTable() { auto * L=dataCore->L.get(); if (L) { return newTable(L); }setError("null lua"); }

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
        case PureLuaCore::LUAType::NONE:return QVariant(); break;
    case PureLuaCore::LUAType::NIL:return QVariant(); break;
    case PureLuaCore::LUAType::BOOLEAN:return QVariant(static_cast<bool>(lua_toboolean(L,n))); break;
    case PureLuaCore::LUAType::LIGHTUSERDATA:break;
    case PureLuaCore::LUAType::NUMBER:
        if (lua_isinteger(L,n)) { return QVariant(lua_tointeger(L,n)); }
        else { return QVariant(lua_tonumber(L,n)); }
        break;
    case PureLuaCore::LUAType::STRING: {
        size_t n_=0; const char * data_=lua_tolstring(L,n,&n_); return QString::fromUtf8(data_,n_);
    }break;
    case PureLuaCore::LUAType::TABLE:break;
    case PureLuaCore::LUAType::FUNCTION: {
        return QVariant::fromValue(lua_tocfunction(L,n));
    }break;
    case PureLuaCore::LUAType::USERDATA:break;
    case PureLuaCore::LUAType::THREAD:break;
    default:return QVariant(); break;
    }

    {
        size_t n_=0;
        const char * ans_=luaL_tolstring(L,n,&n_);
        if ((n_>0)&&ans_) { return QVariant(QString::fromUtf8(ans_,n_)); }
    }      

    return QVariant();
}

QVariant PureLuaCore::toQVariant(int n)const {
    auto * L=dataCore->L.get();
    if (L) { return toQVariant(L,n); }
    return QVariant();
}


}/*cct*/

namespace cct {
void PureLuaCore::__pushKey(const lua_State * const & L_,const char * const &value) { auto & L=const_cast<lua_State * const &>(L_); lua_pushstring(L,value); }
void PureLuaCore::__pushKey(const lua_State * const & L_,const char * const &value,size_t l) {auto & L=const_cast<lua_State * const &>(L_); lua_pushlstring(L,value,l);}
void PureLuaCore::_v_pushKey(const lua_State * const & L_,const QVariant & v) {
    auto & L=const_cast<lua_State * const &>(L_);
    if (v.isValid()) {
        switch (v.userType()) {
            case QVariant::String:return pushKey(L_,v.toString()); break;
            case QVariant::ByteArray:return pushKey(L_,v.toByteArray()); break;
            case QVariant::Int:return pushKey(L_, static_cast<lua_Integer>(v.toInt()) );
            case QVariant::UInt:return pushKey(L_, static_cast<lua_Integer>(v.toUInt()) );
            case QVariant::ULongLong:return pushKey(L_, static_cast<lua_Integer>(v.toULongLong()) );
            case QVariant::LongLong:return pushKey(L_, static_cast<lua_Integer>(v.toLongLong()) );
            case QVariant::Double:return pushKey(L_, static_cast<lua_Integer>(v.toDouble()) );
            case QVariant::Bool:return pushKey(L_, static_cast<lua_Integer>(v.toBool()) );
        }
        
        if (v.canConvert( qMetaTypeId<QString>() ) ) {return pushKey(L_,v.toString());}
        if (v.canConvert( qMetaTypeId<QByteArray>()) ) { return pushKey(L_,v.toByteArray()); }
        if (v.canConvert( qMetaTypeId<double>()) ) { return pushKey(L_,static_cast<lua_Integer>(v.toDouble())); }
    }
    lua_pushnil(L);
    /* there is an error to throw */
}
void PureLuaCore::_i_pushKey(const lua_State * const & L_,const lua_Integer value) { auto & L=const_cast<lua_State * const &>(L_); lua_pushinteger(L,value); }
}

namespace cct {
lua_State * PureLuaCore::__state()const { return const_cast<lua_State *>(dataCore->L.get()); }
}
namespace cct {
void PureLuaCore::_b_pushValue(const lua_State * const & L_,const bool value) { auto & L=const_cast<lua_State * const &>(L_); lua_pushboolean(L,value); }
void PureLuaCore::_n_pushValue(const lua_State * const & L_,const lua_Number value) { auto & L=const_cast<lua_State * const &>(L_); lua_pushnumber(L,value); }
void PureLuaCore::_f_pushValue(const lua_State * const & L_,lua_CFunction value) { auto & L=const_cast<lua_State * const &>(L_); lua_pushcfunction(L,value); }
void PureLuaCore::_v_pushValue(const lua_State * const & L_,const QVariant & v ) {
    auto & L=const_cast<lua_State * const &>(L_);
    if (v.isValid()) {
        const auto ut=v.userType();
        if (ut==qMetaTypeId<lua_CFunction>()) { return pushValue(L_,v.value<lua_CFunction>()); }
        switch (ut) {
            case QVariant::String:return pushValue(L_,v.toString());
            case QVariant::ByteArray:return pushValue(L_,v.toByteArray());
            case QVariant::Int:return pushValue(L_, static_cast<lua_Integer>(v.toInt()));
            case QVariant::UInt:return pushValue(L_, static_cast<lua_Integer>(v.toUInt()));
            case QVariant::ULongLong:return pushValue(L_, static_cast<lua_Integer>(v.toULongLong()));
            case QVariant::LongLong:return pushValue(L_, static_cast<lua_Integer>(v.toLongLong()));
            case QVariant::Double:return pushValue(L_, static_cast<lua_Number>(v.toDouble()));
            case QVariant::Bool:return pushValue(L_,v.toBool());
        }
        if (v.canConvert( qMetaTypeId<QString>() ) ) {return pushKey(L_,v.toString());}
        if (v.canConvert( qMetaTypeId<QByteArray>()) ) { return pushKey(L_,v.toByteArray()); }
        if (v.canConvert( qMetaTypeId<double>()) ) { return pushValue(L_,static_cast<lua_Number>(v.toDouble())); }
    }
    lua_pushnil(L);
    /* there is an error to throw */
}

}

namespace cct {
void PureLuaCore::__setglobal(const lua_State * const & L_,const char * const & name) { auto & L=const_cast<lua_State * const &>(L_); lua_setglobal(L,name); }
void  PureLuaCore::_v_setglobal(const lua_State * const & L_,const QVariant & v) {
    const auto ut=v.userType();
    if (ut==QVariant::String) {return setGlobal(L_,v.toString()); }
    if (ut==QVariant::ByteArray) {return setGlobal(L_,v.toByteArray()); }
    if (v.canConvert( qMetaTypeId<QString>() )) {return setGlobal(L_,v.toString());}
    if (v.canConvert( qMetaTypeId<QByteArray>() )) {return setGlobal(L_,v.toByteArray());}
}
}

namespace cct {
PureLuaCore::LUAType PureLuaCore::___getGlobal(const lua_State * const & L_,const char * const& key ) {
    auto & L=const_cast<lua_State * const &>(L_);
    return static_cast<PureLuaCore::LUAType>( lua_getglobal(L,key) );
}
PureLuaCore::LUAType PureLuaCore::_v__getGlobal(const lua_State * const & L_,const QVariant& v) {
    auto & L=const_cast<lua_State * const &>(L_);
    if (v.isValid()) {
        const auto ut=v.userType();
        if (ut==QVariant::String) { return getGlobal(L_,v.toString()); }
        if (ut==QVariant::ByteArray) { return getGlobal(L_,v.toByteArray()); }
        if (v.canConvert(qMetaTypeId<QString>())) { return getGlobal(L_,v.toString()); }
        if (v.canConvert(qMetaTypeId<QByteArray>())) { return getGlobal(L_,v.toByteArray()); }
    } lua_pushnil(L);return LUAType::NIL;
}
}

namespace cct{
PureLuaCore::LUAType PureLuaCore::getTable(const lua_State * const & L_,const int table) {
    auto & L=const_cast<lua_State * const &>(L_);
    return static_cast<LUAType>( lua_gettable(L,table) );
}
}

namespace cct {
void PureLuaCore::setTable(const lua_State * const & L_,const int table) {
    auto & L=const_cast<lua_State * const &>(L_);
    return lua_settable(L,table);
}
}

namespace cct {

bool PureLuaCore::__doString(const lua_State * const & L_,const char * program,size_t ) { 
    return __doString(L_,program);
}

bool PureLuaCore::__doString(const lua_State * const & L_,const char * program) { 
    auto ans=__loadString(L_,program); 
    if (ans) {ans=__call(L_,0,LUA_MULTRET);}
    return ans;
}

bool PureLuaCore::_v_doString(const lua_State * const & L_,const QVariant & program) {
    const auto ut=program.userType();
    if (program.isValid()==false) { return false; }
    switch (ut) {
        case QVariant::String: return doString(L_,program.toString()); break;
        case QVariant::ByteArray:return doString(L_,program.toByteArray()); break;
    }
    if (program.canConvert( qMetaTypeId<QString>() )) { return doString(L_,program.toString());}
    if (program.canConvert(qMetaTypeId<QByteArray>())) { return doString(L_,program.toByteArray()); }
    return false;
}
}

namespace cct {
bool PureLuaCore::__call(const lua_State * const & L_,int inputNum,int outPutNum,int messageHandle) {
    auto & L=const_cast<lua_State * const &>(L_);
    const auto call_state_ = static_cast<LUAState>( lua_pcall(L,inputNum,outPutNum,messageHandle) );
    return call_state_ == LUAState::OK;
}
bool PureLuaCore::__call(const lua_State * const & L_,int inputNum,int outPutNum) {
    return __call(L_,inputNum,outPutNum,0);
}
}

namespace cct {
bool PureLuaCore::__loadString(const lua_State * const & L_,const char * const &_data_ ) {
    auto & L=const_cast<lua_State * const &>(L_);
    const auto ans_state_ = static_cast<LUAState>( luaL_loadstring(L,_data_) );
    return ans_state_ == LUAState::OK;
}
}

namespace cct {
std::string  PureLuaCore::__toError(const lua_State * const & L_,int index) const{
    auto & L=const_cast<lua_State * const &>(L_);
    index=lua_absindex(L,index);
    if ((lua_gettop(L)<index)) { return std::string{}; }
    size_t n_=0; const char * ans_=nullptr;
    ans_=luaL_tolstring(L,index,&n_);
    if ((n_>0)&&ans_) { return std::string(ans_,n_); }
    return std::string{};
}
}

/*************************************************************************/

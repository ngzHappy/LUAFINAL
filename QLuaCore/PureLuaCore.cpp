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
    n=lua_absindex(L,n);
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
    case PureLuaCore::LUAType::TABLE: {
        auto shared_this_lua_ = cct::lua::makeLUAStateSharedPointer(L);
        return QVariant::fromValue(cct::spr::LUATable(shared_this_lua_,n));
    }break;
    case PureLuaCore::LUAType::FUNCTION: {
        auto * cFunction_=lua_tocfunction(L,n);
        if (cFunction_) { return QVariant::fromValue(cFunction_);}
        auto shared_this_lua_ = cct::lua::makeLUAStateSharedPointer(L);
        return QVariant::fromValue( cct::spr::LuaFunction(shared_this_lua_,n) );
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
void PureLuaCore::_luat_pushValue(const lua_State * const & L_,const std::shared_ptr<cct::LUATable> & v) {
    auto & L=const_cast<lua_State * const &>(L_);
    if (bool(v)==false) { lua_pushnil(L); return; }
    if (bool(v.get()->state())==false) {lua_pushnil(L); return; }
#if defined(_DEBUG)
    assert( L_ == v.get()->state().get() );
#endif
    v->pushTable();
}
void PureLuaCore::_luaf_pushValue(const lua_State * const & L_,const std::shared_ptr<cct::LuaFunction> & v) {
    auto & L=const_cast<lua_State * const &>(L_);
    if (bool(v)==false) { lua_pushnil(L); return; }
    if (bool(v.get()->state())==false) {lua_pushnil(L); return; }
#if defined(_DEBUG)
    assert( L_ == v.get()->state().get() );
#endif
    return v->pushFunction();
}
void PureLuaCore::_b_pushValue(const lua_State * const & L_,const bool value) { auto & L=const_cast<lua_State * const &>(L_); lua_pushboolean(L,value); }
void PureLuaCore::_n_pushValue(const lua_State * const & L_,const lua_Number value) { auto & L=const_cast<lua_State * const &>(L_); lua_pushnumber(L,value); }
void PureLuaCore::_f_pushValue(const lua_State * const & L_,lua_CFunction value) { auto & L=const_cast<lua_State * const &>(L_); lua_pushcfunction(L,value); }
void PureLuaCore::_v_pushValue(const lua_State * const & L_,const QVariant & v ) {
    auto & L=const_cast<lua_State * const &>(L_);
    if (v.isValid()) {
        const auto ut=v.userType();
        if (ut==qMetaTypeId<lua_CFunction>()) { return pushValue(L_,v.value<lua_CFunction>()); }
        if (ut==qMetaTypeId<cct::spr::LuaFunction>()) { return pushValue(L_,v.value<cct::spr::LuaFunction>()); }
        if (ut==qMetaTypeId<std::shared_ptr<cct::LuaFunction>>()) { return pushValue(L_,v.value<std::shared_ptr<cct::LuaFunction>>()); }
        if (ut==qMetaTypeId<cct::spr::LUATable>()) { return pushValue(L_,v.value<cct::spr::LUATable>()); }
        if (ut==qMetaTypeId<std::shared_ptr<cct::LUATable>>()) { return pushValue(L_,v.value<std::shared_ptr<cct::LUATable>>()); }
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

namespace {
namespace cct1 {
namespace _private {
class NextNum {
    std::recursive_mutex mutex;
    typedef std::unique_lock<std::recursive_mutex> lock_t;
    std::set<int> unused;
    std::set<int> used;
    int max=0;
    ~NextNum()=default;
    NextNum():max(10) { for (int i=1; i<max; ++i) { unused.insert(i); } }
    NextNum(const  NextNum &)=delete;
    NextNum(NextNum &&)=delete;
    NextNum&operator=(const  NextNum &)=delete;
    NextNum&operator=(NextNum &&)=delete;
public:
    static NextNum & instance() { static NextNum  * _this=new NextNum; return *_this; }
    int load() {
        lock_t _(mutex);
        if (unused.empty()==false) {
            auto ansp=unused.begin();
            auto ans=*ansp; unused.erase(ansp);
            used.insert(ans); return ans;
        }
        used.insert(max); return max++;
    }
    void store(int i) {
        lock_t _(mutex);
        used.erase(i);
        unused.insert(i);
    }
};
}/*_private*/
}/*cct*/
}

namespace cct {
LuaFunction::~LuaFunction() {
    auto * L__=L_.get();
    if (L__) {
        const auto top__=lua_gettop(L__);
        luaL_newmetatable(L__,__metaTableName());
        lua_pushnil(L__);lua_rawseti(L__,-2,this_index_);
        lua_settop(L__,top__);
        auto & thisIndex__ = cct1::_private::NextNum::instance();
        thisIndex__.store( this_index_ );
    }

}

LuaFunction::LuaFunction(
    std::shared_ptr<lua_State> L,
    int function_index_in_lua_state,
    int max_args )
    :L_(std::move(L))
    ,max_args_(2+max_args){
    if (max_args_>255) { max_args_=255; }
    if (max_args_<1) { max_args_=1; }
    auto * L__=L_.get();
    if (L__) {
        auto & thisIndex__ = cct1::_private::NextNum::instance();
        this_index_=thisIndex__.load();
        function_index_in_lua_state=lua_absindex(L__,function_index_in_lua_state);
        const auto top__=lua_gettop(L__);
        auto top_=max_args_+top__;
        lua_settop(L__,top_);
        lua_pushcclosure(L__,&__wrap,max_args_);
        top_=lua_gettop(L__);/*set top_ to the wrap function*/
        luaL_newmetatable(L__,__metaTableName());
        lua_pushvalue(L__,top_);
        lua_rawseti(L__,-2,this_index_);

        lua_pushvalue(L__,function_index_in_lua_state);
        lua_setupvalue(L__,top_,1);

        lua_settop(L__,top__);
    }

}

void LuaFunction::pushFunction() const{
    auto * L__=L_.get();
    if (L__) {
        luaL_newmetatable(L__,__metaTableName());
        const auto call_pos_=lua_gettop(L__);
        lua_rawgeti(L__,-1,this_index_);
        lua_getupvalue(L__,-1,1);
        lua_copy(L__,-1,call_pos_);
        lua_settop(L__,call_pos_);
    }
}

int LuaFunction::__wrap(lua_State * L) {
    int i=0;
    lua_pushvalue(L,lua_upvalueindex(1));/*function*/
    lua_pushvalue(L,lua_upvalueindex(2));/*argc*/
    const auto argc=3+lua_tointegerx(L,-1,&i);
    if (i==0) { lua_pushstring(L,"__wrap function format error!"); lua_error(L); }
    lua_pop(L,1);/*pop argc*/
    for (i=3; i<argc;++i) {/*push args*/
        lua_pushvalue(L,lua_upvalueindex(i));
    }
    auto ans = lua_pcall(L,argc-3,LUA_MULTRET,0);/*call function*/
    const int top=lua_gettop(L);
    for (i=3; i<argc;++i) {/*clean args*/
        lua_pushnil(L); lua_replace(L,lua_upvalueindex(i));
    }
    lua_settop(L,top);
    return ans;
}

int LuaFunction::operator()() const{
    auto * L__=L_.get();
    if (L__) {
        const auto top__=lua_gettop(L__);
        luaL_newmetatable(L__,__metaTableName());
        const auto call_pos_=lua_gettop(L__);
        lua_rawgeti(L__,-1,this_index_);
        if (lua_isfunction(L__,-1)==false) { return -3; }
        const auto function_=lua_gettop(L__);
        // bool temp_[]{ (cct::lua::pushValue(L_,std::forward<Args>(args)),true)... };
        {/*seg argc*/
            using namespace cct::lua;
            pushValue(L__,0);
            lua_setupvalue(L__,function_,2);
        }
        lua_copy(L__,function_,call_pos_);
        lua_settop(L__,call_pos_);
        return lua_pcall(L__,0,LUA_MULTRET,0);
    }
    return -1;
}

}

namespace cct {
namespace spr {

LuaFunction::LUAFUNCTIONINFO::LUAFUNCTIONINFO() {
    //QMetaType::registerConverter< lua_CFunction,std::shared_ptr<cct::LuaFunction> >();
}
LuaFunction::LUAFUNCTIONINFO::~LUAFUNCTIONINFO() {
}

LuaFunction::LUAFUNCTIONINFO LuaFunction::luafunctioninfo_;

}
}

/*************************************************************************/

namespace {
namespace cct2 {
namespace _private {
class NextNum {
    std::recursive_mutex mutex;
    typedef std::unique_lock<std::recursive_mutex> lock_t;
    std::set<int> unused;
    std::set<int> used;
    int max=0;
    ~NextNum()=default;
    NextNum():max(10) { for (int i=1; i<max; ++i) { unused.insert(i); } }
    NextNum(const  NextNum &)=delete;
    NextNum(NextNum &&)=delete;
    NextNum&operator=(const  NextNum &)=delete;
    NextNum&operator=(NextNum &&)=delete;
public:
    static NextNum & instance() { static NextNum  * _this=new NextNum; return *_this; }
    int load() {
        lock_t _(mutex);
        if (unused.empty()==false) {
            auto ansp=unused.begin();
            auto ans=*ansp; unused.erase(ansp);
            used.insert(ans); return ans;
        }
        used.insert(max); return max++;
    }
    void store(int i) {
        lock_t _(mutex);
        used.erase(i);
        unused.insert(i);
    }
};
}
}
}

namespace cct {
LUATable::~LUATable() {
    auto L=L__.get();
    if (L) {
        const auto top__=lua_gettop(L);
        luaL_newmetatable(L,__metaTableName());
        lua_pushnil(L);
        lua_rawseti(L,-2,this_index_);
        lua_settop(L,top__);
        cct2::_private::NextNum::instance().store(this_index_);
    }
}

void LUATable::pushTable()const {
    auto L=L__.get();
    if (L) {
        luaL_newmetatable(L,__metaTableName());
        const auto metaTable=lua_gettop(L);
        lua_rawgeti(L,metaTable,this_index_);
        lua_copy(L,-1,metaTable);
        lua_settop(L,metaTable);
    }
}

LUATable::LUATable(std::shared_ptr<lua_State> L_,int table_in_lua_state_):L__(std::move(L_)) {
    auto L=L__.get();
    if (L) {
        const auto top__=lua_gettop(L);
        table_in_lua_state_=lua_absindex(L,table_in_lua_state_);
        this_index_=cct2::_private::NextNum::instance().load();
        luaL_newmetatable(L,__metaTableName());
        const auto metaTable=lua_gettop(L);
        lua_pushvalue(L,table_in_lua_state_);
        lua_rawseti(L,metaTable,this_index_);
        lua_settop(L,top__);
    }
}
}

/*************************************************************************/





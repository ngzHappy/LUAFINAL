
#include "QLuaCoreDetail.hpp"

namespace {
enum { FLOAT_PRECISION_=16 };
}

QLuaCoreDetail::QLuaCoreDetail(){

}

QLuaCoreDetail::~QLuaCoreDetail(){

}

namespace  {
namespace _private {

/* input a table and out put a table */
inline int luaL_copyTable( lua_State * L ) {

    if ( lua_istable(L,-1) ) {
        const auto source_table_top_=lua_gettop(L);

        std::list<int> from_,to_;
        from_.push_back( source_table_top_ );
        std::map< const void *,int > all_tables_;

        lua_newtable(L);/*create temp table*/
        const auto temp_table_=lua_gettop(L);

        lua_pushnil(L);
        lua_copy(L,-2,-1);/*copy soure table*/

        lua_newtable(L);
        const auto target_table_=lua_gettop(L);
        to_.push_back( target_table_ );/*create target table*/

        lua_pushnil(L);
        lua_pushnil(L);
        lua_copy(L,-4,-2);/*copy soure table*/
        lua_copy(L,-3,-1);/*copy target table*/

        int copy_table_count_=0;

    copy_next_table:
        while ( from_.empty()==false ) {
            const auto tf_=*from_.crbegin();
            const auto tt_=*to_.crbegin();
            from_.pop_back(); to_.pop_back();

            if (lua_istable(L,-1)) {
                lua_pushnil(L);
            }

            while (lua_next(L,tf_)!=0) {
                /*-1 value -2 key*/
                if ( lua_istable(L,-1) ) {
                    const void * table_pointer = lua_topointer(L,-1);
                    auto end_ = all_tables_.end();
                    auto pos_ = all_tables_.find( table_pointer );
                    if (end_!=pos_) {
                        /* copy value has exist */
                        lua_pop(L,1);/*remove old one*/
                        const auto ttt_=pos_->second;
                        lua_pushinteger( L,ttt_ );
                        lua_gettable(L,temp_table_);/*set value*/
                        lua_pushnil(L); lua_pushnil(L);
                        lua_copy(L,-3,-1); lua_copy(L,-4,-2);
                        lua_settable(L,tt_);
                        lua_pop(L,1);/*remove the table*/
                    }
                    else {
                        /* copy value not exist */
                        ++copy_table_count_;

                        all_tables_[table_pointer]=copy_table_count_;

                        lua_pushnil(L);
                        lua_pushinteger(L,copy_table_count_);
                        lua_newtable(L);
                        lua_copy(L,-1,-3);

                        /*set temp table*/
                        lua_settable(L,temp_table_);

                        /*set current table to temp table*/
                        lua_pushnil(L); lua_pushnil(L);
                        lua_copy(L,-3,-1);lua_copy(L,-5,-2);
                        lua_settable(L,tt_);

                        from_.push_back(tf_);
                        to_.push_back(tt_);
                        auto ttt_=lua_gettop(L);
                        to_.push_back(ttt_);
                        from_.push_back(ttt_-1);

                        goto copy_next_table;
                    }
                }
                else {
                    /* copy value not table */
                    lua_pushnil(L); lua_pushnil(L);
                    lua_copy(L,-3,-1);lua_copy(L,-4,-2);
                    lua_settable( L,tt_ );/*remove key value*/
                    lua_pop(L,1);/*remove key*/
                }
            }/*~while*/

            lua_pop(L,2);/*remove source table , target table*/

        }/*all while*/

         /* return ans */
        lua_copy(L,target_table_,source_table_top_);
        lua_settop(L,source_table_top_);
        return 1;

    }/*~if*/

    lua_newtable(L);
    return 1;
}

inline void luaL_printTable( lua_State * L ,std::function<void(const std::string &)> print_ ) {
    enum DataType : int{
        Bool,Number,Integer,String,NIL
    };

    class Data {public:
        Data(  ):type(NIL) {}
        Data(bool i):type(Bool),boolean(i) {}
        Data(lua_Integer i):type(Integer),integer(i) {}
        Data(lua_Number i):type(Number),number(i) {}
        Data(const std::string & i):type(String),string(i) {}
        Data(std::string && i):type(String),string(std::move(i)) {}
        Data(const Data & v):type(v.type) {
            switch (type) {
                case Bool:boolean=v.boolean; break;
                case Number:number=v.number; break;
                case Integer:integer=v.integer; break;
                case String: {new(&string) std::string(v.string); }; break;
                case NIL:; break;
            }
        }

        Data( Data && v):type(v.type) {
            switch (type) {
                case Bool:boolean=v.boolean; break;
                case Number:number=v.number; break;
                case Integer:integer=v.integer; break;
                case String: {new(&string) std::string( std::move(v.string) ); }; break;
                case NIL:; break;
            }
        }

        Data & operator=( const Data & v ) {
            if (this==&v) { return *this; }
            if (v.type==String) {
                if (type==String) { string=v.string; }
                else {
                    type=String;
                    new (&string) std::string( v.string );
                }
            }
            else {
                if (type==String) {
                    typedef std::string _T_;
                    string.~_T_();
                }
                switch (v.type) {
                    case Bool:{type=Bool; boolean=v.boolean; } break;
                    case Number: {type=Number; number=v.number; }break;
                    case Integer: {type=Integer; integer=v.integer; }break;
                    case String: {}break;
                    case NIL: {type=NIL; }break;
                }
            }

            return *this;
        }

        Data & operator=( Data && v ) {
            if (this==&v) { return *this; }
            if (v.type==String) {
                if (type==String) { string = std::move( v.string ); }
                else {
                    type=String;
                    new (&string) std::string( std::move( v.string ) );
                }
            }
            else {
                if (type==String) {
                    typedef std::string _T_;
                    string.~_T_();
                }
                switch (v.type) {
                    case Bool: {type=Bool; boolean=v.boolean; }break;
                    case Number: {type=Number; number=v.number; }break;
                    case Integer: {type=Integer; integer=v.integer; }break;
                    case String: {}break;
                    case NIL: {type=NIL; }break;
                }
            }

            return *this;
        }

        ~Data() {
            if (type==String) { typedef std::string _T_; string.~_T_(); }
        }

        DataType type;
        union {
            bool boolean;
            lua_Number number;
            lua_Integer integer;
            std::string string;
        };
    };

    struct TableMemory {
        Data data;
        int table;
        bool ipair=false;
        lua_Integer ipair_index=1;
        lua_Integer tableIndex=0;
        TableMemory() {}
        TableMemory(const std::string & v,int t):data(v),table(t) {}
        TableMemory(std::string && v,int t):data( std::move(v) ),table(t) {}
        TableMemory(std::string && v,int t,bool i,lua_Integer ii,lua_Integer ti):data( std::move(v) ),table(t),ipair(i),ipair_index(ii),tableIndex(ti) {}
        TableMemory(Data && v,int t) :data( std::move(v) ),table(t) {}
        TableMemory(const Data & v,int t) :data( v ),table(t) {}
        TableMemory(Data && v,int t,bool i,lua_Integer ii,lua_Integer ti):data( std::move(v) ),table(t),ipair(i),ipair_index(ii) ,tableIndex(ti){}
    };

    const auto is_integer=[]( const std::string & v )->bool {
        if (v.empty()) { return false; }
        const auto i=v[0];if ((i>='0')&&(i<='9')) { return true; }
        return false;
    };
    const auto to_string=[](const auto & i) {
        std::stringstream ss; ss.precision(FLOAT_PRECISION_);
        ss<<i; std::string ans; ss>>ans; return std::move(ans);
    };

    const auto oprint_key=[&print_,&to_string,&is_integer](const Data & v) {
        const int & type=v.type;
        switch (type) {
            case DataType::Number: { print_("["); print_(to_string(v.number)); print_("]"); }; break;
            case DataType::Integer: { print_("["); print_(to_string(v.integer)); print_("]"); }; break;
            case DataType::String:
                if (is_integer(v.string)) {
                    print_("[\""); print_(v.string); print_("\"]");
                }
                else {
                    print_(v.string);
                }break;
        }
    };

    const auto string_find=[](const std::string & s,const auto & v) {return (s.find(v)!=s.npos);};

    const auto oprint_value=[&print_,&to_string,&string_find](const Data & v) {
        const DataType & type=v.type;
        switch (type) {
            case DataType::Bool: { if (v.boolean) { print_("true"); } else {print_("false");} }break;
            case DataType::Number: {  print_(to_string(v.number));  }; break;
            case DataType::Integer: { print_(to_string(v.integer));  }; break;
            case DataType::String: {
                std::string op_="==";
                while ((string_find(v.string,"["+op_+"["))||string_find(v.string,"]"+op_+"]")) { op_+="==="; }
                print_("["+op_+"[");
                print_(v.string);
                print_("]"+op_+"]");
            }; break;
            case DataType::NIL: {print_("nil"); }  break;
        }
    };

    const auto print_endl=[&print_]() {print_("\n"); };
    const auto print_line_endl=[&print_]() {print_(" ;  \n"); };
    const auto print_file_begin=[&print_](const std::string & tbname) {
        print_(tbname);
        print_(" = (function()                    \n\nlocal ");
    };

    const auto print_file_end=[&print_](const std::string & tbname) {
        print_("\nreturn "+tbname);
        print_("; end)()");
    };

    const auto table_name_full=[&to_string]( const std::list< TableMemory > & tables,const Data & ctable )->std::string {
        auto root_name_=tables.begin();
        auto end_name_=tables.end();
        std::string ans;
        ans=root_name_->data.string;
        for (auto next_name_=root_name_;(++next_name_)!=end_name_; ) {
            const auto &current_table_name=(*next_name_).data;
            switch ( int( current_table_name.type) ) {
                case DataType::Integer: ans+=std::move( "["+to_string(current_table_name.integer)+"]" ); break;
                case DataType::Number: ans+=std::move( "["+to_string(current_table_name.number)+"]" ); ; break;
                case DataType::String: ans+=std::move("[\""+current_table_name.string+"\"]") ; break;
            }
        }
        const auto &current_table_name= ctable;
        switch ( int( current_table_name.type) ) {
            case DataType::Integer: ans+=std::move( "["+to_string(current_table_name.integer)+"]" ); break;
            case DataType::Number: ans+=std::move( "["+to_string(current_table_name.number)+"]" ); ; break;
            case DataType::String: ans+=std::move("[\""+current_table_name.string+"\"]") ; break;
        }
        return std::move(ans);
    };

    /*-----------------------------------------------*/
    if (lua_gettop(L)<2) { lua_pushstring(L,"you must input table name and table"); lua_error(L); }
    if (lua_isstring(L,1)==false) {lua_pushstring(L,"table name must a string"); lua_error(L);}
    if (lua_istable(L,2)==false) {lua_pushstring(L,"you must input a table"); lua_error(L);}

    /* copy the table twice */
    lua_pushvalue(L,2);
    lua_pushvalue(L,2);

    const std::string tbname( lua_tostring(L,1) );
    print_file_begin(tbname);

    /*-----------------------------------------------*/
    lua_Integer table_index_=0;
    std::list< TableMemory > print_tables{ {tbname, 2} };
    std::list< std::string > final_print_lines;
    std::map< const void *,std::string > all_tables{ {lua_topointer(L,2),tbname} };

print_next_start:
    while ( print_tables.empty()==false ) {

        auto & current_table_memory=*print_tables.rbegin();
        int current_table= current_table_memory.table;/*the table in the lua state*/
        Data current_table_name= std::move( current_table_memory.data ) ;/*the table name */
        bool ipair_=current_table_memory.ipair;/*the value will be true if the index follow 1,2,3....  */
        lua_Integer ipair_index_=current_table_memory.ipair_index;/*current index if ipair is true*/
        lua_Integer current_table_index=current_table_memory.tableIndex;/*the table index in this function */

        print_tables.pop_back();

        /* reset the value : ipair_ */
        if ( lua_istable(L,-1) ) {

            const static constexpr char start_string[]{"{   "};
            switch ( int( current_table_name.type ) ) {
                case DataType::Integer:if (false==ipair_) { print_("["+to_string(current_table_name.integer)+"] ="); }print_(start_string); break;
                case DataType::Number: if (false==ipair_) { print_("["+to_string(current_table_name.number)+"] ="); }print_(start_string); break;
                case DataType::String: {
                    if (is_integer(current_table_name.string)) {
                        print_("[\""+current_table_name.string+"\"] ="+start_string);
                    }
                    else {
                        print_(current_table_name.string );
                        print_(" =");
                        print_(start_string);
                    }
                } break;
            }

            lua_pushnil(L);
            ipair_=true;
            print_("--[[ " +to_string( current_table_index )+ " --]]  \n");

        }

        while ( lua_next(L,current_table) != 0) {
            /* uses 'key' (at index -2) and 'value' (at index -1) */
            if ( lua_istable(L,-1) ) {

                Data key_;
                auto type_ = lua_type(L,-2);
                switch (type_) {
                    case LUA_TSTRING:ipair_=false; key_=std::string(lua_tostring(L,-2));  break;
                    case LUA_TNUMBER: {
                        key_=lua_tointeger(L,-2);
                        if (ipair_) {
                            if (key_.integer == ipair_index_) {
                                ++ipair_index_;
                            }
                            else {
                                ipair_=false;
                            }
                    } } break;
                    default: lua_pushstring(L,"key must be number or string"); lua_error(L); ; break;
                }

                const void * table_pointer=lua_topointer(L,-1);
                auto ctable = all_tables.find( table_pointer );
                const auto etable=all_tables.end();

                if ( ctable == etable ) {
                    /*find a new table*/
                    ++table_index_;
                    /*save old value*/
                    print_tables.emplace_back( std::move(current_table_name),current_table,ipair_,ipair_index_,current_table_index );

                    all_tables.insert({table_pointer,table_name_full( print_tables ,key_ ) });

                    current_table=lua_gettop(L);
                    current_table_name=std::move(key_);

                    /*set new value*/
                    print_tables.emplace_back( std::move(current_table_name), current_table ,ipair_,1,table_index_);

                    goto print_next_start;
                }
                else {
                    /*circle table*/

                    lua_pop(L,1);/*pop the table*/
                    ipair_=false;/*circle table do not surport ipair*/

                    print_tables.emplace_back( std::move(current_table_name) ,-1);
                    auto this_table_name_ = table_name_full(print_tables,key_);

                    current_table_name= std::move( (* print_tables.rbegin()).data );
                    print_tables.pop_back();

                    final_print_lines.push_back(std::move(this_table_name_)+" = "+ ctable->second + "\n");
                }

            }
            else {
                Data key_,value_;
                bool do_not_support=false;
                auto type_ = lua_type(L,-2);
                switch (type_) {
                    case LUA_TSTRING: ipair_=false; key_=std::string(lua_tostring(L,-2));  break;
                    case LUA_TNUMBER: {key_=lua_tointeger(L,-2);
                        if (ipair_) {
                            if (key_.integer == ipair_index_) {
                                ++ipair_index_;
                            }
                            else {
                                ipair_=false;
                            }
                        }
                    } break;
                    default: lua_pushstring(L,"key must be number or string"); lua_error(L); ; break;
                }

                type_=lua_type(L,-1);
                switch (type_) {
                    case LUA_TNIL: break;
                    case LUA_TBOOLEAN: value_=bool( lua_toboolean(L,-1) ); break;
                    case LUA_TSTRING: value_=std::string(lua_tostring(L,-1)); break;
                    case LUA_TNUMBER: value_=lua_tonumber(L,-1); break;
                    case LUA_TFUNCTION: value_=std::string("function() end"); break;
                    default: do_not_support=true ; break;
                }

                if (do_not_support) { lua_pop(L,1); continue; }

                if ( false == ipair_ ) {
                    oprint_key(key_);
                    print_(" = ");
                }
                oprint_value(value_);
                print_line_endl();

                lua_pop(L,1);
            }
        }/*while table*/

        if ( print_tables.empty() ) {
            print_("} --[[ " +to_string( current_table_index )+ " --]] \n");
        }
        else {
            print_("} , --[[ " +to_string( current_table_index )+ " --]] \n");
        }

        lua_pop(L,1);
    }

    if ( false == final_print_lines.empty() ) {
        print_endl();
        for (const auto & line:final_print_lines) {
            print_(line);
        }
    }

    print_file_end(tbname);

    print_endl();
    print_("--[[***********************--]]\n");
    print_endl();

    return ;
}


}/*~namespace _private*/

}/*~namespace*/


QLUACORESHARED_EXPORT int luaL_copyTable( lua_State * L ){
return _private::luaL_copyTable(L);
}

QLUACORESHARED_EXPORT void luaL_printTable(
        lua_State * L ,
        std::function<void(const std::string &)> print_ ){
return _private::luaL_printTable(L,std::move(print_));
}

class LuaConfigFile::LuaConfigFileCore {
    static int printTable(lua_State * L) {
        luaL_printTable(L,[](const std::string & v) { std::cout<<v; });
        return 0;
    }

    static int saveTable(lua_State * L) {
        if (lua_gettop(L)<3) {
            lua_pushstring(L,"you must input filename tablename and table"); lua_error(L);
        }
        std::ofstream ofs( lua_tostring(L,1) );
        if ( ofs.is_open() ) {
            lua_remove(L,1);
            luaL_printTable(L,[&ofs](const std::string & v) { ofs<<v; });
        }
        else {
            lua_pushstring(L,"file can not be open"); lua_error(L);
        }
        return 0;
    }

    static int tableToString(lua_State * L) {
        std::shared_ptr< std::string > ofs(new std::string);
        ofs->reserve(10*1024);
        luaL_printTable(L,[&ofs](const std::string & v) { (*(ofs))+=v; });
        lua_settop(L,0);
        lua_pushstring(L,ofs->c_str());
        ofs.reset();
        return 1;
    }
public:

    lua_State * L;
    mutable std::string errorString;

    LuaConfigFileCore&operator=(const LuaConfigFileCore &)=delete;
    LuaConfigFileCore&operator=(LuaConfigFileCore &&)=delete;
    LuaConfigFileCore(const LuaConfigFileCore &)=delete;
    LuaConfigFileCore(LuaConfigFileCore &&)=delete;

    LuaConfigFileCore() {
        L=luaL_newstate();
        luaL_openlibs(L);

        lua_pushcfunction(L,&printTable);
        lua_setglobal(L,"printTable");

        lua_pushcfunction(L,&saveTable);
        lua_setglobal(L,"saveTable");

        lua_pushcfunction(L,&tableToString);
        lua_setglobal(L,"table2str");

    }
    ~LuaConfigFileCore() {
        lua_close(L);
    }

    bool load(const QString fileName_) {
        std::string fileName = fileName_.toLocal8Bit().toStdString();
        std::ifstream ifs( fileName );
        if ( ifs.is_open() ) {
            ifs.close();

            std::string op="==";
            while (
                (fileName.find("["+op+"[") != std::string::npos)||
                (fileName.find("]"+op+"]") != std::string::npos)
                ) {op+="=";}

            /*It returns false if there are no errors or true in case of errors.*/
            if ( false == luaL_dostring(L,("loadfile(["+op+"["+fileName+"]"+op+"])()").c_str()) ) {
                return true;
            }
            else {
                size_t len;
                /*
                luaL_tolstring

                [-0, +1, e]
                const char *luaL_tolstring (lua_State *L, int idx, size_t *len);
                Converts any Lua value at the given index to a C string in a reasonable format. The resulting string is pushed onto the stack and also returned by the function. If len is not NULL, the function also sets *len with the string length.

                If the value has a metatable with a "__tostring" field, then luaL_tolstring calls the corresponding metamethod with the value as argument, and uses the result of the call as its result.
                */
                const char * data=luaL_tolstring(L,-1,&len);
                errorString=std::string(data,len);
                return false;
            }
        }
        else {
            errorString = "can not open file "+ std::move(fileName);
            return false;
        }
    }

    template<typename T>
    QVariant read(
        const T & valueName,
        QVariant defauleValue)const {

        auto end_=valueName.end();
        auto pos_=valueName.begin();
        /*Pushes onto the stack the value of the global name. Returns the type of that value.*/
        auto type = lua_getglobal(L,pos_->toString().toUtf8().constData());

        ++pos_;
        if ( (pos_ != end_ ) == false ) {
            switch (type) {
                case LUA_TNIL: return defauleValue; break;
                case LUA_TBOOLEAN:return lua_toboolean(L,-1); break;
                case LUA_TNUMBER:if (lua_isinteger(L,-1)) { return lua_tointeger(L,-1); }
                                 else { return lua_tonumber(L,-1); } break;
                case LUA_TSTRING:return QString::fromUtf8(lua_tostring(L,-1)); break;
                case LUA_TTABLE:break;
                default: {errorString="do not surport this type"; return defauleValue; } break;
            }
            return defauleValue;
        }

        for (; pos_!=end_;++pos_ ) {
            const auto & i=*pos_;
            if (i.isValid()==false) {
                errorString="nil key";
                return defauleValue;
            }
            switch ( i.type()) {
                case QVariant::ByteArray: lua_pushstring(L,i.toByteArray()) ; break;
                case QVariant::String:lua_pushstring(L,i.toString().toUtf8()) ; break;
                case QVariant::Double:lua_pushnumber(L, (i.toDouble()) ); break;
                default: {
                    bool value_=false;
                    auto index_ = i.toLongLong(&value_);
                    if ( value_ ) {
                        lua_pushinteger(L, index_);
                    }
                    else {
                        errorString="key type error";
                        return defauleValue;
                    }
                }break;

            }

            if ( lua_istable(L,-2) ) {
                switch (lua_gettable(L,-2)) {
                    case LUA_TNONE:
                    case LUA_TNIL: errorString="can not find value"; return defauleValue; break;
                    default:break;
                }
            }
            else {
                errorString="can not get value from not a table";
                return defauleValue;
            }

        }/*for*/

        type=lua_type(L,-1);
        switch (type) {
            case LUA_TNIL: return defauleValue; break;
            case LUA_TBOOLEAN:return lua_toboolean(L,-1); break;
            case LUA_TNUMBER:if (lua_isinteger(L,-1)) {
                return lua_tointeger(L,-1);
            }else {
                return lua_tonumber(L,-1) ;
            } break;
            case LUA_TSTRING:return QString::fromUtf8(lua_tostring(L,-1)); break;
            default: {errorString="do not surport this type"; return defauleValue; } break;
        }

        return defauleValue;
    }

    template< typename T >
    bool write(
        const T & valueName,
        const QVariant & value) {

        auto pos_=valueName.begin();

        auto push_value=[this](const QVariant & i) {
            switch (i.type()) {
                case QVariant::ByteArray: lua_pushstring(L,i.toByteArray()); break;
                case QVariant::String:lua_pushstring(L,i.toString().toUtf8()); break;
                case QVariant::Double:lua_pushnumber(L,(i.toDouble())); break;
                default: {
                    bool value_=false;
                    auto index_=i.toLongLong(&value_);
                    if (value_) {
                        lua_pushinteger(L,index_);
                    }
                    else {
                        errorString="key type error";
                        return false;
                    }
                }break;
            }
            return true;
        };

        QByteArray key_;
        if (pos_->type()==QVariant::ByteArray) { key_=pos_->toByteArray(); }
        else { key_=pos_->toString().toUtf8(); }
        if (key_.isEmpty()) { errorString="global key is null"; return false; }

        auto opos_ = ++pos_ ;
        const auto end_=valueName.end();

        if ( (opos_ != end_)==false ) {
            if (push_value(value)) {
                lua_setglobal(L,key_.constData());
                return true;
            }
            else { return false; }
        }

        auto type=lua_getglobal(L,key_.constData());
        if ( (type == LUA_TNIL) ||(type == LUA_TNONE) ) {
            lua_newtable(L);
            lua_setglobal(L,key_.constData());
            type=lua_getglobal(L,key_.constData());
        }

        ++pos_;

        if (LUA_TTABLE==type) {
            for (; pos_!=end_ ;++opos_,++pos_) {
                if (push_value(*opos_)==false) {
                    return false;
                }
                type=lua_gettable(L,-2);
                if (LUA_TTABLE!=type) {
                    lua_pop(L,1);
                    push_value(*opos_);
                    lua_pushvalue(L,-1);
                    lua_newtable(L);
                    lua_settable(L,-4);
                    if (lua_gettable(L,-2)!=LUA_TTABLE) {
                        errorString="can not settable";
                        return false;
                    }
                }
            }
        }
        else {
            errorString=std::string("global : ")+ key_.constData() +" is not table ";
            return false;
        }

        if (push_value(*opos_)&&push_value(value)) {
            lua_settable(L,-3);
            return true;
        }

        return false;
    }

    bool save(
        const QString fileName,
        const QString & tableName)const {

        QByteArray tbName=tableName.toUtf8();
        auto type = lua_getglobal(L,tbName);

        auto push_value=[this](const QVariant & i) {
            switch (i.type()) {
                case QVariant::ByteArray: lua_pushstring(L,i.toByteArray()); break;
                case QVariant::String:lua_pushstring(L,i.toString().toUtf8()); break;
                case QVariant::Double:lua_pushnumber(L,(i.toDouble())); break;
                default: {
                    bool value_=false;
                    auto index_=i.toLongLong(&value_);
                    if (value_) {
                        lua_pushinteger(L,index_);
                    }
                    else {
                        errorString="key type error";
                        return false;
                    }
                }break;
            }
            return true;
        };

        switch (type) {
            case LUA_TNIL:
            case LUA_TNONE: errorString=("can not find global " +tbName).constData() ; return false; break;
            case LUA_TTABLE: {
                lua_pushcfunction(L,&saveTable);
                push_value( fileName );
                push_value( tbName );
                lua_pushvalue(L,-4);
                lua_pcall(L,3,0,0);
                return true;
            } break;
            case LUA_TBOOLEAN: {
                std::ofstream ofs( fileName.toLocal8Bit() );
                ofs<<tbName.constData()<<" = "<<std::boolalpha<<bool(lua_toboolean(L,-1))<<std::endl;
                return true;
            }break;
            case LUA_TNUMBER :{
                std::ofstream ofs( fileName.toLocal8Bit() );
                if ( lua_isinteger(L,-1) ) {
                    ofs<<tbName.constData()<<" = "<<lua_tointeger(L,-1)<<std::endl;
                }
                else {
                    ofs.precision(FLOAT_PRECISION_);
                    ofs<<tbName.constData()<<" = "<<lua_tonumber(L,-1)<<std::endl;
                }
                return true;
            }break;
            case LUA_TSTRING: {
                std::ofstream ofs( fileName.toLocal8Bit() );
                std::string data_ = lua_tostring(L,-1);
                std::string op_="==";
                while ( (data_.find("["+op_+"[")!=op_.npos)|| (data_.find("]"+op_+"]")!=op_.npos) ) {
                    op_+="=====";
                }
                ofs<<tbName.constData()<<" = "<<"["<<op_<<"["<<data_<<"]"<<op_<<"]"<<std::endl;
                return true;
            }break;
        }

        errorString="not surport type";
        return false;
    }

private:

};

LuaConfigFile::LuaConfigFile(){
    dataCore = std::make_shared<LuaConfigFileCore>();
}
LuaConfigFile::~LuaConfigFile(){

}

const std::string & LuaConfigFile::errorCode()const {
    return dataCore->errorString;
}

bool LuaConfigFile::load(const QString & fileName) try{

    class StackLock {
        lua_State * L_;
        int size_;
    public:
        StackLock(lua_State * L__):L_(L__) { size_=lua_gettop(L_); }
        ~StackLock() { lua_settop(L_,size_); }
    };

    StackLock lock( dataCore->L );
    QFileInfo info( fileName );
    if ( info.exists()==false ) {
        dataCore->errorString="can not find " +fileName.toLocal8Bit().toStdString() ;
        return false;
    }
    return dataCore->load( std::move( info.absoluteFilePath() ) );

}
catch (...) {
    dataCore->errorString="unknow error";
    return false;
}

template<typename T>
QVariant LuaConfigFile::_read(
    const T & valueName,
    QVariant defauleValue )const try{

    if ( (valueName.begin() != valueName.end()) == false ) {
        dataCore->errorString="null name";
        return defauleValue;
    }

    class StackLock {
        lua_State * L_;
        int size_;
    public:
        StackLock(lua_State * L__):L_(L__) { size_=lua_gettop(L_); }
        ~StackLock() { lua_settop(L_,size_); }
    };

    StackLock lock( dataCore->L );
    return dataCore->read(valueName,defauleValue);

}
catch (...) {
    dataCore->errorString="unknow error";
    return defauleValue;
}

template<typename T>
bool LuaConfigFile::_write(
    const T & valueName,
    const QVariant & value) try{
    if ( (valueName.begin()!=valueName.end())==false ) { 
        dataCore->errorString="null valueName";
        return false;
    }
    class StackLock {
        lua_State * L_;
        int size_;
    public:
        StackLock(lua_State * L__):L_(L__) { size_=lua_gettop(L_); }
        ~StackLock() { lua_settop(L_,size_); }
    };

    StackLock lock( dataCore->L );
    return dataCore->write( valueName,value );
}
catch (...) {
    dataCore->errorString="unknow error";
    return false;
}

bool LuaConfigFile::save(
    const QString & fileName,
    const QString & tableName)const try{

    QFileInfo info(fileName);

    {
        std::string fileName_ = fileName.toLocal8Bit().constData();
        std::ofstream ofs(fileName_);
        if (false==ofs.is_open()) {
            dataCore->errorString="can not create file"; return false;
        }
    }

    if (info.exists()==false) {
        dataCore->errorString="file do not exist"; return false;
    }

    class StackLock {
        lua_State * L_;
        int size_;
    public:
        StackLock(lua_State * L__):L_(L__) { size_=lua_gettop(L_); }
        ~StackLock() { lua_settop(L_,size_); }
    };

    StackLock lock( dataCore->L );
    return dataCore->save( std::move( info.absoluteFilePath() ) ,tableName);

}
catch (...) {
    dataCore->errorString="unknow error";
    return false;
}

QVariant LuaConfigFile::read(const std::initializer_list<QVariant> & valueName,QVariant defauleValue ) const {
    return _read(valueName,std::move( defauleValue ));
}
QVariant LuaConfigFile::read(const std::vector<QVariant> & valueName,QVariant defauleValue ) const {
    return _read(valueName,std::move( defauleValue));
}
QVariant LuaConfigFile::read(const std::list<QVariant> & valueName,QVariant defauleValue ) const {
    return _read(valueName,std::move( defauleValue ));
}
QVariant LuaConfigFile::read(const QList<QVariant> & valueName,QVariant defauleValue ) const {
    return _read(valueName,std::move( defauleValue ));
}
QVariant LuaConfigFile::read(const QVector<QVariant> & valueName,QVariant defauleValue ) const {
    return _read(valueName,std::move( defauleValue ));
}
QVariant LuaConfigFile::read(const std::deque<QVariant> & valueName,QVariant defauleValue ) const {
    return _read(valueName,std::move( defauleValue ));
}
QVariant LuaConfigFile::read(const std::forward_list<QVariant> & valueName,QVariant defauleValue ) const {
    return _read(valueName,std::move( defauleValue ));
}

bool LuaConfigFile::write(const std::initializer_list<QVariant> & a,const QVariant & b){
    return _write(a,b);
}
bool LuaConfigFile::write(const QList<QVariant> & a,const QVariant & b){
    return _write(a,b);
}
bool LuaConfigFile::write(const QVector<QVariant> & a,const QVariant & b){
    return _write(a,b);
}
bool LuaConfigFile::write(const std::list<QVariant> & a,const QVariant & b){
    return _write(a,b);
}
bool LuaConfigFile::write(const std::vector<QVariant> & a,const QVariant & b){
    return _write(a,b);
}
bool LuaConfigFile::write(const std::deque<QVariant> & a,const QVariant & b){
    return _write(a,b);
}
bool LuaConfigFile::write(const std::forward_list<QVariant> & a,const QVariant & b){
    return _write(a,b);
}
/********************************/


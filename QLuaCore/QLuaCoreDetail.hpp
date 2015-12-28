#if !defined( QLUACOREDETAIL_HPP__ )
#define QLUACOREDETAIL_HPP__

#include <QString>
#include <QByteArray>
#include <QFileInfo>
#include "QLuaCore.hpp"

/*
 * this will include all std headers
*/
//C Library
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <cfloat>
#include <cinttypes>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctgmath>
#include <ctime>
#include <cwchar>
#include <cwctype>
//#include <cuchar>

//Containers
#include <array>
#include <bitset>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//Atomics and threading library
#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>

//Miscellaneous headers
#include <algorithm>
#include <chrono>
#include <complex>
#include <exception>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <locale>
#include <memory>
#include <new>
#include <numeric>
#include <random>
#include <ratio>
#include <regex>
#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <utility>
#include <valarray>
//#include <codecvt>

//Input/Output Stream Library
#include <fstream>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <streambuf>

using namespace std::chrono_literals;
using namespace std::string_literals;


class QLUACORESHARED_EXPORT QLuaCoreDetail
{
public:
    QLuaCoreDetail();
    virtual ~QLuaCoreDetail();
};

/* input a table output a table */
QLUACORESHARED_EXPORT int luaL_copyTable(lua_State *);
/*input tablename and table */
QLUACORESHARED_EXPORT void luaL_printTable(lua_State *,std::function<void(const std::string &)>);


class QLUACORESHARED_EXPORT LuaConfigFile{
    class LuaConfigFileCore;
    std::shared_ptr<LuaConfigFileCore> dataCore;
private:
    template<typename T>
    QVariant _read(const T & ,QVariant ) const;
    template<typename T>
    bool _write(const T & valueName,const QVariant &);
public:
    LuaConfigFile();
    virtual ~LuaConfigFile();

    bool load( const QString & ) ;
    bool save( const QString &/*fileName*/,const QString & /*tablename/global name*/)const;
    const std::string & errorCode()const;

    QVariant read(const std::initializer_list<QVariant> & valueName,QVariant defauleValue=QVariant()) const ;
    QVariant read(const std::vector<QVariant> & valueName,QVariant defauleValue=QVariant()) const ;
    QVariant read(const std::list<QVariant> & valueName,QVariant defauleValue=QVariant()) const ;
    QVariant read(const QList<QVariant> & valueName,QVariant defauleValue=QVariant()) const ;
    QVariant read(const QVector<QVariant> & valueName,QVariant defauleValue=QVariant()) const ;
    QVariant read(const std::deque<QVariant> & valueName,QVariant defauleValue=QVariant()) const;
    QVariant read(const std::forward_list<QVariant> & valueName,QVariant defauleValue=QVariant()) const;

    bool write(const std::initializer_list<QVariant> & valueName,const QVariant &);
    bool write(const std::vector<QVariant> & valueName,const QVariant &);
    bool write(const std::list<QVariant> & valueName,const QVariant &);
    bool write(const QList<QVariant> & valueName,const QVariant &);
    bool write(const QVector<QVariant> & valueName,const QVariant &);
    bool write(const std::deque<QVariant> & valueName,const QVariant &);
    bool write(const std::forward_list<QVariant> & valueName,const QVariant &);
};


#endif // QLUACOREDETAIL_HPP




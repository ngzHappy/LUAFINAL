#if !defined( QSTDSHAREDPOINTER_HPP )
#define QSTDSHAREDPOINTER_HPP

#if defined(QT_CORE_LIB)

#include <type_traits>
#include <QObject>
#include <functional>
#include <memory>

Q_DECLARE_METATYPE( std::weak_ptr<QObject> )
Q_DECLARE_METATYPE( std::shared_ptr<QObject> )
Q_DECLARE_METATYPE( lua_CFunction )

namespace cct {

void qSharedPointerMarkNeedDelete(const QObject * const & p,bool v=false);
void qSharedPointerMarkNeedDelete(const std::shared_ptr<QObject> & p,bool v=false);
template<typename T>void qSharedPointerMarkNeedDelete(const std::shared_ptr<T> & p,bool v=false);
template<typename T,typename ... Args>
std::shared_ptr<T> makeQSharedPointer(Args && ... args);

}

namespace cct {

inline void qPointerDeleteFunction(QObject * obj,const QPointer<QObject> & qPointer_) {
    if ((qPointer_)&&obj) {
        if (obj->parent()) {
            /*this object will be deleted by parent*/
            return;
        }
        QVariant doNotDelete=obj->property("@.@~delete");
        if (doNotDelete.isValid()&&doNotDelete.toBool()) {
            /*user set it do not need delete*/
            return;
        }
        /*delete the pointer*/
        obj->deleteLater();
    }
}

inline void qSharedPointerMarkNeedDelete(const QObject * const & p,bool v ) {
    if (p==nullptr) { return; }
    v=!v;/**/
    (const_cast<QObject *>(p))->setProperty("@.@~delete",v);
}

inline void qSharedPointerMarkNeedDelete(const std::shared_ptr<QObject> & p,bool v ) {
    qSharedPointerMarkNeedDelete(p.get(),v);
}

template<typename T>
void qSharedPointerMarkNeedDelete(const std::shared_ptr<T> & p,bool v ) {
    qSharedPointerMarkNeedDelete(p.get(),v);
}

template<typename T,typename ... Args>
std::shared_ptr<T> makeQSharedPointer(Args && ... args) {
    T * ans_=new T(std::forward<Args>(args)...);
    QPointer<QObject> ans_QPointer_(ans_);
    std::function<void(QObject *)> delete_=std::bind(&qPointerDeleteFunction,
        std::placeholders::_1,std::move(ans_QPointer_));
    auto ans=std::shared_ptr<T>(ans_,std::move(delete_));
    {
        /*add weak pointer*/
        auto wans=std::weak_ptr<QObject>(ans);
        ans_->setProperty("@.@.weak_ptr",QVariant::fromValue(std::move(wans)));
    }
    return std::move(ans);
}

template<typename T_ >
auto makeQSharedPointer( const T_ * const & pointer_ )->std::shared_ptr< typename std::remove_cv<T_>::type > {
    typedef typename std::remove_cv<T_>::type T;
    if (pointer_==nullptr) { return std::shared_ptr<T>(nullptr); }
    {
        /*check weak pointer*/
        QVariant wPointer=pointer_->property("@.@.weak_ptr");
        if ( wPointer.isValid() ) {
            std::weak_ptr<QObject> wp = wPointer.value<std::weak_ptr<QObject>>();
            auto ans = wp.lock();
            if (ans) { return std::move( std::dynamic_pointer_cast<T>(ans) ); }
        }
    }

    auto * ans_=const_cast<T *>(pointer_);
    QPointer<QObject> ans_QPointer_(ans_);
    std::function<void(QObject *)> delete_=std::bind(&qPointerDeleteFunction,
        std::placeholders::_1,std::move(ans_QPointer_));
    auto ans = std::shared_ptr<T>(ans_,std::move(delete_));
    {
        /*add weak pointer*/
        auto wans=std::weak_ptr<QObject>( ans );
        ans_->setProperty("@.@.weak_ptr",QVariant::fromValue(std::move(wans)));
    }
    return std::move(ans);

}

template<typename T>
std::shared_ptr<T> makeQSharedPointer(const std::shared_ptr<T> & pointer_) {
    if (pointer_) {
        QObject * ans_=pointer_.get();
        auto wans=std::weak_ptr<QObject>( const_cast<std::shared_ptr<T> &>( pointer_ ) );
        ans_->setProperty("@.@.weak_ptr",QVariant::fromValue(std::move(wans)));
    }
    return pointer_;
}

}/*~cct*/


#endif // QT_CORE_LIB
#endif // QSTDSHAREDPOINTER_HPP



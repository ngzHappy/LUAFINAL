#if !defined(QLUACORE_GLOBAL_HPP__)
#define QLUACORE_GLOBAL_HPP__

#include <QtCore>
#include <QtCore/qglobal.h>

#if defined(QLUACORE_LIBRARY)
#  define QLUACORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define QLUACORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QLUACORE_GLOBAL_HPP





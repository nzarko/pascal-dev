#ifndef PDUTILS_GLOBAL_H
#define PDUTILS_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef PDUTILS_LIB
# define PDUTILS_EXPORT Q_DECL_EXPORT
#else
# define PDUTILS_EXPORT Q_DECL_IMPORT
#endif

#endif // PDUTILS_GLOBAL_H

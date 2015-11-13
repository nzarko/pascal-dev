#ifndef PDUTILS_H
#define PDUTILS_H

#include "pdutils_global.h"
#include <QtCore/QList>

class PDUTILS_EXPORT PDUtils
{
private:
	QString m_env_path;
	QStringList m_env_path_entries;
public:
	PDUtils();
	~PDUtils();

	QString env_path();
	QStringList env_path_entries();
	bool env_prog_exists(QString &pr_exec_name);

};

#endif // PDUTILS_H

#include "pdutils.h"

#include <QtDebug>
#include <QFileInfo>
#include <QDir>

PDUtils::PDUtils()
{
	m_env_path = "";
	m_env_path_entries.clear();

}

PDUtils::~PDUtils()
{

}

bool PDUtils::env_prog_exists(QString& pr_exec_name)
{
	if (m_env_path_entries.isEmpty())
		env_path_entries();

	QFileInfo file_info;
	QString program_full_path;

	for (int i = 0; i < m_env_path_entries.size(); i++)
	{
		program_full_path = m_env_path_entries[i] + "/" + pr_exec_name;
		file_info.setFile(program_full_path);
		if (file_info.exists() && file_info.isExecutable())
		{
			pr_exec_name = file_info.absoluteFilePath();
			return true;
		}
	}
	return false;
}

QString PDUtils::env_path()
{
	m_env_path = qgetenv("PATH");
	qDebug() << "Path environment variable : " << m_env_path << endl;
	if (!m_env_path.isEmpty())
		return m_env_path;
	else
		return QString("");
}

QStringList PDUtils::env_path_entries()
{
	char sep;
#if defined Q_OS_LINUX || defined Q_OS_FREEBSD
	sep = ':';
#else
	sep = ';';
#endif

	m_env_path_entries.clear();
	m_env_path_entries = env_path().split(sep);

	return m_env_path_entries;
}
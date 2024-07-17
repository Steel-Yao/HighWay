#include "userset.h"
#include <QException>
#include <QFile>
#include <QSaveFile>
#include <QTextStream>

// 从文件读取账户信息
bool UserSet::loadUsers()
{
	QFile file("data/users.txt");
	if (!file.exists()) return false;
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

	QTextStream tStream(&file);
	tStream.setAutoDetectUnicode(true);

	tStream.readLine();      // 跳过标题行
	while (!tStream.atEnd()) // 开始读取用户信息
	{
		QString id, password;
		int     permisson;
		tStream >> id >> password >> permisson;
		users.append(User(id, password, permisson));
	}
	if (!users.isEmpty()) users.removeLast(); // 删除读取的文件尾空行
	file.close();
	return true;
}

// 将账户信息写入文件
bool UserSet::saveUsers()
{
	// 排序
	std::sort(users.begin(), users.end(), [](const User& a, const User& b) {
		if (a.permission != b.permission) return a.permission > b.permission;
		return a.id < b.id;
	});

	QSaveFile file("data/users.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

	try
	{
		QTextStream tStream(&file);
		tStream.setAutoDetectUnicode(true);

		tStream << "账号 密码 权限\n"; // 写入标题行
		for (const auto& i : users)    // 开始保存用户信息
		{
			tStream << i.id << " " << i.password << " " << i.permission << "\n";
		}
		file.commit(); // 保存修改

		return true;
	} catch (QException& e) // 保存出错
	{
		qDebug() << "保存 账户信息 失败！";
		file.cancelWriting(); // 取消修改
		return false;
	}
}

// 增加用户
bool UserSet::addUser(const QString& id, const QString& password, int permission)
{
	for (const auto& i : users) // id验重
	{
		if (i.id == id) return false;
	}
	users.append(User(id, password, permission));
	return true;
}

// 删除用户
bool UserSet::delUser(const QString& id)
{
	for (auto i = users.constBegin(); i != users.constEnd(); i++)
	{
		if (i->id == id)
		{
			users.erase(i);
			return true;
		}
	}
	return false;
}

// 筛选用户
QList<User*> UserSet::filterUser(const Filter<User>& filter)
{
	QList<User*> list;
	for (auto& i : users)
	{
		if (filter.match(i)) list.append(&i);
	}
	return list;
}

// 获取id列表
QStringList UserSet::idList() const
{
	QStringList idList;
	for (const auto& i : users)
	{
		if (!idList.contains(i.id)) idList << i.id;
	}
	return idList;
}

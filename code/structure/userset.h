#ifndef USERSET_H
#define USERSET_H

#include <QList>
#include <QStringList>
#include "filter.h"
#include "user.h"

class UserSet
{
public:
	bool loadUsers(); // 从文件读取账户信息
	bool saveUsers(); // 将账户信息写入文件

	bool addUser(const QString& id, const QString& password, int permission); // 增加用户
	bool delUser(const QString& id);                                          // 删除用户-Id

	QList<User*> filterUser(const Filter<User>& filter);                      // 筛选用户

	QStringList idList() const;                                               // 获取id列表

	const QList<User>& getUsers() const { return users; }

private:
	QList<User> users;
};

#endif // USERSET_H

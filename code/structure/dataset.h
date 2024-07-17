#ifndef DATASET_H
#define DATASET_H

#include <QDateTime>
#include <QList>
#include <QString>
#include "filter.h"

template<typename T>
class DataSet
{
public:
	bool loadData(); // 从文件读取数据
	bool saveData(); // 将数据写入文件

	// 增加数据-人流数据
	bool addData(const QString&   IMEI,
	             const QString&   deviceId,
	             const QString&   placeId,
	             const QDateTime& dateTime);
	// 增加数据-车流数据
	bool addData(int              speed,
	             const QString&   carId,
	             const QString&   deviceId,
	             const QString&   placeId,
	             const QDateTime& dateTime);
	// 删除数据-人流数据
	bool delData(const QString& IMEI, const QDateTime& dateTime);
	// 删除数据-车流数据
	bool delData(int speed, const QString& carId, const QDateTime& dateTime);

	// 筛选数据
	QList<T*> filterData(const Filter<T>& filter)
	{
		QList<T*> list;
		for (auto& i : data)
			if (filter.match(i)) list.append(&i);

		return list;
	}

	// 获取设备编号列表
	QStringList deviceIdList() const
	{
		QStringList deviceIdList;
		for (const auto& i : data)
		{
			if (!deviceIdList.contains(i.deviceId)) deviceIdList << i.deviceId;
		}
		return deviceIdList;
	}

	// 获取桩号列表
	QStringList placeIdList() const
	{
		QStringList placeIdList;
		for (const auto& i : data)
		{
			if (!placeIdList.contains(i.placeId)) placeIdList << i.placeId;
		}
		return placeIdList;
	}

	// 获取IMEI列表
	QStringList IMEIList() const
	{
		QStringList IMEIList;
		for (const auto& i : data)
		{
			if (!IMEIList.contains(i.IMEI)) IMEIList << i.IMEI;
		}
		return IMEIList;
	}

	// 获取最早时间
	QDateTime earliestDateTime() const
	{
		if (data.isEmpty()) return QDateTime::currentDateTime();
		QDateTime earliest = data[0].dateTime;
		for (const auto& i : data)
		{
			if (i.dateTime < earliest) earliest = i.dateTime;
		}
		return earliest;
	}

	// 获取最晚时间
	QDateTime latestDateTime() const
	{
		if (data.isEmpty()) return QDateTime::currentDateTime();
		QDateTime latest = data[0].dateTime;
		for (const auto& i : data)
		{
			if (i.dateTime > latest) latest = i.dateTime;
		}
		return latest;
	}

	// 获取车牌列表
	QStringList carIdList() const
	{
		QStringList carIdList;
		for (const auto& i : data)
		{
			if (!carIdList.contains(i.carId)) carIdList << i.carId;
		}
		return carIdList;
	}

	// 获取最低车速
	int minSpeed()
	{
		if (data.isEmpty()) return 0;
		int minSpeed = data[0].speed;
		for (const auto& i : data)
		{
			if (i.speed < minSpeed) minSpeed = i.speed;
		}
		return minSpeed;
	}

	// 获取最高车速
	int maxSpeed()
	{
		if (data.isEmpty()) return 0;
		int maxSpeed = data[0].speed;
		for (const auto& i : data)
		{
			if (i.speed > maxSpeed) maxSpeed = i.speed;
		}
		return maxSpeed;
	}

	const QList<T>& getData() const { return data; }

private:
	QList<T> data;
};

#endif // DATASET_H

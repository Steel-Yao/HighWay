#include "deviceset.h"
#include <QException>
#include <QFile>
#include <QSaveFile>
#include <QTextStream>

DeviceSet::DeviceSet(DataSet<PersonData>* personData, DataSet<CarData>* carData)
	: personData(personData)
	, carData(carData)
{}

// 从文件读取设备信息
bool DeviceSet::loadDevices()
{
	QFile file("data/devices.txt");
	if (!file.exists()) return false;
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

	QTextStream tStream(&file);
	tStream.setAutoDetectUnicode(true);

	tStream.readLine();      // 跳过标题行
	while (!tStream.atEnd()) // 开始读取设备信息
	{
		QString deviceId, deviceClass, placeId;
		tStream >> deviceId >> deviceClass >> placeId;
		devices.append(Device(deviceId, deviceClass, placeId));
	}
	if (!devices.isEmpty()) devices.removeLast(); // 删除读取的文件尾空行
	file.close();
	return true;
}

// 保存设备信息到文件
bool DeviceSet::saveDevices()
{
	// 排序
	std::sort(devices.begin(), devices.end(), [](const Device& a, const Device& b) {
		return a.deviceId < b.deviceId;
	});

	QSaveFile file("data/devices.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

	try
	{
		QTextStream tStream(&file);
		tStream.setAutoDetectUnicode(true);

		tStream << "设备编号 设备类型 桩号\n"; // 写入标题行
		for (const auto& i : devices)          // 开始保存设备信息
		{
			tStream << i.deviceId << " " << i.deviceClass << " " << i.placeId << "\n";
		}
		file.commit(); // 保存修改

		return true;
	} catch (QException& e) // 保存出错
	{
		qDebug() << "保存 设备信息 失败！";
		file.cancelWriting(); // 取消修改
		return false;
	}
}

// 增加设备
bool DeviceSet::addDevice(const QString& deviceId,
                          const QString& deviceClass,
                          const QString& placeId)
{
	for (const auto& i : devices) // 设备id验重
	{
		if (i.deviceId == deviceId) return false;
	}
	devices.append(Device(deviceId, deviceClass, placeId));
	return true;
}

// 删除设备
bool DeviceSet::delDevice(const QString& deviceId)
{
	for (auto i = devices.constBegin(); i != devices.constEnd(); i++)
	{
		if (i->deviceId == deviceId)
		{
			// 更改对应数据的设备编号信息
			if (i->deviceClass == "监控探头")
			{
				Filter<CarData> filter;
				filter.addCriteria("deviceId", deviceId);
				auto list = carData->filterData(filter);
				for (const auto& i : list) i->setDeviceId(i->getDeviceId() + "（已删除）");
			} else // 4G探针
			{
				Filter<PersonData> filter;
				filter.addCriteria("deviceId", deviceId);
				auto list = personData->filterData(filter);
				for (const auto& i : list) i->setDeviceId(i->getDeviceId() + "（已删除）");
			}
			devices.erase(i);
			return true;
		}
	}
	return false;
}

// 筛选设备
QList<Device*> DeviceSet::filterDevice(const Filter<Device>& filter)
{
	QList<Device*> list;
	for (auto& i : devices)
	{
		if (filter.match(i)) list.append(&i);
	}
	return list;
}

// 获取设备Id列表
QStringList DeviceSet::deviceIdList() const
{
	QStringList deviceIdList;
	for (const auto& i : devices)
	{
		if (!deviceIdList.contains(i.deviceId)) deviceIdList << i.deviceId;
	}
	return deviceIdList;
}

// 获取桩号列表
QStringList DeviceSet::placeIdList() const
{
	QStringList placeIdList;
	for (const auto& i : devices)
	{
		if (!placeIdList.contains(i.placeId)) placeIdList << i.placeId;
	}
	return placeIdList;
}

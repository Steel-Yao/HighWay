#ifndef DEVICESET_H
#define DEVICESET_H

#include <QList>
#include <QString>
#include "cardata.h"
#include "dataset.h"
#include "device.h"
#include "filter.h"
#include "persondata.h"

class DeviceSet
{
public:
	DeviceSet(DataSet<PersonData>* personData, DataSet<CarData>* carData);

	bool loadDevices(); // 从文件读取设备信息
	bool saveDevices(); // 保存设备信息到文件

	bool addDevice(const QString& deviceId,
	               const QString& deviceClass,
	               const QString& placeId);                    // 增加设备
	bool delDevice(const QString& deviceId);                   // 删除设备

	QList<Device*> filterDevice(const Filter<Device>& filter); // 筛选设备

	QStringList deviceIdList() const;                          // 获取设备Id列表
	QStringList placeIdList() const;                           // 获取桩号列表

	const QList<Device>& getDevices() const { return devices; }

private:
	QList<Device> devices;

	DataSet<PersonData>* personData;
	DataSet<CarData>*    carData;
};

#endif // DEVICESET_H

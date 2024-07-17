#include "placeset.h"
#include <QException>
#include <QFile>
#include <QSaveFile>
#include <QTextStream>

PlaceSet::PlaceSet(DeviceSet* deviceSet, DataSet<PersonData>* personData, DataSet<CarData>* carData)
	: deviceSet(deviceSet)
	, personData(personData)
	, carData(carData)
{}

// 从文件读入监测点
bool PlaceSet::loadPlaces()
{
	QFile file("data/places.txt");
	if (!file.exists()) return false;
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

	QTextStream tStream(&file);
	tStream.setAutoDetectUnicode(true);

	tStream.readLine();      // 跳过标题行
	while (!tStream.atEnd()) // 开始读取监测点
	{
		QString placeId, placeName;
		tStream >> placeId >> placeName;
		places.append(Place(placeId, placeName));
	}
	if (!places.isEmpty()) places.removeLast(); // 删除读取的文件尾空行
	file.close();
	return true;
}

// 保存监测点到文档
bool PlaceSet::savePlaces()
{
	std::sort(places.begin(), places.end(), [](const Place& a, const Place& b) {
		return a.placeId < b.placeId;
	});

	QSaveFile file("data/places.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
	try
	{
		QTextStream tStream(&file);
		tStream.setAutoDetectUnicode(true);

		tStream << "桩号	地点\n"; // 写入标题行
		for (const auto& i : places) // 开始保存监测点
		{
			tStream << i.placeId << "	" << i.placeName << "\n";
		}
		file.commit(); // 保存修改

		return true;
	} catch (QException& e) // 保存出错
	{
		qDebug() << "保存 监测点信息 失败！";
		file.cancelWriting(); // 取消保存
		return false;
	}
}

// 增加监测点
bool PlaceSet::addPlace(const QString& placeId, const QString& placeName)
{
	for (const auto& i : places) // 桩号验重
	{
		if (i.placeId == placeId) return false;
	}
	places.append(Place(placeId, placeName));
	return true;
}

// 删除监测点（同时删除相应设备）
bool PlaceSet::delPlace(const QString& placeId)
{
	for (auto i = places.constBegin(); i != places.constEnd(); i++)
	{
		if (i->placeId == placeId)
		{
			places.erase(i); // 删除监测点

			// 删除相应的设备
			Filter<Device> deviceFilter;                  // 设备筛选器
			deviceFilter.addCriteria("placeId", placeId); // 设置筛选规则-桩号
			auto deviceList = deviceSet->filterDevice(deviceFilter);
			QStringList deviceIdList;
			for (const auto& i : deviceList) deviceIdList.append(i->getDeviceId());
			for (const auto& i : deviceIdList) deviceSet->delDevice(i);

			// 更新相应人流数据信息
			Filter<PersonData> personFilter;
			personFilter.addCriteria("placeId", placeId);
			auto personList = personData->filterData(personFilter);
			for (const auto& i : personList) i->setPlaceId(i->getPlaceId() + "（已删除）");

			// 更新相应车流数据信息
			Filter<CarData> carFilter;
			carFilter.addCriteria("placeId", placeId);
			auto carList = carData->filterData(carFilter);
			for (const auto& i : carList) i->setPlaceId(i->getPlaceId() + "（已删除）");

			return true;
		}
	}
	return false;
}

// 筛选监测点
QList<Place*> PlaceSet::filterPlace(const Filter<Place>& filter)
{
	QList<Place*> list;
	for (auto& i : places)
	{
		if (filter.match(i)) list.append(&i);
	}
	return list;
}

// 获取桩号列表
QStringList PlaceSet::placeIdList() const
{
	QStringList placeIdList;
	for (const auto& i : places)
	{
		if (!placeIdList.contains(i.placeId)) placeIdList << i.placeId;
	}
	return placeIdList;
}

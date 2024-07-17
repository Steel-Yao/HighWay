#ifndef PLACESET_H
#define PLACESET_H

#include <QList>
#include <QString>
#include "cardata.h"
#include "dataset.h"
#include "deviceset.h"
#include "filter.h"
#include "persondata.h"
#include "place.h"

class PlaceSet
{
public:
	PlaceSet(DeviceSet* deviceSet, DataSet<PersonData>* personData, DataSet<CarData>* carData);

	bool loadPlaces();                                               // 从文件读入监测点
	bool savePlaces();                                               // 保存监测点到文件

	bool addPlace(const QString& placeId, const QString& placeName); // 增加监测点
	bool delPlace(const QString& placeId); // 删除监测点（同时删除相应设备）

	QList<Place*> filterPlace(const Filter<Place>& filter); // 筛选监测点

	QStringList placeIdList() const;                        // 获取桩号列表

	const QList<Place>& getPlaces() const { return places; }

private:
	QList<Place> places;

	DeviceSet*           deviceSet;
	DataSet<PersonData>* personData;
	DataSet<CarData>*    carData;
};

#endif // PLACESET_H

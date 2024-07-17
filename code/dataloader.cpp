#include "dataloader.h"

#include <QMessageBox>

DataLoader::DataLoader(UserSet*             userSet,
                       DeviceSet*           deviceSet,
                       PlaceSet*            placeSet,
                       DataSet<PersonData>* personData,
                       DataSet<CarData>*    carData,
                       DataSet<VideoData>*  videoSet,
                       QObject*             parent)
	: QThread{parent}
	, userSet(userSet)
	, deviceSet(deviceSet)
	, placeSet(placeSet)
	, personData(personData)
	, carData(carData)
	, videoSet(videoSet)
{}

DataLoader::~DataLoader() {}

void DataLoader::loadData()
{
	// 从文件载入数据
	emit userLoaded(userSet->loadUsers());
	emit carLoaded(carData->loadData());
	emit personLoaded(personData->loadData());
	emit deviceLoaded(deviceSet->loadDevices());
	emit placeLoaded(placeSet->loadPlaces());
	emit videoLoaded(videoSet->loadData());
}

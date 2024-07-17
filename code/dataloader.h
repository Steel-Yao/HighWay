#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>
#include <QThread>
#include "structure/cardata.h"
#include "structure/dataset.h"
#include "structure/deviceset.h"
#include "structure/persondata.h"
#include "structure/placeset.h"
#include "structure/userset.h"
#include "structure/videodata.h"

class DataLoader : public QThread
{
	Q_OBJECT
public:
	explicit DataLoader(UserSet*             userSet,
	                    DeviceSet*           deviceSet,
	                    PlaceSet*            placeSet,
	                    DataSet<PersonData>* personData,
	                    DataSet<CarData>*    carData,
	                    DataSet<VideoData>*  videoSet,
	                    QObject*             parent = nullptr);
	~DataLoader();

signals:
	void userLoaded(bool ok);
	void carLoaded(bool ok);
	void personLoaded(bool ok);
	void deviceLoaded(bool ok);
	void placeLoaded(bool ok);
	void videoLoaded(bool ok);
	void dataLoaded();

protected:
	void run() override
	{
		loadData();
		emit dataLoaded();
	}

private:
	void loadData();

	UserSet*             userSet;
	DeviceSet*           deviceSet;
	PlaceSet*            placeSet;
	DataSet<PersonData>* personData;
	DataSet<CarData>*    carData;
	DataSet<VideoData>*  videoSet;
};

#endif // DATALOADER_H

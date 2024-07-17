#include "dataset.h"
#include <QException>
#include <QFile>
#include <QSaveFile>
#include <QTextStream>
#include "cardata.h"
#include "persondata.h"
#include "videodata.h"

template<> // 从文件读取数据-人流数据
bool DataSet<PersonData>::loadData()
{
	QFile file("data/persondata.txt");
	if (!file.exists()) return false;
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

	QTextStream tStream(&file);
	tStream.setAutoDetectUnicode(true);

	tStream.readLine();      // 跳过标题行
	while (!tStream.atEnd()) // 开始读取人流数据信息
	{
		QString deviceId, dateTime, placeId, IMEI;
		tStream >> deviceId >> dateTime >> placeId >> IMEI;
		data.append(PersonData(IMEI,
		                       deviceId,
		                       placeId,
		                       QDateTime::fromString(dateTime, "yyyy-MM-ddThh:mm:ss")));
	}
	if (!data.isEmpty()) data.removeLast(); // 删除读取的文件尾空行
	file.close();

	// 排序
	std::sort(data.begin(), data.end(), [](const PersonData& a, const PersonData& b) {
		if (a.deviceId != b.deviceId) return a.deviceId < b.deviceId;
		return a.dateTime > b.dateTime;
	});

	return true;
}

template<> // 从文件读取数据-车流数据
bool DataSet<CarData>::loadData()
{
	QFile file("data/cardata.txt");
	if (!file.exists()) return false;
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

	QTextStream tStream(&file);
	tStream.setAutoDetectUnicode(true);

	tStream.readLine();      // 跳过标题行
	while (!tStream.atEnd()) // 开始读取车流数据信息
	{
		QString deviceId, dateTime, placeId, carId;
		int     speed;
		tStream >> deviceId >> dateTime >> placeId >> speed >> carId;
		data.append(CarData(speed,
		                    carId,
		                    deviceId,
		                    placeId,
		                    QDateTime::fromString(dateTime, "yyyy-MM-ddThh:mm")));
	}
	if (!data.isEmpty()) data.removeLast(); // 删除读取的文件尾空行
	file.close();

	// 排序
	std::sort(data.begin(), data.end(), [](const CarData& a, const CarData& b) {
		if (a.deviceId != b.deviceId) return a.deviceId < b.deviceId;
		return a.dateTime > b.dateTime;
	});

	return true;
}

template<> // 从文件读取数据-视频数据
bool DataSet<VideoData>::loadData()
{
	QFile file("data/video.txt");
	if (!file.exists()) return false;
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

	QTextStream tStream(&file);
	tStream.setAutoDetectUnicode(true);

	tStream.readLine();      // 跳过标题行
	while (!tStream.atEnd()) // 开始读取视频数据信息
	{
		QString videoId, dateTime, placeId, deviceId;
		tStream >> videoId >> dateTime >> placeId >> deviceId;
		data.append(VideoData(videoId,
		                      deviceId,
		                      placeId,
		                      QDateTime::fromString(dateTime, "yyyy-MM-ddThh:mm:ss")));
	}
	if (!data.isEmpty()) data.removeLast(); // 删除读取的文件尾空行
	file.close();

	// 排序
	std::sort(data.begin(), data.end(), [](const VideoData& a, const VideoData& b) {
		if (a.deviceId != b.deviceId) return a.deviceId < b.deviceId;
		return a.dateTime > b.dateTime;
	});

	return true;
}

template<> // 将数据写入文件-人流数据
bool DataSet<PersonData>::saveData()
{
	QSaveFile file("data/persondata.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

	try
	{
		QTextStream tStream(&file);
		tStream.setAutoDetectUnicode(true);

		tStream << "设备编号 探测时间 桩号 IMEI号\n"; // 写入标题行
		for (const auto& i : data)                    // 开始保存人流数据
		{
			tStream << i.deviceId << " " << i.dateTime.toString("yyyy-MM-ddThh:mm:ss") << " "
					<< i.placeId << " " << i.IMEI << "\n";
		}
		file.commit(); // 保存修改

		return true;
	} catch (QException& e)   // 保存出错
	{
		file.cancelWriting(); // 取消修改
		return false;
	}
}

template<> // 将数据写入文件-车流数据
bool DataSet<CarData>::saveData()
{
	QSaveFile file("data/cardata.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

	try
	{
		QTextStream tStream(&file);
		tStream.setAutoDetectUnicode(true);

		tStream << "探测器编号 时间 桩号 车速 车牌\n"; // 写入标题行
		for (const auto& i : data)                     // 开始保存车流数据
		{
			tStream << i.deviceId << " " << i.dateTime.toString("yyyy-MM-ddThh:mm") << " "
					<< i.placeId << " " << i.speed << " " << i.carId << "\n";
		}
		file.commit(); // 保存修改

		return true;
	} catch (QException& e)   // 保存出错
	{
		file.cancelWriting(); // 取消修改
		return false;
	}
}

template<> // 将数据写入文件-视频数据
bool DataSet<VideoData>::saveData()
{
	QSaveFile file("data/video.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

	try
	{
		QTextStream tStream(&file);
		tStream.setAutoDetectUnicode(true);

		tStream << "视频编号 录制时间 桩号 录制设备\n"; // 写入标题行
		for (const auto& i : data)                      // 开始保存人流数据
		{
			tStream << i.videoId << " " << i.dateTime.toString("yyyy-MM-ddThh:mm:ss") << " "
					<< i.placeId << " " << i.deviceId << "\n";
		}
		file.commit(); // 保存修改

		return true;
	} catch (QException& e)   // 保存出错
	{
		file.cancelWriting(); // 取消修改
		return false;
	}
}

template<> // 增加数据-人流数据
bool DataSet<PersonData>::addData(const QString&   IMEI,
                                  const QString&   deviceId,
                                  const QString&   placeId,
                                  const QDateTime& dateTime)
{
	data.append(PersonData(IMEI, deviceId, placeId, dateTime));
	return true;
}



template<> // 增加数据-车流数据
bool DataSet<CarData>::addData(int              speed,
                               const QString&   carId,
                               const QString&   deviceId,
                               const QString&   placeId,
                               const QDateTime& dateTime)
{
	data.append(CarData(speed, carId, deviceId, placeId, dateTime));
	return true;
}

template<> // 删除数据-人流数据
bool DataSet<PersonData>::delData(const QString& IMEI, const QDateTime& dateTime)
{
	for (auto i = data.constBegin(); i != data.constEnd(); i++)
	{
		if (i->IMEI == IMEI && i->dateTime == dateTime)
		{
			data.erase(i);
			return true;
		}
	}
	return false;
}

template<> // 删除数据-车流数据
bool DataSet<CarData>::delData(int speed, const QString& carId, const QDateTime& dateTime)
{
	for (auto i = data.constBegin(); i != data.constEnd(); i++)
	{
		if (i->speed == speed && i->carId == carId && i->dateTime == dateTime)
		{
			data.erase(i);
			return true;
		}
	}
	return false;
}

template<typename T>
class DataSet;

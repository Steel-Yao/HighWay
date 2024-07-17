#include "videoplayer.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QMessageBox>
#include <QPalette>
#include <QVBoxLayout>


VideoPlayer::VideoPlayer(const QString& path, QWidget* parent)
	: QDialog(parent)
	, m_resource(path)
	, m_voice(50)
	, currentFormattedTime("00:00")
{
	setAttribute(Qt::WA_DeleteOnClose);
	audiooutput = new QAudioOutput();

	QVBoxLayout* vlayout = new QVBoxLayout(this);
	hlayout              = new QHBoxLayout();

	player = new QMediaPlayer;
	player->setPlaybackRate(1.0); //默认1倍速播放

	videoWidget = new QVideoWidget(this);
	videoWidget->setAspectRatioMode(Qt::IgnoreAspectRatio); //缩放适应videoWidget的大小

	player->setVideoOutput(videoWidget);                    //设置播放窗口
	player->setAudioOutput(audiooutput);                    //设置声音
	audiooutput->setVolume(m_voice);                        //初始音量为50

	Init();                                                 //UI控件初始化

	vlayout->addWidget(videoWidget, 8);
	vlayout->addWidget(m_Progressslider, 1);
	vlayout->addLayout(hlayout, 0);

	connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(getduration(qint64)));
	connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(VideoPosChange(qint64)));

	videoWidget->show(); //显示视频界面

	setFixedSize(1024, 600);

	player->setSource(QUrl::fromLocalFile(m_resource)); //设置播放资源
	player->play();                                     //播放视频
}

void VideoPlayer::ButtonStyleSet(QPushButton* button, QString IconPath)
{
	// 设置图像
	button->setIcon(QIcon(IconPath));
	button->setFlat(true); //去除边框
	button->setStyleSheet("QPushButton:hover {background-color: grey;} QPushButton:pressed "
	                      "{background-color: darkGrey;}");
}

void VideoPlayer::Init()
{
	/*声音控件*/
	m_voicebutton = new QPushButton();
	ButtonStyleSet(m_voicebutton, ":/icons/video_normal.png");
	m_voiceslider = new QSlider(Qt::Horizontal);
	m_voiceslider->setValue(50);
	m_voiceslider->setRange(0, 100);

	/*倍速控件*/
	m_speedbutton = new QPushButton();
	ButtonStyleSet(m_speedbutton, ":/icons/video_speed.png");
	m_speedlabel  = new QLabel();
	m_speedslider = new QSlider(Qt::Horizontal);
	m_speedslider->setRange(0, 8); //0-4倍速
	m_speedslider->setTickInterval(1);
	m_speedslider->setValue(4);
	m_speedlabel->setText(QString::number(4.0 * 2.0 / 8.0, 'g', 3) + QString("倍速"));

	/*控制视频播放按键*/
	m_palybutton = new QPushButton();
	ButtonStyleSet(m_palybutton, ":/icons/video_start.png");
	m_stopbutton = new QPushButton();
	ButtonStyleSet(m_stopbutton, ":/icons/video_stop.png");
	m_backbutton = new QPushButton();
	ButtonStyleSet(m_backbutton, ":/icons/video_back .png");
	m_aheadbutton = new QPushButton();
	ButtonStyleSet(m_aheadbutton, ":/icons/video_goup.png");
	// m_cutscreen = new QPushButton();
	// ButtonStyleSet(m_cutscreen, ":/icons/video_cut.png");
	// m_selectbutton = new QPushButton();
	// ButtonStyleSet(m_selectbutton, ":/icons/video_add.png");

	/*视频进度条*/
	m_Progressslider = new QSlider(Qt::Horizontal);
	m_Progressslider->setRange(0, 100); //设置进度条范围
	m_Progressslider->setValue(0);      //初始值为0

	/*显示时间label*/
	m_timelabel = new QLabel();

	hlayout->addWidget(m_voicebutton);
	hlayout->addWidget(m_voiceslider);
	hlayout->addStretch();
	hlayout->addWidget(m_speedbutton);
	hlayout->addWidget(m_speedslider);
	hlayout->addWidget(m_speedlabel);
	hlayout->addStretch();
	hlayout->addWidget(m_palybutton);
	hlayout->addWidget(m_stopbutton);
	hlayout->addWidget(m_backbutton);
	hlayout->addWidget(m_aheadbutton);
	// hlayout->addWidget(m_selectbutton);
	// hlayout->addWidget(m_cutscreen);
	hlayout->addWidget(m_timelabel);


	connect(m_voicebutton, SIGNAL(clicked()), this, SLOT(voiceclick()));
	connect(m_voiceslider, SIGNAL(valueChanged(int)), this, SLOT(voicechange(int)));
	connect(m_palybutton, SIGNAL(clicked()), this, SLOT(playclick()));
	connect(m_stopbutton, SIGNAL(clicked()), this, SLOT(stopclick()));
	connect(m_Progressslider, SIGNAL(valueChanged(int)), this, SLOT(ProgressChange(int)));
	connect(m_aheadbutton, SIGNAL(clicked()), this, SLOT(aheadclick()));
	connect(m_backbutton, SIGNAL(clicked()), this, SLOT(backclick()));
	// connect(m_selectbutton, SIGNAL(clicked()), this, SLOT(GetPlayResource()));
	connect(m_speedslider, SIGNAL(valueChanged(int)), this, SLOT(SpeedChange(int)));
	// connect(m_cutscreen, SIGNAL(clicked()), this, SLOT(CutPicture()));
	connect(this, &VideoPlayer::aboutTiClose, this, &VideoPlayer::videoclose);
}

VideoPlayer::~VideoPlayer()
{
	//delete ui;
}

void VideoPlayer::closeEvent(QCloseEvent* event)
{
	emit aboutTiClose();
	QWidget::closeEvent(event);
}

void VideoPlayer::captureScreenshot()
{
	// QImage  image     = grabFrame();
	// QString allowedDi = "D:/Users/Lenovo/Desktop/c++/QT/myWuYi/build/"
	// 					"Desktop_Qt_6_7_1_MinGW_64_bit-Debug/cutPh";
	// QString filename = QFileDialog::getSaveFileName(nullptr, "保存", allowedDi, "PNG File(*.png)");
	// image.save(filename);
}

QImage VideoPlayer::grabFrame()
{
	QImage image(videoWidget->size(), QImage::Format_RGB32);
	image.fill(Qt::transparent);
	QPainter painter(&image);
	render(&painter);
	return image;
}

void VideoPlayer::voiceclick()
{
	if (m_voiceslider->value() == 0)
	{
		audiooutput->setVolume(m_voice); //设置回到之前的音量
		ButtonStyleSet(m_voicebutton, ":/icons/video_normal.png");
		m_voiceslider->setValue(m_voice);
	} else {
		audiooutput->setVolume(0); //静音
		ButtonStyleSet(m_voicebutton, ":/icons/video_no.png");
		m_voiceslider->setValue(0);
	}
}

void VideoPlayer::voicechange(int voice)
{
	audiooutput->setVolume(voice);
	if (voice == 0)
	{
		ButtonStyleSet(m_voicebutton, ":/icons/video_no.png");
	} else if (voice <= 33 and voice > 0) {
		ButtonStyleSet(m_voicebutton, ":/icons/video_little.png");
	} else if (voice > 33 and voice <= 66) {
		ButtonStyleSet(m_voicebutton, ":/icons/video_normal.png");
	} else {
		ButtonStyleSet(m_voicebutton, ":/icons/video_large.png");
	}
}

/*继续播放*/
void VideoPlayer::playclick()
{
	player->play();
}

/*暂停视频播放*/
void VideoPlayer::stopclick()
{
	player->pause();
}

void VideoPlayer::ProgressChange(int pos)
{
	if (m_Progressslider->isSliderDown()) player->setPosition(pos * player->duration() / 100);
}

/*获得视频播放总时间*/
void VideoPlayer::getduration(qint64 duration)
{
	QTime totalTime = QTime(0, 0, 0, 0);
	totalTime = totalTime.addMSecs(duration); // 将视频的总时长（毫秒）添加到 QTime 对象中
	totalFormattedTime = totalTime.toString("mm:ss"); // 转换 QTime 对象到分:秒格式字符串
	m_timelabel->setText(currentFormattedTime + " / " + totalFormattedTime);
}

/*按下按键在当前的基础上前进10s*/
void VideoPlayer::aheadclick()
{
	qint64 currentpos = player->position();
	player->setPosition(currentpos + 10000); //在原来的基础上前进10s
}

/*按下按键在当前的基础上后退10s*/
void VideoPlayer::backclick()
{
	qint64 currentpos = player->position();
	player->setPosition(currentpos - 10000); //在原来的基础上前进10s
}

/*当视频播放时进度条改变调用此函数*/
void VideoPlayer::VideoPosChange(qint64 position)
{
	if (m_Progressslider->isSliderDown())
	{
		// 如果正在手动滑动条，则直接退出
		return;
	}
	m_Progressslider->setSliderPosition(100 * position / player->duration());

	QTime currentTime(0, 0, 0, 0); // 初始化一个时间为0的QTime对象
	currentTime = currentTime.addMSecs(
		player->position());       // 将当前播放时间（毫秒）添加到 QTime 对象中
	currentFormattedTime = currentTime.toString("mm:ss"); // 转换 QTime 对象到分:秒格式字符串
	m_timelabel->setText(currentFormattedTime + " / " + totalFormattedTime);
}

/*获得选择的播放源路径并播放*/
// void VideoPlayer::GetPlayResource()
// {
// 	QString allowedDir = "./data/video";
// 	// 打开文件对话框
// 	m_resource = QFileDialog::getOpenFileName(this,
// 	                                          "选择文件",
// 	                                          allowedDir,
// 	                                          "视频文件 (*.mp4 *.avi)");

// 	// 检查选择的文件是否在允许的目录内
// 	if (!m_resource.isEmpty())
// 	{
// 		if (m_resource.startsWith(allowedDir))
// 		{
// 			// 文件在允许的目录内，可以进行后续操作
// 			QMessageBox::information(this, "文件选择", "文件选择成功！");
// 			player->setSource(QUrl::fromLocalFile(m_resource)); //设置播放资源
// 			player->play();                                     //播放视频
// 		} else {
// 			// 文件不在允许的目录内，显示错误消息
// 			QMessageBox::warning(this, "文件选择错误", "选择的文件不在允许的目录内！");
// 		}
// 	}
// }

/*关闭*/
void VideoPlayer::videoclose()
{
	player->stop();
}
/*倍速设置*/
void VideoPlayer::SpeedChange(int value)
{
	m_speedlabel->setText(QString::number(value * 2.0 / 8.0, 'g', 3) + QString("倍速"));
	player->setPlaybackRate(value * 2.0 / 8.0); //设置倍速播放
}
/*截图*/
// void VideoPlayer::CutPicture()
// {
// 	player->pause();
// 	if (m_resource == nullptr)
// 	{
// 		QMessageBox::critical(this, tr("错误"), tr("未打开视频！"));
// 	} else {
// 		int ret1 = QMessageBox::question(this,
// 		                                 tr("保存"),
// 		                                 tr("确定保存吗？"),
// 		                                 QMessageBox::Yes,
// 		                                 QMessageBox::No);
// 		if (ret1 == QMessageBox::Yes)
// 		{
// 			QDir    currentDir = QDir::currentPath();
// 			QString folderPath = currentDir.filePath("cutPh");
// 			if (!QDir(folderPath).exists())
// 			{
// 				QDir().mkdir(folderPath);
// 			}
// 			VideoPlayer::captureScreenshot();
// 			player->play();
// 		} else {
// 			player->play();
// 		}
// 	}
// }

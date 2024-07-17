#ifndef WIDGET_H
#define WIDGET_H

#include <QAudioOutput>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMediaPlayer>
#include <QPainter>
#include <QPushButton>
#include <QSlider>
#include <QTime>
#include <QVideoWidget>
#include <QWidget>

class VideoPlayer : public QDialog
{
	Q_OBJECT

	QMediaPlayer* player;
	QVideoWidget* videoWidget;
	QAudioOutput* audiooutput;

	QPushButton* m_voicebutton;
	QSlider*     m_voiceslider;

	QPushButton* m_speedbutton;
	QLabel*      m_speedlabel;
	QSlider*     m_speedslider;

	QSlider* m_Progressslider;

	QPushButton* m_palybutton;
	QPushButton* m_stopbutton;
	QPushButton* m_backbutton;
	QPushButton* m_aheadbutton;
	// QPushButton* m_selectbutton;
	// QPushButton* m_cutscreen;

	QLabel* m_timelabel;

	QHBoxLayout* hlayout;

	QString m_resource;

	int m_voice;

	QString totalFormattedTime;
	QString currentFormattedTime;

	void Init();

	void ButtonStyleSet(QPushButton* button, QString IconPath);

public:
	VideoPlayer(const QString& path, QWidget* parent = nullptr);
	~VideoPlayer();
signals:
	void aboutTiClose();

protected:
	void closeEvent(QCloseEvent* event) override;
protected slots:
	void voiceclick();
	void voicechange(int voice);
	void playclick();
	void stopclick();
	void getduration(qint64 duration);
	void VideoPosChange(qint64 position);
	void ProgressChange(int pos);
	void aheadclick();
	void backclick();
	// void GetPlayResource();
	void SpeedChange(int value);
	// void CutPicture();
	void captureScreenshot();
	void videoclose();

private:
	QImage grabFrame();
};

#endif // WIDGET_H

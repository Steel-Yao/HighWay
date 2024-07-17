QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += multimedia multimediawidgets
QT += charts

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dataloader.cpp \
    dialogs/adddevicedialog.cpp \
    dialogs/addpersondatadialog.cpp \
    dialogs/addplacedialog.cpp \
    dialogs/adduserdialog.cpp \
    dialogs/deleteuserdialog.cpp \
    dialogs/imeigeneratordialog.cpp \
    dialogs/modifydevicedialog.cpp \
    dialogs/modifypersondatadialog.cpp \
    dialogs/modifyplacedialog.cpp \
    dialogs/videoplayer.cpp \
    main.cpp \
    mainwindow.cpp \
    pages/carpage.cpp \
    pages/currentuserpage.cpp \
    pages/devicepage.cpp \
    pages/homepage.cpp \
    pages/loginpage.cpp \
    pages/managepage.cpp \
    pages/personpage.cpp \
    pages/placepage.cpp \
    pages/videopage.cpp \
    structure/dataset.cpp \
    structure/deviceset.cpp \
    structure/filter.cpp \
    structure/placeset.cpp \
    structure/userset.cpp

HEADERS += \
    dataloader.h \
    dialogs/adddevicedialog.h \
    dialogs/addpersondatadialog.h \
    dialogs/addplacedialog.h \
    dialogs/adduserdialog.h \
    dialogs/deleteuserdialog.h \
    dialogs/imeigeneratordialog.h \
    dialogs/modifydevicedialog.h \
    dialogs/modifypersondatadialog.h \
    dialogs/modifyplacedialog.h \
    dialogs/videoplayer.h \
    mainwindow.h \
    pages/carpage.h \
    pages/currentuserpage.h \
    pages/devicepage.h \
    pages/homepage.h \
    pages/loginpage.h \
    pages/managepage.h \
    pages/personpage.h \
    pages/placepage.h \
    pages/videopage.h \
    structure/cardata.h \
    structure/data.h \
    structure/dataset.h \
    structure/device.h \
    structure/deviceset.h \
    structure/filter.h \
    structure/persondata.h \
    structure/place.h \
    structure/placeset.h \
    structure/user.h \
    structure/userset.h \
    structure/videodata.h

FORMS += \
    dialogs/adddevicedialog.ui \
    dialogs/addpersondatadialog.ui \
    dialogs/addplacedialog.ui \
    dialogs/deleteuserdialog.ui \
    dialogs/imeigeneratordialog.ui \
    dialogs/modifydevicedialog.ui \
    dialogs/modifypersondatadialog.ui \
    dialogs/modifyplacedialog.ui \
    mainwindow.ui \
    pages/carpage.ui \
    pages/currentuserpage.ui \
    pages/devicepage.ui \
    pages/homepage.ui \
    pages/loginpage.ui \
    pages/personpage.ui \
	pages/placepage.ui \
    pages/videopage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = HighWay.ico

RESOURCES += \
    icons.qrc \
	imag.qrc

DISTFILES +=

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addeventwindow.cpp \
    championship.cpp \
    championshipwindow.cpp \
    competitor.cpp \
    deletechampionshipwindow.cpp \
    event.cpp \
    main.cpp \
    mainwindow.cpp \
    managechampionshipwindow.cpp \
    newchampionshipwindow.cpp \
    randomnames.cpp \
    save.cpp \
    savesreader.cpp \
    utils.cpp

HEADERS += \
    addeventwindow.h \
    championship.h \
    championshipwindow.h \
    competitor.h \
    deletechampionshipwindow.h \
    event.h \
    mainwindow.h \
    managechampionshipwindow.h \
    newchampionshipwindow.h \
    randomnames.h \
    save.h \
    savesreader.h \
    utils.h

FORMS += \
    addeventwindow.ui \
    championshipwindow.ui \
    deletechampionshipwindow.ui \
    mainwindow.ui \
    managechampionshipwindow.ui \
    newchampionshipwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

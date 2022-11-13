QT       += core gui

VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_BUILD = 0

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR" \
        "VERSION_MINOR=$$VERSION_MINOR" \
        "VERSION_BUILD=$$VERSION_BUILD"

#Target version
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

QMAKE_TARGET_COPYRIGHT = "Copyright(C) 2022 Ray Lee, All Rights Reserved."

RC_ICONS = Resources/q-md5_2.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MD5Thread.cpp \
    main.cpp \
    dialog.cpp

HEADERS += \
    MD5Thread.h \
    dialog.h

FORMS += \
    dialog.ui

TRANSLATIONS += \
    q-md5sum_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/Resources.qrc

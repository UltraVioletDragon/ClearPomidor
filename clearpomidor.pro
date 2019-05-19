HEADERS       = \
    focusclock.h \
    window.h \
    settings.h \
    dialog.h
SOURCES       = main.cpp \
    focusclock.cpp \
    window.cpp \
    settings.cpp \
    dialog.cpp
RESOURCES     = clearpomidor.qrc

#QT += core
QT += widgets
QT += gui
QT += multimedia
QT -= network
#CONFIG += staticlib
requires(qtConfig(combobox))

# install
target.path = C:/Project/qt/ClearPomidor/build
INSTALLS += target

DISTFILES += \
    tmp

TEMP_SOURCES = $$SOURCES

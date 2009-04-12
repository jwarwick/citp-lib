TEMPLATE = app
TARGET = SampleConsole
QT += network script
CONFIG += debug
DESTDIR = ../debug


#win32: LIBS += wsock32.lib
#macx {
#	QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
#	#####CONFIG += x86 ppc
#	CONFIG += x86
#}

HEADERS += SampleConsole.h \

SOURCES += main.cpp \
           SampleConsole.cpp \

FORMS += SampleConsole.ui \

INCLUDEPATH += ../Library

LIBS += -L../debug -lcitp

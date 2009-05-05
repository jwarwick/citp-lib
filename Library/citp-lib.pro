TEMPLATE = lib
TARGET = citp
QT += network script
CONFIG += debug
DESTDIR = ../debug


#win32: LIBS += wsock32.lib
#macx {
#	QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
#	#####CONFIG += x86 ppc
#	CONFIG += x86
#}

HEADERS += citp-lib.h \
           PacketCreator.h \
           PeerInformationSocket.h \
           Peer.h \
           Visualizer.h \
           Fixture.h \
           FixtureModel.h \

SOURCES += citp-lib.cpp \
           PacketCreator.cpp \
           PeerInformationSocket.cpp \
           Peer.cpp \
           Visualizer.cpp \
           Fixture.cpp \
           FixtureModel.cpp \

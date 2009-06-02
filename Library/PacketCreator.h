// -*- mode: C++ -*-
#ifndef _PACKETCREATOR_H_
#define _PACKETCREATOR_H_

#include <QObject>

class PacketCreator : public QObject
{
  Q_OBJECT
public:
  PacketCreator(QObject *parent=0);
  virtual ~PacketCreator();

  typedef enum
    {
      LightingConsole,
      MediaServer,
      Visualizer,
      OperationHub
    } PLocType;

  // returns NULL on error. bufferLen is the length of the returned buffer.
  static unsigned char * createPLocPacket(PLocType plocType, const QString &name, 
					  const QString &state, int &bufferLen);

  // returns NULL on error. bufferLen is the length of the returned buffer.
  static unsigned char * createUNamPacket(quint8 universeIndex, const QString &name,
					  int &bufferLen);

  // returns NULL on error. bufferLen is the length of the returned buffer.
  static unsigned char * createChBkPacket(bool blind,
					  quint8 universeIndex,
					  quint16 firstChannelIndex,
					  quint16 channelCount,
					  const quint8 *channelLevels,
					  int &bufferLen);

  static unsigned char * createSXSrPacket_ArtNet(int network, int universe, int channel, int &bufferLen);
  static unsigned char * createSXSrPacket_Avab(int network, int universe, int channel, int &bufferLen);
  static unsigned char * createSXSrPacket_E131(int universe, int channel, int &bufferLen);
  static unsigned char * createSXSrPacket_ETCNet2(int channel, int &bufferLen);

  static unsigned char * createPtchPacket(quint16 fixtureIdentifier, 
					  quint8 universeIndex, 
					  quint16 channelIndex,
					  quint16 channelCount, 
					  const QString &fixtureMake, 
					  const QString &fixtureName,
					  int &bufferLen);

  static unsigned char * createSPtcPacket(const QList<quint16> &fixtureIdentifiers, int &bufferLen);

  static unsigned char * createUPtcPacket(const QList<quint16> &fixtureIdentifiers, int &bufferLen);

  static unsigned char * createSelePacket(const QList<quint16> &fixtureIdentifiers, 
					  bool complete, int &bufferLen);

  static unsigned char * createDeSePacket(const QList<quint16> &fixtureIdentifiers, int &bufferLen);

private:

  static unsigned char *createSXSrPacket(const QString &str, int &bufferLen);

};

#endif // _PACKETCREATOR_H_

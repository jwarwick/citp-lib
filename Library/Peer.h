// -*- mode: C++ -*-
#ifndef _PEER_H_
#define _PEER_H_

#include <QObject>
#include <QHostAddress>

class QTcpSocket;

class Fixture;
class FixtureModel;

typedef enum
  {
    UNKNOWN_PEER,
    VISUALIZER_PEER,
    MEDIASERVER_PEER,
    LIGHTINGCONSOLE_PEER,
    OPERATIONSHUB_PEER,
  } PeerType;

class Peer : public QObject
{
  Q_OBJECT
public:
  Peer(const QHostAddress &host, quint16 listeningPort, 
       const QString &peerName, const QString &peerState, QObject *parent=0);
  virtual ~Peer();

  
  virtual QString peerName() const;
  virtual QString peerState() const;
  virtual QString peerHost() const;
  virtual quint16 peerListeningPort() const;

  virtual PeerType peerType() const;

  virtual bool connectToPeer();

  FixtureModel *m_fixtureModel;
  QList<Fixture*> m_fixtureList;

 protected:

  PeerType m_peerType;

  QString m_peerState;
  QString m_peerName;

  QHostAddress m_host;
  quint16 m_listeningPort;

  QTcpSocket *m_tcpSocket;

  bool sendPacket(const unsigned char *buffer, int bufferLen);

  void parseBlockOfData(const QByteArray &byteArray);
  void parsePacket(const QByteArray &byteArray);
  void parseFPTCPacket(const QByteArray &byteArray);
  void parseUPTCPacket(const QByteArray &byteArray);  
  void parseSPTCPacket(const QByteArray &byteArray);
  void parsePTCHPacket(const QByteArray &byteArray);

protected slots:
  void handleReadyRead();
 
signals:
  void connectedToPeer();
  void disconnectedFromPeer();

  void unpatchAllFixtures();
  void unpatchFixtures(const QList<quint16> &fixtureIdentifiers);

  void sendPatchAllFixtures();
  void sendPatchFixtures(const QList<quint16> &fixtureIdentifiers);

  void updatedFixtureList();
};

#endif _PEER_H_

#ifndef _PEER_H_
#define _PEER_H_

#include <QObject>
#include <QHostAddress>

class QTcpSocket;

class Peer : public QObject
{
  Q_OBJECT
public:
  Peer(const QHostAddress &host, quint16 listeningPort, 
       const QString &peerName, const QString &peerState, QObject *parent=0);
  virtual ~Peer();

  
  QString peerName() const;
  QString peerState() const;
  QString peerHost() const;
  quint16 peerListeningPort() const;

  bool connectToPeer();

  bool sendUniverseName(quint8 universeIndex, const QString &universeName);

 protected:

  QString m_peerState;
  QString m_peerName;

  QHostAddress m_host;
  quint16 m_listeningPort;

  QTcpSocket *m_tcpSocket;

 signals:
  void connectedToPeer();
  void disconnectedFromPeer();

};

#endif _PEER_H_

#ifndef _PEER_H_
#define _PEER_H_

#include <QObject>
#include <QHostAddress>

class Peer : public QObject
{
Q_OBJECT
public:
  Peer(const QString &host, quint16 listeningPort, const QString &peerType, 
       const QString &peerName, const QString &peerState, QObject *parent=0);
  //Peer(QObject *parent=0);
  virtual ~Peer();

  
  //QString peerName() const;

  QString m_peerType;
  QString m_peerState;
  QString m_peerName;

  QString m_host;
  quint16 m_listeningPort;


};

#endif _PEER_H_

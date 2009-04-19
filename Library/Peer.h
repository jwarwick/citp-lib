#ifndef _PEER_H_
#define _PEER_H_

#include <QObject>
#include <QHostAddress>

class Peer : public QObject
{
Q_OBJECT
public:
  Peer(const QHostAddress &host, quint16 listeningPort, const QString &type, 
       const QString &name, const QString &state, QObject *parent=0);
  virtual ~Peer();

  QString peerName() const;

  QHostAddress m_host;
  quint16 m_listeningPort;
  QString m_type;
  QString m_name;
  QString m_state;
};

#endif _PEER_H_

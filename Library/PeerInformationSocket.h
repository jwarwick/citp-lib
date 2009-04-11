#ifndef _PEERINFORMATIONSOCKET_H_
#define _PEERINFORMATIONSOCKET_H_

#include <QUdpSocket>

class QTimer;

class PeerInformationSocket : public QUdpSocket
{
  Q_OBJECT
public:
  PeerInformationSocket(QObject *parent=0);
  virtual ~PeerInformationSocket();

  bool init(const QString &name, const QString &state);

  // XXX - need updateState(QString)
  // XXX - need way to get list of current known peers (name, state, type, ip, port)
						      

private slots:
  void transmitPLoc();
  void handleReadReady();

private:
  QString m_name;
  QString m_state;

  QTimer *m_timer;

  unsigned char *m_packetBuffer;
  int m_packetBufferLen;
};

#endif // _PEERINFORMATIONSOCKET_H_

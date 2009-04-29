#ifndef _VISUALIZER_H_
#define _VISUALIZER_H_

#include "Peer.h"

class Visualizer : public Peer
{
  Q_OBJECT
public:
  Visualizer(const QHostAddress &host, quint16 listeningPort, 
	     const QString &peerName, const QString &peerState, QObject *parent=0);
  virtual ~Visualizer();

  bool sendUniverseName(quint8 universeIndex, const QString &universeName);

  bool sendChannelData(bool blind, quint8 universeIndex, 
		       quint16 firstChannelIndex, quint16 channelCount, 
		       const quint8 *channelLevels);

};

#endif // _VISUALIZER_H_

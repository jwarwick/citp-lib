#include "Visualizer.h"

#include <QtDebug>
#include <QTcpSocket>

#include "PacketCreator.h"

Visualizer::Visualizer(const QHostAddress &host, quint16 listeningPort,
       const QString &peerName, const QString &peerState, QObject *parent)
  : Peer(host, listeningPort, peerName, peerState, parent)
{
  qDebug() << "Creating visualizer peer";
  m_peerType = VISUALIZER_PEER;
}

Visualizer::~Visualizer()
{
}

bool Visualizer::sendUniverseName(quint8 universeIndex, const QString &universeName)
{
  if (!m_tcpSocket)
    {
      return false;
    }
  
  if (QAbstractSocket::ConnectedState != m_tcpSocket->state())
    {
      qDebug() << "Peer::sendUniverseName() - Socket not connected";
      return false;
    }

  int bufferLen;
  unsigned char *buffer = PacketCreator::createUNamPacket(universeIndex, universeName, bufferLen);
  if (!buffer)
    {
      qDebug() << "createUNamPacket() failed";
      return false;
    }

  if (bufferLen != m_tcpSocket->write((const char*)buffer, bufferLen))
    {
      qDebug() << "sendUniverseName() write failed:" << m_tcpSocket->error();
      return false;
    }

  delete[] buffer;

  return true;
}

bool Visualizer::sendChannelData(bool blind, quint8 universeIndex, 
				 quint16 firstChannelIndex, quint16 channelCount, 
				 const quint8 *channelLevels) 
{
  if (!m_tcpSocket)
    {
      return false;
    }
  
  if (QAbstractSocket::ConnectedState != m_tcpSocket->state())
    {
      qDebug() << "Peer::sendChannelData() - Socket not connected";
      return false;
    }

  int bufferLen;
  unsigned char *buffer = PacketCreator::createChBkPacket(blind,
							  universeIndex,
							  firstChannelIndex,
							  channelCount,
							  channelLevels,
							  bufferLen);
  if (!buffer)
    {
      qDebug() << "createChBkPacket() failed";
      return false;
    }

  if (bufferLen != m_tcpSocket->write((const char*)buffer, bufferLen))
    {
      qDebug() << "sendChannelData() write failed:" << m_tcpSocket->error();
      return false;
    }

  delete[] buffer;

  return true;
}

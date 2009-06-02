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
  int bufferLen;
  unsigned char *buffer = PacketCreator::createUNamPacket(universeIndex, universeName, bufferLen);
  if (!buffer)
    {
      qDebug() << "createUNamPacket() failed";
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      qDebug() << "Visualizer::sendUniverseName failed";
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

bool Visualizer::sendChannelData(bool blind, quint8 universeIndex, 
				 quint16 firstChannelIndex, quint16 channelCount, 
				 const quint8 *channelLevels) 
{
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

  if (!sendPacket(buffer, bufferLen))
    {
      qDebug() << "Visualizer::sendChannelData failed";
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}


  // Alternate DMX source management
bool Visualizer::sendAlternateSourceMessage_ArtNet(int network, int universe, int channel)
{
  int bufferLen;
  unsigned char *buffer = PacketCreator::createSXSrPacket_ArtNet(network, universe, channel, bufferLen);
  if (!buffer)
    {
      qDebug() << "createSXSrPacket_ArtNet() failed";
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      qDebug() << "Visualizer::sendAlternateSourceMessage_ArtNet failed";
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}
 
bool Visualizer::sendAlternateSourceMessage_Avab(int network, int universe, int channel)
{
  int bufferLen;
  unsigned char *buffer = PacketCreator::createSXSrPacket_Avab(network, universe, channel, bufferLen);
  if (!buffer)
    {
      qDebug() << "createSXSrPacket_Avab() failed";
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      qDebug() << "Visualizer::sendAlternateSourceMessage_Avab failed";
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

bool Visualizer::sendAlternateSourceMessage_E131(int universe, int channel)
{
  int bufferLen;
  unsigned char *buffer = PacketCreator::createSXSrPacket_E131(universe, channel, bufferLen);
  if (!buffer)
    {
      qDebug() << "createSXSrPacket_E131() failed";
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      qDebug() << "Visualizer::sendAlternateSourceMessage_E131 failed";
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

bool Visualizer::sendAlternateSourceMessage_ETCNet2(int channel)
{
  int bufferLen;
  unsigned char *buffer = PacketCreator::createSXSrPacket_ETCNet2(channel, bufferLen);
  if (!buffer)
    {
      qDebug() << "createSXSrPacket_ETCNet2() failed";
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      qDebug() << "Visualizer::sendAlternateSourceMessage_ETCNet2 failed";
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

bool Visualizer::sendPatchMessage(quint16 fixtureIdentifier, quint8 universeIndex, quint16 channelIndex,
				  quint16 channelCount, const QString &fixtureMake, const QString &fixtureName)
{
  int bufferLen;
  unsigned char *buffer = PacketCreator::createPtchPacket(fixtureIdentifier,
							  universeIndex,
							  channelIndex,
							  channelCount,
							  fixtureMake,
							  fixtureName,
							  bufferLen);
  if (!buffer)
    {
      qDebug() << "createPtchPacket() failed";
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      qDebug() << "Visualizer::sendPatchMessage failed";
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

// send empty list to unpatch all fixtures
bool Visualizer::sendUnpatchMessage(const QList<quint16> &fixtureIdentifiers)
{
  int bufferLen;
  unsigned char *buffer = PacketCreator::createUPtcPacket(fixtureIdentifiers, bufferLen);
  if (!buffer)
    {
      qDebug() << "createUPtcPacket() failed";
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      qDebug() << "Visualizer::sendUnpatchMessage failed";
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

// send empty list to request entire patch
bool Visualizer::sendPatchRequest(const QList<quint16> &fixtureIdentifiers)
{
  qDebug() << "in sendPatchRequest";

  int bufferLen;
  unsigned char *buffer = PacketCreator::createSPtcPacket(fixtureIdentifiers, bufferLen);
  if (!buffer)
    {
      qDebug() << "createSPtcPacket() failed";
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      qDebug() << "Visualizer::sendPatchRequest failed";
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

bool Visualizer::sendSelectMessage(bool complete, const QList<quint16> &fixtureIdentifiers)
{
  int bufferLen;
  unsigned char *buffer = PacketCreator::createSelePacket(fixtureIdentifiers, complete, bufferLen);
  if (!buffer)
    {
      qDebug() << "createSelePacket() failed";
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      qDebug() << "Visualizer::sendSelectMessage failed";
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

bool Visualizer::sendDeselectMessage(const QList<quint16> &fixtureIdentifiers)
{
  int bufferLen;
  unsigned char *buffer = PacketCreator::createDeSePacket(fixtureIdentifiers, bufferLen);
  if (!buffer)
    {
      qDebug() << "createDeSePacket() failed";
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      qDebug() << "Visualizer::sendDeselectMessage failed";
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

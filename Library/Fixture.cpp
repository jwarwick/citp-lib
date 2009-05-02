#include "Fixture.h"

Fixture::Fixture(quint16 fixtureIdentifier, quint8 universe, quint16 channel, quint16 channelCount,
		 const QString &fixtureMake, const QString &fixtureName, QObject *parent)
  : QObject(parent),
    m_fixtureIdentifier(fixtureIdentifier),
    m_universe(universe),
    m_channel(channel),
    m_channelCount(channelCount),
    m_fixtureMake(fixtureMake),
    m_fixtureName(fixtureName)
{
}

Fixture::~Fixture()
{
}

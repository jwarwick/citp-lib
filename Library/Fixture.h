// -*- mode: C++ -*-
#ifndef _FIXTURE_H_
#define _FIXTURE_H_

#include <QObject>

class Fixture : public QObject
{
  Q_OBJECT
public:
  Fixture(quint16 fixtureIdentifier, quint8 universe, quint16 channel, quint16 channelCount,
	  const QString &fixtureMake, const QString &fixtureName, QObject *parent=0);
  virtual ~Fixture();


  quint16 m_fixtureIdentifier;
  quint8 m_universe;
  quint16 m_channel;
  quint16 m_channelCount;
  QString m_fixtureMake;
  QString m_fixtureName;

};

#endif // _FIXTURE_H_


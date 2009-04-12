#ifndef _CITPLIB_H_
#define _CITPLIB_H_

#include <qglobal.h>

#ifdef CITPLIB_LIB
# define CITPLIB_EXPORT Q_DECL_EXPORT
#else
# define CITPLIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include <QScriptable>

class PeerInformationSocket;

class CITPLIB_EXPORT CITPLib : public QObject, public QScriptable
{
  Q_OBJECT    
public:
  CITPLib(QObject *parent = 0);
  virtual ~CITPLib();

  bool createPeerInformationSocket(const QString &name, const QString &state);

private:
  
  PeerInformationSocket *m_peerSocket;


};

#endif // _CITPLIB_H_

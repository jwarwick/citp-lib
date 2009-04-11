#ifndef _PACKETCREATOR_H_
#define _PACKETCREATOR_H_

#include <QObject>

class PacketCreator : public QObject
{
  Q_OBJECT
public:
  PacketCreator(QObject *parent=0);
  virtual ~PacketCreator();

  typedef enum
    {
      LightingConsole,
      MediaServer,
      Visualizer,
      OperationHub
    } PLocType;

  // returns NULL on error. bufferLen is the length of the returned buffer.
  static unsigned char * createPLocPacket(PLocType plocType, const QString &name, 
					  const QString &state, int &bufferLen);

};

#endif // _PACKETCREATOR_H_

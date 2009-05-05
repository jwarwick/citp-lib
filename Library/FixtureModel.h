// -*- mode: C++ -*-
#ifndef _FIXTUREMODEL_H_
#define _FIXTUREMODEL_H_

#include <QAbstractTableModel>

class Fixture;

class FixtureModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  FixtureModel(QObject *parent=0);
  virtual ~FixtureModel();

  // methods required for table model
  virtual Qt::ItemFlags flags(const QModelIndex &index) const;
  virtual QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;
  virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent=QModelIndex()) const;

  void addFixture(Fixture *fix);

  QList<Fixture*> m_fixtureList;

};


#endif // _FIXTUREMODEL_H_

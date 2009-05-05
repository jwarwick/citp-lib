#include "FixtureModel.h"
#include "Fixture.h"

#define NUM_COLS 6
// id universe channel channelcount make name

FixtureModel::FixtureModel(QObject *parent)
  : QAbstractTableModel(parent)
{
}

FixtureModel::~FixtureModel()
{
}

Qt::ItemFlags FixtureModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    {
      return Qt::ItemIsEnabled;
    }

  return QAbstractTableModel::flags(index) | Qt::ItemIsSelectable;
}

QVariant FixtureModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    {
      return QVariant();
    }

  if (index.row() >= m_fixtureList.count())
    {
      return QVariant();
    }

  if (index.column() >= NUM_COLS)
    {
      return QVariant();
    }

  if (role != Qt::DisplayRole)
    {
      return QVariant();
    }

  Fixture *f = m_fixtureList.at(index.row());
  if (!f)
    {
      return QVariant();
    }

  switch (index.column())
    {
    case 0:
      return f->m_fixtureIdentifier;
      break;

    case 1:
      return f->m_universe;
      break;
      
    case 2:
      return f->m_channel;
      break;

    case 3:
      return f->m_channelCount;
      break;

    case 4:
      return f->m_fixtureMake;
      break;

    case 5:
      return f->m_fixtureName;
      break;

    default:
      return QVariant();

    }
}

QVariant FixtureModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    {
      return QVariant();
    }

  if (orientation != Qt::Horizontal)
    {
      return QVariant();
    }

  QString header;
  switch (section)
    {
    case 0:
      header = tr("ID");
      break;

    case 1:
      header = tr("Universe");
      break;
      
    case 2:
      header = tr("Channel");
      break;

    case 3:
      header = tr("Channel Count");
      break;

    case 4:
      header = tr("Make");
      break;

    case 5:
      header = tr("Model");
      break;

    default:
      return QVariant();
    }

  return header;
}

int FixtureModel::rowCount(const QModelIndex &) const
{
  return m_fixtureList.count();
}

int FixtureModel::columnCount(const QModelIndex &) const
{
  return NUM_COLS;
}

void FixtureModel::addFixture(Fixture *fix)
{
  m_fixtureList.append(fix);
  //emit dataChanged();
  emit reset();
}

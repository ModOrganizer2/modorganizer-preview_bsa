
/*
    simplefiletreemodel.cpp

    Provides a simple tree model for files and folders
*/

#include "simplefiletreemodel.h"
#include "simplefiletreeitem.h"

#include <QStringList>
#include <QDir>

SimpleFileTreeModel::SimpleFileTreeModel(const QStringList& data, QObject* parent)
  : QAbstractItemModel(parent)
{
  m_RootItem = new SimpleFileTreeItem({ tr("Name") });
  setupModelData(data, m_RootItem);
}

SimpleFileTreeModel::~SimpleFileTreeModel()
{
  delete m_RootItem;
}

int SimpleFileTreeModel::columnCount(const QModelIndex& parent) const
{
  if (parent.isValid())
    return static_cast<SimpleFileTreeItem*>(parent.internalPointer())->columnCount();
  return m_RootItem->columnCount();
}

QVariant SimpleFileTreeModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  SimpleFileTreeItem* item = static_cast<SimpleFileTreeItem*>(index.internalPointer());

  return item->data(index.column());
}

Qt::ItemFlags SimpleFileTreeModel::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::NoItemFlags;

  return QAbstractItemModel::flags(index);
}

QVariant SimpleFileTreeModel::headerData(int section, Qt::Orientation orientation,
  int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return m_RootItem->data(section);

  return QVariant();
}

QModelIndex SimpleFileTreeModel::index(int row, int column, const QModelIndex& parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  SimpleFileTreeItem* parentItem;

  if (!parent.isValid())
    parentItem = m_RootItem;
  else
    parentItem = static_cast<SimpleFileTreeItem*>(parent.internalPointer());

  SimpleFileTreeItem* childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  return QModelIndex();
}

QModelIndex SimpleFileTreeModel::parent(const QModelIndex& index) const
{
  if (!index.isValid())
    return QModelIndex();

  SimpleFileTreeItem* childItem = static_cast<SimpleFileTreeItem*>(index.internalPointer());
  SimpleFileTreeItem* parentItem = childItem->parentItem();

  if (parentItem == m_RootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int SimpleFileTreeModel::rowCount(const QModelIndex& parent) const
{
  SimpleFileTreeItem* parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = m_RootItem;
  else
    parentItem = static_cast<SimpleFileTreeItem*>(parent.internalPointer());

  return parentItem->childCount();
}

void SimpleFileTreeModel::setupModelData(const QStringList& lines, SimpleFileTreeItem* parent)
{
  QVector<SimpleFileTreeItem*> parents;
  QVector<int> indentations;
  parents << parent;
  indentations << 0;

  int number = 0;


  for (QString line : lines) {
    auto fullPath = QDir::cleanPath(line);
    QStringList lineEntries = fullPath.split("/");
    auto currentParent = m_RootItem;

    for (int i = 0; i < lineEntries.count(); i++) {
      QString currentEntryName = lineEntries[i];
      SimpleFileTreeItem* currentEntry = nullptr;

      //check if item was already added
      if (currentParent->childCount() > 0) {
        for (auto child : currentParent->children()) {
          if (child->data(0).toString() == currentEntryName) {
            currentEntry = child;
            break;
          }
        }
      }

      //add tree item if not found
      if (currentEntry == nullptr) {
        QVector<QVariant> columnData;
        columnData.reserve(m_ColumnCount);
        columnData << currentEntryName;
        currentEntry = new SimpleFileTreeItem(columnData, currentParent);
        currentParent->appendChild(currentEntry);
      }

      //as we go deeper into the path
      currentParent = currentEntry;
    }
  }

  /*
  while (number < lines.count()) {

    int position = 0;
    while (position < lines[number].length()) {
      if (lines[number].at(position) != ' ')
        break;
      position++;
    }

    const QString lineData = lines[number].mid(position).trimmed();

    if (!lineData.isEmpty()) {
      // Read the column data from the rest of the line.
      const QStringList columnStrings = lineData.split('\t', QString::SkipEmptyParts);
      QVector<QVariant> columnData;
      columnData.reserve(columnStrings.count());
      for (const QString& columnString : columnStrings)
        columnData << columnString;

      if (position > indentations.last()) {
        // The last child of the current parent is now the new parent
        // unless the current parent has no children.

        if (parents.last()->childCount() > 0) {
          parents << parents.last()->child(parents.last()->childCount() - 1);
          indentations << position;
        }
      }
      else {
        while (position < indentations.last() && parents.count() > 0) {
          parents.pop_back();
          indentations.pop_back();
        }
      }

      // Append a new item to the current parent's list of children.
      parents.last()->appendChild(new SimpleFileTreeItem(columnData, parents.last()));
    }
    ++number;
    
  }*/
}

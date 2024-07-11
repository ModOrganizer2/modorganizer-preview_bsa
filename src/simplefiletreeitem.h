#ifndef SIMPLEFILETREEITEM_H
#define SIMPLEFILETREEITEM_H

#include <QVariant>
#include <QVector>

class SimpleFileTreeItem
{
public:
  explicit SimpleFileTreeItem(const QVector<QVariant>& data,
                              SimpleFileTreeItem* parentItem = nullptr);
  ~SimpleFileTreeItem();

  void appendChild(SimpleFileTreeItem* child);

  SimpleFileTreeItem* child(int row);
  QVector<SimpleFileTreeItem*> children();
  int childCount() const;
  int columnCount() const;
  QVariant data(int column) const;
  int row() const;
  SimpleFileTreeItem* parentItem();

private:
  QVector<SimpleFileTreeItem*> m_childItems;
  QVector<QVariant> m_itemData;
  SimpleFileTreeItem* m_parentItem;
};

#endif  // SIMPLEFILETREEITEM_H

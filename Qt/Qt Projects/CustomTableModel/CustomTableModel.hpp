#ifndef HEADER__FILE__CUSTOMTABLEMODEL
#define HEADER__FILE__CUSTOMTABLEMODEL

#include <QAbstractTableModel>
#include <QObject>

class CustomTableModel : public QAbstractTableModel
{
    Q_OBJECT

  public:
    CustomTableModel(QObject *);

    // QAbstractTableModel 클래스의 함수를 재정의할 수 있다.
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &, int) const; // 실제 테이블의 데이터를 다루는 곳
};

#endif /* HEADER__FILE__CUSTOMTABLEMODEL */

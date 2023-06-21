#include "CustomTableModel.hpp"
#include <QBrush>
#include <QFont>

CustomTableModel::CustomTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

// 행 2개 생성
int CustomTableModel::rowCount(const QModelIndex &index) const
{
    return 2;
}

// 열 3개 생성
int CustomTableModel::columnCount(const QModelIndex &index) const
{
    return 3;
}

// 모든 role 유형을 돌면서 테이블의 데이터를 채움
QVariant CustomTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row(), col = index.column();

    switch (role)
    {
    case Qt::DisplayRole:
        if (row == 0 && col == 1)
            return QString("<--left");
        if (row == 1 && col == 1)
            return QString("right-->");
        return QString("Row %1, Col %2").arg(row).arg(col);

    case Qt::TextAlignmentRole:
        if (row == 0 && col == 1)
            return Qt::AlignLeft;
        if (row == 1 && col == 1)
            return Qt::AlignRight;
        break;

    case Qt::FontRole:
        if (row == 0 && col == 0)
        {
            QFont font;
            font.setBold(true);
            return font;
        }
        break;

    case Qt::BackgroundColorRole:
        if (row == 1 && col == 2)
        {
            QBrush bg(Qt::cyan);
            return bg;
        }
        break;

    default:
        break;
    }

    return QVariant();
}
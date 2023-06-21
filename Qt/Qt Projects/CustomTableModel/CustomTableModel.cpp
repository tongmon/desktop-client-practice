#include "CustomTableModel.hpp"
#include <QBrush>
#include <QFont>

CustomTableModel::CustomTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    timer = new QTimer(this);
    timer->setInterval(1000); // 1초마다 타이머 작동

    // 1초마다 dataChanged() 시그널이 작동하고 해당 시그널은 특정 범위의 index에 대해 data() 함수를 작동시킴
    // dataChanged()의 첫번째 인자는 data() 함수를 작동시킬 범위 중 시작부인 top left 부분
    // 두번째 인자는 data() 함수를 작동시킬 범위 중 끝인 bottom right 부분이다.
    // 밑 구현에서는 테이블의 (0,0) 칸에 대해서만 data 함수가 발생한다.
    connect(timer, &QTimer::timeout, [=]() {
        emit dataChanged(index(0, 0), index(0, 0));
    });

    timer->start(); // 타이머 시작

    // 원래의 플로우는 setData() -> dataChanged() -> data() 순으로 진행된다.
    // 기본 테이블의 QModelIndex 인덱스에는 각 role 마다 특정 QVariant 변수가 저장된다.
    // 즉 특정 QModelIndex 위치에는 QMap<int, QVariant> 변수가 저장된다고 생각하면 된다.
    // QMap<int, QVariant>에서 int는 role이다.
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
        if (row == 0 && col == 0)
            return QString("Current Time: %0").arg(QTime::currentTime().toString());
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
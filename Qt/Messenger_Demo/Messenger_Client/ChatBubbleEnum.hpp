#ifndef HEADER__FILE__CHATBUBBLEENUM
#define HEADER__FILE__CHATBUBBLEENUM

#include <QMetaEnum>
#include <QObject>

class ChatBubbleEnum : public QObject
{
    Q_OBJECT

  public:
    explicit ChatBubbleEnum(QObject *parent = nullptr);

    enum Type
    {
        TEXT = 0,
        IMAGE,
        FILE,
        TYPE_CNT
    };

    Q_ENUMS(Type)

    static QString ConvertEnumToString(Type type)
    {
        const QMetaObject &mo = ChatBubbleEnum::staticMetaObject;
        return QString(mo.enumerator(mo.indexOfEnumerator("Type")).valueToKey(type));
    }
};

#endif /* HEADER__FILE__CHATBUBBLEENUM */

#ifndef HEADER__FILE__CUSTOMBUTTON
#define HEADER__FILE__CUSTOMBUTTON

#include <QEvent>
#include <QPushButton>

class CustomButton : public QPushButton
{
  public:
    CustomButton(const QString &text, QWidget *p = nullptr)
        : QPushButton(text, p)
    {
    }
    bool event(QEvent *evt) override;
    void leaveEvent(QEvent *evt) override;
    void enterEvent(QEvent *evt) override;
};

#endif /* HEADER__FILE__CUSTOMBUTTON */

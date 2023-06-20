#ifndef QWINWIDGET_H
#define QWINWIDGET_H

#include <QVBoxLayout>
#include <QWidget>

#include "WinNativeWindow.h"
#include "widget.h"

class QWinWidget : public QWidget
{
    Q_OBJECT
  public:
    QWinWidget();
    ~QWinWidget();

    void show();
    void center();
    void showCentered();
    void setGeometry(int x, int y, int w, int h);

    HWND getParentWindow() const;

    int BORDERWIDTH = 6;    // Adjust this as you wish for # of pixels on the edges to show resize handles
    int TOOLBARHEIGHT = 35; // Adjust this as you wish for # of pixels from the top to allow dragging the window

  public slots:
    void onMaximizeButtonClicked();
    void onMinimizeButtonClicked();
    void onCloseButtonClicked();

  protected:
    void childEvent(QChildEvent *e) override;
    bool eventFilter(QObject *o, QEvent *e) override;

    bool focusNextPrevChild(bool next) override;
    void focusInEvent(QFocusEvent *e) override;

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

  private:
    QVBoxLayout m_Layout;

    Widget *p_Widget;

    WinNativeWindow *p_ParentWinNativeWindow;
    HWND m_ParentNativeWindowHandle;

    HWND _prevFocus;
    bool _reenableParent;

    void saveFocus();
    void resetFocus();
};

#endif // QWINWIDGET_H

#ifndef ITEMDRAG_H
#define ITEMDRAG_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QTimer>

class ItemDrag : public QLabel
{
    Q_OBJECT
public:
    explicit ItemDrag(QWidget *parent = nullptr, QPoint _baseLocation = QPoint(0,0), QPixmap* _pix = new QPixmap(), QSize _baseSize = QSize(120,120));
    ItemDrag* FocusWidget;
    double PointLength(QPointF target, QPointF orign);
    QPointF PointNormalize(QPointF point);
    //
    void ControlTick(bool condition);
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent* size);
    //


private:
    QPixmap *baseImage;
    QPoint baseLocation;
    QSize baseSize;
    QPoint touchedFingerLocation;
    QPoint touchedWidgetLocation;
    void TouchingWidget();
    void ReleasedWidget();
    QPointF GetCenterPoint();
    //
    QTimer *tick;
    QTimer *backHandle;

signals:
    void OnTouched(ItemDrag* widget);

public slots:
    void TickEvent();
};

#endif // ITEMDRAG_H

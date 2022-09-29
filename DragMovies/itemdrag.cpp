#include "itemdrag.h"
#include "QtMath"

ItemDrag::ItemDrag(QWidget *parent, QPoint _baseLocation, QPixmap* _pix, QSize _baseSize)
    : QLabel(parent)
{
    baseLocation = _baseLocation;
    baseSize = _baseSize;
    baseImage = _pix;
    this->move(baseLocation);
    this->setPixmap(*baseImage);
    this->setAlignment(Qt::AlignCenter);
    //
    tick = new QTimer(this);
    tick->setInterval(20);//0.02s
    connect(tick, SIGNAL(timeout()), this, SLOT(TickEvent()));
}

double ItemDrag::PointLength(QPointF target, QPointF orign)
{
    return sqrt((target - orign).x() * (target - orign).x() +
         (target - orign).y() * (target - orign).y());
}

QPointF ItemDrag::PointNormalize(QPointF point)
{
    return 1 / sqrt(point.x() * point.x() + point.y() * point.y()) * point;
}

void ItemDrag::ControlTick(bool condition)
{
    if(condition) {
        tick->start();
    } else {
        tick->stop();
        //所有还原位置
        if(this->pos() != baseLocation) {
            this->move(baseLocation);
        }

    }
}

void ItemDrag::mousePressEvent(QMouseEvent *e)
{
    if(Qt::MouseButton::LeftButton == e->button()) {
        touchedFingerLocation = e->globalPos();
        touchedWidgetLocation = GetCenterPoint().toPoint();
        TouchingWidget();
        //发信号
        emit OnTouched(this);
    }
}

void ItemDrag::mouseReleaseEvent(QMouseEvent *e)
{
    if(Qt::MouseButton::LeftButton == e->button()) {
        if(this == FocusWidget) {
            //发信号
            emit OnTouched(nullptr);
            //还原
            this->move(baseLocation);
            ReleasedWidget();
        }

    }
}

void ItemDrag::mouseMoveEvent(QMouseEvent *e)
{
    if(this == FocusWidget) {
        this->move(e->globalPos() - touchedFingerLocation + touchedWidgetLocation);
    }
}

void ItemDrag::resizeEvent(QResizeEvent *size)
{
    this->setPixmap(this->pixmap()->scaled(this->size()));
}

void ItemDrag::TouchingWidget()
{
    this->resize(baseSize * 1.5);
    this->move(QPoint(baseSize.width()/2 - baseSize.width() * 1.5/2,
               baseSize.height()/2 - baseSize.height() * 1.5/2)
               + baseLocation);
    //设置控件置于父窗口的顶部
    this->raise();
}

void ItemDrag::ReleasedWidget()
{
    this->resize(baseSize);
}

QPointF ItemDrag::GetCenterPoint()
{
    return this->pos() + QPointF(baseSize.width()/2 - baseSize.width() * 1.5/2,
                         baseSize.height()/2 - baseSize.height() * 1.5/2);
}

void ItemDrag::TickEvent()
{
    if(FocusWidget != this) {
        if(PointLength(baseLocation, FocusWidget->pos()) <= 300) {
            //if(PointLength(this->pos(), FocusWidget->pos()) > 30) {
            QPointF normalize = PointNormalize(GetCenterPoint() - FocusWidget->GetCenterPoint());
            QPointF addPoint = normalize * 300 * 40 / PointLength(GetCenterPoint(), FocusWidget->GetCenterPoint());
            this->move((baseLocation + addPoint).toPoint());
            //}
        } else {
            if(this->pos() != baseLocation) {
                this->move(baseLocation);
            }
        }

    }
}




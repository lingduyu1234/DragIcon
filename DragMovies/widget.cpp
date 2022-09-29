#include "widget.h"
#include "ui_widget.h"
#include "QDir"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //
    this->resize(800, 600);
    row = 10;
    column = 15;
    xUint = this->size().width() / column;
    yUint = this->size().height() / row;
    //获取路径
    QString path = QCoreApplication::applicationDirPath();
    QDir dir(path + "/image");
    //列出dir(path)目录文件下所有文件，存储到file
    if(dir.entryInfoList(QDir::Files).size() > 1) {
        QFileInfo file = dir.entryInfoList(QDir::Files)[1];
        //循环创建
        for (int var = 0; var < row; ++var) {
            for (int j = 0; j < column; ++j) {
                //背景图
                QPixmap *pix = new QPixmap(file.absoluteFilePath());
                //单个可拖拽UI
                ItemDrag * temp = new ItemDrag(this, QPoint(xUint * j, yUint * var), pix, QSize(xUint, yUint));
                temp->resize(xUint, yUint);
                //存储所有到items
                items.append(temp);
                //连接信号与槽函数
                temp->connect(temp, SIGNAL(OnTouched(ItemDrag *)), this, SLOT(OnTouchedSlot(ItemDrag *)));
            }
        }
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::OnTouchedSlot(ItemDrag *widget)
{
    foreach (ItemDrag *var, items) {
        if(widget != nullptr) {
            var->ControlTick(true);
        } else {
            var->ControlTick(false);
        }
        var->FocusWidget = widget;
    }
}

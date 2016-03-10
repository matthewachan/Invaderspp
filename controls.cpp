/** @file controls.cpp
 *  @author Matthew Chan
 *  @date March 9, 2016
 *  @class controls
 *  @brief This class handles the UI displaying the controls/instructions for the game
 */
#include "controls.h"
controls::controls(QWidget* parent) : QWidget(parent), ui(new Ui::controls)
{
    // init ui form's components
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked(bool)), parent, SLOT(game_over()));
}
controls::~controls()
{
    delete ui;
}
void controls::paintEvent(QPaintEvent *)
{
    QPainter* painter = new QPainter();
    painter->begin(this);
    painter->setBrush(Qt::black);
    painter->setPen(Qt::black);
    painter->fillRect(0,0,this->width(),this->height(), Qt::black);
    painter->end();
}

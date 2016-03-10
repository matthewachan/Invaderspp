/** @file victory.cpp
 *  @author Matthew Chan
 *  @date March 9, 2016
 *  @class victory
 *  @brief This class handles the UI displaying the victory screen of the game
 */

#include "victory.h"

victory::victory(QWidget* parent) : QWidget(parent), ui(new Ui::victory){
    // init ui form's components
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked(bool)), parent, SLOT(game_over()));
}

victory::~victory(){
    delete ui;
}

void victory::paintEvent(QPaintEvent *) {
    QPainter* painter = new QPainter();
    painter->begin(this);
    painter->setBrush(Qt::black);
    painter->setPen(Qt::black);
    painter->fillRect(0,0,this->width(),this->height(), Qt::black);
    painter->end();
}

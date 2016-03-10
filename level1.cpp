/** @file level1.cpp
 *  @author Matthew Chan
 *  @date March 9, 2016
 *  @class level1
 *  @brief This class handles the logic and display events for level 1
 */
#include "level1.h"

level1::level1(QWidget* parent) : QWidget(parent), ui(new Ui::level1)
{
    // init ui form's components
    ui->setupUi(this);
    // init pixmaps
    background = new QPixmap(":/sprites/background.png");
    player = new QPixmap(":/sprites/player.png");
    bullet = new QPixmap(":/sprites/laser.png");
    enemy = new QPixmap(":/sprites/enemy.png");
    enemyBullet = new QPixmap(":/sprites/enemyLaser.png");
    // animate the bullet
    animateTimer = new QTimer();
    connect(animateTimer, SIGNAL(timeout()), this, SLOT(animateBullet()));
    connect(animateTimer, SIGNAL(timeout()), this, SLOT(checkHitbox()));
    connect(animateTimer, SIGNAL(timeout()), this, SLOT(animateEnemy()));
    connect(animateTimer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    animateTimer->start(40); // timer times out every 40 ms
    // enemy attacks every 5 seconds
    attackTimer = new QTimer();
    connect(attackTimer, SIGNAL(timeout()), this, SLOT(enemyAttack()));
    attackTimer->start(3000); // timer times out every 5 secs
    // init scoreboard stats
    lives = 3;
    score = 0;
    // game over and next level events
    connect(this, SIGNAL(game_over()), parent, SLOT(game_over()));
    connect(this, SIGNAL(next_level()), parent, SLOT(start_level2()));
    // sounds
    bulletSound = new QMediaPlayer();
    bulletSound->setMedia(QUrl("qrc:/sounds/laser.mp3"));
    enemySound = new QMediaPlayer();
    enemySound->setMedia(QUrl("qrc:/sounds/enemylaser.wav"));
    deathSound = new QMediaPlayer();
    deathSound->setMedia((QUrl("qrc:/sounds/death.mp3")));
}

level1::~level1()
{
    delete ui;
}

void level1::paintEvent(QPaintEvent *)
{
    QPainter* painter = new QPainter();
    painter->begin(this);
    // draw the background image onto the widget
    painter->drawPixmap(0,0,this->width(), this->height(), *background);
    // if the player is alive, draw the player
    if (playerAlive)
        painter->drawPixmap(playerLoc->x(), playerLoc->y(), playerSize->width(), playerSize->height(), *player);
    // if the enemy is alive, draw the enemy
    if (enemyAlive)
        painter->drawPixmap(enemyLoc->x(), enemyLoc->y(), enemySize->width(), enemySize->height(), *enemy);
    // if the player fired a bullet, draw the bullet
    if (canFire == false)
        painter->drawPixmap(bulletLoc->x(), bulletLoc->y(), bulletSize->width(), bulletSize->height(), *bullet);
    // if the enemy fired a bullet, draw the bullet
    if (enemyCanFire == false)
        painter->drawPixmap(enemyBulletLoc->x(), enemyBulletLoc->y(), bulletSize->width(), bulletSize->height(), *enemyBullet);
    painter->end();
}

void level1::keyPressEvent(QKeyEvent *e)
{
    // player cannot go off of the screen
    if (e->key() == Qt::Key_Left) {
        if (playerLoc->x() > 0)
            playerLoc->setX(playerLoc->x()-playerSpeed);
    }
    else if (e->key() == Qt::Key_Right) {
        if (playerLoc->x() + playerSize->width() < this->width())
            playerLoc->setX(playerLoc->x()+playerSpeed);
    }
    // player can only fire one bullet at a time
    else if (e->key() == Qt::Key_Space) {
        if (canFire) {
            bulletLoc = new QPoint(playerLoc->x()+(bulletSize->width()/2), playerLoc->y() - bulletSize->height()/2);
            if (bulletSound->state() == QMediaPlayer::PlayingState)
                bulletSound->setPosition(0);
            bulletSound->play();
            canFire = false;
        }
    }
    // repaint the scene
    this->update();
}

void level1::showEvent(QShowEvent *e)
{
    // show the widget and allow it to take keyboard input
    this->activateWindow();
    this->setFocus();
    QWidget::showEvent(e);
    // init player variables
    playerAlive = true;
    playerSpeed = this->width()/50;
    playerSize = new QSize(this->width()/18, this->width()/10);
    // player starts in the middle-bottom of the screen
    playerLoc = new QPoint((this->width()/2)-playerSize->width(), this->height() - playerSize->height());
    canFire = true; // player is able to shoot
    // init bullet size and speed
    bulletSize = new QSize(playerSize->width()/2, playerSize->height()/2);
    bulletSpeed = this->height()/50;
    // init enemy variables
    enemyAlive = true;
    left = false;
    enemySpeed = this->width()/200;
    enemySize = new QSize(this->width()/18, this->width()/10);
    // enemy starts in the middle-top of the screen
    enemyLoc = new QPoint((this->width()/2)-enemySize->width(), 0 + enemySize->height()/3);
    enemyCanFire = true;
}

void level1::updateFrame()
{
    // update player's size, y-position and speed
    playerSpeed = this->width()/50;
    playerSize->setWidth(this->width()/18);
    playerSize->setHeight(this->width()/10);
    playerLoc->setY(this->height()-playerSize->height());
    // adjust bullet size and speed
    bulletSpeed = this->height()/50;
    bulletSize->setWidth(playerSize->width()/2);
    bulletSize->setHeight(playerSize->height()/2);
    // update enemy's size, y-position and speed
    enemySpeed = this->width()/200;
    enemySize->setWidth(this->width()/18);
    enemySize->setHeight(this->width()/10);
    enemyLoc->setY(0 + enemySize->height()/3);
    // repaint the scene
    update();
}

void level1::animateBullet()
{
    if (canFire == false) {
        // if a bullet has been fired, increment its y-position
        if (bulletLoc->y() > 0)
            bulletLoc->setY(bulletLoc->y()-bulletSpeed);
        // if the bullet has gone out of view, remove it
        else {
            delete bulletLoc;
            bulletSound->stop();
            canFire = true;
        }
        // repaint the scene
        update();
    }
}

void level1::checkHitbox()
{
    // check if player has killed the enemy
    if (canFire == false && enemyAlive == true) {
        // remove enemy and bullet if they have collided
        if (bulletLoc->y() < enemyLoc->y() + enemySize->height()/2 &&
                bulletLoc->x() + bulletSize->width() > enemyLoc->x() &&
                bulletLoc->x() < enemyLoc->x() + enemySize->width()) {
            canFire = true;
            enemyAlive = false;
            // increment the player's score by one
            ui->scoreLabel->setText(QString::number(++score));
            if (deathSound->state() == QMediaPlayer::PlayingState)
                deathSound->setPosition(0);
            deathSound->play();
            enemyAlive = false;
            this->next_level();
        }
        update();
    }
    // check if player has been killed by the enemy
    if (playerAlive == true && enemyCanFire == false) {
        if (enemyBulletLoc->y() + bulletSize->height() > playerLoc->y() &&
                enemyBulletLoc->x() + bulletSize->width() > playerLoc->x() &&
                enemyBulletLoc->x() < playerLoc->x() + playerSize->width()) {
            // respawn the player if they still have lives remaining
            if (lives > 1) {
                playerLoc->setX((this->width()/2)-playerSize->width());
                if (deathSound->state() == QMediaPlayer::PlayingState)
                    deathSound->setPosition(0);
                deathSound->play();
            }
            // restart at game menu if the player has died
            else {
                playerAlive = false;
                enemyAlive = false;
                this->game_over();
            }
            // delete the bullet
            delete enemyBulletLoc;
            enemyCanFire = true;
            // player loses a life
            ui->livesLabel->setText(QString::number(--lives));
        }
        update();
    }
}

void level1::animateEnemy()
{
    // if the enemy is alive, animate it
    if (enemyAlive) {
        // enemy moves left until it hits the right part of the screen
        if (left && enemyLoc->x() > 0) {
            enemyLoc->setX(enemyLoc->x() - enemySpeed);
        }
        // enemy moves right until it hits the left part of the screen
        else if (!left && enemyLoc->x() + enemySize->width() < this->width()) {
            enemyLoc->setX(enemyLoc->x() + enemySpeed);
        }
        // flip direction
        else {
            left = !left;
        }
        update();
    }
    // if the enemy has fired a bullet, animate the bullet
    if (enemyCanFire == false) {
        if (enemyBulletLoc->y() + bulletSize->height() < this->height()) {
            enemyBulletLoc->setY(enemyBulletLoc->y() + bulletSpeed);
        }
        // if the bullet goes off screen, delete it
        else {
            delete enemyBulletLoc;
            enemyCanFire = true;
        }
        update();
    }
}

void level1::enemyAttack()
{
    if (enemyAlive) {
        enemyBulletLoc = new QPoint(enemyLoc->x() + enemySize->width()/2, enemyLoc->y() + enemySize->height()/2);
        enemySound->play();
        enemyCanFire = false;
    }
}

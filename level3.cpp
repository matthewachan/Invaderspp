/** @file level3.cpp
 *  @author Matthew Chan
 *  @date March 9, 2016
 *  @class level3
 *  @brief This class handles the logic and display events for level 3
 */
#include "level3.h"

level3::level3(QWidget* parent) : QWidget(parent), ui(new Ui::level3)
{
    // init ui form's components
    ui->setupUi(this);
    // init pixmaps
    background = new QPixmap(":/sprites/background.png");
    player = new QPixmap(":/sprites/player.png");
    bullet = new QPixmap(":/sprites/laser.png");
    enemy = new QPixmap(":/sprites/boss.png");
    enemyBullet = new QPixmap(":/sprites/enemy2Laser.png");
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
    connect(this, SIGNAL(next_level()), parent, SLOT(victory_screen()));
    // sounds
    bulletSound = new QMediaPlayer();
    bulletSound->setMedia(QUrl("qrc:/sounds/laser.mp3"));
    bossSound = new QMediaPlayer();
    bossSound->setMedia(QUrl("qrc:/sounds/bosslaser.wav"));
    deathSound = new QMediaPlayer();
    deathSound->setMedia((QUrl("qrc:/sounds/death.mp3")));
}

level3::~level3()
{
    delete ui;
}

void level3::paintEvent(QPaintEvent *)
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
    if (enemyCanFire == false) {
        painter->drawPixmap(enemyBulletLoc->x(), enemyBulletLoc->y(), enemyBulletSize->width(), enemyBulletSize->height(), *enemyBullet);
        painter->drawPixmap(enemyBulletLoc2->x(), enemyBulletLoc2->y(), enemyBulletSize->width(), enemyBulletSize->height(), *enemyBullet);
        painter->drawPixmap(enemyBulletLoc3->x(), enemyBulletLoc3->y(), enemyBulletSize->width(), enemyBulletSize->height(), *enemyBullet);
        painter->drawPixmap(enemyBulletLoc4->x(), enemyBulletLoc4->y(), enemyBulletSize->width(), enemyBulletSize->height(), *enemyBullet);
    }
    painter->end();
}

void level3::keyPressEvent(QKeyEvent *e)
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

void level3::showEvent(QShowEvent *e)
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
    // init enemy variables
    enemyAlive = true;
    left = false;
    enemySpeed = this->width()/200;
    enemySize = new QSize(this->width()/3, this->width()/4);
    // enemy starts in the middle-top of the screen
    enemyLoc = new QPoint((this->width()/2)-enemySize->width(), 0 + enemySize->height()/10);
    enemyCanFire = true;
    enemyHp = 10;
    numEnemyBullets = 4;
    // init bullet size and speed
    bulletSize = new QSize(playerSize->width()/2, playerSize->height()/2);
    enemyBulletSize = new QSize(enemySize->width()/5, enemySize->height()/4);
    bulletSpeed = this->height()/50;
}

bool level3::playerHit(QPoint *bullet)
{
    if (bullet->y() + enemyBulletSize->height() > playerLoc->y() &&
            bullet->x() + enemyBulletSize->width() > playerLoc->x() &&
            bullet->x() < playerLoc->x() + playerSize->width()) {
        return true;
    }
    return false;
}

bool level3::bulletInView(QPoint *bullet)
{
    if (bullet->y() + enemyBulletSize->height() > this->height()) {
        return false;
    }
    return true;
}

void level3::updateFrame()
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
    enemyBulletSize->setWidth(enemySize->width()/5);
    enemyBulletSize->setHeight(enemySize->height()/4);
    // update enemy's size, y-position and speed
    enemySpeed = this->width()/200;
    enemySize->setWidth(this->width()/3);
    enemySize->setHeight(this->width()/4);
    enemyLoc->setY(0 + enemySize->height()/10);
    // repaint the scene
    update();
}

void level3::animateBullet()
{
    if (canFire == false) {
        // if a bullet has been fired, increment its y-position
        if (bulletLoc->y() > 0)
            bulletLoc->setY(bulletLoc->y()-bulletSpeed);
        // if the bullet has gone out of view, remove it
        else {
            delete bulletLoc;
            canFire = true;
        }
        // repaint the scene
        update();
    }
}

void level3::checkHitbox()
{
    // check if player has killed the enemy
    if (canFire == false && enemyAlive == true) {
        // remove enemy and bullet if they have collided
        if (bulletLoc->y() < enemyLoc->y() + enemySize->height() &&
                bulletLoc->x() + bulletSize->width() > enemyLoc->x() &&
                bulletLoc->x() < enemyLoc->x() + enemySize->width()) {
            canFire = true;
            enemyHp--;
            if (enemyHp < 1) {
                enemyAlive = false;
                if (deathSound->state() == QMediaPlayer::PlayingState)
                    deathSound->setPosition(0);
                deathSound->play();
                enemyAlive = false;
                this->next_level();
            }
            // increment the player's score by one
            ui->scoreLabel->setText(QString::number(++score));

        }
        update();
    }
    // check if player has been killed by the enemy
    if (playerAlive == true && enemyCanFire == false) {
        if (playerHit(enemyBulletLoc) || playerHit(enemyBulletLoc2) ||
                playerHit(enemyBulletLoc3) || playerHit(enemyBulletLoc4)) {
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
            enemyCanFire = true;
            // player loses a life
            ui->livesLabel->setText(QString::number(--lives));
        }
        update();
    }
}

void level3::animateEnemy()
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
        if (bulletInView(enemyBulletLoc)) {
            enemyBulletLoc->setY(enemyBulletLoc->y() + bulletSpeed);
            enemyBulletLoc->setX(enemyBulletLoc->x() + bulletSpeed);
        }
        if (bulletInView(enemyBulletLoc2)) {
            enemyBulletLoc2->setY(enemyBulletLoc2->y() + bulletSpeed);
            enemyBulletLoc2->setX(enemyBulletLoc2->x() - bulletSpeed);
        }
        if (bulletInView(enemyBulletLoc3)) {
            enemyBulletLoc3->setY(enemyBulletLoc3->y() + bulletSpeed);
            enemyBulletLoc3->setX(enemyBulletLoc3->x() - bulletSpeed/2);
        }
        if (bulletInView(enemyBulletLoc4)) {
            enemyBulletLoc4->setY(enemyBulletLoc4->y() + bulletSpeed);
            enemyBulletLoc4->setX(enemyBulletLoc4->x() + bulletSpeed/2);
        }
        // if the bullet goes off screen, delete it
        if (bulletInView(enemyBulletLoc) == false) {
            delete enemyBulletLoc;
        }
        if (bulletInView(enemyBulletLoc2) == false) {
            delete enemyBulletLoc2;
        }
        if (bulletInView(enemyBulletLoc3) == false) {
            delete enemyBulletLoc3;
        }
        if (bulletInView(enemyBulletLoc4) == false) {
            delete enemyBulletLoc4;
        }

        if (!bulletInView(enemyBulletLoc) && !bulletInView(enemyBulletLoc2) &&
                !bulletInView(enemyBulletLoc3) && !bulletInView(enemyBulletLoc4))
            enemyCanFire = true;
        update();
    }
}

void level3::enemyAttack()
{
    if (enemyAlive) {
        enemyBulletLoc = new QPoint(enemyLoc->x() + enemySize->width()/2, enemyLoc->y() + enemySize->height()/2);
        enemyBulletLoc2 = new QPoint(enemyLoc->x() + enemySize->width()/2, enemyLoc->y() + enemySize->height()/2);
        enemyBulletLoc3 = new QPoint(enemyLoc->x() + enemySize->width()/2, enemyLoc->y() + enemySize->height()/2);
        enemyBulletLoc4 = new QPoint(enemyLoc->x() + enemySize->width()/2, enemyLoc->y() + enemySize->height()/2);
        if (bossSound->state() == QMediaPlayer::PlayingState)
            bossSound->setPosition(0);
        bossSound->play();
        enemyCanFire = false;
    }
}

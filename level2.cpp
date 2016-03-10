/** @file level2.cpp
 *  @author Matthew Chan
 *  @date March 9, 2016
 *  @class level2
 *  @brief This class handles the logic and display events for level 2
 */
#include "level2.h"

level2::level2(QWidget* parent) : QWidget(parent), ui(new Ui::level2)
{
    // init ui form's components
    ui->setupUi(this);
    // init pixmaps
    background = new QPixmap(":/sprites/background.png");
    player = new QPixmap(":/sprites/player.png");
    bullet = new QPixmap(":/sprites/laser.png");
    enemy = new QPixmap(":/sprites/enemy2.png");
    enemyBullet = new QPixmap(":/sprites/bossAttack.png");
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
    connect(this, SIGNAL(next_level()), parent, SLOT(start_level3()));
    // seed rng
    unsigned int seed = std::chrono::system_clock().now().time_since_epoch().count();
    generator = std::default_random_engine(seed);
    unif = std::uniform_int_distribution<int>(1,4);
    // sounds
    bulletSound = new QMediaPlayer();
    bulletSound->setMedia(QUrl("qrc:/sounds/laser.mp3"));
    enemySound = new QMediaPlayer();
    enemySound->setMedia(QUrl("qrc:/sounds/enemylaser.wav"));
    deathSound = new QMediaPlayer();
    deathSound->setMedia((QUrl("qrc:/sounds/death.mp3")));
}

level2::~level2()
{
    delete ui;
}

void level2::paintEvent(QPaintEvent *)
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
    if (enemy2Alive)
        painter->drawPixmap(enemy2Loc->x(), enemy2Loc->y(), enemySize->width(), enemySize->height(), *enemy);
    if (enemy3Alive)
        painter->drawPixmap(enemy3Loc->x(), enemy4Loc->y(), enemySize->width(), enemySize->height(), *enemy);
    if (enemy4Alive)
        painter->drawPixmap(enemy4Loc->x(), enemy4Loc->y(), enemySize->width(), enemySize->height(), *enemy);
    // if the player fired a bullet, draw the bullet
    if (canFire == false)
        painter->drawPixmap(bulletLoc->x(), bulletLoc->y(), bulletSize->width(), bulletSize->height(), *bullet);
    // if the enemy fired a bullet, draw the bullet
    if (enemyCanFire == false)
        painter->drawPixmap(enemyBulletLoc->x(), enemyBulletLoc->y(), enemyBulletSize->width(), enemyBulletSize->height(), *enemyBullet);
    painter->end();
}

void level2::keyPressEvent(QKeyEvent *e)
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

void level2::showEvent(QShowEvent *e)
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
    enemySize = new QSize(this->width()/10, this->width()/10);
    // enemy starts in the middle-top of the screen
    enemyLoc = new QPoint((this->width()/2)-enemySize->width(), 0 + enemySize->height()/3);
    enemyCanFire = true;
    // enemy2 variables
    enemy2Loc = new QPoint((this->width()/2)-2*enemySize->width(), 0 + enemySize->height()/3);
    enemy2Alive = true;
    // enemy3 variables
    enemy3Loc = new QPoint((this->width()/2)-3*enemySize->width(), 0 + enemySize->height()/3);
    enemy3Alive = true;
    // enemy4 variables
    enemy4Loc = new QPoint((this->width()/2)-4*enemySize->width(), 0 + enemySize->height()/3);
    enemy4Alive = true;
    // init bullet size and speed
    bulletSize = new QSize(playerSize->width()/2, playerSize->height()/2);
    enemyBulletSize = new QSize(enemySize->width()/4, enemySize->width()/2);
    bulletSpeed = this->height()/50;
}

void level2::moveEnemy(QPoint *enemy)
{
    // enemy moves left until it hits the right part of the screen
    if (left && enemy->x() > 0) {
        enemy->setX(enemy->x() - enemySpeed);
    }
    // enemy moves right until it hits the left part of the screen
    else if (!left && enemy->x() + enemySize->width() < this->width()) {
        enemy->setX(enemy->x() + enemySpeed);
    }
    // flip direction
    else {
        left = !left;
    }
    update();
}

bool level2::enemyHit(QPoint *enemy)
{
    if (bulletLoc->y() < enemy->y() + enemySize->height()/2 &&
            bulletLoc->x() + bulletSize->width() > enemy->x() &&
            bulletLoc->x() < enemy->x() + enemySize->width()) {
        return true;
    }
    return false;
}

void level2::updateFrame()
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
    enemyBulletSize->setWidth(enemySize->width()/4);
    enemyBulletSize->setHeight(enemySize->width()/2);
    // update enemy's size, y-position and speed
    enemySpeed = this->width()/200;
    enemySize->setWidth(this->width()/10);
    enemySize->setHeight(this->width()/10);
    enemyLoc->setY(0 + enemySize->height()/3);
    enemy2Loc->setY(0 + enemySize->height()/3);
    enemy3Loc->setY(0 + enemySize->height()/3);
    enemy4Loc->setY(0 + enemySize->height()/3);
    // repaint the scene
    update();
}

void level2::animateBullet()
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

void level2::checkHitbox()
{
    // check if player has killed the enemy
    if (canFire == false) {
        // remove enemy and bullet if they have collided
        if (enemyAlive == true) {
            if (enemyHit(enemyLoc)) {
                canFire = true;
                enemyAlive = false;
                // increment the player's score by one
                ui->scoreLabel->setText(QString::number(++score));
                if (deathSound->state() == QMediaPlayer::PlayingState)
                    deathSound->setPosition(0);
                deathSound->play();
            }
        }
        if (enemy2Alive == true) {
            if (enemyHit(enemy2Loc)) {
                canFire = true;
                enemy2Alive = false;
                // increment the player's score by one
                ui->scoreLabel->setText(QString::number(++score));
                if (deathSound->state() == QMediaPlayer::PlayingState)
                    deathSound->setPosition(0);
                deathSound->play();
            }
        }
        if (enemy3Alive == true) {
            if (enemyHit(enemy3Loc)) {
                canFire = true;
                enemy3Alive = false;
                // increment the player's score by one
                ui->scoreLabel->setText(QString::number(++score));
                if (deathSound->state() == QMediaPlayer::PlayingState)
                    deathSound->setPosition(0);
                deathSound->play();
            }
        }
        if (enemy4Alive == true) {
            if (enemyHit(enemy4Loc)) {
                canFire = true;
                enemy4Alive = false;
                // increment the player's score by one
                ui->scoreLabel->setText(QString::number(++score));
                if (deathSound->state() == QMediaPlayer::PlayingState)
                    deathSound->setPosition(0);
                deathSound->play();
            }
        }
        if (!enemyAlive && !enemy2Alive && !enemy3Alive && !enemy4Alive) {
            enemyAlive = false;
            enemy2Alive = false;
            enemy3Alive = false;
            enemy4Alive = false;
            this->next_level();
        }
        update();
    }
    // check if player has been killed by the enemy
    if (playerAlive == true && enemyCanFire == false) {
        if (enemyBulletLoc->y() + enemyBulletSize->height() > playerLoc->y() &&
                enemyBulletLoc->x() + enemyBulletSize->width() > playerLoc->x() &&
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
                enemy2Alive = false;
                enemy3Alive = false;
                enemy4Alive = false;
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

void level2::animateEnemy()
{
    // if the enemy is alive, animate it
    if (enemyAlive) {
        moveEnemy(enemyLoc);
        update();
    }
    if (enemy2Alive){
        moveEnemy(enemy2Loc);
        update();
    }
    if (enemy3Alive){
        moveEnemy(enemy3Loc);
        update();
    }
    if (enemy4Alive){
        moveEnemy(enemy4Loc);
        update();
    }
    // if the enemy has fired a bullet, animate the bullet
    if (enemyCanFire == false) {
        if (enemyBulletLoc->y() + enemyBulletSize->height() < this->height()) {
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

void level2::enemyAttack()
{
    int rng = unif(generator);
    if (rng == 1) {
        if (enemyAlive) {
            enemyBulletLoc = new QPoint(enemyLoc->x() + enemySize->width()/2, enemyLoc->y() + enemySize->height()/2);
            if (enemySound->state() == QMediaPlayer::PlayingState)
                enemySound->setPosition(0);
            enemySound->play();
            enemyCanFire = false;
        }
    }
    else if (rng == 2) {
        if (enemy2Alive) {
            enemyBulletLoc = new QPoint(enemy2Loc->x() + enemySize->width()/2, enemy2Loc->y() + enemySize->height()/2);
            if (enemySound->state() == QMediaPlayer::PlayingState)
                enemySound->setPosition(0);
            enemySound->play();
            enemyCanFire = false;
        }
    }
    else if (rng == 3) {
        if (enemy3Alive) {
            enemyBulletLoc = new QPoint(enemy3Loc->x() + enemySize->width()/2, enemy3Loc->y() + enemySize->height()/2);
            if (enemySound->state() == QMediaPlayer::PlayingState)
                enemySound->setPosition(0);
            enemySound->play();
            enemyCanFire = false;
        }
    }
    else if (rng == 4) {
        if (enemy4Alive) {
            enemyBulletLoc = new QPoint(enemy4Loc->x() + enemySize->width()/2, enemy4Loc->y() + enemySize->height()/2);
            if (enemySound->state() == QMediaPlayer::PlayingState)
                enemySound->setPosition(0);
            enemySound->play();
            enemyCanFire = false;
        }
    }
}

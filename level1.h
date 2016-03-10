/** @file level1.h
 * @author Matthew Chan
 * @date March 9, 2016
 * @class level1
 * @brief This class handles the logic and display events for level 1
 */
#ifndef LEVEL1_H
#define LEVEL1_H

#include <QMainWindow>
#include "ui_level1.h"
#include "ui_mainwindow.h"
// imports for paintEvent
#include <QPainter>
#include <QPixmap>
// import for animation
#include <QTimer>
// imports for player and enemy attributes
#include <QPoint>
#include <QSize>
// import for keyPressEvent
#include <QKeyEvent>
// import for sounds
#include <QMediaPlayer>

namespace Ui {
    class level1;
}

class level1 : public QWidget {
    Q_OBJECT
public:
    /// Create the level
    /// @param parent is the parent QWidget of this object
    explicit level1(QWidget *parent = 0);
    /// Deletes the UI
    ~level1();
    /// Draw all objects (player, enemies, bullets, etc.) onto the screen
    void paintEvent(QPaintEvent*);
    /// Handle keyboard input. Left and right keys move the player, space shoots a bullet.
    /// @param e is the event corresponding with each keypress
    void keyPressEvent(QKeyEvent* e);
    /// Initialize specific class variables when the QWidget is shown
    /// @param e is the event object created when the QWidget is first displayed on screen
    void showEvent(QShowEvent *e);

signals:
    /// Signal called when the player has lost all of their lives.
    void game_over();
    /// Signal called when all enemies in the level have been defeated.
    void next_level();

public slots:
    /// Scale the game to fit the current window dimensions
    void updateFrame();
    /// Handles the logic for animating the player's bullet
    void animateBullet();
    /// Check player and enemy hitboxes for collisions
    void checkHitbox();
    /// Handles the logic for moving the enemy and making the enemy attack
    void animateEnemy();
    /// Fires the enemy's bullet
    void enemyAttack();

private:
    Ui::level1* ui; ///< The .ui file for Level 1
    // pixmaps
    QPixmap* background; ///< Background image
    QPixmap* player; ///< Player sprite
    QPixmap* bullet; ///< Player's bullet sprite
    QPixmap* enemy; ///< Enemy sprite
    QPixmap* enemyBullet; ///< Enemy's bullet sprite
    // player variables
    QPoint* playerLoc; ///< Location of the player
    QSize* playerSize; ///< Height and width of the player
    int playerSpeed; ///< Speed that the player travels at
    bool canFire; ///< Flag for whether the player is allowed to fire a bullet
    bool playerAlive; ///< Flag for whether the player has died
    // enemy variables
    QPoint* enemyLoc; ///< Location of the enemy
    QSize* enemySize; ///< Height and width of the enemy
    int enemySpeed; ///< Speed that the enemy travels at
    bool left;  ///< Direction the enemy is traveling
    bool enemyCanFire; ///< Flag for whether the enemy is allowed to fire a bullet
    bool enemyAlive;///< Flag for whether the enemy has died
    // bullet variables
    QPoint* bulletLoc; ///< Location of the player's bullet
    QPoint* enemyBulletLoc; ///< Location of the enemy's bullet
    int bulletSpeed; ///< Speed of the bullet
    QSize* bulletSize;  ///< Height and width of the bullet
    QTimer* animateTimer; ///< Timer to animate the scene
    QTimer* attackTimer; ///< Timer indicating when the enemy should attack
    QMediaPlayer* bulletSound; ///< Sound to play when the player fires a bullet
    QMediaPlayer* enemySound; ///< Sound to play when the enemy fires a bullet
    QMediaPlayer* deathSound; ///< Sound to play when the player/enemy has died
    // scoreboard
    int lives; ///< Player's lives
    int score; ///< Player's score
};
#endif // LEVEL1_H

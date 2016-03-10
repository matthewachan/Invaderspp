/** @file level2.h
 * @author Matthew Chan
 * @date March 9, 2016
 * @class level2
 * @brief This class handles the logic and display events for level 2
 */
#ifndef LEVEL2_H
#define LEVEL2_H


#include <QMainWindow>
#include "ui_level2.h"
#include "ui_mainwindow.h"
// imports for paintEvent
#include <QPainter>
#include <QPixmap>
// imports for animation
#include <QTimer>
// imports for player and enemy attributes
#include <QPoint>
#include <QSize>
// imports for keyPressEvent
#include <QKeyEvent>
// imports for RNG
#include <chrono>
#include <random>
#include <QMediaPlayer>

namespace Ui {
    class level2;
}

class level2 : public QWidget {
    Q_OBJECT

public:
    /// Create the level
    /// @param parent is the parent QWidget of this object
    explicit level2(QWidget *parent = 0);
    /// Deletes the UI
    ~level2();

    /// Draw all objects (player, enemies, bullets, etc.) onto the screen
    void paintEvent(QPaintEvent*);

    /// Handle keyboard input. Left and right keys move the player, space shoots a bullet.
    /// @param e is the event corresponding with each keypress
    void keyPressEvent(QKeyEvent* e);

    /// Initialize specific class variables when the QWidget is shown
    /// @param e is the event object created when the QWidget is first displayed on screen
    void showEvent(QShowEvent *e);
    /// Moves the enemy back and forth across the screen.
    /// @param enemy is the enemy that we want to move around on the screen
    void moveEnemy(QPoint* enemy);
    /// Checks whether an enemy was hit by the player's bullet
    /// @param enemy checks the given enemy for collisions with the player's bullet
    bool enemyHit(QPoint* enemy);

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
    Ui::level2* ui; ///< The .ui file for Level 2
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
    //enemy 2
    QPoint* enemy2Loc; ///< Location of the enemy 2
    bool enemy2Alive; ///< Flag for whether the enemy 2 has died
    //enemy 3
    QPoint* enemy3Loc; ///< Location of the enemy 3
    bool enemy3Alive; ///< Flag for whether the enemy 3 has died
    //enemy 4
    QPoint* enemy4Loc; ///< Location of the enemy 4
    bool enemy4Alive; ///< Flag for whether the enemy 4 has died
    // bullet variables
    QPoint* bulletLoc; ///< Location of the player's bullet
    QPoint* enemyBulletLoc; ///< Location of the enemy's bullet
    int bulletSpeed; ///< Speed of the bullet
    QSize* bulletSize;  ///< Height and width of the bullet
    QSize* enemyBulletSize; ///< Size of the enemy's bullets
    QTimer* animateTimer; ///< Timer to animate the scene
    QTimer* attackTimer; ///< Timer indicating when the enemy should attack
    // scoreboard
    int lives; ///< Player's lives
    int score; ///< Player's score
    // RNG
    std::default_random_engine generator; ///< Random number generator
    std::uniform_int_distribution<int> unif; ///< Uniform distribution between 1 and 4
    // sounds
    QMediaPlayer* bulletSound; ///< Sound to play when the player fires a bullet
    QMediaPlayer* enemySound; ///< Sound to play when the enemy fires a bullet
    QMediaPlayer* deathSound; ///< Sound to play when the player/enemy has died
};



#endif // LEVEL2_H

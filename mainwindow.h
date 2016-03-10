/** @file mainwindow.h
 *  @author Matthew Chan
 *  @date March 9, 2016
 *  @class MainWindow
 *  @brief This class handles the UI displaying the main menu of the game
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include "victory.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "controls.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /// Create the main menu
    /// @param parent is the parent QWidget of this object
    explicit MainWindow(QWidget *parent = 0);
    /// Delete the main window
    ~MainWindow();

public slots:
    /// Create level1 and set it as the main widget
    void start_level1();
    /// Create level2 and set it as the main widget
    void start_level2();
    /// Create level3 and set it as the main widget
    void start_level3();
    /// Create the victory screen and set it as the main widget
    void victory_screen();
    /// Create the main menu screen and set it as the main widget
    void game_over();
    /// Create the game controls screen and set it as the main widget
    void show_controls();

private:
    Ui::MainWindow *ui; ///< The .ui file corresponding with this object
    QMediaPlayer* menu_music; ///< Main menu music
    QMediaPlayer* bgm; ///< In-game background music
    QMediaPlayer* victory_music; ///< Victory screen music
};

#endif // MAINWINDOW_H

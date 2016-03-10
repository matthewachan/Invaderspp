/** @file mainwindow.cpp
 *  @author Matthew Chan
 *  @date March 9, 2016
 *  @class MainWindow
 *  @brief This class handles the UI displaying the main menu of the game
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // Set up the music for the game
    menu_music = new QMediaPlayer();
    menu_music->setMedia(QUrl("qrc:/sounds/menu.mp3"));
    menu_music->play();
    bgm = new QMediaPlayer();
    bgm->setMedia(QUrl("qrc:/sounds/bgm.mp3"));
    victory_music = new QMediaPlayer();
    victory_music->setMedia(QUrl("qrc:/sounds/victory.mp3"));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::start_level1() {
    // delete the current central widget
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);
    // make level1 the central widget
    level1* level = new level1(this);
    this->setCentralWidget(level);
    if (menu_music->state() == QMediaPlayer::PlayingState)
        menu_music->stop();
    if (bgm->state() == QMediaPlayer::StoppedState)
        bgm->setPosition(0);
    bgm->play();
}

void MainWindow::start_level2() {
    // delete the current central widget
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);
    // make level2 the central widget
    level2* level = new level2(this);
    this->setCentralWidget(level);
    if (bgm->state() == QMediaPlayer::StoppedState) {
        bgm->setPosition(0);
        bgm->play();
    }
}

void MainWindow::start_level3() {
    // delete the current central widget
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);
    // make level3 the central widget
    level3* level = new level3(this);
    this->setCentralWidget(level);
    if (bgm->state() == QMediaPlayer::StoppedState) {
        bgm->setPosition(0);
        bgm->play();
    }
}

void MainWindow::victory_screen() {
    // delete the current central widget
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);
    // make victory the central widget
    victory* vic = new victory(this);
    this->setCentralWidget(vic);
    // set sounds
    if (bgm->state() == QMediaPlayer::PlayingState)
        bgm->stop();
    victory_music->setPosition(0);
    victory_music->play();

}

void MainWindow::game_over() {
    // delete the current central widget
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);
    // reset the mainwindow with the initial startup screen
    ui->setupUi(this);
    // set sounds
    if (bgm->state() == QMediaPlayer::PlayingState)
        bgm->stop();
    if (victory_music->state() == QMediaPlayer::PlayingState)
        victory_music->stop();
    menu_music->setPosition(0);
    menu_music->play();
}

void MainWindow::show_controls() {
    // delete the current central widget
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);
    // make controls the central widget
    controls* ctrl = new controls(this);
    this->setCentralWidget(ctrl);
}

/** @file controls.h
 *  @author Matthew Chan
 *  @date March 9, 2016
 *  @class controls
 *  @brief This class handles the UI displaying the controls/instructions for the game
 */
#ifndef CONTROLS_H
#define CONTROLS_H

#include <QMainWindow>
#include "ui_controls.h"
// import for paintEvent
#include <QPainter>

namespace Ui {
    class controls;
}

class controls : public QWidget {
    Q_OBJECT

public:
    /// Constructs a controls object
    /// @param parent is the parent QWidget of this object
    explicit controls(QWidget *parent = 0);
    /// Deletes the UI
    ~controls();
    /// Fills the background with a black rectangle
    void paintEvent(QPaintEvent *);

private:
    Ui::controls* ui; ///< The .ui file corresponding with this object
};
#endif // CONTROLS_H

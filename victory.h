/** @file victory.h
 *  @author Matthew Chan
 *  @date March 9, 2016
 *  @class victory
 *  @brief This class handles the UI displaying the victory screen of the game
 */

#ifndef VICTORY_H
#define VICTORY_H

#include <QMainWindow>
#include "ui_victory.h"
// imports for paintEvent
#include <QPainter>

namespace Ui {
    class victory;
}

class victory : public QWidget {
    Q_OBJECT

public:
    /// Create the victory screen
    /// @param parent is the parent QWidget of this object
    explicit victory(QWidget *parent = 0);
    /// Delete the victory screen
    ~victory();
    // Paint a black background onto the screen
    void paintEvent(QPaintEvent *);

private:
    Ui::victory* ui; ///< The .ui file corresponding with this object
};
#endif // VICTORY_H

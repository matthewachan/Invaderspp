/** @mainpage Space Invaders Game for PIC 10C Project
 *  @author Matthew Chan
 *  @date March 9, 2016
 *  @brief This game is written in C++ and built using Qt 5.5. The player controls a spaceship using the left/right
 *  keys and fires a laser using the space key (similar to the original Space Invaders). This game contains 3 levels,
 *  each with it's own unique enemies, and the last level is a boss level. The player has 3 lives per level and must
 *  kill all enemies in order to proceed to the next level.
 *
 *  @section mainmenu Main Menu
 *  The game starts out by displaying a menu system.  This was created using the Qt Designer drag-and-drop
 *  GUI widgets.
 *
 *  @section controlsmenu Controls Menu
 *  In the main menu, the user can select the "Controls" option, which will open a new screen showing the basic
 *  movement and fire buttons.
 *
 *  @section level1menu Level 1
 *  When the user presses "Play", Level 1 starts with 1 enemy. The user has 3 lives and if all of them are lost,
 *  they are returned to the main menu. If they defeat the enemy, they move on to Level 2.
 *
 *  @section level2menu Level 2
 *  This level contains 4 enemies. Only one enemy fires at a time, but the enemy which fires is chosen at random.
 *  Once all 4 enemies are defeated, the user moves on to the final boss in Level 3.
 *
 *  @section level3menu Level 3
 *  This level contains one boss that fires 4 large projectiles at the player. The boss also has 10 HP, meaning that
 *  it takes 10 shots to kill. Once the boss is defeated the player has cleared the game and is shown the victory screen.
 *
 *  @section victorymenu Victory Screen
 *  A simple screen acknowledging that the player has cleared the game. The player has an option "Return to Main Menu", which
 *  will return the player back to the main menu, from which they can either play the game again or exit the program.
 *
*/

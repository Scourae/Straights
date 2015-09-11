#ifndef MVC_CONTROLLER_H
#define MVC_CONTROLLER_H

#include "model.h"

class Model;

class Controller {
public:
   Controller( Model* );
   void setPlayers(int playerTypes []);
   void validCardButtonClicked(int i);
   void discarCardButtonClicked(int i);
   void ragequitClicked();
   void newGame(int seedNumber);
   void quitGame();
   void shownComputerPlay();
private:
   Model *model_;
}; // Controller


#endif


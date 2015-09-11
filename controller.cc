#include "controller.h"

Controller::Controller(Model *m) : model_(m) {}

void Controller::setPlayers(int playerTypes [])
{
    model_->setPlayers(playerTypes);
}

void Controller::validCardButtonClicked(int i)
{
    model_->playCard(i);
}

void Controller::discarCardButtonClicked(int i)
{
    model_->discardCard(i);
}

void Controller::ragequitClicked()
{
    model_->ragequit();
}

void Controller::newGame(int seedNumber)
{
    model_->newGame(seedNumber);
}

void Controller::quitGame()
{
    model_->quitGame();
}

void Controller::shownComputerPlay()
{
    model_->nextPlayer(-1);
}

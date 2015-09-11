#include "subject.h"
#include "observer.h"

void Subject::subscribe (Observer *newView)
{
    observers_.insert(newView);
}


void Subject::unsubscribe (Observer *formerView)
{
     observers_.erase(formerView);
}


void Subject::notifyNewRound()
{
    Observers::iterator i;
    for (i = observers_.begin(); i != observers_.end(); ++i)
    {
        (*i)->clearBoard();
        (*i)->updateAllScores();
    }
}

void Subject::notifyEndOfRound()
{
    Observers::iterator i;
    for (i = observers_.begin(); i != observers_.end(); ++i)
    {
        (*i)->endOfRound();
    }
}

void Subject::notifyCardPlayed(int cardIndex)
{
    Observers::iterator i;
    for (i = observers_.begin(); i != observers_.end(); ++i)
    {
        (*i)->update(cardIndex);
    }
}

void Subject::notifyEndGame(int winner, int minScore)
{
    Observers::iterator i;
    for (i = observers_.begin(); i != observers_.end(); ++i)
    {
        (*i)->endGame(winner, minScore);
    }
}

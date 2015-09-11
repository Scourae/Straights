#include "ComputerPlayer.h"

ComputerPlayer::ComputerPlayer()
    : Player()
{}

ComputerPlayer::ComputerPlayer(const std::vector <Card*> cards_, const std::vector <Card*> discardedCards_, int oldScore, int roundScore)
    : Player(cards_,
             discardedCards_,
             oldScore,
             roundScore)
{}

bool ComputerPlayer::isHuman()
{
    return false;
}

bool ComputerPlayer::hasValidMove(int deckGrid [4][13])
{
    for (int i = 0; i < cards_.size(); i++)
    {
        if (deckGrid[(*cards_[i]).getSuit()][(*cards_[i]).getRank()] == 1)
            return true;
    }
    return false;
}

Card* ComputerPlayer::playCard(int deckGrid [4][13])
{
    for (int i = 0; i < cards_.size(); i++)
    {
        if (deckGrid[(*cards_[i]).getSuit()][(*cards_[i]).getRank()] == 1)
        {
            Card* result = cards_[i];
            cards_.erase(cards_.begin()+i);
            return result;
        }
    }
}

Card* ComputerPlayer::discardCard()
{
    Card* result = cards_[0];
    cards_.erase(cards_.begin());
    discardedCards_.push_back(result);
    incrementRoundScore();
    return result;
}

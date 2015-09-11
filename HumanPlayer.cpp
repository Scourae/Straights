#include "HumanPlayer.h"
#include "Player.h"
#include "Card.h"

HumanPlayer::HumanPlayer()
    : Player()
{}

bool HumanPlayer::isHuman()
{
    return true;
}

bool HumanPlayer::hasValidMove()
{
    for (int i = 0; i < 13; i++)
    {
        if (validPlays[i] == 1) return true;
    }
    return false;
}

std::vector <Card*> HumanPlayer::getPlayerDeck()
{
    return cards_;
}

int* HumanPlayer::getValidPlays()
{
    return validPlays;
}

Card* HumanPlayer::playCard(int i)
{
    Card* result = cards_[i];
    cards_.erase(cards_.begin()+i);
    return result;
}

void HumanPlayer::discard(int i)
{
    discardedCards_.push_back(cards_[i]);
    cards_.erase(cards_.begin()+i);
    incrementRoundScore();
}

void HumanPlayer::determineValidPlays(int deckGrid [4][13])
{
    for (int i = 0; i < cards_.size(); i++)
    {
        if (deckGrid[(*cards_[i]).getSuit()][(*cards_[i]).getRank()] == 1)
            validPlays[i] = 1;
        else validPlays[i] = 0;
    }
    for (int i = cards_.size(); i < 13; i++)
    {
        validPlays[i] = 0;
    }
}

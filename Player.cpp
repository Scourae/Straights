#include "Card.h"
#include "Player.h"

Player::Player()
    :oldScore(0),
    roundScore(0)
{}

Player::Player(const std::vector <Card*> cards_, const std::vector <Card*> discardedCards_, int oldScore, int roundScore)
    :cards_(cards_),
    discardedCards_(discardedCards_),
    oldScore(oldScore),
    roundScore(roundScore)
{}

/*
    This destructor does not deallocate memory they are pointed to.
    Since they point to Cards created in Straights,
    Straights will free the memories when the game is over
*/
Player::~Player()
{
    cards_.clear();
    discardedCards_.clear();
}

int Player::getOldScore()
{
    return oldScore;
}

int Player::getRoundScore()
{
    return roundScore;
}

void Player::setDeck(const std::vector <Card*> cards_)
{
    this->cards_.clear();
    discardedCards_.clear();
    for (int i = 0; i < cards_.size(); i++)
    {
        this->cards_.push_back(cards_[i]);
    }
}

int Player::increaseScore()
{
    oldScore += roundScore;
    roundScore = 0;
    return oldScore;
}

void Player::wipeScore()
{
    oldScore = 0;
    roundScore = 0;
}

std::vector <Card*> Player::getDiscardedDeck()
{
    return discardedCards_;
}

void Player::clearDiscardedDeck()
{
    discardedCards_.clear();
}

void Player::incrementRoundScore()
{
    roundScore++;
}

int Player::findVector(std::vector <Card*> cards, Card* target)
{
    for (int i = 0; i < cards.size(); i++)
    {
        if (*target == *cards[i]) return i;
    }
    return -1;
}


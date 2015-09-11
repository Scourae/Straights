#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"

class Player
{
    public:
        Player();
        Player(const std::vector <Card*> cards_, const std::vector <Card*> discardedCards_, int oldScore, int roundScore);
        ~Player();
        virtual bool isHuman() = 0;
        int getOldScore();
        int getRoundScore();
        void setDeck(const std::vector <Card*> cards_);
        int increaseScore();
        void wipeScore();
        std::vector <Card*> getDiscardedDeck();
        void clearDiscardedDeck();
    protected:
        void incrementRoundScore();
        int findVector(std::vector <Card*> cards, Card* target);

        // These only hold pointers to memory location allocated in Straights
        // There will not be any new Cards created
        std::vector <Card*> cards_;
        std::vector <Card*> discardedCards_;
    private:
        int oldScore;
        int roundScore;
};

#endif

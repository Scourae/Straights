#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include <vector>
#include "Player.h"
class HumanPlayer : public Player
{
    public:
        HumanPlayer();
        bool isHuman();
        void determineValidPlays(int deckGrid [4][13]);
        bool hasValidMove();
        std::vector <Card*> getPlayerDeck();
        int* getValidPlays();
        Card* playCard(int i);
        void discard(int i);
    private:
        int validPlays[13];
};

#endif

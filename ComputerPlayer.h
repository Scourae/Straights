#include "Player.h"
class ComputerPlayer : public Player
{
    public:
        ComputerPlayer();
        ComputerPlayer(const std::vector <Card*> cards_, const std::vector <Card*> discardedCards_, int oldScore, int roundScore);
        bool isHuman();
        bool hasValidMove(int deckGrid [4][13]);
        Card* playCard(int deckGrid [4][13]);
        Card* discardCard();
};


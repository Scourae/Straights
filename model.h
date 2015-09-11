#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include "Card.h"
#include "Player.h"
#include "subject.h"

#define CARD_COUNT 52

class Model : public Subject {
    public:
        Model();
        ~Model();
        void setPlayers(int playerTypes []);
        void newGame(const int randomValue);
        void newRound();
        Player* getPlayer(int index);
        int getCurrentPlayer();
        bool firstCard();
        Card* getComputerPlayed();
        bool didComputerDiscard();
        void playCard(int i);
        void discardCard(int i);
        void ragequit();
        void nextPlayer(int index);
        void quitGame();
    private:
        void shuffle();
        void resetDeckGrid();
        int findFirst();
        void processCurrentPlayer();
        void playedCard(Card* card);
        std::vector <Card*> cards_;
        std::vector <Player*> players_;

        /*
            An array to reprensent all cards in the deck,
            in order of AC, 2C...
                        AD, 2D...
                        ...
            0 is not played yet and not legal,
            1 is not played but legal,
            and 2 is played cards
        */
        int deckGrid [4] [13];

        int currentPlayer;
        int cardsPlayed;

        Card* computerPlayed;
        bool computerDiscard;

        bool finished;
};

#endif

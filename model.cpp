#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "model.h"
#include "Card.h"
#include "Command.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"

//Initialize by filling the card vector with all the cards in a deck
Model::Model()
{
    for (Suit i = CLUB; i != SUIT_COUNT; i = Suit(i+1))
    {
        for (Rank k = ACE; k != RANK_COUNT; k = Rank(k+1))
        {
            cards_.push_back(new Card(i, k));
        }
    }

}

Model::~Model()
{
    for (int i = 0; i < 52; i++)
    {
        delete cards_[i];
        cards_[i] = NULL;
    }
    for (int i = 0; i < 4; i++)
    {
        delete players_[i];
        players_[i] = NULL;
    }
}

void Model::setPlayers(int playerTypes [])
{
    for (int i = 0; i < 4; i++)
    {
        if (playerTypes[i] == 0)
        {
            players_.push_back(new HumanPlayer());
        }
        else if (playerTypes[i] == 1)
        {
            players_.push_back(new ComputerPlayer());
        }
    }
    newGame(0);
}

void Model::newGame(const int randomValue)
{
    finished = false;
    for (int i = 0; i < 4; i++)
    {
        players_[i]->wipeScore();
    }
    srand48(randomValue);
    newRound();
}

void Model::newRound()
{
    shuffle();
    for (int i = 0; i < 4; i++)
    {
        std::vector <Card*> deck;
        for (int j = 0; j < 13; j++)
        {
            deck.push_back(cards_[i*13+j]);
        }
        players_[i]->setDeck(deck);
    }
    cardsPlayed = 0;
    resetDeckGrid();
    currentPlayer = findFirst();
    processCurrentPlayer();
    notifyNewRound();
}

Player* Model::getPlayer(int index)
{
    return players_[index];
}

int Model::getCurrentPlayer()
{
    return currentPlayer;
}

bool Model::firstCard()
{
    return cardsPlayed == 0;
}

Card* Model::getComputerPlayed()
{
   return computerPlayed;
}

bool Model::didComputerDiscard()
{
    return computerDiscard;
}

void Model::nextPlayer(int index)
{
    cardsPlayed++;
    if (cardsPlayed == 52)
    {
        for (int i = 0; i < 4; i++)
        {
            int scores = players_[i]->increaseScore();
            if (scores >= 80) finished = true;
        }
        if (finished)
        {
            int winner = 0;
            int minScore = players_[0]->getOldScore();
            for (int i = 1; i < 4; i++)
            {
                if (players_[i]->getOldScore() < minScore)
                minScore = players_[i]->getOldScore();
            }
            for (int i = 3; i >= 0; i--)
            {
                if (players_[i]->getOldScore() == minScore)
                winner = winner*10+(i+1);
            }
            notifyEndGame(winner, minScore);
        }
        else
        {
            notifyEndOfRound();
            newRound();
        }
    }
    else
    {
        if (currentPlayer == 3) currentPlayer = 0;
        else currentPlayer++;
        processCurrentPlayer();
        notifyCardPlayed(index);
    }
}

void Model::quitGame()
{
    std::exit(0);
}

void Model::playCard(int i)
{
    HumanPlayer* humanPointer = static_cast <HumanPlayer*> (players_[currentPlayer]);
    Card* played = humanPointer->playCard(i);
    playedCard(played);
    int index = played->getRank()+played->getSuit()*13;
    nextPlayer(index);
}

void Model::discardCard(int i)
{
    HumanPlayer* humanPointer = static_cast <HumanPlayer*> (players_[currentPlayer]);
    humanPointer->discard(i);
    nextPlayer(-1);
}

void Model::ragequit()
{
    HumanPlayer* humanPointer = static_cast <HumanPlayer*> (players_[currentPlayer]);
    std::vector <Card*> deck = humanPointer->getPlayerDeck();
    std::vector <Card*> discardedDeck = humanPointer->getDiscardedDeck();
    int oldScore = humanPointer->getOldScore();
    int currentScore = humanPointer->getRoundScore();
    // delete old human player
    delete players_[currentPlayer];
    players_[currentPlayer] = new ComputerPlayer(deck, discardedDeck, oldScore, currentScore);

    ComputerPlayer* computerPointer = static_cast <ComputerPlayer*> (players_[currentPlayer]);
    if (computerPointer->hasValidMove(deckGrid))
    {
        computerDiscard = false;
        computerPlayed = computerPointer->playCard(deckGrid);
        playedCard(computerPlayed);
    }
    else
    {
        computerDiscard = true;
        computerPlayed = computerPointer->discardCard();
    }
    notifyCardPlayed(-1);
}

void Model::resetDeckGrid()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            // At the beginning of the game only 7S is legal
            if ((j == 6)&&(i == 3))
                deckGrid[i][j] = 1;
            else
                deckGrid[i][j] = 0;
        }
    }
}

/*
    This function looks through the cards_ vector to find the location of 7S,
    since cards are given to players in increments of 13, for exmaple 0-12 goes to player 1 etc.
    So if we divide the index by 13, we get which player index (0-3) has the card.
*/
int Model::findFirst()
{
    Card S7 (SPADE, SEVEN);
    for (int i = 0; i < cards_.size(); i++)
    {
        if (S7 == *cards_[i])
        {
            return i/13;
        }
    }
    return -1;
}

void Model::processCurrentPlayer()
{
    if (players_[currentPlayer]->isHuman())
    {
        HumanPlayer* humanPointer = static_cast <HumanPlayer*> (players_[currentPlayer]);
        humanPointer->determineValidPlays(deckGrid);
    }
    else if (!players_[currentPlayer]->isHuman())
    {
        ComputerPlayer* computerPointer = static_cast <ComputerPlayer*> (players_[currentPlayer]);
        if (computerPointer->hasValidMove(deckGrid))
        {
            computerDiscard = false;
            computerPlayed = computerPointer->playCard(deckGrid);
            playedCard(computerPlayed);
        }
        else
        {
            computerDiscard = true;
            computerPlayed = computerPointer->discardCard();
        }
    }
}

/*
    This function modifies the deckGrid according to which card was played,
    Editing status of potential valid cards
*/
void Model::playedCard(Card* card)
{
    // All suits of the cards played should now be valid
    for (int i = 0; i < 4; i++)
    {
        if (card->getSuit() == i)
            deckGrid[card->getSuit()][card->getRank()] = 2;
        else if (deckGrid[i][card->getRank()] == 0)
            deckGrid[i][card->getRank()] = 1;
    }
    // The adjacent cards are dealt with here
    if ((card->getRank() != 12)&&(deckGrid[card->getSuit()][card->getRank()+1] == 0))
        deckGrid[card->getSuit()][card->getRank()+1] = 1;
    if ((card->getRank() != 0)&&(deckGrid[card->getSuit()][card->getRank()-1] == 0))
        deckGrid[card->getSuit()][card->getRank()-1] = 1;
}


#include "observer.h"
#include "view.h"
#include "controller.h"
#include "model.h"
#include "subject.h"
#include "DeckGUI.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "Card.h"

View::View(Controller *c, Model *m)
    :model_(m),
    controller_(c),
    welcomeScreen(true,10),
    optionBox(true,10),
    human_button("HUMAN"),
    computer_button("COMPUTER"),
    newGame("NEW GAME"),
    quitGame("QUIT GAME"),
    currentlyChosen(0),
    cardTable(4, 13, true),
    cardsBox(true,10),
    playerBox(true,10),
    next_button("next"),
    reset_button("reset")
{
    set_title( "STRAIGHTS" );
	set_border_width(10);

    // Welcome screen
    add(welcomeScreen);
    welcomeScreen.add(welcomeMsg);
    welcomeScreen.add(askPlayerType);
    welcomeScreen.add(optionBox);
    optionBox.add(human_button);
    optionBox.add(computer_button);
    welcomeMsg.set_markup("<span size=\"xx-large\" weight=\"bold\">Welcome to Straights!</span>");
    std::stringstream strim;
    strim << "Player " << (currentlyChosen+1) << ", are you a human or a computer";
    std::string askLine = strim.str();
    askPlayerType.set_text(askLine);
    human_button.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &View::addPlayer), 0));
    computer_button.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &View::addPlayer), 1));

    show_all();
	// Register view as observer of model
	model_->subscribe(this);

} // View::View

View::~View() {}

void View::update(int index) {
    std::stringstream strim2;
    strim2.str("");
    if (index != -1)
    {
        deckTable[index].set(deck.image(Rank(index%13), Suit(index/13)));
    }
    else
    {
        int updatingPlayerNumber = model_->getCurrentPlayer();
        if (updatingPlayerNumber == 0) updatingPlayerNumber = 4;
        Player* updatingPlayer = model_->getPlayer(updatingPlayerNumber-1);
        strim2 << updatingPlayer->getOldScore() << " points\n" << updatingPlayer->getRoundScore() << " discards";
        scorePoints[updatingPlayerNumber-1].set_text(strim2.str());
        strim2.str("");
    }
    std::stringstream strim7;
    if (model_->firstCard())
    {
        strim7 << "A new round begins! ";
    }
    Player* updatingPlayer = model_->getPlayer(model_->getCurrentPlayer());
    Gdk::Color bgColorWhite("white");
    Gdk::Color bgColorRed("red");
    Gdk::Color bgColorGreen("green");
    if (updatingPlayer->isHuman())
    {
        HumanPlayer* humanPointer = static_cast <HumanPlayer*> (updatingPlayer);
        strim7 << "It is now player " << model_->getCurrentPlayer()+1 << "'s turn to play.";
        std::vector <Card*> playerDeck = humanPointer->getPlayerDeck();
        if (!humanPointer->hasValidMove())
        {
            strim7 << "\nThere are no valid moves, discard a card.";
            for (int i = 0; i < playerDeck.size(); i++)
            {
                cards_button[i].modify_bg(Gtk::STATE_NORMAL, bgColorRed);
                cards_button[i].modify_bg(Gtk::STATE_PRELIGHT, bgColorRed);
                card[i].set(deck.image(playerDeck[i]->getRank(), playerDeck[i]->getSuit()));
                cards_button[i].set_sensitive(true);
            }
        }
        else
        {
            strim7 << "\nThere are valid moves, pick a card to play.";
            int* validPlays = humanPointer->getValidPlays();
            for (int i = 0; i < playerDeck.size(); i++)
            {
                if (validPlays[i] == 1)
                {
                    cards_button[i].modify_bg(Gtk::STATE_NORMAL, bgColorGreen);
                    cards_button[i].modify_bg(Gtk::STATE_PRELIGHT, bgColorGreen);
                    cards_button[i].set_sensitive(true);
                }
                else
                {
                    cards_button[i].modify_bg(Gtk::STATE_NORMAL, bgColorWhite);
                    cards_button[i].modify_bg(Gtk::STATE_PRELIGHT, bgColorWhite);
                    cards_button[i].set_sensitive(false);
                }
                card[i].set(deck.image(playerDeck[i]->getRank(), playerDeck[i]->getSuit()));
            }
        }
        for (int i = playerDeck.size(); i < 13; i++)
        {
            cards_button[i].modify_bg(Gtk::STATE_NORMAL, bgColorWhite);
            cards_button[i].modify_bg(Gtk::STATE_PRELIGHT, bgColorWhite);
            card[i].set(deck.null());
            cards_button[i].set_sensitive(false);
        }
        currentPlayer.set_text(strim7.str());
        strim7.str("");
    }
    else if (!updatingPlayer->isHuman())
    {
        if (!model_->didComputerDiscard())
        {
            Card* played = model_->getComputerPlayed();
            deckTable[played->getRank()+played->getSuit()*13].set(deck.image(played->getRank(), played->getSuit()));
        }
        shownComputerPlay();
    }
}

void View::clearBoard()
{
    for(int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            deckTable[i*13+j].set(deck.null());
        }
    }
}

void View::updateAllScores()
{
    std::stringstream strim3;
    strim3.str("");
    for (int i = 0; i < 4; i++)
    {
        Player* updatingPlayer = model_->getPlayer(i);
        strim3 << updatingPlayer->getOldScore() << " points\n" << updatingPlayer->getRoundScore() << " discards";
        scorePoints[i].set_text(strim3.str());
        strim3.str("");
    }
    std::stringstream strim7;
    if (model_->firstCard())
    {
        strim7 << "A new round begins! ";
    }
    Player* updatingPlayer = model_->getPlayer(model_->getCurrentPlayer());
    Gdk::Color bgColorWhite("white");
    Gdk::Color bgColorRed("red");
    Gdk::Color bgColorGreen("green");
    if (updatingPlayer->isHuman())
    {
        HumanPlayer* humanPointer = static_cast <HumanPlayer*> (updatingPlayer);
        strim7 << "It is now player " << model_->getCurrentPlayer()+1 << "'s turn to play.";
        std::vector <Card*> playerDeck = humanPointer->getPlayerDeck();
        if (!humanPointer->hasValidMove())
        {
            strim7 << "\nThere are no valid moves, discard a card.";
            for (int i = 0; i < playerDeck.size(); i++)
            {
                cards_button[i].modify_bg(Gtk::STATE_NORMAL, bgColorRed);
                cards_button[i].modify_bg(Gtk::STATE_PRELIGHT, bgColorRed);
                card[i].set(deck.image(playerDeck[i]->getRank(), playerDeck[i]->getSuit()));
                cards_button[i].set_sensitive(true);
            }
        }
        else
        {
            strim7 << "\nThere are valid moves, pick a card to play.";
            int* validPlays = humanPointer->getValidPlays();
            for (int i = 0; i < playerDeck.size(); i++)
            {
                if (validPlays[i] == 1)
                {
                    cards_button[i].modify_bg(Gtk::STATE_NORMAL, bgColorGreen);
                    cards_button[i].modify_bg(Gtk::STATE_PRELIGHT, bgColorGreen);
                    cards_button[i].set_sensitive(true);
                }
                else
                {
                    cards_button[i].modify_bg(Gtk::STATE_NORMAL, bgColorWhite);
                    cards_button[i].modify_bg(Gtk::STATE_PRELIGHT, bgColorWhite);
                    cards_button[i].set_sensitive(false);
                }
                card[i].set(deck.image(playerDeck[i]->getRank(), playerDeck[i]->getSuit()));
            }
        }
        for (int i = playerDeck.size(); i < 13; i++)
        {
            cards_button[i].modify_bg(Gtk::STATE_NORMAL, bgColorWhite);
            cards_button[i].modify_bg(Gtk::STATE_PRELIGHT, bgColorWhite);
            card[i].set(deck.null());
            cards_button[i].set_sensitive(false);
        }
        currentPlayer.set_text(strim7.str());
        strim7.str("");
    }
    else if (!updatingPlayer->isHuman())
    {
        if (!model_->didComputerDiscard())
        {
            Card* played = model_->getComputerPlayed();
            deckTable[played->getRank()+played->getSuit()*13].set(deck.image(played->getRank(), played->getSuit()));
        }
        shownComputerPlay();
    }
}

void View::endGame(int winner, int minScore)
{
    Gtk::Dialog dialog( "End Of the Game", *this );
    Gtk::VBox* contentArea = dialog.get_vbox();
    contentArea->pack_start(gameOver, true, false);
    gameOver.set_markup("<span size=\"xx-large\" weight=\"bold\">GAME OVER!!!</span>");
    gameOver.show();
    contentArea->pack_start(winners, true, false);
    std::stringstream strim4;
    strim4.str("");
    if (winner > 9)
    {
        strim4 << "<span size =\"large\"> <b>Player " << winner%10;
        winner = winner/10;
        while(winner != 0)
        {
            strim4 << ", player " << winner%10;
            winner = winner/10;
        }
        strim4 << " are your winners!</b></span>";
    }
    else
    {
        strim4 << "<span size =\"large\"><b>Player " << winner << " is your winner!</b></span>";
    }
    winners.set_markup(strim4.str());
    winners.show();
    contentArea->pack_start(winningScore, true, false);
    strim4.str("");
    strim4 << "With the score of " << minScore << "!";
    winningScore.show();
    winningScore.set_text(strim4.str());
    strim4.str("");
    Gtk::Button *newGameButton = dialog.add_button( "PLAY AGAIN", Gtk::RESPONSE_OK);
    newGameButton->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &View::newGameClicked), 2));
    Gtk::Button *quitGameButton = dialog.add_button( "QUIT", Gtk::RESPONSE_CANCEL);
    quitGameButton->signal_clicked().connect(sigc::mem_fun(*this, &View::quitGameClicked));
    contentArea->pack_start(newGameSeed2, true, false);
    newGameSeed2.set_text("");
    newGameSeed2.show();
    dialog.run();
}

void View::endOfRound()
{
    Gtk::Dialog dialog( "End Of a Round", *this );
    Gtk::VBox* contentArea = dialog.get_vbox();
    std::stringstream strim;
    Gtk::Label endOfRoundMsg;
    endOfRoundMsg.set_markup("<span size=\"large\" weight=\"bold\">It's the end of a round, here are the scores of all the players:</span>");
    contentArea->pack_start(endOfRoundMsg, true, false);
    endOfRoundMsg.show();
    Gtk::Label playerScores [4];
    Gtk::HBox discards [4];
    Gtk::Image discardedCards [4][13];

    for (int i = 0 ;i < 4; i++)
    {
        Player* player = model_->getPlayer(i);
        strim << "<b>Player "<< i+1 << "</b>'s current score is " << player->getOldScore() << ".";
        vector <Card*> playerDiscards = player->getDiscardedDeck();
        contentArea->pack_start(playerScores[i], true, false);
        contentArea->pack_start(discards[i], true, false);
        if (playerDiscards.empty())
        {
            strim << "\n There are no discards for <b>player " << i+1 << "</b>. ";
        }
        else
        {
            strim << "\nHere are the discards for <b>player " << i+1 << "</b>. ";
            for (int j = 0; j < playerDiscards.size(); j++)
            {
                discardedCards[i][j].set(deck.image(playerDiscards[j]->getRank(), playerDiscards[j]->getSuit()));
                discards[i].add(discardedCards[i][j]);
                discardedCards[i][j].show();
            }
        }
        playerScores[i].set_markup(strim.str());
        strim.str("");

        for (int j = playerDiscards.size(); j < 13; j++)
        {
            discardedCards[i][j].set(deck.null());
            discards[i].add(discardedCards[i][j]);
            discardedCards[i][j].show();
        }

        playerScores[i].show();
        discards[i].show();
    }
    Gtk::Button * okButton = dialog.add_button( Gtk::Stock::OK, Gtk::RESPONSE_OK);
    dialog.run();
}

void View::setupGame()
{
    Gtk::Container::remove(welcomeScreen);
    add(panels);
    panels.add(topBox);
    topBox.add(newGame);
    topBox.add(newGameSeed);
    newGameSeed.set_text("");
    newGame.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &View::newGameClicked), 1));
    topBox.add(quitGame);
    quitGame.signal_clicked().connect(sigc::mem_fun(*this, &View::quitGameClicked));
    panels.add(cardDeckName);
    cardDeckName.set_text("----------------------------------------------card table----------------------------------------------");
    panels.add(cardTable);
    for(int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            deckTable[i*13+j].set(deck.image(Rank(j), Suit(i)));
            cardTable.attach(deckTable[i*13+j], j, j+1, i, i+1, Gtk::EXPAND|Gtk::FILL, Gtk::EXPAND|Gtk::FILL, 10, 10);
        }
    }
    panels.add(yourCards);
    yourCards.set_text("----------------------------------------------your cards----------------------------------------------");
	panels.add(cardsBox);

	for (int i = 0; i < 13; i++)
    {
        cardsBox.add(cards_button[i]);
        cards_button[i].signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &View::cardButtonClicked), i));
        card[i].set(deck.null());
        cards_button[i].set_image(card[i]);
    }

	panels.add(playerBox);

    ragequit.set_label("RAGEQUIT!!!");
    playerBox.add(currentPlayer);
    std::string playerText = "NULL";
    currentPlayer.set_text(playerText);
    playerBox.add(ragequit);
    ragequit.signal_clicked().connect(sigc::mem_fun(*this, &View::ragequitClicked));
    playerBox.add(scores);
    for (int i = 0 ;i < 4; i++)
    {
        scores.add(scoresPerPlayer[i]);
        scoresPerPlayer[i].add(scorePlayer[i]);
        std::stringstream strim5;
        strim5.str("");
        strim5 << "Player" << (i+1) << ": ";
        scorePlayer[i].set_text(strim5.str());
        strim5.str("");
        strim5 << "0 points\n" << "0 discards";
        scoresPerPlayer[i].add(scorePoints[i]);
        scorePoints[i].set_text(strim5.str());
        strim5.str("");
    }

	show_all();
}

void View::addPlayer(int type)
{
    playerType[currentlyChosen] = type;
    currentlyChosen++;
    if (currentlyChosen == 4)
    {
        setupGame();
        controller_->setPlayers(playerType);
        return;
    }
    std::stringstream strim6;
    strim6.str("");
    strim6 << "Player " << (currentlyChosen+1) << ", are you a human or a computer";
    std::string askLine = strim6.str();
    askPlayerType.set_text(askLine);
    strim6.str("");
}

void View::cardButtonClicked(int i)
{
    Player* updatingPlayer = model_->getPlayer(model_->getCurrentPlayer());
    HumanPlayer* humanPointer = static_cast <HumanPlayer*> (updatingPlayer);
    if (!humanPointer->hasValidMove())
    {
        controller_->discarCardButtonClicked(i);
    }
    else if (humanPointer->hasValidMove())
    {
        controller_->validCardButtonClicked(i);
    }
}

void View::ragequitClicked()
{
    controller_->ragequitClicked();
}

void View::newGameClicked(int buttonNumber)
{
    std::string gameSeed;
    if (buttonNumber == 1)
        gameSeed = newGameSeed.get_text();
    else
        gameSeed = newGameSeed2.get_text();
    std::istringstream seed(gameSeed);
    if (seed.str() != "")
    {
        int seedNumber;
        seed >> seedNumber;
        controller_->newGame(seedNumber);
    }
}

void View::quitGameClicked()
{
    controller_->quitGame();
}

void View::shownComputerPlay()
{
    controller_->shownComputerPlay();
}

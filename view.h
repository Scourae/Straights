#ifndef VIEW_H
#define VIEW_H

#include <gtkmm.h>
#include "DeckGUI.h"
#include "observer.h"

class Controller;
class Model;


class View : public Gtk::Window, public Observer {
public:
    View( Controller*, Model* );
	virtual ~View();
	virtual void update(int index);
	void clearBoard ();
	void updateAllScores();
	void endGame(int winner, int minScore);
	void endOfRound();

private:
	// Observer Pattern: to access Model accessors without having to downcast subject
	Model *model_;

	// Strategy Pattern member (plus signal handlers)
	Controller *controller_;

	// Card Images
	DeckGUI deck;

	// Member widgets P1:
	Gtk::VBox welcomeScreen;
	Gtk::Label welcomeMsg;
	Gtk::Label askPlayerType;
	Gtk::HBox optionBox;
	Gtk::Button human_button;
	Gtk::Button computer_button;
    int currentlyChosen;
    int playerType[4];

	// Member widgets P2:
	Gtk::HBox topBox;
	Gtk::Button newGame;
	Gtk::Entry newGameSeed;
	Gtk::Button quitGame;
	Gtk::Label cardDeckName;
	Gtk::Table cardTable;
	Gtk::Image deckTable [52];
	Gtk::VBox panels;
	Gtk::HBox cardsBox;
	Gtk::VBox playerBox;
    Gtk::Label currentPlayer;
    Gtk::HBox scores;
    Gtk::VBox scoresPerPlayer[4];
    Gtk::Label scorePlayer[4];
    Gtk::Label scorePoints[4];;
    Gtk::Button ragequit;
    Gtk::Label yourCards;
	Gtk::Button cards_button [13];
	Gtk::Image card [13];
	Gtk::Button next_button;
	Gtk::Button reset_button;

    //Member widgets P3:
    Gtk::Label gameOver;
    Gtk::Label winners;
    Gtk::Label winningScore;
	Gtk::Entry newGameSeed2;

    // view changes
    void setupGame();

	// Signal handlers:
	void addPlayer(int type);
	void playersSelected();
	void cardButtonClicked(int i);
	void ragequitClicked();
	void newGameClicked(int buttonNumber);
	void quitGameClicked();
	void shownComputerPlay();

}; // View

#endif

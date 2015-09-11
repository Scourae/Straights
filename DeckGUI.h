#ifndef DECK_GUI_H
#define DECK_GUI_H
#include <gdkmm/pixbuf.h>
#include <vector>
using std::vector;

#include "model.h"
#include "Card.h"


class DeckGUI {
public:
	DeckGUI();
	virtual ~DeckGUI();
	Glib::RefPtr<Gdk::Pixbuf> image( Rank f, Suit s );   // Returns the image for the specified card.
	Glib::RefPtr<Gdk::Pixbuf> null();                 // Returns the image to use for the placeholder.

private:
	vector< Glib::RefPtr< Gdk::Pixbuf > > deck;                   // Contains the pixel buffer images.
};
#endif

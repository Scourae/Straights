#ifndef OBSERVER_H
#define OBSERVER_H

class Subject;


class Observer {
public:
	virtual void update (int i) = 0;
	virtual void endOfRound() = 0;
	virtual void clearBoard() = 0;
	virtual void updateAllScores() = 0;
	virtual void endGame(int winner, int minScore) = 0;
};


#endif

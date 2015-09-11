CXX= g++
CXXFLAGS = -Wall -O -g `pkg-config gtkmm-2.4 --cflags`
LDFLAGS=`pkg-config gtkmm-2.4 --libs`
OBJS = Player.o HumanPlayer.o ComputerPlayer.o shuffle.o Command.o Card.o subject.o model.o controller.o DeckGUI.o view.o main.o
EXEC = straights

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LDFLAGS) -o $(EXEC)

clean:
	rm $(EXEC) $(OBJS)


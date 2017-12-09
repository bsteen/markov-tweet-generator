compile: markov_generator.cpp
	g++ markov_generator.cpp -o markov_generator
	
run: compile
	./markov_generator textfiles/trump.txt
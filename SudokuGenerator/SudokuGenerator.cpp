#include "Generator.h"

#include <iostream>
#include <ctime>

#define STANDARD .63

// Runs a benchmark of the sudoku generations by generating an amount of puzzles equal to numPuzzles.
void runBenchmark(int, bool);

int main(int argc, char* argv[])
{
    Generator g;

    g.generateBoard();
    g.removePercent(STANDARD);
    g.printBoard();
    
    return 0;
}

// Runs a benchmark of the sudoku generations by generating an amount of puzzles equal to numPuzzles.
void runBenchmark(int numPuzzles, bool verbose=true)
{
    float totNumberOfTries = 0;
    int initTime = time(nullptr);
    Generator myGenerator;

    for (int i = 0; i < numPuzzles; i ++)
    {
        myGenerator.generateBoard();
        totNumberOfTries += myGenerator.getTries();
        myGenerator.clearBoard();
        
        if (!verbose)
        {
            continue;
        }
        
        std::cout << "Time: " << time(nullptr) - initTime << " Puzzle #" << i << ", Avg Attempts: " << totNumberOfTries * 1.0 / (i+1) << "\r";
    }

    std::cout << "Made " << numPuzzles << " boards with an average of " << totNumberOfTries * 1.0 / numPuzzles << " tries each.\nAverage time to generate each puzzle: " << (time(nullptr) - initTime) * 1.0 / numPuzzles << " seconds.";

}

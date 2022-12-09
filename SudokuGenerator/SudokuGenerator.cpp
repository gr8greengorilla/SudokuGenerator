#include "Generator.h"

#include <iostream>
#include <ctime>

int main(int argc, char* argv[])
{
    
    int numPuzzles = 10000;
    float totNumberOfTries = 0;
    int initTime = time(nullptr);
    

    for (int i = 0; i < numPuzzles; i ++)
    {
        Generator myGenerator;
        myGenerator.generateBoard();
        totNumberOfTries += myGenerator.getTries();
        
        std::cout << "Time: " << time(nullptr) - initTime << " Puzzle #" << i << ", Avg Attempts: " << totNumberOfTries * 1.0 / (i+1) << "\r";
    }

    std::cout << "Made " << numPuzzles << " boards with an average of " << totNumberOfTries * 1.0 / numPuzzles << " tries each.\nAverage time to generate each puzzle: " << (time(nullptr) - initTime) * 1.0 / numPuzzles << " seconds.";

    
    return 0;
}

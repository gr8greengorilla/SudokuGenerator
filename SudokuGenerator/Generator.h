#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>

#define DIM 9
#define RETRY_ROW_COUNT 25

class Generator
{
private:
    int board[9][9];

    // Used for isValid.
    int primes[9] = {2,3,5,7,11,13,17,19,23};
    int set[9];

    // Used to create the board.
    int tryCounter;
    int retryCount;
    // This will keep track of the frequency of each number. We want to prioritize numbers that there are less of.
    int freq[9] = {0,0,0,0,0,0,0,0,0};
    int numbers[9] = {1,2,3,4,5,6,7,8,9};
    
public:
    
    Generator()
    {
        
        // Init board to be a 9x9 grid of 0s
        for (int i = 0; i < DIM; i++)
        {
            for (int j = 0; j < DIM; j++)
            {
                board[i][j] = 0;
            }
        }
        
        // Seed the rng.
        srand(time(NULL));
    }

    void generateBoard()
    {
        tryCounter = 0; // Used to track generation tries for the puzzle
        retryCount = RETRY_ROW_COUNT + 1; // Used to count how many times the puzzle reverted back to an old row.
        int freqRetry[9]; // Used store the freq numbers in order to revert back to.
        int startingI = 0; // Used to store which row must be reverted back to.

        // Create saved copies of the board
        int startingBoards[2][9][9];
        
        // Create a saved copy of the blank board
        copyBoard(startingBoards[0], board);
    
        
        outer_loop:
        // If the retry count has exceeded the max count, then reset everything.
        if (retryCount > RETRY_ROW_COUNT)
        {
            // Reset the board
            copyBoard(board, startingBoards[0]);
            for (int& i : freq)
            {
                i = 0;
            }
            retryCount = 0;
            startingI = 0;
        } else
        {
            // Reset the board to current row
            copyBoard(board, startingBoards[1]);
            copyArray(freq, freqRetry);
            retryCount++;
        }
        
        
        
        // Fill the board with random values 1-9, if it cannot place a number, revert or reset the board.
        for (int i = 0; i < DIM; i++)
        {
            if (i < startingI)
            {
                i = startingI;
            }
            
            for (int j = 0; j < DIM; j++)
            {
                // Define an array of the numbers 0-8, then randomize it's order. This will be used for the seed of our randomization.
                int seed[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
                std::mt19937 rng(std::random_device{}());
                std::shuffle(seed, seed + 9, rng);

                // Shuffle freq and numbers in the same way.
                shuffleArray(freq, seed);
                shuffleArray(numbers, seed);
                
                // Create a pickList that goes in order from least frequent numbers to most, and randomizes the order of
                // equally often numbers.
                int pickList[9];
                int decoder[9];
                int index = 0;
                for (int r = 0; r < DIM; r++)
                {
                    for (int g = 0; g < DIM; g++)
                    {
                        if (freq[g] == r)
                        {
                            pickList[index] = numbers[g];
                            decoder[index] = g;
                            index++;
                        }
                    }
                }
                
                // Try to place numbers in order from the pick list generated earlier
                for (int r = 0; r < DIM; r++)
                {
                    if (canPlace(i, j, pickList[r]))
                    {
                        board[i][j] = pickList[r];
                        freq[decoder[r]] = freq[decoder[r]] + 1;
                        break;
                    }
                }

                // If a number failed to place
                if (board[i][j] == 0)
                {
                    // Inc the try counter and skip to the outer loop.
                    tryCounter++;
                    goto outer_loop; // Yes im using a goto fight me.
                }
            }

            // After every row, save the current board and the freq count so it can be reverted back to later.
            copyBoard(startingBoards[1], board);
            copyArray(freqRetry, freq);
            startingI = i; // We must know which row to revert back to.
        }
        
    }
    
    // Copy reference array in to dest array.
    void copyArray(int dest[DIM], int reference[DIM])
    {
        for (int i = 0; i < DIM; i++)
        {
            dest[i] = reference[i];
        }
    }

    // Fill board with 0s.
    void clearBoard()
    {
        // Init board to be a 9x9 grid of 0s
        for (int i = 0; i < DIM; i++)
        {
            for (int j = 0; j < DIM; j++)
            {
                board[i][j] = 0;
            }
        }
    }

    // Shuffles an array based on a seed
    void shuffleArray(int arr[9], int seed[9])
    {
        // copy arr into a reference var
        int reference[9];
        for (int i = 0; i < 9; i++)
        {
            reference[i] = arr[i];
        }
        
        // commence shuffling.
        for (int i = 0; i < 9; i++)
        {
            arr[seed[i]] = reference[i];
        }
    }

    // Try to place 1-9 in a random order on spot x, y. If fails, return false
    bool canPlace(int x, int y, int num)
    {
        int numAt = 0;
        
        // Check row
        for (int i = 0; i < DIM; i++)
        {
            numAt = board[i][y];
            // If we run into a 0, then the rest of the row must be empty.
            if (numAt == 0)
            {
                break;
            }

            // If a dupe is found, return false.
            if (numAt == num)
            {
                return false;
            }
        }

        // Only check col if we have enough values on the board to warrant it
        if (board[0][1] != 0)
        {
            // Check col
            for (int i = 0; i < DIM; i++)
            {
                numAt = board[x][i];
                
                // If we run into a 0, then the rest of the col must be empty.
                if (numAt == 0)
                {
                    break;
                }
                
                // If a dupe is found, return false.
                if (numAt == num)
                {
                    return false;
                }
            }
        }
        

        // Check box
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                numAt = board[(x / 3) * 3 + i][(y / 3) * 3 + j];
                
                // If we run into a 0, then the rest of the box must be empty.
                if (numAt == 0)
                {
                    break;
                }
                
                // If a dupe is found, return false.
                if (numAt == num)
                {
                    return false;
                }
            }
        }

        return true;
    }
    
    // Copies a board from target to dest
    void copyBoard(int dest[][DIM], int reference[][DIM])
    {
        for (int i = 0; i < DIM; i++)
        {
            for (int j = 0; j < DIM; j++)
            {
                dest[i][j] = reference[i][j];
            }
        }
    }

    // Return true if board is a valid sudoku board. Not used to generate the board.
    bool isValid()
    {
        // Check rows
        for (auto& i : board)
        {
            if (!hasAllNums(i)) {return false;}
        }

        
        // Check columns
        for (int i = 0; i < DIM; i++)
        {
            for (int j = 0; j < DIM; j++)
            {
                set[j] = board[j][i];
            }
            // Return false if an unsatisfied col is found.
            if (!hasAllNums(set)) {return false;}
        }

        
        // Check each 3x3 box
        for (int a = 0; a < 3; a++)
        {
            for (int b = 0; b < 3; b++)
            {
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        set[i + j * 3] = board[a * 3 + i][b * 3 + j];
                    }
                }
                // If a box doesnt have 1-9, return false.
                if (!hasAllNums(set)) {return false;}
            }
        }
        
        return true;
    }

    // Test if an array of 9 nums has 1-9
    bool hasAllNums(int nums[])
    {
        int product = 1;
        for (int i = 0; i < 9; i++)
        {
            product *= primes[nums[i] - 1];
        }
        
        return product == 223092870; // product == product of the first 9 primes.
    }

    // Prints the board
    void printBoard()
    {
        for (int i = 0; i < DIM; i++)
        {
            for (int j = 0; j < DIM; j++)
            {
                std::cout << board[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    int getTries()
    {
        return tryCounter;
    }

    int getRetries()
    {
        return retryCount;
    }

    // Remove x% of the numbers from the board.
    void removePercent(float x)
    {
        for (int i = 0; i < DIM; i++)
        {
            for (int j = 0; j < DIM; j++)
            {
                board[i][j] = (rand() % 100 <= x * 100) ? 0 : board[i][j];
            }
        }
    }
};

#endif
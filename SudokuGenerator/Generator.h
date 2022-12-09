#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>

#define DIM 9

class Generator
{
private:
    int board[9][9];
    
    int primes[9] = {2,3,5,7,11,13,17,19,23};
    int set[9];

    int c;
    
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
        c = 0;
        
        // Create a saved copy of the board
        int startingBoard[9][9];
        copyBoard(startingBoard, board);

        
        outer_loop:
        // Reset the board
        copyBoard(board, startingBoard);
        
        // Fill the board with random values 1-9, if it cannot place a number, reset the board.
        for (int i = 0; i < DIM; i++)
        {
            for (int j = 0; j < DIM; j++)
            {
                // Define an array of the numbers 1-9, then randomize it's order
                int numbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                std::mt19937 rng(std::random_device{}());
                std::shuffle(numbers, numbers + 9, rng);
                
                for (int r = 0; r < DIM; r++)
                {
                    if (canPlace(i, j, numbers[r]))
                    {
                        board[i][j] = numbers[r];
                        break;
                    }
                }

                if (board[i][j] == 0)
                {
                    c++;
                    goto outer_loop; // Yes im using a goto fight me.
                }

            }
        }
        
    }

    // Try to place 1-9 in a random order on spot x, y. If fails, return false
    bool canPlace(int x, int y, int num)
    {
        // Check row
        for (int i = 0; i < DIM; i++)
        {
            if (board[i][y] == num)
            {
                return false;
            }
        }

        //Check col
        for (int i = 0; i < DIM; i++)
        {
            if (board[x][i] == num)
            {
                return false;
            }
        }

        //Check box
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[(x / 3) * 3 + i][(y / 3) * 3 + j] == num)
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
            for (int b = 0; b < 3; a++)
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
            product *= primes[nums[i]];
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
        return c;
    }
};

#endif
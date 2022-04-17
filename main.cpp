#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <set>
#include <iterator>

void sleep(int seconds)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

class matrix
{
    public:
    static const int X_SIZE = 100;
    static const int Y_SIZE = 50;

    private:
    char character_map[X_SIZE][Y_SIZE];

    public:

    matrix()
    {
        for(int y = 0; y < Y_SIZE; y++)
        {
            for(int x = 0; x < X_SIZE; x++)
            {
                character_map[x][y] = ' ';
            }
        }
    }

    void clean()
    {
        for(int y = 0; y < Y_SIZE; y++)
        {
            for(int x = 0; x < X_SIZE; x++)
            {
                character_map[x][y] = ' ';
            }
        }
    }

    void display()
    {
        for(int y = 0; y < Y_SIZE; y++)
        {
            for(int x = 0; x < X_SIZE; x++)
            {
                std::cout << character_map[x][y];
            }
            std::cout << '\n';
        }
    }

    bool edit(int pos_x, int pos_y, char character)
    {
        if(pos_x <= X_SIZE && pos_y <= Y_SIZE && pos_x >= 1 && pos_y >= 1)
        {
            character_map[pos_x - 1][pos_y - 1] = character;
            return true;
        }else{return false;}
    }

    static void window_clear_screen()
    {
        system("cls");
    }

};

class vector2
{
    public:
    int x = 0;
    int y = 0;

    bool operator==(vector2& second_operand)
    {
        if(x == second_operand.x && y == second_operand.y)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

class stated_vector2
{
    public:
    int x = 0;
    int y = 0;
    char state = ' ';

    bool operator==(stated_vector2& second_operand)
    {
        if(x == second_operand.x && y == second_operand.y && state == second_operand.state)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};



int main()
{
    std::set<vector2> automaton_location;
    std::set<stated_vector2> automaton_changes;
    std::set<vector2> dead_automaton;
    matrix screen;

    while(true)
    {
        /*
        Any live cell with two or three live neighbours survives.
        Any dead cell with three live neighbours becomes a live cell.
        All other live cells die in the next generation. Similarly, all other dead cells stay dead.
        */

        // Conway's Algorithm

        /////////////////////////////////////////////////////////////// EXTRAPOLATING AUTOMATON_CHANGE, BEGIN //////////////////////////////
        // compile automaton_changes based on living automaton
        for(vector2 &automaton: automaton_location)
        {
                // Counting the amount of neighbours
                short neighbourhood_population = 0;
                vector2 target1, target2, target3, target4, target5, target6, target7, target8; //neighbour location
            
                //up
                target1.x = automaton.x;
                target1.y = automaton.y - 1;

                //up-right
                target2.x = automaton.x + 1;
                target2.y = automaton.y - 1;

                //right
                target3.x = automaton.x + 1;
                target3.y = automaton.y;

                //down-right
                target4.x = automaton.x + 1;
                target4.y = automaton.y + 1;

                //down
                target5.x = automaton.x;
                target5.y = automaton.y + 1;

                //down-left
                target6.x = automaton.x - 1;
                target6.y = automaton.y + 1;

                //left
                target7.x = automaton.x - 1;
                target7.y = automaton.y;

                //left-up
                target8.x = automaton.x - 1;
                target8.y = automaton.y - 1;

                for(vector2 buffer: automaton_location)
                {
                    if(buffer == target1){neighbourhood_population++;}
                    if(buffer == target2){neighbourhood_population++;}
                    if(buffer == target3){neighbourhood_population++;}
                    if(buffer == target4){neighbourhood_population++;}
                    if(buffer == target5){neighbourhood_population++;}
                    if(buffer == target6){neighbourhood_population++;}
                    if(buffer == target7){neighbourhood_population++;}
                    if(buffer == target8){neighbourhood_population++;}
                }

                // Determining what to do based on neighbour count

                if(neighbourhood_population == 2 || neighbourhood_population == 3)
                {
                    stated_vector2 buffer;
                    buffer.x = automaton.x; 
                    buffer.y = automaton.y;
                    buffer.state = 'l';           // the cell live
                    automaton_changes.insert(buffer);
                }
                else
                {
                    stated_vector2 buffer;
                    buffer.x = automaton.x; 
                    buffer.y = automaton.y;
                    buffer.state = 'd';           // the cell die
                    automaton_changes.insert(buffer);
                }
        }


        // compile automaton_changes based on dead automaton
        for(vector2 &automaton: automaton_location)
        {

            /////////////////////////////////////////////////////////// SCANNING DEAD AUTOMATON LOCATION, BEGIN /////////////////////
            // dead automaton location (on the periphery of living automaton)
            vector2 target1, target2, target3, target4, target5, target6, target7, target8; 

            //up
            target1.x = automaton.x;
            target1.y = automaton.y - 1;

            //up-right
            target2.x = automaton.x + 1;
            target2.y = automaton.y - 1;

            //right
            target3.x = automaton.x + 1;
            target3.y = automaton.y;

            //down-right
            target4.x = automaton.x + 1;
            target4.y = automaton.y + 1;

            //down
            target5.x = automaton.x;
            target5.y = automaton.y + 1;

            //down-left
            target6.x = automaton.x - 1;
            target6.y = automaton.y + 1;

            //left
            target7.x = automaton.x - 1;
            target7.y = automaton.y;

            //left-up
            target8.x = automaton.x - 1;
            target8.y = automaton.y - 1;

                
            dead_automaton.insert(target1);
            dead_automaton.insert(target2);
            dead_automaton.insert(target3);
            dead_automaton.insert(target4);
            dead_automaton.insert(target5);
            dead_automaton.insert(target6);
            dead_automaton.insert(target7);
            dead_automaton.insert(target8);
            /////////////////////////////////////////////////////////// SCANNING DEAD AUTOMATON LOCATION, END /////////////////////

            for(vector2 &automaton: dead_automaton)
            {
                // Counting the amount of neighbours
                short neighbourhood_population = 0;
                vector2 target1, target2, target3, target4, target5, target6, target7, target8; //neighbour location
            
                //up
                target1.x = automaton.x;
                target1.y = automaton.y - 1;

                //up-right
                target2.x = automaton.x + 1;
                target2.y = automaton.y - 1;

                //right
                target3.x = automaton.x + 1;
                target3.y = automaton.y;

                //down-right
                target4.x = automaton.x + 1;
                target4.y = automaton.y + 1;

                //down
                target5.x = automaton.x;
                target5.y = automaton.y + 1;

                //down-left
                target6.x = automaton.x - 1;
                target6.y = automaton.y + 1;

                //left
                target7.x = automaton.x - 1;
                target7.y = automaton.y;

                //left-up
                target8.x = automaton.x - 1;
                target8.y = automaton.y - 1;

                for(vector2 buffer: automaton_location)
                {
                    if(buffer == target1){neighbourhood_population++;}
                    if(buffer == target2){neighbourhood_population++;}
                    if(buffer == target3){neighbourhood_population++;}
                    if(buffer == target4){neighbourhood_population++;}
                    if(buffer == target5){neighbourhood_population++;}
                    if(buffer == target6){neighbourhood_population++;}
                    if(buffer == target7){neighbourhood_population++;}
                    if(buffer == target8){neighbourhood_population++;}
                }

                // Determining what to do based on neighbour count

                if(neighbourhood_population == 3)
                {
                    stated_vector2 buffer;
                    buffer.x = automaton.x; 
                    buffer.y = automaton.y;
                    buffer.state = 'l';           // the cell live
                    automaton_changes.insert(buffer);
                }
                else
                {
                    stated_vector2 buffer;
                    buffer.x = automaton.x; 
                    buffer.y = automaton.y;
                    buffer.state = 'd';           // the cell die
                    automaton_changes.insert(buffer);
                }
            }
        }
        /////////////////////////////////////////////////////////////// EXTRAPOLATING AUTOMATON_CHANGE, END //////////////////////////////

        /////////////////////////////////////////////////////////////// APPLYING CHANGES, BEGIN //////////////////////////////////////////
        for(stated_vector2 automata_change: automaton_changes)
        {
            if(automata_change.state == 'l')
            {
                vector2 buffer;
                buffer.x = automata_change.x;
                buffer.y = automata_change.y;
                automaton_location.insert(buffer);
            }
            else if(automata_change.state == 'd')
            {
                vector2 buffer;
                buffer.x = automata_change.x;
                buffer.y = automata_change.y;
                automaton_location.erase(buffer);
            }
            
        }
        /////////////////////////////////////////////////////////////// APPLYING CHANGES, END //////////////////////////////////////////
        screen.clean();
        screen.window_clear_screen();
        for(vector2 renderer: automaton_location)
        {
            screen.edit(renderer.x, renderer.y, '#');
        }
        screen.display();

    }
    
    return 0;
}

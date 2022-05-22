#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <math.h>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <chrono>
#include <thread>
#include <atomic>

// only applicable to data type that conform to is_equal operator
template<class data_type>
class set
{
    private:
    std::vector<data_type> container;

    public:

    // index begin with zero
    void index_erase(int index)
    {
        if(index < container.size())
        {
            typename std::vector<data_type>::iterator iterator = container.begin();
            for(int n = 0; n < index; n++)
            {
                iterator++;
            }
            container.erase(iterator);
        }
        else
        {
            // cannot access out of bound memory blocks
        }
    }

    // erase a desired element
    void erase_data(data_type data)
    {
        for(int n = 0; n < container.size(); n++)
        {
            if(container[n] == data)
            {
                index_erase(n);
                break;
            }
        }
    }

    // add new element if the element doesnt overlap with the container
    void insert(data_type data)
    {
        bool IsFound = false;
        for(int n = 0; n < container.size(); n++)
        {
            if(container[n] == data)
            {
                IsFound = true;
                break;
            }
        }
        if(!IsFound)
        {
            container.push_back(data);
        }
    }

    void clean()
    {
        container.clear();
    }

    data_type access(int index)
    {
        return container[index];
    }

    bool check(data_type data)
    {
        for(int n = 0; n < container.size(); n++)
        {
            if(container[n] == data)
            {
                return true;
            }
        }
        return false;
    }

    std::vector<data_type> get_data()
    {
        return container;
    }

    void substract_from_set(set<data_type> SET)
    {
        std::vector<data_type> DATA = SET.get_data();
        for(data_type &data: DATA)
        {
            erase_data(data);
        }
    }

    void add_from_set(set<data_type> SET)
    {
        std::vector<data_type> DATA = SET.get_data();
        for(data_type &data: DATA)
        {
            add(data);
        }
    }
};

struct vector2
{
    int x = 0; int y = 0;;
    bool operator==(vector2 operand2){
    if((this->x == operand2.x) && (this->y == operand2.y)){return true;}else{return false;}}
};

struct stated_vector2
{
    int x = 0; int y = 0; char state = ' ';
    bool operator==(stated_vector2 operand2){
    if((this->x == operand2.x) && (this->y == operand2.y) && (this->state == operand2.state)){return true;}else{return false;}}
};

int main(int argc, char* args[])
{   
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* WINDOW = NULL;
    SDL_Renderer* RENDERER = NULL;

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    const int SCREEN_WIDTH = DM.w;
    const int SCREEN_HEIGHT = DM.h;
    const int RESOLUTION_WIDTH = 200;
    const int RESOLUTION_HEIGHT = 125;
    const int PIXEL_WIDTH = SCREEN_WIDTH / RESOLUTION_WIDTH;
    const int PIXEL_HEIGHT = SCREEN_HEIGHT / RESOLUTION_HEIGHT;

    while(true)
    {
       WINDOW = SDL_CreateWindow("Cellular Automata", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
       if(WINDOW != NULL){break;}
    }
    while(true)
    {
       RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_SOFTWARE);
       if(RENDERER != NULL){break;}
    }

    ///////////////////////////////////////////// PROGRAM START //////////////////////////////////////////

    /*
        Any live cell with two or three live neighbours survives.
        Any dead cell with three live neighbours becomes a live cell.
        All other live cells die in the next generation. Similarly, all other dead cells stay dead.
    */

   /*
        for each living cell, create dead cell on the peripheral
        substract dead cells with live cells
        determine whether a living cell is alive or dead
        determine whether a dead cell reman dead or get back to life
        apply changes
        repeat
    */

    set<vector2> automaton_location;
    set<stated_vector2> automaton_changes;
    set<vector2> dead_automaton;

    vector2 buffer;

    buffer.x = 10; buffer.y = 10;
    automaton_location.insert(buffer);
    buffer.x = 11; buffer.y = 10;
    automaton_location.insert(buffer);
    buffer.x = 12; buffer.y = 10;
    automaton_location.insert(buffer);
    buffer.x = 12; buffer.y = 9;
    automaton_location.insert(buffer);
    buffer.x = 11; buffer.y = 8;
    automaton_location.insert(buffer);

    while(true)
    {
        ///////////////////////////////////////////////////////// CREATING DEAD CELLS, BEGIN /////////////////////////////

        // resetting previous dead cells list
        dead_automaton.clean();

        // creating dead cells
        for(vector2 &live_cell: automaton_location.get_data())
        {
            vector2 target;

            // upper left
            target.x = live_cell.x - 1;
            target.y = live_cell.y - 1;
            dead_automaton.insert(target);

            // up
            target.x = live_cell.x;
            target.y = live_cell.y - 1;
            dead_automaton.insert(target);

            // upper right
            target.x = live_cell.x + 1;
            target.y = live_cell.y - 1;
            dead_automaton.insert(target);

            // right
            target.x = live_cell.x + 1;
            target.y = live_cell.y;
            dead_automaton.insert(target);

            // lower right
            target.x = live_cell.x + 1;
            target.y = live_cell.y + 1;
            dead_automaton.insert(target);

            // down
            target.x = live_cell.x;
            target.y = live_cell.y + 1;
            dead_automaton.insert(target);

            // lower left
            target.x = live_cell.x - 1;
            target.y = live_cell.y + 1;
            dead_automaton.insert(target);

            // left
            target.x = live_cell.x - 1;
            target.y = live_cell.y;
            dead_automaton.insert(target);
        }

        ///////////////////////////////////////////////////////// CREATING DEAD CELLS, END ///////////////////////////////



        ///////////////////////////////////////////////////////// SUBSTRACTING DEAD CELLS, BEGIN /////////////////////////
        
        dead_automaton.substract_from_set(automaton_location);

        ///////////////////////////////////////////////////////// SUBSTRACTING DEAD CELLS, END ///////////////////////////



        ///////////////////////////////////////////////////////// DETERMINING LIVE CELLS, BEGIN //////////////////////////

        for(vector2 live_cell: automaton_location.get_data())
        {
            // counting neighbours
            short neighbour_count = 0;

            vector2 target1, target2, target3, target4, target5, target6, target7, target8;

            // upper left
            target1.x = live_cell.x - 1;
            target1.y = live_cell.y - 1;

            // up
            target2.x = live_cell.x;
            target2.y = live_cell.y - 1;

            // upper right
            target3.x = live_cell.x + 1;
            target3.y = live_cell.y - 1;
            
            // right
            target4.x = live_cell.x + 1;
            target4.y = live_cell.y;

            // lower right
            target5.x = live_cell.x + 1;
            target5.y = live_cell.y + 1;

            // down
            target6.x = live_cell.x;
            target6.y = live_cell.y + 1;

            // lower left
            target7.x = live_cell.x - 1;
            target7.y = live_cell.y + 1;

            // left
            target8.x = live_cell.x - 1;
            target8.y = live_cell.y;

            if(automaton_location.check(target1)){neighbour_count++;}
            if(automaton_location.check(target2)){neighbour_count++;}
            if(automaton_location.check(target3)){neighbour_count++;}
            if(automaton_location.check(target4)){neighbour_count++;}
            if(automaton_location.check(target5)){neighbour_count++;}
            if(automaton_location.check(target6)){neighbour_count++;}
            if(automaton_location.check(target7)){neighbour_count++;}
            if(automaton_location.check(target8)){neighbour_count++;}

            // determining cells (currently alive) future state

            if(neighbour_count == 2 || neighbour_count == 3)
            {
                stated_vector2 buffer;
                buffer.x = live_cell.x;
                buffer.y = live_cell.y;
                buffer.state = 'l';
                automaton_changes.insert(buffer);
            }
            else
            {
                stated_vector2 buffer;
                buffer.x = live_cell.x;
                buffer.y = live_cell.y;
                buffer.state = 'd';
                automaton_changes.insert(buffer);
            }
        }

        ///////////////////////////////////////////////////////// DETERMINING LIVE CELLS, END ////////////////////////////



        ///////////////////////////////////////////////////////// DETERMINING DEAD CELLS, BEGIN //////////////////////////

        for(vector2 dead_cell: dead_automaton.get_data())
        {
            // counting neighbours
            short neighbour_count = 0;

            vector2 target1, target2, target3, target4, target5, target6, target7, target8;

            // upper left
            target1.x = dead_cell.x - 1;
            target1.y = dead_cell.y - 1;

            // up
            target2.x = dead_cell.x;
            target2.y = dead_cell.y - 1;

            // upper right
            target3.x = dead_cell.x + 1;
            target3.y = dead_cell.y - 1;
            
            // right
            target4.x = dead_cell.x + 1;
            target4.y = dead_cell.y;

            // lower right
            target5.x = dead_cell.x + 1;
            target5.y = dead_cell.y + 1;

            // down
            target6.x = dead_cell.x;
            target6.y = dead_cell.y + 1;

            // lower left
            target7.x = dead_cell.x - 1;
            target7.y = dead_cell.y + 1;

            // left
            target8.x = dead_cell.x - 1;
            target8.y = dead_cell.y;

            if(automaton_location.check(target1)){neighbour_count++;}
            if(automaton_location.check(target2)){neighbour_count++;}
            if(automaton_location.check(target3)){neighbour_count++;}
            if(automaton_location.check(target4)){neighbour_count++;}
            if(automaton_location.check(target5)){neighbour_count++;}
            if(automaton_location.check(target6)){neighbour_count++;}
            if(automaton_location.check(target7)){neighbour_count++;}
            if(automaton_location.check(target8)){neighbour_count++;}

            // determining cells (currently alive) future state

            if(neighbour_count == 3)
            {
                stated_vector2 buffer;
                buffer.x = dead_cell.x;
                buffer.y = dead_cell.y;
                buffer.state = 'l';
                automaton_changes.insert(buffer);
            }
            else
            {
                stated_vector2 buffer;
                buffer.x = dead_cell.x;
                buffer.y = dead_cell.y;
                buffer.state = 'd';
                automaton_changes.insert(buffer);
            }
        }

        ///////////////////////////////////////////////////////// DETERMINING DEAD CELLS, END ////////////////////////////

        
        
        ///////////////////////////////////////////////////////// APPLYING CHANGES, BEGIN ////////////////////////////////

        for(stated_vector2 automata_change: automaton_changes.get_data())
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
                automaton_location.erase_data(buffer);
            }
            
        }

        // reset automaton changes
        automaton_changes.clean();

        ///////////////////////////////////////////////////////// APPLYING CHANGES, END //////////////////////////////////
        


        SDL_Rect DimensionBuffer;
        SDL_SetRenderDrawColor(RENDERER, 100, 100, 100, 255);
        SDL_RenderClear(RENDERER);
        SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);
        for(int n = 0; n < automaton_location.get_data().size(); n++)
        {
            DimensionBuffer.x = (automaton_location.access(n).x - 0) * PIXEL_WIDTH;
            DimensionBuffer.y = (automaton_location.access(n).y - 0) * PIXEL_WIDTH;
            DimensionBuffer.w = PIXEL_WIDTH;
            DimensionBuffer.h = PIXEL_HEIGHT;
            SDL_RenderFillRect(RENDERER, &DimensionBuffer);
        }
        SDL_RenderPresent(RENDERER);
        SDL_Delay(100);
    }

    SDL_DestroyWindow(WINDOW);
    SDL_DestroyRenderer(RENDERER);
    SDL_Quit();

    return 0;
}

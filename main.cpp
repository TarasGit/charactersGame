#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <typeinfo>
#include <future>
#include "Characters.h"


static std::string get_command()
{
     // disabled string commands with space, e.g attack dragon
    // because it is not always recognized as a whole string

    std::string command = "";
    //cout << std::flush;
    //getline(cin, command);
    cin >> command;
    return command;
}

static void print_description(const Character &character){
    cout << "Type:  " << typeid(character).name() << endl;
    cout << "Name:  " << character.name << std::endl;
    cout << "Damage:" << character.my_damage << std::endl;
    cout << "Health:" << character.health << std::endl;
}

static void game_over(std::string creature){
    cout << "##################################" << endl;
    cout << "Hero was killed by " << creature << endl;
    cout << "GAME OVER" << endl;
    cout << "##################################" << endl;

    exit(0);
}


static void game_won(string hero_name){
    cout << "##################################" << endl;
    cout << "Hero won: " << hero_name << endl;
    cout << "GAME OVER" << endl;
    cout << "##################################" << endl;

    exit(0);
}


int main()
{
    bool dragon_dead = false;
    bool orc_dead = false;

    int time = 1;
    std::chrono::seconds timeout(5);
    string hero_command;
    Hero hero("Taras", HERO_HEALTH, HERO_DAMAGE);
    Dragon dragon("Scylla", DRAGON_HEALTH, DRAGON_DAMAGE, DRAGON_ATTACK_SPEED_MS);
    Orc orc("Tanwen", ORC_HEALTH, ORC_DAMAGE, ORC_ATTACK_SPEED_MS);

    cout << "########## GAME CHARACTERS ##########" << std::endl;
    cout << "Use: 1 - attack dragon or 2 - attack orc for attacks" << endl;
    cout << "-------------------------------------" << std::endl;
    print_description(hero);
    cout << "-------------------------------------" << std::endl;
    print_description(dragon);
    cout << "-------------------------------------" << std::endl;
    print_description(orc);
    cout << "#####################################" << std::endl;


    // start time
    std::chrono::steady_clock::time_point begin_dragon = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point begin_orc = std::chrono::steady_clock::now();


    // game loop
    auto future = std::async(std::launch::async, get_command);
    while(time){
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        hero_command = "default";

        // Hero read command
        // this struct for asynchrounous read was taken from: https://gist.github.com/vmrob/ff20420a20c59b5a98a1
        if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            hero_command = future.get();

            future = std::async(std::launch::async, get_command);
        }

        // Hero attacks
        if(hero_command.compare(ATTACK_ORC) == 0){
            int orc_current_health = hero.attack(orc);
            if (orc_current_health == DEAD){
                orc_dead = true;
            }
            cout << "Orc Health:" << orc_current_health << endl << std::flush;
            cout << "------------------------------------------------------------------------------" << endl;

        }

        if(hero_command.compare(ATTACK_DRAGON) == 0){
            int dragon_current_health = hero.attack(dragon);
            if (dragon_current_health == DEAD){
                dragon_dead = true;
            }
            cout << "Dragon Health:" << dragon_current_health << endl;// << std::flush;
            cout << "------------------------------------------------------------------------------" << endl;

        }

        // orc and dragon dead
        if(orc_dead && dragon_dead){
            game_won(hero.get_name());
        }

        // Dragon time diff
        std::chrono::steady_clock::time_point end_dragon = std::chrono::steady_clock::now();
        long timediff_dragon = std::chrono::duration_cast<std::chrono::milliseconds>(end_dragon - begin_dragon).count();


        // Creatures attacks
        // Dragon
        if (timediff_dragon > dragon.attack_speed_ms){
            begin_dragon = std::chrono::steady_clock::now(); // reset time for dragon
            cout << "Dragon attacks" << endl << std::flush;

            int hero_current_health = dragon.attack(hero);

            cout << "Hero Health:" << hero_current_health << endl; // << std::flush;
            cout << "------------------------------------------------------------------------------" << endl;


            if (hero_current_health == DEAD){
                game_over(dragon.get_name());
            }
        }


        // Orc time diff
        std::chrono::steady_clock::time_point end_orc = std::chrono::steady_clock::now();
        long timediff_orc = std::chrono::duration_cast<std::chrono::milliseconds>(end_orc - begin_orc).count();
        //cout << timediff_orc << endl;

        // Orc attacks
        if (timediff_orc > orc.attack_speed_ms){
            begin_orc = std::chrono::steady_clock::now(); // reset time for orc
            cout << "Orc attacks" << endl; // << std::flush;

            int hero_current_health = orc.attack(hero);

            cout << "Hero Health:" << hero_current_health << endl; //<< std::flush;

            if (hero_current_health == DEAD){
                game_over(orc.get_name());
            }
        }

    }

    return 0;
}

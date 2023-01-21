#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <typeinfo>
#include <future>

static const bool DEAD = 0;

static const int DRAGON_HEALTH = 20;
static const int ORC_HEALTH = 7;
static const int HERO_HEALTH = 40;

static const int DRAGON_ATTACK_SPEED_MS = 2000;
static const int ORC_ATTACK_SPEED_MS = 1500;

static const int DRAGON_DAMAGE = 3;
static const int ORC_DAMAGE = 1;
static const int HERO_DAMAGE = 2;

static const std::string ATTACK_ORC = "1"; // "attack orc";
static const std::string ATTACK_DRAGON = "2"; // attack dragon";



using namespace std;

class Character{
public:
    Character(string name, int health, int damage, int attack_speed_ms): name(name), health(health), my_damage(damage), attack_speed_ms(attack_speed_ms) {}
    virtual int attack(Character &enemy) = 0;
    virtual int reduce_health(int enemy_damage) = 0;

    int get_health() {
        return health;
    }
    int get_attack_speed(){
        return attack_speed_ms;
    }

    string get_name(){
        return name;
    }

//protected: //commented out because of static function print_description()
    int my_damage;
    int health;
    string name;
    int attack_speed_ms;

};

// 1. Dragon
class Dragon: public Character{
public:
    Dragon(string name, int health, int damage, int attack_speed_ms): Character(name, health, damage, attack_speed_ms) {}
    int attack(Character&) override;
    int reduce_health(int enenmy_damage) override;
};

int Dragon::attack(Character &enemy){
    return enemy.reduce_health(my_damage);
}

int Dragon::reduce_health(int enemy_dagame){
    health -= enemy_dagame;
    if(health <= DEAD){
        return DEAD;
    }
    return health;
}

// 2. Orc
class Orc: public Character{
public:
    Orc(string name, int health, int damage, int attack_speed_ms): Character(name, health, damage, attack_speed_ms) {}
    int attack(Character&) override;
    int reduce_health(int enenmy_damage) override;
};

int Orc::attack(Character &enemy) {
    return enemy.reduce_health(my_damage);
}

int Orc::reduce_health(int enemy_dagame) {
    health -= enemy_dagame;
    if(health <= DEAD){
        return DEAD;
    }
    return health;
}

// 3. Hero
class Hero: public Character{
public:
    Hero(string name, int health, int damage): Character(name, health, damage, 0) {}
    int attack(Character&) override;
    int reduce_health(int enenmy_damage) override;
};

int Hero::attack(Character &enemy) {
    return enemy.reduce_health(my_damage);
}

int Hero::reduce_health(int enemy_dagame) {
    health -= enemy_dagame;
    if(health <= DEAD){
        return DEAD;
    }
    return health;
}




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

            // Set a new line. Subtle race condition between the previous line
            // and this. Some lines could be missed. To aleviate, you need an
            // io-only thread. I'll give an example of that as well.
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

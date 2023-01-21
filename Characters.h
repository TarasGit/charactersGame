#ifndef CHARACTERS_H
#define CHARACTERS_H
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

// no specific dragon or orc features available, so Character class could be used instead
// but for extension purposes Dragon and Orc classes are created
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



#endif // CHARACTERS_H

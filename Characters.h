#ifndef CHARACTERS_H
#define CHARACTERS_H

// no specific dragon or orc features available, so Character class could be used instead
// but for extension purposes Dragon and Orc classes are created
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

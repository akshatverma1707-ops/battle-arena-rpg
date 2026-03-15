#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;
// ==========================
// Character Base Class
// ==========================
class Character
{
protected:
    string name;
    int health;
    int maxHealth;
    int attackPower;
    int defense;

public:
    Character(string n, int m, int a, int d)
    {
        name = n;
        health = m;
        maxHealth = m;
        attackPower = a;
        defense = d;
    }
    bool isAlive()
    {
        return health > 0;
    }
    virtual int attack()
    {
        return attackPower;
    }
    void takeDamage(int damage)
    {
        health -= damage;
        if (health < 0)
        {
            health = 0;
        }
        cout << name << " took " << damage << " damage." << endl;
        cout << "Health remaining: " << health << "/" << maxHealth << endl;
    }
    int getDefense()
    {
        return defense;
    }
    int getHealth()
    {
        return health;
    }
    string getName()
    {
        return name;
    }
    virtual void displayStats()
    {
        cout << "----Character Stats----" << endl;
        cout << "Name: " << name << endl;
        cout << "Health: " << health << "/" << maxHealth << endl;
        cout << "Attack: " << attackPower << endl;
        cout << "Defense: " << defense << endl;
    }
    void showHealthBar()
    {
        int barLength = 20;

        float ratio = (float)health / maxHealth;
        int filled = ratio * barLength;

        cout << name << " [";

        for (int i = 0; i < filled; i++)
            cout << "█";

        for (int i = filled; i < barLength; i++)
            cout << "░";

        cout << "] " << health << "/" << maxHealth << endl;
    }
};

// ==========================
// Player Class
// ==========================
class Player : public Character
{
    int level;
    int experience;
    int potions;
    int weaponbonus;

public:
    Player(int l, int e, int p, int w, string n, int m, int a, int d) : Character(n, m, a, d)
    {
        level = l;
        experience = e;
        potions = p;
        weaponbonus = w;
    }
    int getLevel()
    {
        return level;
    }
    int getExperience()
    {
        return experience;
    }
    int getPotions()
    {
        return potions;
    }
    int getWeaponBonus()
    {
        return weaponbonus;
    }
    void gainExperience(int enemyXp, int requiredXp)
    {
        experience += enemyXp;
        if (experience >= requiredXp)
        {
            levelUp();
        }
    }
    void levelUp()
    {
        level++;
        cout << "\n🔥 LEVEL UP! You are now level " << level << "!\n";
        maxHealth += 10;
        attackPower += 5;
        defense += 2;
        health = maxHealth;
        experience = 0;
    }
    void usePotion()
    {
        if (potions > 0)
        {
            int healAmount = 20;

            health += healAmount;

            if (health > maxHealth)
                health = maxHealth;

            potions--;

            cout << name << " used a potion!" << endl;
            cout << "Potion used! Health restored." << endl;
            cout << "Potions Remaining: " << potions << endl;
        }
        else
        {
            cout << "No potions left for use!!" << endl;
        }
    }
    void addPotion(int amount)
    {
        potions += amount;
        cout << amount << " potion(s) added to inventory!" << endl;
    }
    void equipWeapon(int bonus)
    {
        weaponbonus = bonus;
    }
    int getTotalAttack()
    {
        return attackPower + weaponbonus;
    }
    void showInventory()
    {
        cout << "Potions: " << potions << endl;
        cout << "Weapon Bonus: " << weaponbonus << endl;
    }
    void displayStats() override
    {
        Character ::displayStats();
        cout << "Level: " << level << endl;
        showXPBar();
        cout << "Potions:  " << potions << endl;
        cout << "Weapon bonus: +" << weaponbonus << endl;
    }
    void showXPBar()
    {
        int barLength = 20;
        int requiredXP = 50;

        float ratio = (float)experience / requiredXP;
        int filled = ratio * barLength;

        cout << "XP [";

        for (int i = 0; i < filled; i++)
            cout << "█";

        for (int i = filled; i < barLength; i++)
            cout << "░";

        cout << "] " << experience << "/" << requiredXP << endl;
    }
};
// ==========================
// Enemy Class
// ==========================
class Enemy : public Character
{
    int rewardXp;
    int attackvariation;

public:
    Enemy() : Character("Unknown", 0, 0, 0)
    {
        rewardXp = 0;
        attackvariation = 0;
    }
    Enemy(int r, int av, string n, int m, int a, int d) : Character(n, m, a, d)
    {
        rewardXp = r;
        attackvariation = av;
    }
    int attack() override
    {
        int variation = rand() % attackvariation;
        return attackPower + variation;
    }
    int getRewardXp()
    {
        return rewardXp;
    }
    void displayEnemyInfo()
    {
        cout << "A wild " << name << " appeared!!" << endl;
        if (name == "Goblin")
        {
            cout << "Goblin: 'Your gold will be mine!'" << endl;
        }
        else if (name == "Barbarian")
        {
            cout << "Barbarian: 'I will crush you!'" << endl;
        }
        else if (name == "Bandit")
        {
            cout << "Bandit: 'Hand over your loot!'" << endl;
        }
        else if (name == "Dragon")
        {
            cout << "Dragon: 'Foolish Human... you dare challenge me?'" << endl;
        }

        cout << "Health: " << health << endl;
        cout << "Attack: " << attackPower << endl;
    }
};

// ==========================
// BattleSystem Class
// ==========================
class BattleSystem
{
    Player *player;
    Enemy *enemy;

public:
    BattleSystem(Player *p, Enemy *e)
    {
        player = p;
        enemy = e;
    }
    bool playerTurn()
    {
        int choice;
        cout << "Enter your choice: " << endl;
        cout << "Potions available: " << player->getPotions() << endl;
        cout << "1. Attack" << endl;
        cout << "2. Use Potion(Increases 20 health pts)" << endl;
        cout << "3. View stats" << endl;
        cin >> choice;
        if (choice == 1)
        {
            int damage = player->getTotalAttack() - enemy->getDefense();
            if (damage < 0)
            {
                damage = 0;
            }
            int crit = rand() % 10;
            if (crit == 0)
            {
                cout << "🔥 CRITICAL HIT! Damage doubled!\n";
                damage *= 2;
            }
            cout << player->getName() << " attacks!!" << endl;
            enemy->takeDamage(damage);
            enemy->showHealthBar();
            return true;
        }
        else if (choice == 2)
        {
            player->usePotion();
            return false;
        }
        else if (choice == 3)
        {
            player->displayStats();
            return false;
        }
        return false;
    }
    void enemyTurn()
    {
        cout << "Enemy attacks!!" << endl;
        int damage = enemy->attack() - player->getDefense();
        if (damage < 0)
        {
            damage = 0;
        }
        player->takeDamage(damage);
        player->showHealthBar();
    }
    void startBattle()
    {
        enemy->displayEnemyInfo();
        player->showHealthBar();
        enemy->showHealthBar();
        while (player->isAlive() && enemy->isAlive())
        {
            bool enemyCanAttack = playerTurn();
            if (enemyCanAttack && enemy->isAlive())
            {
                enemyTurn();
            }
        }
        if (player->isAlive())
        {
            cout << player->getName() << " Wins!!" << endl;
            xpReward();
        }
        else
        {
            cout << player->getName() << " has been defeated" << endl;
        }
    }
    void xpReward()
    {
        int xp = enemy->getRewardXp();
        cout << "\nYou gained " << xp << " XP!" << endl;
        player->gainExperience(xp, 50);

        player->showXPBar();
        if (rand() % 3 == 0)
        {
            cout << "You found  a potion on " << enemy->getName() << endl;
            player->addPotion(1);
        }
    }
};
Player choosecharacter()
{
    int choice;
    cout << "=====Choose Your Character=====" << endl;
    cout << "1. Knight(High Defense)" << endl;
    cout << "2. Wizard(High Attack)" << endl;
    cout << "3. Archer(Balanced)" << endl;
    cout << "4. Assassin(Very High Attack, Low Defense)" << endl;

    cout << "Enter your choice" << endl;
    cin >> choice;
    if (choice == 1)
    {
        cout << "You chose Knight!" << endl;
        return Player(1, 0, 4, 5, "Knight", 120, 18, 10);
    }
    else if (choice == 2)
    {
        cout << "You chose Wizard!" << endl;
        return Player(1, 0, 5, 5, "Wizard", 80, 25, 4);
    }
    else if (choice == 3)
    {
        cout << "You chose Archer!" << endl;
        return Player(1, 0, 4, 5, "Archer", 95, 20, 6);
    }
    else
    {
        cout << "You chose Assassin!" << endl;
        return Player(1, 0, 5, 5, "Assassin", 85, 28, 3);
    }
}

// ==========================
// GameEngine Class
// ==========================
class GameEngine
{
    Player player;
    int battleCount = 0;
    bool dragonDefeated = false;
    string lastEnemy;

public:
    GameEngine() : player(1, 0, 3, 5, "Hero", 100, 20, 5)
    {
    }
    void startGame()
    {
        cout << "Welcome to the Battle arena game!" << endl;
        player = choosecharacter();
        player.displayStats();
        while (player.isAlive())
        {
            Enemy enemy;
            cout << "\n===== Battle " << battleCount + 1 << " =====\n";

            if (battleCount == 0)
            {
                enemy = Enemy(10, 4, "Goblin", 60, 12, 3);
            }
            else if (battleCount == 1)
            {
                enemy = Enemy(10, 6, "Barbarian", 80, 16, 5);
            }
            else if (battleCount == 2)
            {
                enemy = Enemy(20, 7, "Bandit", 100, 18, 6);
            }
            else
            {
                cout << "\nFINAL BOSS BATTLE!\n";
                enemy = Enemy(40, 8, "Dragon", 120, 25, 8);
            }
            lastEnemy = enemy.getName();
            BattleSystem battle(&player, &enemy);
            battle.startBattle();
            if (!player.isAlive())
            {
                break;
            }
            if (enemy.getName() == "Dragon")
            {
                dragonDefeated = true;
                break;
            }
            battleCount++;
        }
        if (dragonDefeated)
        {
            cout << "Dragon defeated!" << endl;
            cout << "🏆 YOU SAVED THE KINGDOM!" << endl;
            cout << "GAME COMPLETED!" << endl;
        }
        else
        {
            cout << "You were defeated by the " << lastEnemy << "!" << endl;
            cout << "GAME OVER" << endl;
        }
    }
};

// ==========================
// Main Function
// ==========================
int main()
{
    srand(time(0));
    GameEngine game;
    game.startGame();
    return 0;
}
// Battle of Sea.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.


#include <iostream>
#include <string>
using namespace std;

class Ship {
private:
    string name;
protected:
    int health;
public:
    Ship(int health, string name) : health(health), name(name){}
    int getHealth() const {
        return health;
    }
    void getHit() {
        if (health <= 0) {
            cout << "Dead" << endl;
        }
        else {
            --health;
            cout << name << " was attacked" << endl;
            displayHealth();
        }
    }
    void displayHealth() {
        cout << "Current health:"<< health << endl;
    }
    void displayName() {
        cout << "Name of ship:" << name << endl;
    }
};

class Pepe : public Ship {
public:
    Pepe():Ship(1,"Pepe") {}
};

class Destroyer : public Ship {
public:
    Destroyer() :Ship(2,"Destroyer") {}
};

class Cruiser : public Ship {
public:
    Cruiser() :Ship(3,"Cruiser") {}
};

class Battleship : public Ship {
public:
    Battleship() :Ship(4,"Battleship") {}
};

class Carrier : public Ship {
public:
    Carrier() :Ship(5,"Carrier") {}
};



class Point {
private:
    int x;
    int y;
};

int main()
{
    Battleship battleship_one;
    Destroyer destroyer_one;
    battleship_one.displayHealth();
    battleship_one.getHit();
    battleship_one.getHit();
    destroyer_one.getHit();
    destroyer_one.getHit();
    destroyer_one.getHit();
 


}
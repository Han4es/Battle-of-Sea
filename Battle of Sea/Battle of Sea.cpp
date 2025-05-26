#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <set>
#include <algorithm>


using namespace std;

#define RED     "\033[31m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

int inputCoordinate(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0 && value <= 9)
            return value;
        cout << "Invalid input. Enter a number from 0 to 9.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

char inputDirection() {
    char dir;
    while (true) {
        cout << "Horizontal? (y/n): ";
        if (cin >> dir && (dir == 'y' || dir == 'n'))
            return dir;
        cout << "Invalid input. Enter 'y' or 'n'.\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

class Ship {
protected:
    int health;
    string name;
public:
    Ship(int health, string name) : health(health), name(name) {}
    int getHealth() const { return health; }
    string getName()  const { return name; }
};

class Pepe : public Ship { public: Pepe() : Ship(1, "Pepe") {} };
class Destroyer : public Ship { public: Destroyer() : Ship(2, "Destroyer") {} };
class Cruiser : public Ship { public: Cruiser() : Ship(3, "Cruiser") {} };
class Battleship : public Ship { public: Battleship() : Ship(4, "Battleship") {} };
class Carrier : public Ship { public: Carrier() : Ship(5, "Carrier") {} };

class Battlefield {
private:
    static const int SIZE = 10;
    char grid[SIZE][SIZE];

public:
    Battlefield() {
        for (int i = 0; i < SIZE; ++i)
            for (int j = 0; j < SIZE; ++j)
                grid[i][j] = '.';
    }

    void display(bool hideShips = false) const {
        cout << "  ";
        for (int i = 0; i < SIZE; ++i) cout << i << " ";
        cout << endl;
        for (int i = 0; i < SIZE; ++i) {
            cout << i << " ";
            for (int j = 0; j < SIZE; ++j) {
                char c = grid[i][j];
                if (hideShips && c == 'S') cout << ". ";
                else if (c == 'S')              cout << BLUE << c << RESET << " ";
                else if (c == '#')              cout << RED << c << RESET << " ";
                else if (c == 'X')              cout << RED << c << RESET << " ";
                else                            cout << c << " ";
            }
            cout << endl;
        }
    }

    bool placeShip(int x, int y, int length, bool horizontal) {
        if (horizontal) {
            if (y + length > SIZE) return false;
            for (int i = -1; i <= 1; ++i)
                for (int j = -1; j <= length; ++j) {
                    int nx = x + i, ny = y + j;
                    if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE && grid[nx][ny] == 'S')
                        return false;
                }
            for (int i = 0; i < length; ++i) grid[x][y + i] = 'S';
        }
        else {
            if (x + length > SIZE) return false;
            for (int i = -1; i <= length; ++i)
                for (int j = -1; j <= 1; ++j) {
                    int nx = x + i, ny = y + j;
                    if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE && grid[nx][ny] == 'S')
                        return false;
                }
            for (int i = 0; i < length; ++i) grid[x + i][y] = 'S';
        }
        return true;
    }

    bool isShipSunkFrom(int x, int y) const {
        static const int dx[] = { -1, 1, 0, 0 };
        static const int dy[] = { 0, 0, -1, 1 };
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            while (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE) {
                if (grid[nx][ny] == 'S') return false;
                if (grid[nx][ny] != 'X') break;
                nx += dx[d]; ny += dy[d];
            }
        }
        return true;
    }

    void markSunkShip(int x, int y) {
        if (x < 0 || y < 0 || x >= 10 || y >= 10 || grid[x][y] != 'X') return;
        grid[x][y] = '#';
        markSunkShip(x + 1, y); markSunkShip(x - 1, y);
        markSunkShip(x, y + 1); markSunkShip(x, y - 1);
    }

    bool attack(int x, int y, vector<pair<int, int>>& botHits) {
        if (x < 0 || x >= 10 || y < 0 || y >= 10) return false;
        if (grid[x][y] == 'X' || grid[x][y] == 'o' || grid[x][y] == '#') {
            cout << "You already shot there.\n";
            return false;
        }
        if (grid[x][y] == 'S') {
            grid[x][y] = 'X';
            cout << "Hit!\n";
            if (isShipSunkFrom(x, y)) {
                markSunkShip(x, y);
                cout << "Ship destroyed!\n";
                botHits.clear();  
            }
            return true;
        }
        else {
            grid[x][y] = 'o';
            cout << "Miss.\n";
            return false;
        }
    }

    bool hasShips() const {
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                if (grid[i][j] == 'S') return true;
        return false;
    }

    char getCell(int x, int y) const {
        return grid[x][y];
    }
};

void placePlayerShips(Battlefield& field, const vector<Ship*>& ships) {
    for (const auto& ship : ships) {
        bool placed = false;
        while (!placed) {
            cout << "Place your " << ship->getName() << " (size " << ship->getHealth() << ")\n";
            int x = inputCoordinate("Enter row (0-9): ");
            int y = inputCoordinate("Enter column (0-9): ");
            char dir = inputDirection();
            placed = field.placeShip(x, y, ship->getHealth(), dir == 'y');
            if (!placed) cout << "Invalid position, try again.\n";
            else field.display();
        }
    }
}

void placeBotShips(Battlefield& field, const vector<Ship*>& ships) {
    srand((unsigned)time(0));
    for (const auto& ship : ships) {
        bool placed = false;
        while (!placed) {
            int x = rand() % 10;
            int y = rand() % 10;
            bool horizontal = rand() % 2;
            placed = field.placeShip(x, y, ship->getHealth(), horizontal);
        }
    }
}

void botTurn(Battlefield& playerField, vector<pair<int, int>>& botHits) {
    static const int dx[] = { -1, 1, 0, 0 };
    static const int dy[] = { 0, 0, -1, 1 };

    if (botHits.size() >= 2) {
        sort(botHits.begin(), botHits.end());
        auto a = botHits.front();
        auto b = botHits.back();

        bool horizontal = (a.first == b.first);
        int fx = a.first, fy = a.second;
        int lx = b.first, ly = b.second;

        for (int dir = 1; dir >= -1; dir -= 2) {
            for (int i = 1; i < 5; ++i) {
                int x = horizontal ? fx : fx + dir * i;
                int y = horizontal ? fy + dir * i : fy;
                if (x < 0 || y < 0 || x >= 10 || y >= 10) break;

                char c = playerField.getCell(x, y);
                if (c != 'X' && c != 'o' && c != '#') {
                    cout << "Enemy fires at (" << x << ", " << y << ")...\n";
                    bool result = playerField.attack(x, y, botHits);
                    if (result) botHits.emplace_back(x, y);
                    return;
                }
                if (c == '.') break; // нет смысла продолжать по пустой
            }
        }
    }

    // Один удар — пробуем 4 стороны
    if (!botHits.empty()) {
        auto base = botHits.front();
        for (int d = 0; d < 4; ++d) {
            int x = base.first + dx[d];
            int y = base.second + dy[d];
            if (x >= 0 && x < 10 && y >= 0 && y < 10) {
                char c = playerField.getCell(x, y);
                if (c != 'X' && c != 'o' && c != '#') {
                    cout << "Enemy fires at (" << x << ", " << y << ")...\n";
                    bool result = playerField.attack(x, y, botHits);
                    if (result) botHits.emplace_back(x, y);
                    return;
                }
            }
        }
    }

    // fallback — случайный выстрел
    int x, y;
    char c;
    do {
        x = rand() % 10;
        y = rand() % 10;
        c = playerField.getCell(x, y);
    } while (c == 'X' || c == 'o' || c == '#');

    cout << "Enemy fires at (" << x << ", " << y << ")...\n";
    bool result = playerField.attack(x, y, botHits);
    if (result) {
        botHits.clear();
        botHits.emplace_back(x, y);
    }
}




int main() {
    Battlefield playerField;
    Battlefield enemyField;
    vector<pair<int, int>> botHits;

    vector<Ship*> fleet = {
        new Carrier(), new Battleship(), new Cruiser(), new Destroyer(), new Pepe()
    };

    vector<Ship*> botFleet = {
        new Carrier(), new Battleship(), new Cruiser(), new Destroyer(), new Pepe()
    };

    cout << "=== Set up your fleet ===\n";
    playerField.display();
    placePlayerShips(playerField, fleet);

    cout << "\n=== Enemy is placing ships... ===\n";
    placeBotShips(enemyField, botFleet);

    cout << "\n=== Battle begins ===\n";
    bool playerTurn = true;
    while (playerField.hasShips() && enemyField.hasShips()) {
        if (playerTurn) {
            cout << "\nYour battlefield:\n";
            playerField.display();
            cout << "\nEnemy battlefield (hidden):\n";
            enemyField.display(true);

            int x = inputCoordinate("Enter row (0-9): ");
            int y = inputCoordinate("Enter column (0-9): ");
            char cell = enemyField.getCell(x, y);

            if (cell == 'X' || cell == 'o' || cell == '#') {
                cout << "You already shot there. Try again.\n";
                continue;
            }

            bool result = enemyField.attack(x, y, botHits);
            playerTurn = result;
        }
        else {
            size_t before = botHits.size();
            botTurn(playerField, botHits);
            size_t after = botHits.size();
            playerTurn = (after == before);
        }
    }

    cout << (playerField.hasShips() ? "\nYou win!\n" : "\nYou lost.\n");

    for (auto ship : fleet) delete ship;
    for (auto ship : botFleet) delete ship;

    return 0;
}

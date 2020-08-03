#include "BattleController.h"

int main() {
    system("chcp 65001");
    srand(time(NULL));

    Monster player_1(2, "");
    Monster player_2(6, "");
    BattleController Battle(player_1, player_2, true);

    while (true) {
        Battle.start();
        system("pause");
    }

    return 0;
}

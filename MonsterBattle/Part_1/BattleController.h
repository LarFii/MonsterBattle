//
// Created by LarFii on 2020/8/3.
//

#ifndef MAIN_BATTLECONTROLLER_H
#define MAIN_BATTLECONTROLLER_H

#include "MonsterBase.h"

class BattleController {
public:
    BattleController(
            Monster &Monster_1,
            Monster &Monster_2,
            bool    autoFight = true
    );
    ~BattleController() {};

    void start();

private:
    Monster &p1, &p2;
    bool m_auto;
    int timer_1;
    int timer_2;
};

#endif //MAIN_BATTLECONTROLLER_H

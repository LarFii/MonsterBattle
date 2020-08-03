//
// Created by LarFii on 2020/8/3.
//

#include "BattleController.h"

BattleController::BattleController(
        Monster &Monster_1,
        Monster &Monster_2,
        bool     autoFight
) : p1(Monster_1), p2(Monster_2) {
    m_auto = autoFight;
}

void
    BattleController::start() {
        cout << p1.get_NickName() << " VS " << p2.get_NickName() << endl;
        p1.printINFO();
        p2.printINFO();
        cout << "战斗开始！" << endl;

        p1.Recover();
        p2.Recover();

        timer_1 = 0;
        timer_2 = 0;

        while (true) {
            while (timer_1 < p1.get_Battle_Speed() && \
                   timer_2 < p2.get_Battle_Speed()) {
                ++timer_1;
                ++timer_2;
            }

            if (timer_1 >= p1.get_Battle_Speed() && \
                timer_2 >= p2.get_Battle_Speed()) {
                if (p1.get_Battle_Speed() >= p2.get_Battle_Speed()) {
                    if (p1.attack(p2, m_auto))
                        break;
                    if (p2.attack(p1, m_auto))
                        break;
                } else {
                    if (p2.attack(p1, m_auto))
                        break;
                    if (p1.attack(p2, m_auto))
                        break;
                }
                timer_1 = timer_2 = 0;
            } else if (timer_1 >= p1.get_Battle_Speed()) {
                if (p2.attack(p1, m_auto))
                    break;
                timer_1 = 0;
            } else {
                if (p1.attack(p2, m_auto))
                    break;
                timer_2 = 0;
            }
        }

        if (p1.get_Battle_HP()) {
            int exp;

            cout << p1.get_NickName() << " 胜利！" << endl;

            exp = p2.get_Level() * 10 + random(p1.get_Level() * 3);
            if (p1.takeExp(exp)) {
                cout << p1.get_NickName() << "获得" << exp;
                cout << "点经验 " << "当前经验：" << p1.get_Exp() << endl;
                cout << p1.get_NickName() << " 升级！" << endl;
                p1.pNeedExp();
                p1.printINFO();

                return;
            }
            cout << p1.get_NickName() << "获得" << exp;
            cout << "点经验 " << "当前经验：" << p1.get_Exp() << endl;
            p1.pNeedExp();

            return;
        } else {
            int exp;

            cout << p2.get_NickName() << " 胜利！" << endl;

            exp = p1.get_Level() * 10 + random(p2.get_Level() * 3);
            if (p2.takeExp(exp)) {
                cout << p2.get_NickName() << "获得" << exp;
                cout << "点经验 " << "当前经验：" << p2.get_Exp() << endl;
                cout << p2.get_NickName() << "升级！" << endl;
                p2.pNeedExp();
                p2.printINFO();

                return;
            }
            cout << p2.get_NickName() << "获得" << exp;
            cout << "点经验 " << "当前经验：" << p2.get_Exp() << endl;
            p2.pNeedExp();

            return;
        }
    }
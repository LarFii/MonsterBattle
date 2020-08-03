//
// Created by LarFii on 2020/7/30.
//

#include "MonsterBase.h"

#define randDmg(n) rand() % (2 * n + 1) - n;    // 伤害浮动

// Init
constexpr int InitHP        = 25;
constexpr int InitExp       = 0;
constexpr int InitLevel     = 1;
constexpr int InitSpeed     = 10;
constexpr int InitAttack    = 10;
constexpr int InitDefense   = 5;
constexpr int RandPotential = 5;

// LevelUp
constexpr int LevelUp        = 1;
constexpr int LevelUpHp      = 8;
constexpr int LevelUpSpeed   = 5;
constexpr int LevelUpAttack  = 4;
constexpr int LevelUpDefense = 2;

const MonsterBase *Monster::s_Races[8] = {
    new Race<0>(), new Race<1>(), new Race<2>(), new Race<3>(),
    new Race<4>(), new Race<5>(), new Race<6>(), new Race<7>()
};

MonsterBase::MonsterBase(MonsterType type) {

    // 初始化各种属性
    set_Race(type);
    set_HP(InitHP);
    set_Speed(InitSpeed);
    set_Attack(InitAttack);
    set_Defense(InitDefense);

    switch (type) {
        case HHP:
            update_HP(InitHP);
            break;
        case SPE:
            update_Speed(InitSpeed);
            break;
        case ATK:
            update_Attack(InitAttack);
            break;
        case DEF:
            update_Defense(InitDefense);
            break;
        default:
            break;
    }
}

string
    MonsterBase::getSkillName(int index) const {
        if (index >= 0 && index <= 3)
            return m_Skills[index][0];
        cout << "(get)SkillNameIndex out of size." << endl;
        return "";
    }

string
    MonsterBase::getSkillDetail(int index) const {
        if (index >= 0 && index <= 3)
            return m_Skills[index][1];
        cout << "(get)SkillDetailIndex out of size." << endl;
        return "";
    }

int
    MonsterBase::getPP(int index) const {
        if (index >= 0 && index <= 2)
            return m_pp[index];
        cout << "(get)PPIndex out of size." << endl;
        return 0;
    }

int
    MonsterBase::getUpCurve(int level) const {
        if (level >= 2 && level <= 15)
            return m_UpCurve[level - 2];
        cout << "Level out of size." << endl;
        return 0;
    }

void
    MonsterBase::setSkillName(const string& Name, int index) {
        if (index >= 0 && index <= 3)
            m_Skills[index][0] = Name;
        else
            cout << "(set)SkillNameIndex out of size." << endl;
    }

void
    MonsterBase::setSkillDetail(const string &Detail, int index) {
        if (index >= 0 && index <= 3)
            m_Skills[index][1] = Detail;
        else
            cout << "(set)SkillDetailIndex out of size." << endl;
    }

void
    MonsterBase::setPP(const int& num, int index) {
        if (index >= 0 && index <= 2)
            m_pp[index] = num;
        else
            cout << "(set)PPIndex out of size." << endl;
    }

Monster::Monster(int RaceIndex, const string& NickName) {
    set_Exp(InitExp);
    set_Level(InitLevel);
    set_RaceIndex(RaceIndex);
    set_Potential(random(RandPotential));

    set_Init_HP(s_Races[m_RaceIndex]->get_HP() + \
                    get_Potential());
    set_Init_Speed(s_Races[m_RaceIndex]->get_Speed() + \
                       get_Potential());
    set_Init_Attack(s_Races[m_RaceIndex]->get_Attack() + \
                        get_Potential());
    set_Init_Defense(s_Races[m_RaceIndex]->get_Defense() + \
                         get_Potential());

    if (!NickName.length())
        m_NickName = s_Races[m_RaceIndex]->get_Name();      // 昵称默认为种族名
    else
        m_NickName = NickName;

    for (int i = 0; i < 3; ++i) {
        setBattlePP(i);
    }

    printINFO();
}

void
    Monster::setBattlePP(int index) {
        if (index >= 0 && index <= 2)
            m_Battle_pp[index] = s_Races[m_RaceIndex]->getPP(index);
        else
            cout << "(set)BattlePPIndex out of size." << endl;
    }

string
    Monster::getRaceType() const {
        switch (s_Races[m_RaceIndex]->get_Race()) {
            case HHP:
                return "肉盾型";
            case SPE:
                return "速度型";
            case ATK:
                return "攻击型";
            case DEF:
                return "防御型";
            default:
                break;
        }
        return "";
    }

int
    Monster::getBattlePP(int index) const {
    if (index >= 0 && index <= 2)
        return m_Battle_pp[index];
    cout << "(get)BattlePPIndex out of size." << endl;
    return 0;
}

void
    Monster::Recover() {
        set_Battle_HP(get_Init_HP());
        set_Battle_Speed(get_Init_Speed());
        set_Battle_Attack(get_Init_Attack());
        set_Battle_Defense(get_Init_Defense());

        for (int i = 0; i < 3; ++i) {
            setBattlePP(i);
        }
    }

bool
    Monster::takeExp(int amt) {
        bool Level_Up = false;

        if (m_Level == 15) return false;

        if (amt <= 0) amt = 1;

        m_Exp += amt;
        while (m_Level < 15 && m_Exp > \
                s_Races[m_RaceIndex]->getUpCurve(m_Level + 1)) {
            int amt_HP, amt_Speed, amt_Attack, amt_Defense;

            Level_Up = true;
            update_Level(LevelUp);

            amt_HP = LevelUpHp;
            amt_Speed = LevelUpSpeed;
            amt_Attack = LevelUpAttack;
            amt_Defense = LevelUpDefense;

            switch (s_Races[m_RaceIndex]->get_Race()) {
                case HHP:
                    amt_HP += LevelUpHp + get_Potential();
                    break;
                case SPE:
                    amt_Speed += LevelUpSpeed + get_Potential();
                    break;
                case ATK:
                    amt_Attack += LevelUpAttack + get_Potential();
                    break;
                case DEF:
                    amt_Defense += LevelUpDefense + get_Potential();
                    break;
                default:
                    break;
            }

            update_Init_HP(amt_HP);
            update_Init_Speed(amt_Speed);
            update_Init_Attack(amt_Attack);
            update_Init_Defense(amt_Defense);
        }

        if (Level_Up) return true;

        return false;       // default
    }

bool
    Monster::attack(Monster &aim, bool autoBattle) {
        int SkillIndex = 0;

        if (autoBattle) {
            bool usable[3] = { false };
            int usableCount = 1;

            for (int i = 0; i < 3; ++i) {
                if (m_Level >= (i + 1) * 5 && m_Battle_pp[i]) {
                    usable[i] = true;
                    ++usableCount;
                }
            }

            int use = rand() % usableCount;

            if (!use)
                SkillIndex = 0;
            else {
                for (int i = 0; i < 3; ++i) {
                    if (usable[i]) {
                        --use;

                        if (!use) {
                            SkillIndex = i + 1;
                            break;
                        }
                    }
                }
            }

            if (SkillIndex > 0) changePP(SkillIndex - 1);

            cout << get_NickName() <<" 使用技能 ";
            cout << getSkillName(SkillIndex) << endl;
            if (SkillIndex > 0)
                cout << "技能剩余次数：" << getBattlePP(SkillIndex - 1) << endl;
            return s_Races[m_RaceIndex]->attack(
                    *this, aim, SkillIndex);
        }

        cout << "轮到" << get_NickName() << "行动" << endl;
        cout << "请输入你需要释放的技能编号：";
        cin >> SkillIndex;

        --SkillIndex;

        if (SkillIndex < 0 || SkillIndex > 3) SkillIndex = 0;

        if (SkillIndex * 5 <= m_Level && m_Battle_pp[SkillIndex - 1] && \
                SkillIndex != 0) {
            changePP(SkillIndex - 1);

            cout << get_NickName() <<" 使用技能 ";
            cout << getSkillName(SkillIndex) << endl;
            cout << "技能剩余次数：" << getBattlePP(SkillIndex - 1) << endl;

            return s_Races[m_RaceIndex]->attack(
                    *this, aim, SkillIndex);
        }

        cout << get_NickName() <<" 使用技能 ";
        cout << getSkillName(SkillIndex) << endl;

        return s_Races[m_RaceIndex]->attack(
                *this, aim, 0);
    }

bool
    Monster::takeDamage(int n) {
        if (n < 1) n = 1;

        cout << "造成" << n << "点伤害" << endl;

        return changeHP(-n);
    }

void
    Monster::changePP(int index) {
        --m_Battle_pp[index];
    }

bool
    Monster::changeHP(int count) {
        update_Battle_HP(count);

        if (m_Battle_HP > m_Init_HP) set_Battle_HP(m_Init_HP);

        if (m_Battle_HP < 0) set_Battle_HP(0);

        cout << get_NickName() << "剩余生命值为：";
        cout << get_Battle_HP() << endl;

        if (!m_Battle_HP) return true;

        return false;
    }

void
    Monster::changeSpeed(int count) {
        update_Battle_Speed(count);

        if (m_Battle_Speed < 1) set_Battle_Speed(1);

        cout << get_NickName() << "的速度变为：" << get_Battle_Speed() << endl;
    }

void
    Monster::changeAttack(int count) {
        update_Battle_Attack(count);

        if (m_Battle_Attack < 1) set_Battle_Attack(1);

        cout << get_NickName() << "的攻击力变为：";
        cout << get_Battle_Attack() << endl;
    }

void
    Monster::changeDefense(int count) {
        update_Battle_Defense(count);

        if (m_Battle_Defense < 1) set_Battle_Defense(1);

        cout << get_NickName() << "的防御力变为：";
        cout << get_Battle_Defense() << endl;
    }

void
    Monster::printINFO() const {
        cout << "宠物昵称：" << get_NickName();
        cout << " 类型：" << getRaceType() << endl;
        cout << "等级：" << get_Level() << " 经验：" << get_Exp() << endl;
        cout << "生命值：" << get_Init_HP() << endl;
        cout << "攻击力：" << get_Init_Attack() << endl;
        cout << "防御力：" << get_Init_Defense() << endl;
        cout << "速度：" << get_Init_Speed() << endl;
        cout << "潜力值：" << get_Potential() << endl;
        cout << "技能1：" << getSkillName(0);
        cout << " 使用次数：∞";
        cout << " 效果：" << getSkillDetail(0) << endl;
        cout << "技能2：" << getSkillName(1);
        cout << " 使用次数：" << getPP(0);
        cout << " 效果：" << getSkillDetail(1);
        cout << " 解锁等级：5级" << endl;
        cout << "技能3：" << getSkillName(2);
        cout << " 使用次数：" << getPP(1);
        cout << " 效果：" << getSkillDetail(2);
        cout << " 解锁等级：10级" << endl;
        cout << "技能4：" << getSkillName(3);
        cout << " 使用次数：" << getPP(2);
        cout << " 效果：" << getSkillDetail(3);
        cout << " 解锁等级：15级" << endl;
    }

void
    Monster::pNeedExp() const {
        if (get_Level() < 15) {
            int NeedExp;

            NeedExp = s_Races[m_RaceIndex]-> \
                    getUpCurve(get_Level() + 1) - get_Exp();

            cout << "距离下一级级还需" << NeedExp << "点经验" << endl;
        } else
            cout << "已满级！" << endl;
    }

template <>
Race<0>::Race() : MonsterBase(HHP) {
    string Skill_0, Skill_1, Skill_2, Skill_3;
    string Dscp_0, Dscp_1, Dscp_2, Dscp_3;

    set_Name("龙神哈莫");

    Skill_0 = "龙爪";
    setSkillName(Skill_0, 0);
    Skill_1 = "绿光珠";
    setSkillName(Skill_1, 1);
    Skill_2 = "锁敌";
    setSkillName(Skill_2, 2);
    Skill_3 = "龙·御天神";
    setSkillName(Skill_3, 3);

    Dscp_0 = "进行一次打击，造成自身攻击力*1的伤害";
    setSkillDetail(Dscp_0, 0);
    Dscp_1 = "进行一次打击，造成自身攻击力*1.2的伤害";
    setSkillDetail(Dscp_1, 1);
    Dscp_2 = "永久降低敌方我方等级*1的攻击力";
    setSkillDetail(Dscp_2, 2);
    Dscp_3 = "恢复自身75%的生命值";
    setSkillDetail(Dscp_3, 3);

    setPP(10, 0);
    setPP(3, 1);
    setPP(1, 2);
}

template <>
bool
    Race<0>::attack(Monster &attacker, Monster &aim, int SkillIndex) const {
        int dmg = randDmg(3);

        switch (SkillIndex) {
            case 1:
                dmg += (int)floor(attacker.get_Battle_Attack() * 1.2) - \
                        aim.get_Battle_Defense();

                return aim.takeDamage(dmg);
            case 2:
                aim.changeAttack(-attacker.get_Level());

                return false;
            case 3:
                attacker.changeHP(
                        (int)floor(attacker.get_Init_HP() * 0.75)
                        );

                break;
            default:
                dmg += attacker.get_Battle_Attack() - \
                        aim.get_Battle_Defense();

                return aim.takeDamage(dmg);
        }

        return false;
    }

template <>
Race<1>::Race() : MonsterBase(SPE) {
    string Skill_0, Skill_1, Skill_2, Skill_3;
    string Dscp_0, Dscp_1, Dscp_2, Dscp_3;

    set_Name("哀影疾风");

    Skill_0 = "啄击";
    setSkillName(Skill_0, 0);
    Skill_1 = "掠天影刹";
    setSkillName(Skill_1, 1);
    Skill_2 = "惊魂嘶叫";
    setSkillName(Skill_2, 2);
    Skill_3 = "浑天疾影";
    setSkillName(Skill_3, 3);

    Dscp_0 = "进行一次打击，造成自身攻击力*1的伤害";
    setSkillDetail(Dscp_0, 0);
    Dscp_1 = "进行一次打击，造成自身速度*1的伤害";
    setSkillDetail(Dscp_1, 1);
    Dscp_2 = "永久降低敌方自身等级*1的速度";
    setSkillDetail(Dscp_2, 2);
    Dscp_3 = "自身速度提高50%";
    setSkillDetail(Dscp_3, 3);

    setPP(3, 0);
    setPP(3, 1);
    setPP(1, 2);
}

template <>
bool
    Race<1>::attack(Monster &attacker, Monster &aim, int SkillIndex) const {
        int dmg = randDmg(3);

        switch (SkillIndex) {
            case 1:
                dmg += attacker.get_Battle_Speed() - \
                        aim.get_Battle_Defense();

                return aim.takeDamage(dmg);
            case 2:
                aim.changeSpeed(-attacker.get_Level());

                break;
            case 3:
                attacker.changeSpeed(
                        (int)floor(attacker.get_Battle_Speed() * 0.5)
                        );
                break;
            default:
                dmg += attacker.get_Battle_Attack() - \
                        aim.get_Battle_Defense();

                return aim.takeDamage(dmg);
        }

        return false;
    }

template <>
Race<2>::Race() : MonsterBase(ATK) {
    string Skill_0, Skill_1, Skill_2, Skill_3;
    string Dscp_0, Dscp_1, Dscp_2, Dscp_3;

    set_Name("圣灵谱尼");

    Skill_0 = "极光";
    setSkillName(Skill_0, 0);
    Skill_1 = "灵光之怒";
    setSkillName(Skill_1, 1);
    Skill_2 = "神灵救世光";
    setSkillName(Skill_2, 2);
    Skill_3 = "圣灵悲魂曲";
    setSkillName(Skill_3, 3);

    Dscp_0 = "进行一次打击，造成攻击力*1的伤害";
    setSkillDetail(Dscp_0, 0);
    Dscp_1 = "进行一次打击，造成攻击力*1.2的伤害，当自身生命值低于50%时,伤害变为*1.5";
    setSkillDetail(Dscp_1, 1);
    Dscp_2 = "消除自身所有减益效果，消除对方所有增益效果";
    setSkillDetail(Dscp_2, 2);
    Dscp_3 = "进行一次打击，造成攻击力*1.5的伤害，并有30%的几率秒杀对方";
    setSkillDetail(Dscp_3, 3);

    setPP(3, 0);
    setPP(2, 1);
    setPP(1, 2);
}

template <>
bool
    Race<2>::attack(Monster &attacker, Monster &aim, int SkillIndex) const {
        int dmg  = randDmg(3);
        int flag = random(10);

        switch (SkillIndex) {
            case 1:
                if (attacker.get_Battle_HP() >= \
                        (int)floor(attacker.get_Init_HP() * 0.5)) {
                    dmg += (int)floor(attacker.get_Battle_Attack() * 1.2) - \
                            aim.get_Battle_Defense();
                } else {
                    dmg += (int)floor(attacker.get_Battle_Attack() * 1.5) - \
                            aim.get_Battle_Defense();
                }

                return aim.takeDamage(dmg);
            case 2:
                attacker.set_Battle_Speed(attacker.get_Init_Speed());
                attacker.set_Battle_Attack(attacker.get_Init_Attack());
                attacker.set_Battle_Defense(attacker.get_Battle_Defense());

                if (aim.get_Battle_Speed() > aim.get_Init_Speed())
                    aim.set_Battle_Speed(aim.get_Init_Speed());
                if (aim.get_Battle_Attack() > aim.get_Init_Attack())
                    aim.set_Battle_Attack(aim.get_Init_Attack());
                if (aim.get_Battle_Defense() > aim.get_Init_Defense());
                    aim.set_Battle_Defense(aim.get_Battle_Defense());

                break;
            case 3:
                if (flag > 6)       // 30%概率秒杀
                    dmg = aim.get_Battle_HP();
                else
                    dmg = (int)floor(attacker.get_Battle_Attack() * 1.5) - \
                            aim.get_Battle_Defense();

                return aim.takeDamage(dmg);
            default:
                dmg += attacker.get_Battle_Attack() - \
                            aim.get_Battle_Defense();

                return aim.takeDamage(dmg);
        }

        return false;
    }

template <>
Race<3>::Race() : MonsterBase(DEF) {
    string Skill_0, Skill_1, Skill_2, Skill_3;
    string Dscp_0, Dscp_1, Dscp_2, Dscp_3;

    set_Name("御象灵尊");

    Skill_0 = "重拳";
    setSkillName(Skill_0, 0);
    Skill_1 = "业障尽散";
    setSkillName(Skill_1, 1);
    Skill_2 = "天赐佛甲";
    setSkillName(Skill_2, 2);
    Skill_3 = "福泽聚宝象";
    setSkillName(Skill_3, 3);

    Dscp_0 = "进行一次打击，造成攻击力*1的伤害";
    setSkillDetail(Dscp_0, 0);
    Dscp_1 = "移除对方增益效果";
    setSkillDetail(Dscp_1, 1);
    Dscp_2 = "提升自身30%的防御值";
    setSkillDetail(Dscp_2, 2);
    Dscp_3 = "造成自身攻击力加上对方攻击力的伤害";
    setSkillDetail(Dscp_3, 3);

    setPP(3, 0);
    setPP(3, 1);
    setPP(1, 2);
}

template <>
bool
    Race<3>::attack(Monster &attacker, Monster &aim, int SkillIndex) const {
        int dmg = randDmg(3);

        switch (SkillIndex) {
            case 1:
                if (aim.get_Battle_Speed() > aim.get_Init_Speed())
                    aim.set_Battle_Speed(aim.get_Init_Speed());
                if (aim.get_Battle_Attack() > aim.get_Init_Attack())
                    aim.set_Battle_Attack(aim.get_Init_Attack());
                if (aim.get_Battle_Defense() > aim.get_Init_Defense());
                    aim.set_Battle_Defense(aim.get_Battle_Defense());

                cout << "敌方的一切增益效果消除了！" << endl;

                break;
            case 2:
                attacker.changeDefense(
                    (int)floor(attacker.get_Battle_Defense() * 0.3)
                    );

                break;
            case 3:
                dmg += attacker.get_Battle_Attack() + \
                            aim.get_Battle_Attack() - aim.get_Battle_Defense();

                return aim.takeDamage(dmg);
            default:
                dmg += attacker.get_Battle_Attack() - \
                            aim.get_Battle_Defense();

                return aim.takeDamage(dmg);
        }

        return false;
    }

template <>
Race<4>::Race() : MonsterBase(HHP) {
    string Skill_0, Skill_1, Skill_2, Skill_3;
    string Dscp_0, Dscp_1, Dscp_2, Dscp_3;

    set_Name("地葬界神");

    Skill_0 = "冲击";
    setSkillName(Skill_0, 0);
    Skill_1 = "苍茫守护";
    setSkillName(Skill_1, 1);
    Skill_2 = "七伤秘术";
    setSkillName(Skill_2, 2);
    Skill_3 = "天地同变";
    setSkillName(Skill_3, 3);

    Dscp_0 = "进行一次打击，造成自身等级*1的固定真实伤害, 最少10点";
    setSkillDetail(Dscp_0, 0);
    Dscp_1 = "恢复自身等级*3的生命值";
    setSkillDetail(Dscp_1, 1);
    Dscp_2 = "对敌方造成攻击力*2的伤害但是我方也会受到攻击力*1的伤害（无法自杀）";
    setSkillDetail(Dscp_2, 2);
    Dscp_3 = "对敌方造成自身已损失生命值的伤害（无视防御）";
    setSkillDetail(Dscp_3, 3);

    setPP(5, 0);
    setPP(3, 1);
    setPP(1, 2);
}

template <>
bool
    Race<4>::attack(Monster &attacker, Monster &aim, int SkillIndex) const {
        int dmg     = randDmg(3);
        int dmgSelf = randDmg(3);

        switch (SkillIndex) {
            case 1:
                attacker.changeHP(attacker.get_Level() * 3);

                break;
            case 2:
                dmg     += attacker.get_Battle_Attack() * 2 - \
                                aim.get_Battle_Defense();
                dmgSelf += attacker.get_Battle_Attack() - \
                                attacker.get_Battle_Defense();
                if (dmgSelf > attacker.get_Battle_HP())
                    --dmgSelf;

                cout << "对自身";

                attacker.takeDamage(dmgSelf);

                return aim.takeDamage(dmg);
            case 3:
                dmg = attacker.get_Init_HP() - attacker.get_Battle_HP();

                return aim.takeDamage(dmg);
            default:
                if (attacker.get_Level() < 10)
                    dmg = 10;
                else
                    dmg = attacker.get_Level();

                return aim.takeDamage(dmg);
        }

        return false;
    }

template <>
Race<5>::Race() : MonsterBase(SPE) {
    string Skill_0, Skill_1, Skill_2, Skill_3;
    string Dscp_0, Dscp_1, Dscp_2, Dscp_3;

    set_Name("云华");

    Skill_0 = "冲顶";
    setSkillName(Skill_0, 0);
    Skill_1 = "扰乱";
    setSkillName(Skill_1, 1);
    Skill_2 = "迷幻之翼";
    setSkillName(Skill_2, 2);
    Skill_3 = "婀娜拂风";
    setSkillName(Skill_3, 3);

    Dscp_0 = "进行一次打击，造成攻击力*1的伤害";
    setSkillDetail(Dscp_0, 0);
    Dscp_1 = "降低对方我方等级*1的速度";
    setSkillDetail(Dscp_1, 1);
    Dscp_2 = "造成自身速度*1.2的伤害";
    setSkillDetail(Dscp_2, 2);
    Dscp_3 = "自身速度提高40%";
    setSkillDetail(Dscp_3, 3);

    setPP(3, 0);
    setPP(3, 1);
    setPP(1, 2);
}

template <>
bool
    Race<5>::attack(Monster &attacker, Monster &aim, int SkillIndex) const {
        int dmg = randDmg(3);

        switch (SkillIndex) {
            case 1:
                aim.changeSpeed(-attacker.get_Level());

                break;
            case 2:
                dmg += (int)floor(attacker.get_Battle_Speed() * 1.2) - \
                            aim.get_Battle_Defense();

                return aim.takeDamage(dmg);
            case 3:
                attacker.changeSpeed(
                        (int)floor(attacker.get_Battle_Speed() * 0.4)
                        );

                break;
            default:
                dmg += attacker.get_Battle_Attack() - \
                            aim.get_Battle_Defense();

                return aim.takeDamage(dmg);
        }

        return false;
    }

template <>
Race<6>::Race() : MonsterBase(ATK) {
    string Skill_0, Skill_1, Skill_2, Skill_3;
    string Dscp_0, Dscp_1, Dscp_2, Dscp_3;

    set_Name("战之狂热·英卡洛斯");

    Skill_0 = "突斩";
    setSkillName(Skill_0, 0);
    Skill_1 = "破邪十字军";
    setSkillName(Skill_1, 1);
    Skill_2 = "斗气护体";
    setSkillName(Skill_2, 2);
    Skill_3 = "战荒战威破";
    setSkillName(Skill_3, 3);

    Dscp_0 = "进行一次打击，造成攻击力*1的伤害";
    setSkillDetail(Dscp_0, 0);
    Dscp_1 = "进行一次打击，造成攻击力*1.2的真实伤害(无视防御)";
    setSkillDetail(Dscp_1, 1);
    Dscp_2 = "提升自身攻击*0.3的防御值";
    setSkillDetail(Dscp_2, 2);
    Dscp_3 = "造成自身攻击力*2.5的伤害";
    setSkillDetail(Dscp_3, 3);

    setPP(5, 0);
    setPP(2, 1);
    setPP(1, 2);
}

template <>
bool
    Race<6>::attack(Monster &attacker, Monster &aim, int SkillIndex) const {
        int dmg = randDmg(3);

        switch (SkillIndex) {
            case 1:
                dmg += (int)floor(attacker.get_Battle_Attack() * 1.2);

                return aim.takeDamage(dmg);
            case 2:
                attacker.changeDefense(
                    (int)floor(attacker.get_Battle_Attack() * 0.3)
                    );

                break;
            case 3:
                dmg += (int)floor(attacker.get_Battle_Attack() * 2.5) - \
                            aim.get_Battle_Defense();

                return aim.takeDamage(dmg);
            default:
                dmg += attacker.get_Battle_Attack() - \
                            aim.get_Battle_Defense();

                return aim.takeDamage(dmg);
        }

        return false;
    }

template <>
Race<7>::Race() : MonsterBase(DEF) {
    string Skill_0, Skill_1, Skill_2, Skill_3;
    string Dscp_0, Dscp_1, Dscp_2, Dscp_3;

    set_Name("圣辰之铠");

    Skill_0 = "重击";
    setSkillName(Skill_0, 0);
    Skill_1 = "惩戒";
    setSkillName(Skill_1, 1);
    Skill_2 = "痛苦压制";
    setSkillName(Skill_2, 2);
    Skill_3 = "魂·勇毅佑灵";
    setSkillName(Skill_3, 3);

    Dscp_0 = "进行一次打击，造成攻击力*1的伤害";
    setSkillDetail(Dscp_0, 0);
    Dscp_1 = "造成自身等级*3的固定伤害（无视防御）";
    setSkillDetail(Dscp_1, 1);
    Dscp_2 = "永久降低敌方自身等级*1的攻击力";
    setSkillDetail(Dscp_2, 2);
    Dscp_3 = "自身防御值翻倍，然后造成自身防御值加攻击力的伤害";
    setSkillDetail(Dscp_3, 3);

    setPP(5, 0);
    setPP(3, 1);
    setPP(1, 2);
}

template <>
bool
    Race<7>::attack(Monster &attacker, Monster &aim, int SkillIndex) const {
        int dmg = randDmg(3);

        switch (SkillIndex) {
            case 1:
                dmg += attacker.get_Level() * 3;

                return aim.takeDamage(dmg);
            case 2:
                aim.changeDefense(-aim.get_Level());

                break;
            case 3:
                attacker.changeDefense(attacker.get_Battle_Defense());
                dmg += attacker.get_Battle_Defense() + \
                            attacker.get_Battle_Attack() - aim.get_Battle_Defense();

                return aim.takeDamage(dmg);
            default:
                dmg += attacker.get_Battle_Attack() - \
                            aim.get_Battle_Defense();

                return aim.takeDamage(dmg);
        }

        return false;
    }

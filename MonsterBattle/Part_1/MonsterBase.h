//
// Created by LarFii on 2020/7/29.
//

#ifndef MAIN_MONSTERBASE_H
#define MAIN_MONSTERBASE_H

#ifdef _WIN32
    #pragma execution_character_set("utf-8")
#endif

#include <ctime>
#include <cmath>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

// get set update 三合一宏
#define GET(_type_, _property_) \
    inline _type_ get_##_property_() const { \
        return m_##_property_; } \

#define SET(_type_, _property_) \
    inline void set_##_property_(const _type_ &var) { \
        m_##_property_ = var; } \

#define UPDATE(_property_) \
    inline void update_##_property_(const int &amt) { \
        m_##_property_ += amt; } \

#define PROPERTY(_type_, _property_name_) \
    GET(_type_, _property_name_) \
    SET(_type_, _property_name_) \
    UPDATE(_property_name_) \

#define random(n) rand() % (n)

#define FUNCTION
#define RESOURCE

class Monster;

enum MonsterType {
    HHP,        // 肉盾型
    SPE,        // 速度型
    ATK,        // 攻击型
    DEF,        // 防御型
};

class MonsterBase {
public:
    MonsterBase(MonsterType type);
    virtual
        ~MonsterBase() = default;

public FUNCTION:
    GET(string, Name);          // 获取种族名
    SET(string, Name);          // 设置种族名
    GET(MonsterType, Race);      // 获取种族
    SET(MonsterType, Race);      // 设置种族

    // get set update 三合一
    PROPERTY(int, HP);
    PROPERTY(int, Speed);
    PROPERTY(int, Attack);
    PROPERTY(int, Defense);

public FUNCTION:
    // get
    string
        getSkillName(int index)const;       // 获取技能名
    string
        getSkillDetail(int index)const;     // 获取技能详细信息
    int
        getPP(int index)const;              // 获取技能剩余使用次数
    int
        getUpCurve(int level)const;         // 获取升级所需经验

    // set
    void
        setSkillName(const string &Name, int index);        // 设置技能名
    void
        setSkillDetail(const string &Detail, int index);    // 设置技能详细信息
    void
        setPP(const int &num, int index);                   // 设置技能使用次数

    virtual bool
        attack(
            Monster& attacker,
            Monster& aim,
            int     SkillIndex = 0
        ) const = 0;

protected RESOURCE:
    MonsterType
        m_Race;              // 种族类型
    string
        m_Name;              // 种族名称
    int
        m_HP;                // 基础生命值
    int
        m_Speed;             // 基础速度
    int
        m_Attack;            // 基础攻击力
    int
        m_Defense;           // 基础防御力
    string
        m_Skills[4][2];      // 一个普通攻击三个特殊技能 第一行是技能名 第二行是技能效果
    int
        m_pp[3];             // 特殊技能使用次数
    int
        m_UpCurve[14] = { 50, 110, 180, 260, 350, 450, 560,
                          680, 710, 850, 1000, 1160, 1330, 1510};
                             // 升级所需经验
};

class Monster {
public:
    Monster(int RaceIndex, const string &NickName);
    Monster(const Monster &) = delete;
    ~Monster() {};

    static const
        MonsterBase *s_Races[8];

public FUNCTION:
    GET(string, NickName);      // 获取精灵昵称
    SET(string, NickName);      // 设置精灵昵称

    // get set update 三合一
    PROPERTY(int, Exp);
    PROPERTY(int, Level);
    PROPERTY(int, Potential);
    PROPERTY(int, Init_HP);
    PROPERTY(int, Init_Speed);
    PROPERTY(int, Init_Attack);
    PROPERTY(int, Init_Defense);
    PROPERTY(int, RaceIndex);
    PROPERTY(int, Battle_HP);
    PROPERTY(int, Battle_Speed);
    PROPERTY(int, Battle_Attack);
    PROPERTY(int, Battle_Defense);

public FUNCTION:
    // get
    // 获取精灵种族名
    string
        getRaceName() const { \
            return s_Races[m_RaceIndex]->get_Name(); };
    // 获取精灵技能名
    string
        getSkillName(int index) const { \
            return s_Races[m_RaceIndex]->getSkillName(index); };
    // 获取精灵技能详细信息
    string
        getSkillDetail(int index) const { \
            return s_Races[m_RaceIndex]->getSkillDetail(index); };
    // 获取精灵技能初始使用次数
    int
        getPP(int index) const { \
            return s_Races[m_RaceIndex]->getPP(index); };
    // 获取精灵种族
    string
        getRaceType() const;
    // 获取精灵技能剩余使用次数
    int
        getBattlePP(int index) const;

    // set
    void
        setBattlePP(int index);     // 设置精灵技能剩余使用次数

    // battle相关
    void
        Recover();                  // 恢复初始状态
    bool
        takeExp(int amt);           // 获取经验值
    bool
        attack(Monster &aim, bool autoBattle = true);     // 攻击函数
    bool
        takeDamage(int n);          // 造成伤害，最小伤害不小于1
    void
        changePP(int index);        // 战斗中精灵技能剩余使用次数变化
    bool
        changeHP(int count);        // 战斗中精灵生命值变化
    void
        changeSpeed(int count);     // 战斗中精灵速度变化
    void
        changeAttack(int count);    // 战斗中精灵攻击力变化
    void
        changeDefense(int count);   // 战斗中精灵防御力变化

    // print
    void
        printINFO() const;                // 打印基本信息
    void
        pNeedExp() const;                 // 打印升级所需经验

    Monster
        &operator=(const Monster &) = delete;

private RESOURCE:
    int
        m_RaceIndex;        // 种族编号
    string
        m_NickName;         // 昵称
    int
        m_Exp;              // 经验
    int
        m_Level;            // 等级
    int
        m_Potential;         // 潜力值
    int
        m_Init_HP;          // 初始生命值
    int
        m_Init_Speed;       // 初始速度
    int
        m_Init_Attack;      // 初始攻击力
    int
        m_Init_Defense;     // 初始防御力

    // 战斗中数值
    int
        m_Battle_HP;         // 战斗中生命值
    int
        m_Battle_Speed;      // 战斗中速度
    int
        m_Battle_Attack;     // 战斗中攻击力
    int
        m_Battle_Defense;    // 战斗中防御力
    int
        m_Battle_pp[3];      // 战斗中特殊技能次数
};

template <int N>
class Race : public MonsterBase {
public:
    Race();
    virtual
        ~Race() = default;

    bool
        attack(
            Monster &attacker,
            Monster &aim,
            int    SkillIndex = 0
        ) const;
};

// declaration
template <>
Race<0>::Race();
template <>
Race<1>::Race();
template <>
Race<2>::Race();
template <>
Race<3>::Race();
template <>
Race<4>::Race();
template <>
Race<5>::Race();
template <>
Race<6>::Race();
template <>
Race<7>::Race();

#endif //MAIN_MONSTERBASE_H

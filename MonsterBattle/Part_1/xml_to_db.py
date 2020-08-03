'''
@Description: xml_to_db
@Autor: LarFii
@LastEditTime: 2020-07-29 15:25:54
'''

import re
import os
import base64
import sqlite3

index = 1
conn = sqlite3.connect('database.db')
database = conn.cursor()

path = "E:/Workspace/面向对象程序设计/宠物小精灵/赛尔号精灵数据/赛尔号精灵数据.xml"

with open(path, "r", encoding="utf-8") as f:
    content = f.read()
    f.close()

    spiritList = re.findall("<item>.*?</item>", content, re.S)

    for i in spiritList:
        try:
            Name = re.findall("<名字>(.*?)</名字>", i, re.S)[0]
        except:
            pass
        try:
            HP = re.findall("<体力>(.*?)</体力>", i, re.S)[0]
        except:
            pass
        try:
            Speed = re.findall("<速度>(.*?)</速度>", i, re.S)[0]
        except:
            pass
        try:
            Attack1 = re.findall("<攻击>(.*?)</攻击>", i, re.S)[0]
            Attack2 = re.findall("<特攻>(.*?)</特攻>", i, re.S)[0]
            if int(Attack1) > int(Attack2):
                Attack = Attack1
            else:
                Attack = Attack2
        except:
            pass
        try:
            Defense1 = re.findall("<防御>(.*?)</防御>", i, re.S)[0]
            Defense2 = re.findall("<特防>(.*?)</特防>", i, re.S)[0]
            if int(Defense1) > int(Defense2):
                Defense = Defense1
            else:
                Defense = Defense2
        except:
            pass
        try:
            IMG = ""
            Img = re.findall("<图片_bitmap>(.*?)</图片_bitmap>", i, re.S)[0]
            with open("E:/Workspace/面向对象程序设计/宠物小精灵/{}".format(Img), "rb") as img:
                IMG = base64.b64encode(img.read())
        except:
            pass

        if index % 4 == 1:
            Race = "力量型"
        if index % 4 == 2:
            Race = "肉盾型"
        if index % 4 == 3:
            Race = "防御型"
        if index % 4 == 0:
            Race = "敏捷型"

        if IMG == "":
            pass
        else:
            database.execute("insert into Spirit (ID, Name, Attack, Defense, Speed, HP, Race, img) \
                        values (?, ?, ?, ?, ?, ?, ?, ?);", (index, Name, Attack, Defense, Speed, HP, Race,IMG)) 
            index = index + 1

conn.commit()
conn.close()
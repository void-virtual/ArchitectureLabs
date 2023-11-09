import random
import string
import random

from faker import Faker

import basemod


fake = Faker()

def generate_user_to_chat_list(users, chats, n=1):
    def generate_user_to_chat():
        index = random.randrange(0, len(chats))
        chat = chats[index]
        return {
            "chat_id": index,
            "user_id": random.choice(users)["uuid"],
        }

    return [generate_user_to_chat() for _ in range(n)]

def insert_user_to_chats(fake_user_to_chats):
    connection = None

    connection = basemod.MySQLConnection(
        host="proxysql",
        port="6033",
        database="archdb",
        user="stud",
        password="stud"
    )

    connection.execute("""CREATE TABLE IF NOT EXISTS `UserToChat` (
        `chat_id` INT NOT NULL,
        `user_id` VARCHAR(256) NOT NULL,
        PRIMARY KEY(chat_id,user_id)); -- sharding:0""")

    connection = basemod.MySQLConnection(
        host="proxysql",
        port="6033",
        database="archdb",
        user="stud",
        password="stud"
    )

    connection.insert_values(("INSERT INTO `UserToChat` "
        "(`chat_id`, `user_id`) "
        "VALUES (%(chat_id)s, %(user_id)s)"), fake_user_to_chats, lambda x : 0)


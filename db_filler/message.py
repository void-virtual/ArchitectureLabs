import random
import string

from faker import Faker

import basemod


fake = Faker()

def generate_message_list(user_to_chats, n=1):
    def generate_message():
        user_to_chat = random.choice(user_to_chats)
        user = user_to_chat["user_id"]
        chat = user_to_chat["chat_id"]
        return {
            "user_id": user,
            "chat_id": chat,
            "message": ' '.join(fake.words(nb=5))
        }

    return [generate_message() for _ in range(n)]

def insert_messages(fake_messages):
    connection = None

    connection = basemod.MySQLConnection(
        host="proxysql",
        port="6033",
        database="archdb",
        user="stud",
        password="stud"
    )

    connection.execute(f"""CREATE TABLE IF NOT EXISTS `Message` (
        `id` INT NOT NULL AUTO_INCREMENT,
        `chat_id` INT NOT NULL,
        `user_id` VARCHAR(256) NOT NULL,
        `message` VARCHAR(1024) NOT NULL,
        PRIMARY KEY (`id`)); -- sharding:0""")

    connection = basemod.MySQLConnection(
        host="proxysql",
        port="6033",
        database="archdb",
        user="stud",
        password="stud"
    )


    connection.insert_values(("INSERT INTO `Message` "
        "(`user_id`, `chat_id`, `message`) "
        "VALUES (%(user_id)s, %(chat_id)s, %(message)s)"), fake_messages, lambda x : 0)

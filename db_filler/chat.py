import random
import string
import random

from faker import Faker

import basemod


fake = Faker()

def generate_chat_list(users, n=1):
    def generate_product():
        return {
            "name": ' '.join(fake.words(nb=3)),
            "creator_id": random.choice(users)["uuid"],
        }

    return [generate_product() for _ in range(n)]

def insert_chats(fake_chats):
    connection = None


    connection = basemod.MySQLConnection(
        host="proxysql",
        port="6033",
        database="archdb",
        user="stud",
        password="stud"
    )

    connection.execute("""CREATE TABLE IF NOT EXISTS `Chat` (
        `id` INT NOT NULL AUTO_INCREMENT,
        `name` VARCHAR(1024) NOT NULL,
        `creator_id` VARCHAR(256) NOT NULL,
        PRIMARY KEY (`id`)); -- sharding:0""")

    connection = basemod.MySQLConnection(
        host="proxysql",
        port="6033",
        database="archdb",
        user="stud",
        password="stud"
    )

    connection.insert_values(("INSERT INTO `Chat` "
        "(`name`, `creator_id`) "
        "VALUES (%(name)s, %(creator_id)s)"), fake_chats, lambda x : 0)


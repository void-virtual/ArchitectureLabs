import random
import string
import uuid
import subprocess

from faker import Faker

import basemod


fake = Faker()

def get_uid(login):
    seed = uuid.uuid3(uuid.UUID(int=0), "User")
    return str(uuid.uuid3(seed, login))

def generate_password(length, alphabet=[
            *string.ascii_lowercase, *string.ascii_uppercase,
            '-', '_', '.', '@', '&']
        ):
    return ''.join([random.choice(alphabet) for _ in range(length)])

def generate_profile_list(n=1):
    genders = {'M': 'male', 'F': 'female'}
    def generate_profile():
        faker_profile = fake.simple_profile()
        first_name, last_name = faker_profile['name'].split()[-2:]
        return {
            'uuid': str(get_uid(faker_profile['username'])),
            'first_name': first_name,
            'last_name': last_name,
            'email': faker_profile['mail'],
            'login': faker_profile['username'],
            'password': generate_password(30),
            'title': genders[faker_profile['sex']]
        }

    return [generate_profile() for _ in range(n)]

def insert_users(fake_users):
    connection = None

    for i in range(basemod.SHARD_COUNT):
        connection = basemod.MySQLConnection(
            host="proxysql",
            port="6033",
            database="archdb",
            user="stud",
            password="stud"
        )

        connection.execute(f"""CREATE TABLE IF NOT EXISTS `User` (
            `id` INT NOT NULL AUTO_INCREMENT,
            `uuid` VARCHAR(256) NOT NULL,
            `first_name` VARCHAR(256) NOT NULL,
            `last_name` VARCHAR(256) NOT NULL,
            `login` VARCHAR(256) NOT NULL,
            `password` VARCHAR(256) NOT NULL,
            `email` VARCHAR(256) NULL,
            `title` VARCHAR(1024) NULL,
            PRIMARY KEY (`id`),KEY `uuid_key` (`uuid`),KEY `fn` (`first_name`),KEY `ln` (`last_name`)
            ); {basemod.get_hint(i)}""")

    connection = basemod.MySQLConnection(
        host="proxysql",
        port="6033",
        database="archdb",
        user="stud",
        password="stud"
    )


    connection.insert_values(("INSERT INTO `User` "
        "(`uuid`, `first_name`, `last_name`, `login`, `password`, `email`, `title`) "
        "VALUES (%(uuid)s, %(first_name)s, %(last_name)s, %(login)s, %(password)s, %(email)s, %(title)s)"), fake_users, lambda x : basemod.get_hash(x['uuid']))


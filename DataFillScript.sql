-- Для тестирования подойдут пользователи с name1, name2, name3


-- MUST BE EXECUTED ON 1 SHARD


CREATE TABLE IF NOT EXISTS `User` (`id` INT NOT NULL AUTO_INCREMENT,
                        `first_name` VARCHAR(256) NOT NULL,
                        `last_name` VARCHAR(256) NOT NULL,
                        `login` VARCHAR(256) NOT NULL,
                        `password` VARCHAR(256) NOT NULL,
                        `email` VARCHAR(256) NULL,
                        `title` VARCHAR(1024) NULL,
                        PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`))
                        -- sharding:0
                        ; 

CREATE TABLE IF NOT EXISTS `Chat` (`id` INT NOT NULL AUTO_INCREMENT,
                        `name` VARCHAR(1024) NOT NULL,
                        `creator_id` INT NOT NULL,
                        PRIMARY KEY (`id`))-- sharding:0
                        ; 


CREATE TABLE IF NOT EXISTS `Message` (`id` INT NOT NULL AUTO_INCREMENT,
                        `chat_id` INT NOT NULL,
                        `user_id` INT NOT NULL,
                        `message` VARCHAR(1024) NOT NULL,
                        PRIMARY KEY (`id`))-- sharding:0
;


CREATE TABLE IF NOT EXISTS `UserToChat` (`chat_id` INT NOT NULL,
                        `user_id` INT NOT NULL,
                        PRIMARY KEY(chat_id,user_id)-- sharding:0
                        ; 
  
                                                          

insert into User(first_name, last_name, login, password, email, title) values ('name1', 'name1', 'name1', 'name1', 'name1@email.com','name1')-- sharding:0
;
insert into User(first_name, last_name, login, password, email, title) values ('name4', 'name4', 'name4', 'name4', 'name4@email.com','name4')-- sharding:0
;
insert into User(first_name, last_name, login, password, email, title) values ('name5', 'name5', 'name5', 'name5', 'name5@email.com','name5')-- sharding:0
;
insert into User(first_name, last_name, login, password, email, title) values ('name6', 'name6', 'name6', 'name6', 'name6@email.com','name6')-- sharding:0
;
insert into User(first_name, last_name, login, password, email, title) values ('name8', 'name8', 'name8', 'name8', 'name8@email.com','name8')-- sharding:0
;
insert into User(first_name, last_name, login, password, email, title) values ('name9', 'name9', 'name9', 'name9', 'name9@email.com','name9')-- sharding:0
;
insert into User(first_name, last_name, login, password, email, title) values ('name11', 'name11', 'name11', 'name11', 'name11@email.com','name11')-- sharding:0
;
insert into User(first_name, last_name, login, password, email, title) values ('name12', 'name12', 'name12', 'name12', 'name12@email.com','name12')-- sharding:0
;
insert into User(first_name, last_name, login, password, email, title) values ('name14', 'name14', 'name14', 'name14', 'name14@email.com','name14')-- sharding:0
;
insert into User(first_name, last_name, login, password, email, title) values ('name15', 'name15', 'name15', 'name15', 'name15@email.com','name15')-- sharding:0
;

insert into Chat(name, creator_id) values ("chat1", 1)-- sharding:0
;
insert into Chat(name, creator_id) values ("chat2", 2)-- sharding:0
;
insert into Chat(name, creator_id) values ("chat3", 3)-- sharding:0
;
insert into Chat(name, creator_id) values ("chat4", 4)-- sharding:0
;
insert into Chat(name, creator_id) values ("chat5", 5)-- sharding:0
;
insert into Chat(name, creator_id) values ("chat6", 6)-- sharding:0
;
insert into Chat(name, creator_id) values ("chat7", 7)-- sharding:0
;
insert into Chat(name, creator_id) values ("chat8", 8)-- sharding:0
;
insert into Chat(name, creator_id) values ("chat9", 9)-- sharding:0
;
insert into Chat(name, creator_id) values ("chat10", 10)-- sharding:0
;
insert into Chat(name, creator_id) values ("chat11", 11)-- sharding:0
;
insert into Chat(name, creator_id) values ("chat12", 12)-- sharding:0
;

insert into Message(chat_id, user_id, message) values (1, 1, "message1")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (1, 2, "message2")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (1, 3, "message3")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (2, 1, "message1")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (2, 2, "message2")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (2, 3, "message3")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (3, 1, "message1")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (3, 2, "message2")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (3, 3, "message3")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (4, 1, "message1")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (4, 2, "message2")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (4, 3, "message3")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (5, 1, "message1")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (5, 2, "message2")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (5, 3, "message3")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (6, 1, "message1")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (6, 2, "message2")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (6, 3, "message3")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (7, 1, "message1")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (7, 2, "message2")-- sharding:0
;
insert into Message(chat_id, user_id, message) values (7, 3, "message3")-- sharding:0
;


insert into UserToChat(chat_id, user_id) values(1, 1)-- sharding:0
;
insert into UserToChat(chat_id, user_id) values(1, 2)-- sharding:0
;
insert into UserToChat(chat_id, user_id) values(1, 3)-- sharding:0
;
insert into UserToChat(chat_id, user_id) values(2, 1)-- sharding:0
;
insert into UserToChat(chat_id, user_id) values(2, 2)-- sharding:0
;
insert into UserToChat(chat_id, user_id) values(2, 3)-- sharding:0
;
insert into UserToChat(chat_id, user_id) values(3, 1)-- sharding:0
;
insert into UserToChat(chat_id, user_id) values(3, 2)-- sharding:0
;
insert into UserToChat(chat_id, user_id) values(3, 3)-- sharding:0
;
insert into UserToChat(chat_id, user_id) values(4, 1)-- sharding:0
;
insert into UserToChat(chat_id, user_id) values(4, 2)-- sharding:0
;
insert into UserToChat(chat_id, user_id) values(4, 3)-- sharding:0
;
insert into UserToChat(chat_id, user_id) values(5, 1)-- sharding:0
;
insert into UserToChat(chat_id, user_id) values(5, 2)-- sharding:0
;
insert into UserToChat(chat_id, user_id) values(6, 3)-- sharding:0
;


-- MUST BE EXECUTED ON 2 SHARD

-- CREATE TABLE IF NOT EXISTS `User` (`id` INT NOT NULL AUTO_INCREMENT,
--                         `first_name` VARCHAR(256) NOT NULL,
--                         `last_name` VARCHAR(256) NOT NULL,
--                         `login` VARCHAR(256) NOT NULL,
--                         `password` VARCHAR(256) NOT NULL,
--                         `email` VARCHAR(256) NULL,
--                         `title` VARCHAR(1024) NULL,
--                         PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`))
--                         -- sharding:1
--                         ; 



-- insert into User(first_name, last_name, login, password, email, title) values ('name2', 'name2', 'name2', 'name2', 'name2@email.com','name2')-- sharding:1
-- ;
-- insert into User(first_name, last_name, login, password, email, title) values ('name3', 'name3', 'name3', 'name3', 'name3@email.com','name3')-- sharding:1
-- ;
-- insert into User(first_name, last_name, login, password, email, title) values ('name7', 'name7', 'name7', 'name7', 'name7@email.com','name7')-- sharding:1
-- ;
-- insert into User(first_name, last_name, login, password, email, title) values ('name10', 'name10', 'name10', 'name10', 'name10@email.com','name10')-- sharding:1
-- ;
-- insert into User(first_name, last_name, login, password, email, title) values ('name13', 'name13', 'name13', 'name13', 'name13@email.com','name13')-- sharding:1
-- ;
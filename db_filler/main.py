from user import generate_profile_list, insert_users
from chat import generate_chat_list, insert_chats
from user_to_chat import generate_user_to_chat_list, insert_user_to_chats
from message import generate_message_list, insert_messages


def main():
    user_profiles = generate_profile_list(200)
    insert_users(user_profiles)
    chats = generate_chat_list(user_profiles, 50)
    insert_chats(chats)
    user_to_chats = generate_user_to_chat_list(user_profiles, chats, 20)
    insert_user_to_chats(user_to_chats)
    messages = generate_message_list(user_to_chats, 100)
    insert_messages(messages)


if __name__ == "__main__":
    main()
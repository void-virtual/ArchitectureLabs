#include <iostream>
#include <csignal>
#include <cppkafka/consumer.h>
#include <cppkafka/configuration.h>

#include "../config/config.h"
#include "../database/user.h"


bool running = true;

int main()
{
    try
    {
        // Stop processing on SIGINT
        signal(SIGINT, [](int)
               { running = false; });

        // Construct the configuration
        cppkafka::Configuration config = {
            {"metadata.broker.list", Config::get().get_queue_host()},
            {"group.id", Config::get().get_queue_group_id()},
            // Disable auto commit
            {"enable.auto.commit", false}};

        // Create the consumer
        cppkafka::Consumer consumer(config);

        // Print the assigned partitions on assignment
        consumer.set_assignment_callback([](const cppkafka::TopicPartitionList &partitions)
                                         { std::cout << "Got assigned: " << partitions << std::endl; });

        // Print the revoked partitions on revocation
        consumer.set_revocation_callback([](const cppkafka::TopicPartitionList &partitions)
                                         { std::cout << "Got revoked: " << partitions << std::endl; });

        // Subscribe to the topic
        consumer.subscribe({Config::get().get_queue_topic()});

        std::cout << "Consuming messages from topic " << Config::get().get_queue_topic() << std::endl;

        // Now read lines and write them into kafka
        while (running)
        {
            // Try to consume a message
            cppkafka::Message msg = consumer.poll();
            if (msg)
            {
                // If we managed to get a message
                if (msg.get_error())
                {
                    // Ignore EOF notifications from rdkafka
                    if (!msg.is_eof())
                    {
                        std::cout << "[+] Received error notification: " << msg.get_error() << std::endl;
                    }
                    else {
                        std::cout << "[-] Found an EOF notification" << std::endl;
                    }
                }
                else
                {
                    std::cout << "[-] Got a message" << std::endl;
                    // Print the key (if any)
                    if (msg.get_key())
                    {
                        std::cout << msg.get_key() << " -> ";
                    }
                    // Print the payload
                    std::string payload = msg.get_payload();
                    std::cout << msg.get_payload() << std::endl;
                    database::User u = database::User::fromJSON(payload);
                    u.save_to_mysql();

                    // Now commit the message
                    consumer.commit(msg);
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 1;
}
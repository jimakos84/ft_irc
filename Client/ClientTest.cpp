
#include <iostream>
#include <vector>
#include "Client.hpp" 

// Simulates what the server would do when data arrives from the network
void feed(Client& c, const std::string& data)
{
    c.appendtoClientBuffer(data);

    while (c.completeMessageExists() == true)
    {
        std::string msg = c.extractMessage();
        std::cout << "\n\n[PARSED] " << msg << std::endl;

        //send extracted msg to Server, who then processes the cmd
    }
}

int main()
{
    // Create a fake client connection
    Client client(4, "127.0.0.1");

    // Simulate network packets arriving in weird chunks
    feed(client, "PASS hunter2\r\nN");
    feed(client, "ICK alice\r\nUSER alice 0 * :Alice Smi");
    feed(client, "th\r\nJOIN #42\r\n");
    feed(client, "PRIVMSG #42 :Hello world!\r\n");

    std::cout << "\nSimulation finished.\n";
}

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main() {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;
    int count = 0;
    
    std::cout << "Starting Program" << std::endl;

    portno = 30000;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error opening socket" << std::endl;
        return 1;
    }
    
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }
    
    std::cout << "Listening" << std::endl;

    listen(sockfd, 5);
    
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
        std::cerr << "Error accepting connection" << std::endl;
        return 1;
    }
    
    std::cout << "Connected to " << inet_ntoa(cli_addr.sin_addr) << std::endl;

    try {
        memset(buffer, 0, sizeof(buffer));
        ssize_t n = read(newsockfd, buffer, sizeof(buffer) - 1);
        if (n < 0) {
            std::cerr << "Error reading from socket" << std::endl;
            return 1;
        }

        std::cout << buffer << std::endl;
        count++;
        std::cout << "The count is: " << count << std::endl;
        usleep(500000);
        std::cout << std::endl;
        usleep(500000);

        std::string firstMessage = "(1.1)";
        ssize_t m1 = write(newsockfd, firstMessage.c_str(), firstMessage.length());
        if (m1 < 0) {
            std::cerr << "Error writing first message to socket" << std::endl;
            return 1;
        }
        std::cout << "Sent 1.1" << std::endl;

        std::string secondMessage = "(100.0,100.0,0.0,0.0,0.0,0.0)";
        ssize_t m2 = write(newsockfd, secondMessage.c_str(), secondMessage.length());
        if (m2 < 0) {
            std::cerr << "Error writing second message to socket" << std::endl;
            return 1;
        }
        std::cout << "Sent (100.0,100.0,0.0,0.0,0.0,0.0)" << std::endl;

        // Wait for client to be ready
        memset(buffer, 0, sizeof(buffer));
        ssize_t n3 = read(newsockfd, buffer, sizeof(buffer) - 1);
        if (n3 < 0) {
            std::cerr << "Error reading from socket" << std::endl;
            return 1;
        }

        std::cout << "Received acknowledgment from the client: " << buffer << std::endl;

        std::string thirdMessage = "(2.2)";
        ssize_t m3 = write(newsockfd, thirdMessage.c_str(), thirdMessage.length());
        if (m3 < 0) {
            std::cerr << "Error writing third message to socket" << std::endl;
            return 1;
        }
        std::cout << "Sent 2.2" << std::endl;


        std::string fourthMessage = "(0.0,0.0,0.0,0.0,0.0,0.0)";
        ssize_t m4 = write(newsockfd, fourthMessage.c_str(), fourthMessage.length());
        if (m4 < 0) {
            std::cerr << "Error writing fourth message to socket" << std::endl;
            return 1;
        }
        std::cout << "Sent (0.0,0.0,0.0,0.0,0.0,0.0)" << std::endl;
        
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    
    close(newsockfd);
    close(sockfd);
    
    std::cout << "Program finish" << std::endl;

    return 0;
}


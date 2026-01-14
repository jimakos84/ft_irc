
#include "ParentCommand.hpp"

class TestCmd : public ParentCommand {
    private:
        std::string _test_msg;

    public:
        TestCmd();
        ~TestCmd(); 
        bool cmdNeedsRegistration() const override;
        void executeCmd(Server &server, Client &client, const std::vector<std::string> cmdParams);
}
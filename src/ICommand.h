#ifndef ICOMMAND_H
#define ICOMMAND_H

class ICommand {
    public:
    virtual void execute(std::vector<std::string>) = 0;
    virtual void printcommand() = 0;
}; 

#endif 
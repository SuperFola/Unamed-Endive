#ifndef DEF_TRIGGERSMGR
#define DEF_TRIGGERSMGR

#include <map>
#include <string>

class TriggersManager
{
private:
    const std::string notrigger = "";
    std::map<int, std::map<int, std::string>> triggs;

public:
    TriggersManager();
    void add_trigger(int, int, std::string);
    std::string get_trigg(int, int);
    bool is_notrigger(const std::string&);
};

#endif // DEF_TRIGGERSMGR

#ifndef __JUNK_AGENT__H
#define __JUNK_AGENT__H 

#include "enviro.h"

using namespace enviro;

class JunkController : public Process, public AgentInterface {

    public:
    JunkController() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {
        
    }
    void stop() {}

};

class Junk : public Agent {
    public:
    Junk(json spec, World& world) : Agent(spec, world) {}
    void start() {
       add_process(c);
    }
    private:
    JunkController c;
};


DECLARE_INTERFACE(Junk)

#endif
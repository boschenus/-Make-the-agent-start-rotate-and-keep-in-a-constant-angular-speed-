#ifndef __JUNK_TR_AGENT__H
#define __JUNK__TR_AGENT__H 

#include "enviro.h"

using namespace enviro;

class JunkTrController : public Process, public AgentInterface {

    public:
    JunkTrController() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() {
        
    }
    void stop() {}

};

class JunkTr : public Agent {
    public:
    JunkTr(json spec, World& world) : Agent(spec, world) {}
    void start() {
       add_process(c);
    }
    private:
    JunkTrController c;
};


DECLARE_INTERFACE(JunkTr)

#endif
#ifndef __CLEANER_A_AGENT__H
#define __CLEANER_A_AGENT__H 

#include <vector>
#include <cmath>
#include <iostream>
#include "enviro.h"

using namespace enviro;

/**The Controller of red Cleaner, will serach the junk and take it to the red side*/
class CleanerAController : public Process, public AgentInterface {

    public:
    CleanerAController() : Process(), AgentInterface() {}

    void init() {
        junk_id = -1;
        goal_x = 0;
        goal_y = 0;
        search = true;
        stuck = false;
        go = false;
        adjust = false;
        get = false;
        deliver = false;
        stop_r = false;
        square = true;
        
        watch("ignore_a", [this](Event e){
            ignore_id = e.value()["id"];
        });

        watch("screen_click", [this](Event e) {
            if (square) {
                Agent& new_thing = add_agent("Junk", e.value()["x"], e.value()["y"], 0, {{"fill", "orange"}});
            } else {
                Agent& new_thing = add_agent("JunkTr", e.value()["x"], e.value()["y"], 0, {{"fill", "orange"}});
            }
            
        });

        watch("keydown", [&](Event& e) {
            std::string k = e.value()["key"];
            if ( k == "s" ) {
                square = !square;
            }
        });

        notice_collisions_with("Junk", [&](Event &e) {
            if ( (junk_id == -1 || (e.value()["id"] != junk_id && !agent_exists(junk_id) && e.value()["id"] != ignore_id)) && !get) {
                junk_id = e.value()["id"];
                if (agent_exists(junk_id)) {
                    Agent& junk = find_agent(junk_id);
                    attach_to(junk);
                    get = true;
                    go = false;
                    adjust = true;
                    goal_x = -335;
                    goal_y = y();
                    emit(Event("ignore_b", { 
                        { "id", junk_id }
                    }));
                }
            }
            if (e.value()["id"] == ignore_id) {
                stuck = true;
                search = true;
            }
        });
         notice_collisions_with("JunkTr", [&](Event &e) {
            if ( (junk_id == -1 || (e.value()["id"] != junk_id && !agent_exists(junk_id) && e.value()["id"] != ignore_id)) && !get) {
                junk_id = e.value()["id"];
                if (agent_exists(junk_id)) {
                    Agent& junk = find_agent(junk_id);
                    attach_to(junk);
                    get = true;
                    go = false;
                    adjust = true;
                    goal_x = -335;
                    goal_y = y();
                    emit(Event("ignore_b", { 
                        { "id", junk_id }
                    }));
                }
            }
            if (e.value()["id"] == ignore_id) {
                stuck = true;
                search = true;
            }
        });
    }
    void start() {}
    void update() {
        v_a = angular_velocity();
        v_x = velocity().x;
        v_y = velocity().y;
        p_x = x();
        p_y = y();

        if (search == true) {
            go = false;
            adjust = false;
            deliver = false;
            get = false;
            rotate();
            
            if (sensor_reflection_type(0) == "Junk" || sensor_reflection_type(0) == "JunkTr") {
                goal_x = position().x + cos(angle()) * 1.2 * sensor_value(0);
                goal_y = position().y + sin(angle()) * 1.2 * sensor_value(0);
                search = false;
                go = true;
            }
            if (stuck == true) {
                if (p_x > 0 && p_y > 0) {
                    goal_x = p_x - 55;
                    goal_y = p_y - 50;
                } else if (p_x > 0 && p_y < 0) {
                    goal_x = p_x - 50;
                    goal_y = p_y + 50;
                } else if (p_x < 0 && p_y > 0) {
                    goal_x = p_x + 50;
                    goal_y = p_y - 50;
                } else {
                    goal_x = p_x + 50;
                    goal_y = p_y + 50;
                }
                apply_force(0, 1000);
            }

            
            //std::cout << v_a << std::endl;
        }
        if (go == true) {
            omni_move_toward(goal_x, goal_y, 0.5);
            if(!get && (fabs(p_x - goal_x) < 2 && fabs(p_y - goal_y) < 2)) {
                search = true;
            }
            if (stuck) {
                goal_x = -200;
                goal_y = 0;
            }
        }
        if (adjust == true) {
            if (!stop_r) {
                rotate();
            }
            if (agent_exists(junk_id)) {
                Agent& junk = find_agent(junk_id);
                if (p_x > junk.x() && fabs(p_y - junk.y()) <= 8) {
                    stop_r = true;
                    rotate_stop();
                }
                if (stop_r) {
                    rotate_stop();
                    if (fabs(v_a) < 0.002 && v_x < 0.01 && v_y < 0.01) {
                        adjust = false;
                        deliver = true;
                        stop_r = false;
                    }
                }
            } 
        }
        if (deliver == true) {
            adjust = false;
            if (stuck == true) {
                omni_move_toward(0, 0, 0.5);
                adjust = true;
                deliver = false;
            }
            omni_move_toward(goal_x, goal_y, 0.35);
        }
        if (agent_exists(junk_id)) {
            Agent& junk = find_agent(junk_id);
            if (junk.x() < -325 || junk.x() > 325) {
                remove_agent(junk_id);
                get = false;
                search = true;
            }
        }
        
        if(!agent_exists(junk_id)) {
                deliver = false;
                adjust = false;
        }

        if(fabs(v_a) < 0.002 && v_x < 0.001 && v_y < 0.001 && (p_x <= -320 || p_x >=320 || p_y <= -180 || p_y >= 180)){
            stuck = true;
        } else {
            stuck = false;
        }

        if (!deliver && !go && !adjust && !search && stuck) {
            search = true;
            rotate();
            stuck = false;
        }

        // std::cout << "d:" << deliver << std::endl;
        // std::cout << "s:" << search << std::endl;
        // std::cout << "a:" << adjust << std::endl;
        // std::cout << "st:" << stuck << std::endl;
        // std::cout << "g:" << go << std::endl;
        // std::cout << "sr:" << stop_r << std::endl;
        // std::cout << v_a << std::endl;
    }
    void stop() {}

    /**Make the agent start rotate and keep in a constant angular speed*/
    void rotate() {
        if (v_a <= 0.1 && v_a >=0) {
            apply_force(0, 50);
        }else if (v_a >= 0.5) {
            apply_force(0, -50);
        }
        if (v_a <= -0.5) {
            apply_force(0, -50);
        } else if (v_a >= -0.1 && v_a <= 0) {
            apply_force(0, 50);
        }
        return;
    }

    /**Make the agent stop rotate*/
    void rotate_stop() {
        if (v_a >= 0.001) {
            apply_force(0, -5500 * v_a);
        } else if (v_a > 0 && v_a < 0.0001) {
            apply_force(0, 55000 * v_a);
        }
        if (v_a <= -0.001) {
            apply_force(0, -5500 * v_a);
        } else if (v_a < 0 && v_a < -0.0001) {
            apply_force(0, 55000 * v_a);
        }
    }
    
    double goal_x, goal_y, junk_x, junk_y, v_a, v_x, v_y, p_x, p_y, d_x, d_y;
    bool get, stop_r, search, stuck, deliver, adjust, go, square;
    int junk_id, ignore_id;
};
class CleanerA : public Agent {
    public:
    CleanerA(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    CleanerAController c;
};

DECLARE_INTERFACE(CleanerA)

#endif

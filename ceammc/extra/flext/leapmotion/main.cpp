#include "Dispatcher.h"
#include "Leap.h"
#include "LeapMath.h"
#include <flext.h>

// check for appropriate flext version
#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif

class leapmotion : public flext_base {
    FLEXT_HEADER(leapmotion, flext_base)

    // Create a sample listener and controller
    Dispatcher dispatcher;
    Controller controller;

    int general_flag;

    int hands_sphere_radius_flag;
    int hands_sphere_center_flag;

    int hands_direction_flag;
    int hands_palm_normal_flag;
    int hands_palm_position_flag;
    int hands_palm_velocity_flag;

    int hands_tool_count_flag;
    int hands_finger_count_flag;

    int tools_direction_flag;
    int tools_position_flag;
    int tools_velocity_flag;
    int tools_size_flag;

    int fingers_direction_flag;
    int fingers_position_flag;
    int fingers_velocity_flag;
    int fingers_size_flag;
    int gestures_flag;

public:
    leapmotion()
    {
        AddInAnything();
        AddOutList(); // general info
        AddOutList(); // hands, fingers, tools
        AddOutList(); // gesture info

        // register methods
        FLEXT_ADDMETHOD(0, m_bang);
        FLEXT_ADDMETHOD_(0, "general", m_general);
        FLEXT_ADDMETHOD_(0, "hands_direction", m_hands_direction);
        FLEXT_ADDMETHOD_(0, "hands_palm_normal", m_hands_palm_normal);
        FLEXT_ADDMETHOD_(0, "hands_palm_position", m_hands_palm_position);
        FLEXT_ADDMETHOD_(0, "hands_palm_velocity", m_hands_palm_velocity);

        FLEXT_ADDMETHOD_(0, "hands_sphere_center", m_hands_sphere_center);
        FLEXT_ADDMETHOD_(0, "hands_sphere_radius", m_hands_sphere_radius);

        FLEXT_ADDMETHOD_(0, "hands_finger_count", m_hands_finger_count);
        FLEXT_ADDMETHOD_(0, "hands_tool_count", m_hands_tool_count);

        FLEXT_ADDMETHOD_(0, "tools_direction", m_tools_direction);
        FLEXT_ADDMETHOD_(0, "tools_position", m_tools_position);
        FLEXT_ADDMETHOD_(0, "tools_velocity", m_tools_velocity);
        FLEXT_ADDMETHOD_(0, "tools_size", m_tools_size);

        FLEXT_ADDMETHOD_(0, "fingers_direction", m_fingers_direction);
        FLEXT_ADDMETHOD_(0, "fingers_position", m_fingers_position);
        FLEXT_ADDMETHOD_(0, "fingers_velocity", m_fingers_velocity);
        FLEXT_ADDMETHOD_(0, "fingers_size", m_fingers_size);
        FLEXT_ADDMETHOD_(0, "gestures", m_gestures);
        FLEXT_ADDMETHOD_(0, "info", m_info);

        // Have the sample listener receive events from the controller
        controller.addListener(dispatcher);
        general_flag = true;

        hands_direction_flag = false;
        hands_palm_position_flag = true;
        hands_palm_velocity_flag = false;
        hands_palm_normal_flag = false;

        hands_sphere_center_flag = false;
        hands_sphere_radius_flag = false;

        hands_finger_count_flag = false;
        hands_tool_count_flag = false;

        tools_direction_flag = false;
        tools_position_flag = false;
        tools_velocity_flag = false;
        tools_size_flag = false;

        fingers_direction_flag = false;
        fingers_position_flag = false;
        fingers_velocity_flag = false;
        fingers_size_flag = false;
        gestures_flag = false;
    }
    ~leapmotion()
    {
        // Remove the sample listener when done
        controller.removeListener(dispatcher);
    }

    void m_bang()
    {

        Frame frame;
        t_atom generalInfo[6];
        int num_hands, num_tools, num_gestures;
        frame = dispatcher.frame;
        num_hands = frame.hands().count();
        num_tools = frame.tools().count();
        num_gestures = frame.gestures().count();

        if (general_flag) {
            SETFLOAT(&generalInfo[0], (float)frame.id());
            SETFLOAT(&generalInfo[1], (float)frame.timestamp());
            SETFLOAT(&generalInfo[2], (float)num_hands);
            SETFLOAT(&generalInfo[3], (float)frame.fingers().count());
            SETFLOAT(&generalInfo[4], (float)frame.tools().count());
            SETFLOAT(&generalInfo[5], (float)frame.gestures().count());
            ToOutList(0, 6, generalInfo);
        }
        // tools
        for (int i = 0; i < num_tools; i++) {
            Tool tool;
            tool = frame.tools()[i];

            t_atom toolInfo[5];
            if (tools_position_flag) {
                SETFLOAT(&toolInfo[0], i);
                SETSYMBOL(&toolInfo[1], gensym("direction"));
                SETFLOAT(&toolInfo[2], tool.direction().x);
                SETFLOAT(&toolInfo[3], tool.direction().y);
                SETFLOAT(&toolInfo[4], tool.direction().z);
                ToOutAnything(1, gensym("tool"), 5, toolInfo);
            }
            if (tools_position_flag) {
                SETFLOAT(&toolInfo[0], i);
                SETSYMBOL(&toolInfo[1], gensym("position"));
                SETFLOAT(&toolInfo[2], tool.tipPosition().x);
                SETFLOAT(&toolInfo[3], tool.tipPosition().y);
                SETFLOAT(&toolInfo[4], tool.tipPosition().z);
                ToOutAnything(1, gensym("tool"), 5, toolInfo);
            }
            if (tools_velocity_flag) {
                SETFLOAT(&toolInfo[0], i);
                SETSYMBOL(&toolInfo[1], gensym("velocity"));
                SETFLOAT(&toolInfo[2], tool.tipVelocity().x);
                SETFLOAT(&toolInfo[3], tool.tipVelocity().y);
                SETFLOAT(&toolInfo[4], tool.tipVelocity().z);
                ToOutAnything(1, gensym("tool"), 5, toolInfo);
            }
            if (tools_size_flag) {
                SETFLOAT(&toolInfo[0], i);
                SETSYMBOL(&toolInfo[1], gensym("size"));
                SETFLOAT(&toolInfo[2], tool.width());
                SETFLOAT(&toolInfo[3], tool.length());
                ToOutAnything(1, gensym("tool"), 4, toolInfo);
            }
        }
        // hands and fingers
        for (int i = 0; i < num_hands; i++) {
            Hand hand;
            hand = frame.hands()[i];
            int num_fingers = hand.fingers().count();
            int num_tools = hand.tools().count();
            t_atom handInfo[5];

            if (hands_direction_flag) {
                // direction
                SETFLOAT(&handInfo[0], i);
                SETSYMBOL(&handInfo[1], gensym("direction"));
                SETFLOAT(&handInfo[2], hand.direction().x);
                SETFLOAT(&handInfo[3], hand.direction().y);
                SETFLOAT(&handInfo[4], hand.direction().z);
                ToOutAnything(1, gensym("hand"), 5, handInfo);
            }
            if (hands_palm_position_flag) {
                // position
                SETFLOAT(&handInfo[0], i);
                SETSYMBOL(&handInfo[1], gensym("palm_position"));
                SETFLOAT(&handInfo[2], hand.palmPosition().x);
                SETFLOAT(&handInfo[3], hand.palmPosition().y);
                SETFLOAT(&handInfo[4], hand.palmPosition().z);
                ToOutAnything(1, gensym("hand"), 5, handInfo);
            }
            if (hands_palm_velocity_flag) {
                // velocity
                SETFLOAT(&handInfo[0], i);
                SETSYMBOL(&handInfo[1], gensym("palm_velocity"));
                SETFLOAT(&handInfo[2], hand.palmVelocity().x);
                SETFLOAT(&handInfo[3], hand.palmVelocity().y);
                SETFLOAT(&handInfo[4], hand.palmVelocity().z);
                ToOutAnything(1, gensym("hand"), 5, handInfo);
            }
            if (hands_palm_normal_flag) {
                // normal
                SETFLOAT(&handInfo[0], i);
                SETSYMBOL(&handInfo[1], gensym("palm_normal"));
                SETFLOAT(&handInfo[2], hand.palmVelocity().x);
                SETFLOAT(&handInfo[3], hand.palmVelocity().y);
                SETFLOAT(&handInfo[4], hand.palmVelocity().z);
                ToOutAnything(1, gensym("hand"), 5, handInfo);
            }
            if (hands_sphere_radius_flag) {
                // sphere radius
                SETFLOAT(&handInfo[0], i);
                SETSYMBOL(&handInfo[1], gensym("sphere_radius"));
                SETFLOAT(&handInfo[2], hand.sphereRadius());
                ToOutAnything(1, gensym("hand"), 3, handInfo);
            }
            if (hands_sphere_center_flag) {
                // sphere center
                SETFLOAT(&handInfo[0], i);
                SETSYMBOL(&handInfo[1], gensym("sphere_center"));
                SETFLOAT(&handInfo[2], hand.sphereCenter().x);
                SETFLOAT(&handInfo[3], hand.sphereCenter().y);
                SETFLOAT(&handInfo[4], hand.sphereCenter().z);
                ToOutAnything(1, gensym("hand"), 5, handInfo);
            }
            if (hands_finger_count_flag) {
                // finger count
                SETFLOAT(&handInfo[0], i);
                SETSYMBOL(&handInfo[1], gensym("finger_count"));
                SETFLOAT(&handInfo[2], num_fingers);
                ToOutAnything(1, gensym("hand"), 3, handInfo);
            }
            if (hands_tool_count_flag) {
                // tool count
                SETFLOAT(&handInfo[0], i);
                SETSYMBOL(&handInfo[1], gensym("tool_count"));
                SETFLOAT(&handInfo[2], num_tools);
                ToOutAnything(1, gensym("hand"), 3, handInfo);
            }
            for (int j = 0; j < num_fingers; j++) {
                Finger finger;
                finger = hand.fingers()[j];
                t_atom fingerInfo[7];
                if (fingers_direction_flag) {
                    SETFLOAT(&fingerInfo[0], i); // index
                    SETSYMBOL(&fingerInfo[1], gensym("fingers"));
                    SETFLOAT(&fingerInfo[2], j);
                    SETSYMBOL(&fingerInfo[3], gensym("direction"));
                    SETFLOAT(&fingerInfo[4], finger.direction().x);
                    SETFLOAT(&fingerInfo[5], finger.direction().y);
                    SETFLOAT(&fingerInfo[6], finger.direction().z);
                    ToOutAnything(1, gensym("hand"), 7, fingerInfo);
                }
                if (fingers_position_flag) {
                    SETFLOAT(&fingerInfo[0], i); // index
                    SETSYMBOL(&fingerInfo[1], gensym("fingers"));
                    SETFLOAT(&fingerInfo[2], j);
                    SETSYMBOL(&fingerInfo[3], gensym("position"));
                    SETFLOAT(&fingerInfo[4], finger.tipPosition().x);
                    SETFLOAT(&fingerInfo[5], finger.tipPosition().y);
                    SETFLOAT(&fingerInfo[6], finger.tipPosition().z);
                    ToOutAnything(1, gensym("hand"), 7, fingerInfo);
                }
                if (fingers_velocity_flag) {
                    SETFLOAT(&fingerInfo[0], i); // index
                    SETSYMBOL(&fingerInfo[1], gensym("fingers"));
                    SETFLOAT(&fingerInfo[2], j);
                    SETSYMBOL(&fingerInfo[3], gensym("velocity"));
                    SETFLOAT(&fingerInfo[4], finger.tipVelocity().x);
                    SETFLOAT(&fingerInfo[5], finger.tipVelocity().y);
                    SETFLOAT(&fingerInfo[6], finger.tipVelocity().z);
                    ToOutAnything(1, gensym("hand"), 7, fingerInfo);
                }
                if (fingers_size_flag) {
                    SETFLOAT(&fingerInfo[0], i); // index
                    SETSYMBOL(&fingerInfo[1], gensym("fingers"));
                    SETFLOAT(&fingerInfo[2], j);
                    SETSYMBOL(&fingerInfo[3], gensym("size"));
                    SETFLOAT(&fingerInfo[4], finger.width());
                    SETFLOAT(&fingerInfo[5], finger.length());
                    ToOutAnything(1, gensym("hand"), 6, fingerInfo);
                }
            }
        }
        t_atom gestureCountInfo[1];
        for (int i = 0; i < num_gestures; i++) {
            Gesture gesture;
            gesture = frame.gestures()[i];
            //type
            t_atom gestureTypeInfo[3];
            SETFLOAT(&gestureTypeInfo[0], i);
            SETSYMBOL(&gestureTypeInfo[1], gensym("type"));
            switch (gesture.type()) {
            case Gesture::TYPE_INVALID:
                SETSYMBOL(&gestureTypeInfo[2], gensym("TYPE_INVALID"));
                break;
            case Gesture::TYPE_SWIPE:
                SETSYMBOL(&gestureTypeInfo[2], gensym("TYPE_SWIPE"));
                break;
            case Gesture::TYPE_CIRCLE:
                SETSYMBOL(&gestureTypeInfo[2], gensym("TYPE_CIRCLE"));
                break;
            case Gesture::TYPE_SCREEN_TAP:
                SETSYMBOL(&gestureTypeInfo[2], gensym("TYPE_SCREEN_TAP"));
                break;
            case Gesture::TYPE_KEY_TAP:
                SETSYMBOL(&gestureTypeInfo[2], gensym("TYPE_KEY_TAP"));
                break;
            }
            ToOutList(2, 3, gestureTypeInfo);

            //state
            t_atom gestureStateInfo[3];
            SETFLOAT(&gestureStateInfo[0], i);
            SETSYMBOL(&gestureStateInfo[1], gensym("state"));
            switch (gesture.state()) {
            case Gesture::STATE_INVALID:
                SETSYMBOL(&gestureStateInfo[2], gensym("STATE_INVALID"));
                break;
            case Gesture::STATE_START:
                SETSYMBOL(&gestureStateInfo[2], gensym("TYPE_START"));
                break;
            case Gesture::STATE_UPDATE:
                SETSYMBOL(&gestureStateInfo[2], gensym("STATE_UPDATE"));
                break;
            case Gesture::STATE_STOP:
                SETSYMBOL(&gestureStateInfo[2], gensym("TYPE_STOP"));
                break;
            }
            ToOutList(2, 3, gestureStateInfo);

            t_atom gestureDurationInfo[3];
            SETFLOAT(&gestureDurationInfo[0], i);
            SETSYMBOL(&gestureDurationInfo[1], gensym("duration"));
            SETFLOAT(&gestureDurationInfo[2], gesture.duration());
            ToOutList(2, 3, gestureDurationInfo);

            t_atom gestureIdInfo[3];
            SETFLOAT(&gestureIdInfo[0], i);
            SETSYMBOL(&gestureIdInfo[1], gensym("id"));
            SETFLOAT(&gestureIdInfo[2], gesture.id());
            ToOutList(2, 3, gestureIdInfo);
        }
    }
    void m_general(int s)
    {
        general_flag = s;
    }
    void m_hands_direction(int s)
    {
        hands_direction_flag = s;
    }
    void m_hands_palm_position(int s)
    {
        hands_palm_position_flag = s;
    }
    void m_hands_palm_velocity(int s)
    {
        hands_palm_velocity_flag = s;
    }
    void m_hands_palm_normal(int s)
    {
        hands_palm_normal_flag = s;
    }
    void m_hands_finger_count(int s)
    {
        hands_finger_count_flag = s;
    }
    void m_hands_tool_count(int s)
    {
        hands_tool_count_flag = s;
    }
    void m_hands_sphere_radius(int s)
    {
        hands_sphere_radius_flag = s;
    }
    void m_hands_sphere_center(int s)
    {
        hands_sphere_center_flag = s;
    }
    void m_tools_direction(int s)
    {
        tools_direction_flag = s;
    }
    void m_tools_position(int s)
    {
        tools_position_flag = s;
    }
    void m_tools_velocity(int s)
    {
        tools_velocity_flag = s;
    }
    void m_tools_size(int s)
    {
        tools_size_flag = s;
    }
    void m_fingers_direction(int s)
    {
        fingers_direction_flag = s;
    }
    void m_fingers_position(int s)
    {
        fingers_position_flag = s;
    }
    void m_fingers_velocity(int s)
    {
        fingers_velocity_flag = s;
    }
    void m_fingers_size(int s)
    {
        fingers_size_flag = s;
    }
    void m_gestures(int argc, const t_atom* argv)
    {
        if (argc > 2) {
            post("leapmotion: gestures too many arguments");
        } else if (argc < 2) {
            post("leapmotion: gestures too few arguments");
        } else {
            if ((argv[0].a_type != A_SYMBOL) || (argv[1].a_type != A_FLOAT)) {
                post("leapmotion: gestures wrong arguments");
                return;
            }
            char* type = argv[0].a_w.w_symbol->s_name;
            float s = argv[1].a_w.w_float;

            if (strcmp(type, "TYPE_CIRCLE") == 0) {
                if (s == 0.0) {
                    controller.enableGesture(Gesture::TYPE_CIRCLE, false);
                } else {
                    controller.enableGesture(Gesture::TYPE_CIRCLE, true);
                }
            }
            if (strcmp(type, "TYPE_SWIPE") == 0) {
                if (s == 0.0) {
                    controller.enableGesture(Gesture::TYPE_SWIPE, false);
                } else {
                    controller.enableGesture(Gesture::TYPE_SWIPE, true);
                }
            }
            if (strcmp(type, "TYPE_KEY_TAP") == 0) {
                if (s == 0.0) {
                    controller.enableGesture(Gesture::TYPE_KEY_TAP, false);
                } else {
                    controller.enableGesture(Gesture::TYPE_KEY_TAP, true);
                }
            }
            if (strcmp(type, "TYPE_SCREEN_TAP") == 0) {
                if (s == 0.0) {
                    controller.enableGesture(Gesture::TYPE_SCREEN_TAP, false);
                } else {
                    controller.enableGesture(Gesture::TYPE_SCREEN_TAP, true);
                }
            }
        }
    }
    void m_info()
    {
        post("-LEAP SETTING-");
        post("General Info:%s", flag_to_string(general_flag));

        post("-Hands-");
        post("direction:%s", flag_to_string(hands_direction_flag));
        post("palm position:%s", flag_to_string(hands_palm_position_flag));
        post("palm velocity:%s", flag_to_string(hands_palm_velocity_flag));
        post("palm normal:%s", flag_to_string(hands_palm_normal_flag));

        post("finger count:%s", flag_to_string(hands_finger_count_flag));
        post("tool count:%s", flag_to_string(hands_tool_count_flag));
        post("sphere radius:%s", flag_to_string(hands_sphere_radius_flag));
        post("sphere center:%s", flag_to_string(hands_sphere_center_flag));

        post("-Tools-");
        post("direction:%s", flag_to_string(tools_direction_flag));
        post("position:%s", flag_to_string(tools_position_flag));
        post("velocity:%s", flag_to_string(tools_velocity_flag));
        post("size:%s", flag_to_string(tools_size_flag));

        post("-Fingers-");
        post("direction:%s", flag_to_string(fingers_direction_flag));
        post("position:%s", flag_to_string(fingers_position_flag));
        post("velocity:%s", flag_to_string(fingers_velocity_flag));
        post("size:%s", flag_to_string(fingers_size_flag));

        post("-Gestures-");
        post("TYPE_CIRCLE:%s", flag_to_string(controller.isGestureEnabled(Gesture::TYPE_CIRCLE)));
        post("TYPE_SWIPE:%s", flag_to_string(controller.isGestureEnabled(Gesture::TYPE_SWIPE)));
        post("TYPE_KEY_TAP:%s", flag_to_string(controller.isGestureEnabled(Gesture::TYPE_KEY_TAP)));
        post("TYPE_SCREEN_TAP:%s", flag_to_string(controller.isGestureEnabled(Gesture::TYPE_SCREEN_TAP)));
    }

private:
    const char* flag_to_string(int flag)
    {
        if (flag) {
            return "on";
        } else {
            return "off";
        }
    }
    FLEXT_CALLBACK(m_info)
    FLEXT_CALLBACK(m_bang)
    FLEXT_CALLBACK_I(m_general)
    FLEXT_CALLBACK_I(m_hands_direction)
    FLEXT_CALLBACK_I(m_hands_palm_position)
    FLEXT_CALLBACK_I(m_hands_palm_velocity)
    FLEXT_CALLBACK_I(m_hands_palm_normal)

    FLEXT_CALLBACK_I(m_hands_sphere_radius)
    FLEXT_CALLBACK_I(m_hands_sphere_center)
    FLEXT_CALLBACK_I(m_hands_finger_count)
    FLEXT_CALLBACK_I(m_hands_tool_count)

    FLEXT_CALLBACK_I(m_tools_direction)
    FLEXT_CALLBACK_I(m_tools_position)
    FLEXT_CALLBACK_I(m_tools_velocity)
    FLEXT_CALLBACK_I(m_tools_size)
    FLEXT_CALLBACK_I(m_fingers_direction)
    FLEXT_CALLBACK_I(m_fingers_position)
    FLEXT_CALLBACK_I(m_fingers_velocity)
    FLEXT_CALLBACK_I(m_fingers_size)
    FLEXT_CALLBACK_V(m_gestures)
};

FLEXT_NEW("leapmotion", leapmotion)

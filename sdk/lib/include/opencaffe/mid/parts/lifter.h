#ifndef _OPENCAFFE_MID_PARTS_LIFTER_H
#define _OPENCAFFE_MID_PARTS_LIFTER_H

#include "opencaffe/base/base.h"
#include "opencaffe/base/devices/motor.h"

namespace OpenCaffe {

class Lifter : public Base{
public:
    enum class Type{
        Stepper = 0
    };
    enum class Position {
        Bottom = 0,
        Top,
        Unknown
    };
    enum class Process {
        Stop = 0,
        MovingUp,
        MovingDown,
        Error
    };
    class InputDevice;

    Lifter(Type type, std::shared_ptr<OpenCaffeObject> &oco);
    ~Lifter();
    
    int init();
    int main();
    int deinit();

    int go_up();
    int go_down();
    int stop();
    Process get_status();
    Position get_position();

private:
    int check_fault();
    std::unique_ptr<MotorDevice> motor_;
    std::unique_ptr<InputDevice> overvoltage_;
    std::unique_ptr<InputDevice> ctrl_error_;
};

} //namespace OpenCaffe

#endif //_OPENCAFFE_MID_PARTS_LIFTER_H
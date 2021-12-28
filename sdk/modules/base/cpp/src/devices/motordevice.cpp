#include "opencaffe/sdk/base/devices/motordevice.h"

namespace OpenCaffe {

inline MotorDevice::MotorPhase &operator++(MotorDevice::MotorPhase &p) {
    switch (p) {
    case MotorDevice::MotorPhase::E_MID_MTR_PHASE_0:
        p = MotorDevice::MotorPhase::E_MID_MTR_PHASE_1;
    case MotorDevice::MotorPhase::E_MID_MTR_PHASE_1:
        p = MotorDevice::MotorPhase::E_MID_MTR_PHASE_2;
    case MotorDevice::MotorPhase::E_MID_MTR_PHASE_2:
        p = MotorDevice::MotorPhase::E_MID_MTR_PHASE_3;
    case MotorDevice::MotorPhase::E_MID_MTR_PHASE_3:
        p = MotorDevice::MotorPhase::E_MID_MTR_PHASE_0;
    }
    return p;
}

inline MotorDevice::MotorPhase &operator--(MotorDevice::MotorPhase &p) {
    switch (p) {
    case MotorDevice::MotorPhase::E_MID_MTR_PHASE_0:
        p = MotorDevice::MotorPhase::E_MID_MTR_PHASE_3;
    case MotorDevice::MotorPhase::E_MID_MTR_PHASE_1:
        p = MotorDevice::MotorPhase::E_MID_MTR_PHASE_0;
    case MotorDevice::MotorPhase::E_MID_MTR_PHASE_2:
        p = MotorDevice::MotorPhase::E_MID_MTR_PHASE_1;
    case MotorDevice::MotorPhase::E_MID_MTR_PHASE_3:
        p = MotorDevice::MotorPhase::E_MID_MTR_PHASE_2;
    }
    return p;
}

MotorDevice::MotorDevice(MotorType type, uint8_t id) : Device(id), type_(type) {
    dir_   = MotorDir::Stop;
    power_ = MotorPower::None;
    phase_ = MotorPhase::E_MID_MTR_PHASE_0;
}

void MotorDevice::set_direction(const MotorDir &dir) {
    dir_ = dir;
}

const MotorDevice::MotorDir &MotorDevice::get_direction() {
    return dir_;
}

void MotorDevice::set_power(const MotorDevice::MotorPower &pow) {
    power_ = pow;
}

const MotorDevice::MotorPower &MotorDevice::get_power() {
    return power_;
}

int MotorDevice::update() {
    if (execute_move_() == 0) {
        if (fptr_(get_id(), calc_value()) == 0) {
            set_status(Status::OK);
            return 0;
        }
    }
    set_status(Status::Error);
    return 1;
}

uint8_t MotorDevice::calc_value() {
    uint8_t value = 0;
    switch (type_) {
    case MotorType::STEPPER_MOTOR:
        value = (uint8_t)phase_ | (uint8_t)power_ << 2;
        break;
    case MotorType::HBRIDGE_MOTOR:
        value = (uint8_t)dir_;
        break;
    }
    return value;
}

int MotorDevice::execute_move_() {
    int res = 0;

    switch (type_) {
    case MotorType::STEPPER_MOTOR:
        res = set_step_phase_();
        break;
    case MotorType::HBRIDGE_MOTOR:
        break;
    default:
        res = -1;
        break;
    }

    return res;
}

int MotorDevice::set_step_phase_() {
    int res = 0;

    switch (dir_) {
    case MotorDir::Stop:
        phase_ = MotorPhase::E_MID_MTR_PHASE_0;
        break;
    case MotorDir::Forward:
        ++phase_;
        break;
    case MotorDir::Backward:
        --phase_;
        break;
    }
    return res;
}

} // namespace OpenCaffe
use log::error;
use rsruckig::prelude::*;

use crate::gpio::trajectory_planner::hw_trajectory_result::Finished;

#[derive(Default, Debug)]
pub struct LogErrorHandler;

impl RuckigErrorHandler for LogErrorHandler {
    fn handle_validation_error(message: &str) -> Result<(), RuckigError> {
        error!("Validation error: {}", message);
        Ok(())
    }

    fn handle_calculator_error(message: &str) -> Result<(), RuckigError> {
        error!("Calculator error: {}", message);
        Ok(())
    }
}

type TrajPlanner = Ruckig<1, LogErrorHandler>;
type Input = InputParameter<1>;
type Output = OutputParameter<1>;

#[derive(Debug)]
pub struct hw_trajectory {
    otg: TrajPlanner,
    input: Input,
    output: Output,
    last_result: hw_trajectory_result,
}

#[repr(C)]
#[derive(Debug, PartialEq, Clone, Copy)]
pub enum hw_trajectory_result {
    Working,
    Finished,
    Error,
    NullPtr,
}

impl hw_trajectory {
    fn new(delta_time_ms: f64) -> Self {
        hw_trajectory {
            otg: TrajPlanner::new(None, delta_time_ms * 0.001),
            input: Input::new(None),
            output: Output::new(None),
            last_result: hw_trajectory_result::Finished,
        }
    }

    fn current_input(&self, pos: &mut f64, vel: &mut f64, accel: &mut f64) {
        *pos = self.input.current_position[0];
        *vel = self.input.current_velocity[0];
        *accel = self.input.current_acceleration[0];
    }

    fn new_output(&self, pos: &mut f64, vel: &mut f64, accel: &mut f64, jerk: &mut f64) {
        *pos = self.output.new_position[0];
        *vel = self.output.new_velocity[0];
        *accel =  self.output.new_acceleration[0];
        *jerk =  self.output.new_jerk[0];
    }

    fn set_target_pos(&mut self, pos: f64) {
        if self.last_result == Finished {
            self.output.time = 0.0;
        }

        self.input.target_position[0] = pos;
        self.input.target_velocity[0] = 0.0;
    }

    fn set_limits(&mut self, vel: f64, accel: f64, jerk: f64) {
        self.input.max_velocity[0] = vel;
        self.input.max_acceleration[0] = accel;
        self.input.max_jerk[0] = jerk;
    }

    fn current_pos(&self) -> f64 {
        self.input.current_position[0]
    }

    fn target_pos(&self) -> f64 {
        self.input.target_position[0]
    }

    fn new_pos(&self) -> f64 {
        self.output.new_position[0]
    }

    fn update(&mut self) -> hw_trajectory_result {
        self.last_result = match self.otg.update(&self.input, &mut self.output) {
            Ok(res) => match res {
                RuckigResult::Working => {
                    self.output.pass_to_input(&mut self.input);
                    hw_trajectory_result::Working
                }
                RuckigResult::Finished => hw_trajectory_result::Finished,
                _ => hw_trajectory_result::Error,
            },
            Err(err) => {
                log::error!("{err}");
                hw_trajectory_result::Error
            }
        };

        self.last_result
    }
}

/// create trajectory struct
/// @param delta_time_ms - calculation step in milliseconds
#[no_mangle]
pub extern "C" fn ceammc_hw_trajectory_new(delta_time_ms: f64) -> *mut hw_trajectory {
    Box::into_raw(Box::new(hw_trajectory::new(delta_time_ms)))
}

/// free trajectory struct
/// @param traj - pointer to trajectory struct
#[no_mangle]
pub extern "C" fn ceammc_hw_trajectory_free(traj: *mut hw_trajectory) {
    if !traj.is_null() {
        drop(unsafe { Box::from_raw(traj) })
    }
}

/// set trajectory max velocity
/// @param traj - pointer to trajectory struct
/// @param vel - maximum velocity
#[no_mangle]
pub extern "C" fn ceammc_hw_trajectory_set_max_velocity(traj: *mut hw_trajectory, vel: f64) -> bool {
    if traj.is_null() {
        false
    } else {
        let traj = unsafe { &mut *traj };
        traj.input.max_velocity[0] = vel;
        true
    }
}

/// set trajectory max accel
/// @param traj - pointer to trajectory struct
/// @param acc - maximum accelration
#[no_mangle]
pub extern "C" fn ceammc_hw_trajectory_set_max_accel(traj: *mut hw_trajectory, acc: f64) -> bool {
    if traj.is_null() {
        false
    } else {
        let traj = unsafe { &mut *traj };
        traj.input.max_acceleration[0] = acc;
        true
    }
}

/// set trajectory max jerk
/// @param traj - pointer to trajectory struct
/// @param jerk - maximum jerk
#[no_mangle]
pub extern "C" fn ceammc_hw_trajectory_set_max_jerk(traj: *mut hw_trajectory, jerk: f64) -> bool {
    if traj.is_null() {
        false
    } else {
        let traj = unsafe { &mut *traj };
        traj.input.max_jerk[0] = jerk;
        true
    }
}

/// set trajectory target position
/// @param traj - pointer to trajectory struct
/// @param pos - target position
#[no_mangle]
pub extern "C" fn ceammc_hw_trajectory_set_target_pos(traj: *mut hw_trajectory, pos: f64) -> bool {
    if traj.is_null() {
        false
    } else {
        let traj = unsafe { &mut *traj };
        traj.set_target_pos(pos);
        true
    }
}

/// update trajectory (calculate next step)
/// @param traj - pointer to trajectory struct
#[no_mangle]
pub extern "C" fn ceammc_hw_trajectory_update(traj: *mut hw_trajectory) -> hw_trajectory_result {
    if traj.is_null() {
        hw_trajectory_result::NullPtr
    } else {
        unsafe { &mut *traj }.update()
    }
}

/// get current trajectory input values
/// @param traj - pointer to trajectory struct
/// @param pos - write current input position at this address (not NULL!)
/// @param vel - writes current input velocity at this address (not NULL!)
/// @param accel - writes current input acceleration at this address (not NULL!)
#[no_mangle]
pub extern "C" fn ceammc_hw_trajectory_current_input(
    traj: *mut hw_trajectory,
    pos: &mut f64,
    vel: &mut f64,
    accel: &mut f64,
) -> bool {
    if traj.is_null() {
        true
    } else {
        let traj = unsafe { &*traj };
        traj.current_input(pos, vel, accel);
        true
    }
}

/// get trajectory output new values
/// @param traj - pointer to trajectory struct
/// @param pos - write new output position at this address (not NULL!)
/// @param vel - writes new output velocity at this address (not NULL!)
/// @param accel - writes new output acceleration at this address (not NULL!)
/// @param jerk - writes new output jerk at this address (not NULL!)
#[no_mangle]
pub extern "C" fn ceammc_hw_trajectory_new_output(
    traj: *mut hw_trajectory,
    pos: &mut f64,
    vel: &mut f64,
    accel: &mut f64,
    jerk: &mut f64,
) -> bool {
    if traj.is_null() {
        true
    } else {
        let traj = unsafe { &*traj };
        traj.new_output(pos, vel, accel, jerk);
        true
    }
}

#[cfg(test)]
mod tests {
    use crate::gpio::trajectory_planner::{hw_trajectory, hw_trajectory_result, LogErrorHandler};
    use approx::{assert_abs_diff_eq, assert_relative_eq};
    use rsruckig::{
        input_parameter::InputParameter, output_parameter::OutputParameter, result::RuckigResult, ruckig::Ruckig,
    };

    #[test]
    fn full_test() {
        let mut t = hw_trajectory::new(10.0);
        t.set_target_pos(10.0);
        assert_eq!(t.current_pos(), 0.0);
        assert_eq!(t.target_pos(), 10.0);
        assert_eq!(t.new_pos(), 0.0);
        t.set_limits(1.0, 2.0, 3.0);
        while t.update() == hw_trajectory_result::Working {}

        assert_eq!(t.update(), hw_trajectory_result::Finished);
        assert_abs_diff_eq!(t.current_pos(), 10.0, epsilon = 1e-6);
        t.set_target_pos(5.0);
        t.set_limits(1.0, 2.0, 3.0);
        while t.update() == hw_trajectory_result::Working {}
        assert_abs_diff_eq!(t.current_pos(), 5.0, epsilon = 1e-6);
    }
}

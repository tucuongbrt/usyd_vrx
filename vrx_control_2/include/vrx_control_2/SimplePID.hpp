#pragma once

#include <chrono>
#include <iostream>

namespace usyd_vrx {

class SimplePID
{
  public:
    //! Enumerator for defining error calculation type.
    enum ERROR_TYPE {ERROR_STANDARD, ERROR_CIRCULAR};

    /*!
    * Constructor.
    * @param Kp the proportional term gain.
    * @param Ki the integral term gain.
    * @param Kd the derivative term gain.
    * @param max_integral the maximum value the error integral can 
    *   accumulate to.
    * @param error takes enum value to specify type of error, either
    *   standard or circular (wrapping error from -PI to PI).
    */
    SimplePID(float Kp, float Ki, float Kd, float max_integral, 
      ERROR_TYPE error=ERROR_STANDARD);

    /*!
    * Updates PID controller with a new setpoint.
    * @param setpoint the new target value of the controlled variable.
    */
    void setSetpoint(double setpoint);

    /*!
    * Updates PID controller with a new observation.
    * @param observation the observed value of the controlled variable.
    */
    void setObservation(double observation);

    /*!
    * Calculates control signal based on setpoint, observation and PID gains.
    * @return new control signal; the PID controller output.
    */
    double getControlSignal();

    /*!
    * Resets PID controller private variables.
    */
    void resetPID();

    /*!
    * Get the current error calculation.
    * @return the current error between setpoint and observation.
    */
    double getError();

  private:
    //! Proportional gain.
    float Kp_;

    //! Integral gain.
    float Ki_;

    //! Derivative gain.
    float Kd_;

    //! Previous error term.
    double prev_error_;

    //! Integral of the error term.
    double error_integral_;

    //! Time at which previous control signal was calculated.
    std::chrono::time_point<std::chrono::high_resolution_clock> time_prev_;

    //! Maximum allowed value of integral of error term.
    float max_integral_;

    //! PID setpoint; the target value of the controlled variable.
    double setpoint_;

    //! Observed value of the controlled variable.
    double observation_;

    //! Function pointer to error calculation function.
    double (SimplePID::* error_function_) ();

    /*!
    * Updates integral term by accumulating error.
    * @param error the current error between setpoint and observation.
    */
    void accumulate(double error, double dt);

    /*!
    * Calculates error term simply by subtracting observation from setpoint.
    */
    double errorStandard();

    /*!
    * Calculates error term, assuming values are between -PI and PI and wrap.
    */
    double errorCircular();
};

}
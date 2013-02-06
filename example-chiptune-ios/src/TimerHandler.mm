//
//  TimerHandler.mm
//
//

#include "TimerHandler.h"

//--------------------------------------------------------------
// static methods
//--------------------------------------------------------------
TimerHandler& TimerHandler::getInstance() {
  static TimerHandler instance;
  return instance;
}

//--------------------------------------------------------------
// constractor
//--------------------------------------------------------------
TimerHandler::TimerHandler()
    : next_step_time_(0), timer_(0) {
  step_ = kMinutesInMicroSeconds / kDefaultBPM * kDefaultBeat /
      kDefaultResolution;
  next_step_time_ = step_;
}

//--------------------------------------------------------------
// public methods
//--------------------------------------------------------------
void TimerHandler::init(const Poco::AbstractTimerCallback &callback) {
  if (!timer_) {
    timer_ = new Poco::Timer(0, 10); // delay of 10 milliseconds
    timer_->start(callback, Poco::Thread::PRIO_HIGHEST);
  }
  stop_watch_.restart();
}

void TimerHandler::deallocate() {
  stop_watch_.stop();
  delete timer_;
}

bool TimerHandler::isNextStepTime() const {
  return stop_watch_.elapsed() >= next_step_time_;
}

void TimerHandler::incrementNextStepTime() {
  next_step_time_ += step_;
}

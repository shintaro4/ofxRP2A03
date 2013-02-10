//
//  TimerHandler.h
//
//

#pragma once

#include "Poco/Stopwatch.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"
#include "Poco/Timer.h"

using pTimeDiff = Poco::Timestamp::TimeDiff;

class TimerHandler {

public:

  static TimerHandler& getInstance();
    
  void init(const Poco::AbstractTimerCallback& callback);
  void deallocate();
  bool isNextStepTime() const;
  void incrementNextStepTime();
  
private:

  static const u_int32_t kMinutesInMicroSeconds = 60000000;
  static const u_int32_t kMinutesInMiliSeconds  =    60000;
  static const u_int32_t kSecondsInMicroSeconds =  1000000;

  static const int kDefaultBPM  =      100;
  static const int kDefaultBeat =        4;
  static const int kDefaultResolution = 16;
  
  TimerHandler();
  TimerHandler(const TimerHandler&);
    
  Poco::Timer* timer_;
  Poco::Stopwatch stop_watch_;
  int step_;
  pTimeDiff next_step_time_;

};

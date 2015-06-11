#pragma once


/**
 * Struct containing entries representing the average load of the system over
 * various time intervals.
 */
struct LoadAverage {
  // The average load of the system over the last one minute.
  double one_min;

  // The average load of the system over the last five minutes.
  double five_mins;

  // The average load of the system over the last fifteen minutes.
  double fifteen_mins;
};

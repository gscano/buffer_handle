#ifndef BUFFER_HANDLE_DATE_HPP
#define BUFFER_HANDLE_DATE_HPP

#include <ctime> // struct tm

#include <buffer_handle/action.hpp> // action
#include <buffer_handle/align.hpp> // align
#include <buffer_handle/config.hpp> // config

namespace buffer_handle
{
  template<config Config, char InsteadOfALeadingZeroForDay, char Separator, bool YearOn4Digits, action Action,
	   typename Day, typename Month, typename Year>
  char * day_month_year(char * buffer, Day day, Month month, Year year);

  template<config Config, action Action,
	   typename Month, typename Day>
  char * month_day(char * buffer, Month month, Day day);

  template<config Config, action Action, typename Weekday>
  char * wkday(char * buffer, Weekday weekday);

  template<config Config, action Action, typename Weekday>
  char * weekday(char * buffer, Weekday weekday);

  template<config Config, action Action, typename Month>
  char * month(char * buffer, Month month);
  
  namespace asc
  {
    template<config Config, action Action,
	     typename Weekday, typename Day, typename Month, typename Year,
	     typename Hours, typename Minutes, typename Seconds>
    char * date(char * buffer,
		Weekday weekday, Day day, Month month, Year year,
		Hours hours, Minutes minutes, Seconds seconds);

    template<config Config, action Action>
    char * date(char * buffer, std::tm date_time);
  };

  namespace rfc822//§5
  {
    template<config Config, bool HandleWeekday, bool HandleSeconds, class Timezone, action Action,
	     typename Weekday, typename Day, typename Month, typename Year,
	     typename Hours, typename Minutes, typename Seconds>
    char * date(char * buffer,
		Weekday weekday, Day day, Month month, Year year,
		Hours hours, Minutes minutes, Seconds seconds,
		const Timezone & timezone);

    template<config Config, bool HandleWeekday, bool HandleSeconds, class Timezone, action Action>
    char * date(char * buffer, std::tm date_time, const Timezone & timezone);
  };

  namespace rfc850//§2.1.4
  {
    template<config Config, class Timezone, action Action,
	     typename Weekday, typename Day, typename Month, typename Year,
	     typename Hours, typename Minutes, typename Seconds>
    char * date(char * buffer,
		Weekday weekday, Day day, Month month, Year year,
		Hours hours, Minutes minutes, Seconds seconds,
		const Timezone & timezone);

    template<config Config, class Timezone, action Action>
    char * date(char * buffer, std::tm date_time, const Timezone & timezone);
  };

  namespace rfc1123//§5.2.14
  {
    template<config Config, bool HandleWeekday, bool HandleSeconds, class Timezone, action Action,
	     typename Weekday, typename Day, typename Month, typename Year,
	     typename Hours, typename Minutes, typename Seconds>
    char * date(char * buffer,
		Weekday weekday, Day day, Month month, Year year,
		Hours hours, Minutes minutes, Seconds seconds,
		const Timezone & timezone);

    template<config Config, bool HandleWeekday, bool HandleSeconds, class Timezone, action Action>
    char * date(char * buffer, std::tm date_time, const Timezone & timezone);
  };

  namespace rfc5322//§3.3
  {
    template<config Config, action Action,
	     typename Weekday, typename Day, typename Month, typename Year,
	     typename Hours, typename Minutes, typename Seconds,
	     typename TimezoneHours, typename TimezoneMinutes>
    char * date(char * buffer,
		Weekday weekday, Day day, Month month, Year year,
		Hours hours, Minutes minutes, Seconds seconds,
		bool timezone_sign, TimezoneHours timezone_hours, TimezoneMinutes timezone_minutes);

    template<config Config, action Action, typename TimezoneHours, typename TimezoneMinutes>
    char * date(char * buffer, std::tm date_time,
		bool timezone_sign, TimezoneHours timezone_hours, TimezoneMinutes timezone_minutes);
  };

  namespace rfc7231//§7.1.1.1
  {
    template<config Config, action Action,
	     typename Weekday, typename Day, typename Month, typename Year,
	     typename Hours, typename Minutes, typename Seconds>
    char * date(char * buffer,
		Weekday weekday, Day day, Month month, Year year,
		Hours hours, Minutes minutes, Seconds seconds);

    template<config Config, action Action>
    char * date(char * buffer, std::tm date_time);
  };
};

#include <buffer_handle/date.hcp>

#endif/*BUFFER_HANDLE_DATE_HPP*/

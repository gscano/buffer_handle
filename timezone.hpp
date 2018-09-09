#ifndef BUFFER_HANDLE_TIMEZONE_HPP
#define BUFFER_HANDLE_TIMEZONE_HPP

#include <cstdint> // uint8_t

#include <buffer_handle/action.hpp> // action
#include <buffer_handle/align.hpp> // align
#include <buffer_handle/config.hpp> // config

namespace buffer_handle
{
  enum class universal_timezone : uint8_t
  {
    UT,
      GMT
      };

  template<config Config, align Align, char Pad, action Action>
  char * universal_timezone(char * buffer, enum universal_timezone value);

  template<config Config, align Align, char Pad>
  struct universal_timezone_t
  {
  public:
    universal_timezone_t(enum universal_timezone value = universal_timezone::GMT);

  public:
    enum universal_timezone value;

  public:
    template<action Action>
    char * handle(char * buffer) const;
  };

  enum class north_american_timezone : uint8_t
  {
    EST, EDT,
      CST, CDT,
      MST, MDT,
      PST, PDT
      };

  template<config Config, action Action>
  char * north_american_timezone(char * buffer, enum north_american_timezone value);

  template<config Config>
  struct north_american_timezone_t
  {
  public:
    north_american_timezone_t(enum north_american_timezone value = north_american_timezone::EST);

  public:
    enum north_american_timezone value;

  public:
    template<action Action>
    char * handle(char * buffer) const;
  };

  template<config Config, action Action>
  char * military_timezone(char * buffer, char letter);

  template<config Config, action Action, typename T>
  char * military_timezone(char * buffer, T offset);

  template<config Config>
  struct military_timezone_t
  {
  public:
    military_timezone_t(char letter = 'Z');

    template<typename T>
    military_timezone_t(T offset);

  public:
    template<typename T>
    void set(T offset);

  public:
    char letter;

  public:
    template<action Action>
    char * handle(char * buffer) const;
  };

  template<config Config, action Action, typename Hours, typename Minutes>
  char * differential_timezone(char * buffer, bool sign, Hours hours, Minutes minutes);

  template<config Config, typename Hours, typename Minutes>
  struct differential_timezone_t
  {
  public:
    differential_timezone_t(bool positive = true, Hours hours = 0, Minutes minutes = 0);

  public:
    bool positive;
    uint8_t hours;
    uint8_t minutes;

  public:
    template<action Action>
    char * handle(char * buffer) const;
  };
};

#include <buffer_handle/timezone.hcp>

#endif/*BUFFER_HANDLE_TIMEZONE_HPP*/

# Handle buffer content

When buffers are extensively used to send textual output, their management requires a work to be eased by this **C++** library.

* [Concept](#concept)
* [Example](#example)
* [Reference](#reference)
* [Tests](#tests)

## Concept

[Functions](#functions) and [functors](#functors) of this library can perform [**actions**](#action) on buffers. These actions are determined by function arguments and template parameters. The former will determine what to print in the buffer while the latter specify behavior and content [**configurations**](#configuration).

### Action

* **Size** a buffer would require for the longest output
* **Prepare** a buffer with constant expressions and default values
* **Write** changing portions of a buffer
* **Reset** default or given values to these portions

### Configuration

* **static** to set a buffer based on arguments and parameters
* **dynamic** to edit a buffer using parameters

Only **size** and **prepare** actions have side effects for the **static** configuration, while **write** and **reset** are silently ignored.
For some functions, more *specific* configurations can be achieved regarding [alignment](#alignment), [padding](#padding) and [case](#case).

#### Alignment

When values could be shorter than or equal to the allocated space, the assigned content can be **left** or **right** justified.

#### Padding

Since *alignment* may leave some space untouched from previous actions, it is necessary to fill the buffer with a default character.

#### Case

When localization does not apply, constant strings could be written with either a **lower**, **upper** or **lower with a first upper letter** case.

## Example

Let's consider as an example a buffer which would hold `At 09:23, the weather is sunny with a temperature of 23°C.` and where the `09:23`, `is`, `sunny`, `23` and `C` parts would be subject to modifications.

It can be written as a function

```cpp
template<config Config, action Action, std::size_t MaxDescriptionLength>
char * handle(char * buffer, int hours, int minutes, int when,
	      const char * description, int temperature, char degree)
{
  buffer = string<config::static_, Action>(buffer, "At ", 3);
  buffer = time_<Config, Action>(buffer, hours, minutes);

  const char * tmp1 = ", the weather ";
  buffer = string<config::static_, Action>(buffer, tmp1, std::strlen(tmp1));

  if(when < 0)
    {
      buffer = string<Config, align::left, ' ', Action>(buffer, "will be", 7, 7);
    }
  else if(when == 0)
    {
      buffer = string<Config, align::left, ' ', Action>(buffer, "is", 2, 7);
    }
  else
    {
      buffer = string<Config, align::left, ' ', Action>(buffer, "was", 3, 7);
    }

  buffer = space<Config, Action>(buffer);

  int length = description == nullptr ? 0 : std::strlen(description);
  buffer = string<Config, align::right, ' ', Action>(buffer, description, length, MaxDescriptionLength);

  const char * tmp2 = " with a temperature of ";
  buffer = string<config::static_, Action>(buffer, tmp2, std::strlen(tmp2));

  buffer = two_digits_number<Config, ' ', Action>(buffer, temperature);
  buffer = string<config::static_, Action>(buffer, "°", std::strlen("°"));
  buffer = character<Config, Action>(buffer, degree);
  buffer = dot<config::static_, Action>(buffer);

  return buffer;
}
```
which would output
```console
At 09:23, the weather is sunny with a temperature of 23°C.
At 02:00, the weather was cloudy with a temperature of 68°F.
At 23:58, the weather will be freezing with a temperature of 99°K.
```
for ***static*** configurations, inn which case the buffer size would be specific for each invocation, and
```console
At 09:23, the weather is           sunny with a temperature of 23°C.
At 02:00, the weather was         cloudy with a temperature of 68°F.
At 23:58, the weather will be   freezing with a temperature of 99°K.
```
for ***dynamic*** configurations where the buffer is overwritten.

## Reference

All code is scoped in `namespace buffer_handle`.

### Types

```cpp
#include <buffer_handle/action.hpp>
enum class action { size, prepare, write, reset };

#include <buffer_handle/align.hpp>
enum class align { left, right };

#include <buffer_handle/case.hpp>
enum class case_ { lower, first_upper, upper };

#include <buffer_handle/config.hpp>
enum class config { static_, dynamic };
```

### Functions

###### Character
```cpp
#include <buffer_handle/character.hpp>

template<config Config, action Action>
char * character(char * buffer, char c);
```

###### Token
```cpp
#include <buffer_handle/token.hpp>

template<config Config, action Action>
char * TOKEN(char * buffer);//TOKEN = new_line, carriage_return, space, comma, dot, colon, semicolon, equal
```

###### Boolean
```cpp
#include <buffer_handle/boolean.hpp>

template<config Config, case_ Case, align Align, char Pad, action Action>
char * boolean(char * buffer, bool value);
```

###### String
```cpp
#include <buffer_handle/string.hpp>

template<config Config, action Action>
char * string(char * buffer, const char * value, std::size_t length);

template<config Config, align Align, char Pad, action Action>
char * string(char * buffer, const char * value, std::size_t length, std::size_t max_length);
```

* If ```value == nullptr``` then the buffer will be padded with ```max_length``` characters.
* If ```value != nullptr``` then its ```length``` must be ```<=``` to ```max_length```.

###### Number
```cpp
#include <buffer_handle/number.hpp>

template<config Config, char InsteadOfALeadingZero, action Action, typename I>
char * two_digits_number(char * buffer, I i);

template<config Config, action Action, typename I>
char * four_digits_number(char * buffer, I i);

template<config Config, align Align, char Pad, class Itoa, action Action,
	 typename I, typename MaxDigits = uint8_t>
char * integral_number(char * buffer, I i, MaxDigits & max_digits, const Itoa & itoa = Itoa());
```

* The template parameter ```InsteadOfALeadingZero``` can be ```'\0'``` in order to have a leading zero.
* The ```uint8_t & max_digits``` parameter is a reference because the function will assign the value based on the value passed in ```i``` when **prepare** is called. This value should not be modified for later invocations.
* ```uint8_t``` to encode the number of digits should be enough for most applications.
* The ```itoa``` functor must conform to the provided [adapter](#itoa) contract.

###### Time
```cpp
#include <buffer_handle/time.hpp>

template<config Config, align Align, char Pad, class Itoa, action Action, typename MaxDigits = uint8_t>
char * time_(char * buffer, time_t time, MaxDigits & max_digits, const Itoa & itoa = Itoa());

template<config Config, action Action, typename Hours, typename Minutes>
char * time_(char * buffer, Hours hours, Minutes minutes);

template<config Config, action Action, typename Hours, typename Minutes, typename Seconds>
char * time_(char * buffer, Hours hours, Minutes minutes, Seconds seconds);

template<config Config, action Action>
char * time_(char * buffer, std::tm time);
```

###### Timezone
```cpp
#include <buffer_handle/timezone.hpp>

enum class universal_timezone : uint8_t { UT, GMT };

template<config Config, align Align, char Pad, action Action>
char * universal_timezone(char * buffer, enum universal_timezone value);

enum class north_american_timezone : uint8_t { EST, EDT, CST, CDT, MST, MDT, PST, PDT };

template<config Config, action Action>
char * north_american_timezone(char * buffer, enum north_american_timezone value);

template<config Config, action Action>
char * military_timezone(char * buffer, char letter);

template<config Config, action Action, typename T>
char * military_timezone(char * buffer, T offset);

template<config Config, action Action, typename Hours, typename Minutes>
char * differential_timezone(char * buffer, bool sign, Hours hours, Minutes minutes);
```

###### Date
```cpp
#include <buffer_handle/date.hpp>

template<config Config, char InsteadOfALeadingZeroForDay, char Separator, bool YearOn4Digits, action Action,
	 typename Day, typename Month, typename Year>
char * day_month_year(char * buffer, Day day, Month month, Year year);

template<config Config, action Action, typename Month, typename Day>
char * month_day(char * buffer, Month month, Day day);

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
```

###### Container
```cpp
#include <buffer_handle/container.hpp>

template<config Config, align Align, char Pad, action Action,
	 class Iterator, class Handler, class Separator>
char * container(char * buffer, const Iterator & begin, const Iterator & end, std::size_t max_length,
		 Handler & handler, Separator & separator);
```

### Functors

Every functor must conform to the following contract:

```cpp
struct functor_t
{
  template<action Action, ...>
  char * handle(char * buffer);
};
```

where the function ```handle``` may be ```const```. Template argument deduction must always be possible so that calling functions may only pass the ***action*** as first parameter.

###### Nothing
```cpp
#include <buffer_handle/nothing.hpp>

struct nothing_t
{
  template<action Action>
  char * handle(char * buffer) const;
};
```

###### String
```cpp
#include <buffer_handle/string.hpp>

template<config Config, align Align, char Pad>
struct string_t
{
  string_t(std::size_t max_length, const char * value = nullptr, std::size_t length = 0);

  const char * value;
  std::size_t length;

  template<action Action>
  char * handle(char * buffer) const;
};
```

###### Number
```cpp
#include <buffer_handle/number.hpp>

template<config Config, align Align, char Pad, class Itoa, typename I, typename MaxDigits>
struct integral_number_t
{
  integral_number_t(I value = I());

  I value;

  template<action Action>
  char * handle(char * buffer, const Itoa & itoa = Itoa());
};
```

###### Timezone
```cpp
#include <buffer_handle/timezone.hpp>

template<config Config, align Align, char Pad>
struct universal_timezone_t
{
  universal_timezone_t(enum universal_timezone value = universal_timezone::GMT);

  enum universal_timezone value;

  template<action Action>
  char * handle(char * buffer) const;
};

template<config Config>
struct north_american_timezone_t
{
  north_american_timezone_t(enum north_american_timezone value = north_american_timezone::EST);

  enum north_american_timezone value;

  template<action Action>
  char * handle(char * buffer) const;
};

template<config Config>
struct military_timezone_t
{
  military_timezone_t(char letter = 'Z');

  template<typename T>
  military_timezone_t(T offset);

  template<typename T>
  void set(T offset);

  char letter;

  template<action Action>
  char * handle(char * buffer) const;
};

template<config Config>
struct differential_timezone_t
{
  differential_timezone_t(bool positive = true, uint8_t hours = 0, uint8_t minutes = 0);

  bool positive;
  uint8_t hours;
  uint8_t minutes;

  template<action Action>
  char * handle(char * buffer) const;
};
```
###### Container
```cpp
template<config Config, align Align, char Pad, class Handler, class Separator>
struct container_t
{
  container_t(std::size_t max_length,
	      const Handler & handler = Handler(), const Separator & separator = Separator());

  template<action Action, class Iterator>
  char * handle(char * buffer, const Iterator & begin, const Iterator & end);
};
```

### Adapters

###### [Itoa](https://github.com/amdn/itoa)

```cpp
namespace adapter
{
  struct itoa
  {
    template<typename I>
    char * fwd(char * buffer, I i) const;

    template<typename I>
    char * bwd(char * buffer, I i) const;
  };
};
```

## Tests

Run ```make test``` to compile and ```make run-test``` to execute, or simply ```make```.

### Dependencies

* [Itoa header only version](https://github.com/gscano/itoa)
* [Catch2](https://github.com/catchorg/Catch2) (tested with version [2.3.0](https://github.com/catchorg/Catch2/releases/tag/v2.3.0))

To change the path of these dependencies, create a ```config.mk``` file and then assign ```CATCH``` and ```ITOA``` variables with the appropriate locations (```.``` is used by default).
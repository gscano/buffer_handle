# Handle buffer content

When buffers are extensively used to send textual output, their management requires a work to be eased by this **C++ 14** library under [MIT license](LICENSE).

* [Concept](#concept)
* [Example](#example)
* [Reference](#reference)
* [Tests](#tests)

## Concept

[Functions](#functions) and [functors](#functors) of this library perform [**actions**](#actions) on buffers. These actions are determined by function arguments and template parameters. The former will determine what to print in the buffer while the latter specify behavior and content [**configurations**](#configurations).

The purpose of this library is to provide a single function to perform different actions.

### Actions

There are four possible actions:

* Evaluate the **size** a buffer would require for the longest output
* **Prepare** a buffer with constant expressions and default values
* **Write** changing portions of a buffer
* **Reset** default or given values to these portions

In this library, **prepare**, **write** and **reset** actions all have the same behavior: they simply write the arguments. However, the **prepare** action may change arguments passed by reference and should be called only once followed by **write** or **reset** actions. They could also be used by applications in order to preempt and change their behavior as for instance:
```cpp
if(Action == action::reset)
{
  std::memset(buffer, ' ', size);
}
else
{
  handle<Config, ..., Action, ...>(buffer, ...);
}
```

This would typically be used in applications where buffers could be reused and must be cleared to prevent unwanted disclosure.

### Configurations

There are two available configurations:

* **static** to set a buffer based on arguments and parameters
* **dynamic** to edit a buffer using arguments

For the **static** configuration, only **size** and **prepare** actions modify the buffer, while **write** and **reset** actions are silently ignored.
For some functions, more *specific* configurations can be achieved regarding [alignment](#alignment), [padding](#padding) and [case](#case).

#### Alignment

When values could be shorter than the buffer capacity, the content can be **left** or **right** justified.

#### Padding

Since *alignment* may leave some space untouched, it is necessary to fill the rest of the buffer with a default character.

#### Case

When localization does not apply, constant strings could be written with either a **lower**, **upper** or **lower with a first upper letter** case.

### Coding style

#### Function signature

The configuration comes as the first template parameter. It is then followed by other configuration template parameters, such as *alignment*, *padding*, *case*, and functors. Then the action parameter is supplied, followed by template parameters for which argument deduction would happen frequently. For clarity, the first argument is the buffer.

```cpp
template<config Config, ..., action Action, ...>
char * handle(char * buffer, ...);
```

#### Structure layout

Similarly to functions, the functor is defined by configuration and functor template parameters. The function `handle` is templated on the action and function argument types. It is a plus if template argument deduction could be possible so that a calling function may only pass the action parameter.

```cpp
template<config Config, ...>
struct functor_t
{
  template<action Action, ...>
  char * handle(char * buffer, ...) /* const */;
};
```

## Example

Let's consider as an example a buffer which would hold `At 09:23, the weather is sunny with a temperature of 23°C.` and where the `09:23`, `is`, `sunny`, `23` and `C` parts would be subject to modifications.

First, the signature of the function handling such buffer would be as follow:

```cpp
template<config Config, std::size_t MaxDescriptionLength, action Action,
	 typename Hours, typename Minutes, typename Temperature>
char * handle(char * buffer, Hours hours, Minutes minutes, int when,
	      const char * description, Temperature temperature, char scale);
```

In this case, the description `sunny` may be replaced by a string which length must be bounded. So the `MaxDescriptionLength` template parameter is part of the configuration of the object and comes before the action parameter. `Hours`, `Minutes` and `Temperature` are here to be more convenient for a user who may give any type of integral type with positive values. The compiler will deduce them from the arguments.
The `when` parameter is positive for future, negative for past and null for present.

Moving to the implementation, the first thing is to write constant strings:

```cpp
  buffer = string<config::static_, Action>(buffer, "At ", 3);
  //time
  const char * tmp1 = ", the weather ";
  buffer = string<config::static_, Action>(buffer, tmp1, std::strlen(tmp1));
  //when
  buffer = space<config::static_, Action>(buffer);
  //description
  const char * tmp2 = " with a temperature of ";
  buffer = string<config::static_, Action>(buffer, tmp2, std::strlen(tmp2));
  //temperature
  buffer = string<config::static_, Action>(buffer, "°", std::strlen("°"));
  //scale
  buffer = dot<config::static_, Action>(buffer);
```

Note that `<config::static_, Action>` is used and not `<config::static_, action::prepare>` because in the second case the content would be rewritten every time the function is called may it be with **write** or **reset**.

Then the varying strings could be introduced

```cpp
  ...
  //when
  if(when < 0)
    {
      buffer = string<Config, align::left, ' ', Action>(buffer, "was", 3, 7);
    }
  else if(when == 0)
    {
      buffer = string<Config, align::left, ' ', Action>(buffer, "is", 2, 7);
    }
  else
    {
      buffer = string<Config, align::left, ' ', Action>(buffer, "will be", 7, 7);
    }
  //when
  ...
  //description
  int length = description == nullptr ? 0 : std::strlen(description);
  buffer = string<Config, align::right, ' ', Action>(buffer, description, length, MaxDescriptionLength);
  //description
  ...
  //scale
  buffer = character<Config, Action>(buffer, scale);
  //scale
```

with the verb left-aligned and the description right-aligned. They are both padded with spaces	.
Then the time and the temperature are left

```cpp
  ...
  //time
  buffer = time_<Config, Action>(buffer, hours, minutes);
  //time
  ...
  //temperature
  buffer = two_digits_number<Config, ' ', Action>(buffer, temperature);
  //temperature
  ...
```

and the temperature will start with a space when a single digit number is provided. Note that the `two_digits_number` function will always be two characters long.

For **static** configurations, the required buffer size would be obtained for each unique call with
```cpp
(std::size_t)handle<config::static_, MaxDescriptionLength, action::size>
	     (nullptr, hours, minutes, when, description, temperature, scale);
```

(note that `nullptr` is passed so that a cast to `std::size_t` of the return value yields the size the buffer would require if really written) and the output would be

```console
At 09:23, the weather is sunny with a temperature of 23°C.
At 02:00, the weather was cloudy with a temperature of 68°F.
At 23:58, the weather will be freezing with a temperature of 99°K.
```

while for **dynamic** ones, the buffer would be overwritten as such:

```console
At 09:23, the weather is           sunny with a temperature of 23°C.
At 02:00, the weather was         cloudy with a temperature of 68°F.
At 23:58, the weather will be   freezing with a temperature of 99°K.
```

after a maximum size is obtained with

```cpp
(std::size_t)handle<config::dynamic, 10, action::size>(nullptr, 0, 0, 0, nullptr, 0, 'K');
```

where dummy arguments are passed.

## Reference

All code is scoped in `namespace buffer_handle`.

### Types

```cpp
//Declared in buffer_handle/action.hpp
enum class action { size, prepare, write, reset };

//Declared in buffer_handle/align.hpp
enum class align { left, right };

//Declared in buffer_handle/case.hpp
enum class case_ { lower, first_upper, upper };

//Declared in buffer_handle/config.hpp
enum class config { static_, dynamic };
```

### Functions

###### Character
```cpp
//Defined in buffer_handle/character.hpp

<config Config, action Action>
char * character(char * buffer, char c);
```

###### Token
```cpp
//Defined in buffer_handle/token.hpp

template<config Config, action Action>
char * TOKEN(char * buffer);
//TOKEN = new_line, carriage_return, space, comma, dot, slash, colon, semicolon, equal
```

###### Boolean
```cpp
//Defined in buffer_handle/boolean.hpp

template<config Config, case_ Case, align Align, char Pad, action Action>
char * boolean(char * buffer, bool value);
```

###### String
```cpp
//Defined in buffer_handle/string.hpp

template<config Config, action Action>
char * string(char * buffer, const char * value, std::size_t length);

template<config Config, align Align, char Pad, action Action> inline
char * string(char * buffer, const char * value, std::size_t length,
	      std::size_t max_length, std::size_t & previous_length);

template<config Config, align Align, char Pad, action Action>
char * string(char * buffer, const char * value, std::size_t length, std::size_t max_length);
```

* If ```value == nullptr``` then the buffer will be padded with ```max_length``` characters.
* If ```value != nullptr``` then its ```length``` must be ```<=``` to ```max_length```.
* The `previous_length` argument is used to avoid padding large buffers which content are slightly modified.

###### Number
```cpp
//Defined in buffer_handle/number.hpp

template<config Config, char InsteadOfALeadingZero, action Action, typename I>
char * two_digits_number(char * buffer, I i);

template<config Config, action Action, typename I>
char * four_digits_number(char * buffer, I i);

template<config Config, align Align, char Pad, action Action, class Itoa,
	 typename I, typename MaxDigits = uint8_t>
char * integral_number(char * buffer, I i, MaxDigits & max_digits, MaxDigits & previous_digits,
		       const Itoa & itoa);

template<config Config, align Align, char Pad, action Action, class Itoa,
	 typename I, typename MaxDigits = uint8_t>
char * integral_number(char * buffer, I i, MaxDigits & max_digits, const Itoa & itoa = Itoa());
```

* The template parameter `InsteadOfALeadingZero` can be `'\0'` in order to have a leading zero.
* Type `I` must be integral and `i`must be positive.
* The `max_digits` argument is a reference because the function will assign it when **prepare** is called based on the value passed in `i`. This value should not be modified in later invocations.
* `uint8_t` should be enough to encode the number of digits for most applications.
* The `itoa` functor must conform to the provided [adapter](#itoa) contract.
* The `previous_length` argument is used to avoid padding large buffers which content are slightly modified.

###### Time
```cpp
//Defined in buffer_handle/time.hpp

template<config Config, align Align, char Pad, action Action, class Itoa, typename MaxDigits = uint8_t>
char * time_(char * buffer, time_t time, MaxDigits & max_digits, const Itoa & itoa = Itoa());

template<config Config, action Action, typename Hours, typename Minutes>
char * time_(char * buffer, Hours hours, Minutes minutes);

template<config Config, action Action, typename Hours, typename Minutes, typename Seconds>
char * time_(char * buffer, Hours hours, Minutes minutes, Seconds seconds);

template<config Config, action Action>
char * time_(char * buffer, std::tm time);
```

* The `itoa` functor must conform to the provided [adapter](#itoa) contract.

###### Timezone
```cpp
//Defined in buffer_handle/timezone.hpp

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
//Defined in buffer_handle/date.hpp

template<config Config, char InsteadOfALeadingZeroForDay, char Separator, bool YearOn4Digits,
	 action Action,
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

* The template parameters
  * `InsteadOfALeadingZeroForDay` can be `'\0'` in order to have a leading zero
  * `YearOn4Digits` toggles the year format to 'YYYY' instead of 'YY'
* For functions accepting  directly a month or a year
  * 1 is for January;
  * years start at 0 not 1900.

###### Container
```cpp
//Defined in buffer_handle/container.hpp

template<config Config, align Align, char Pad, action Action,
	   class Element, class Separator, class Iterator>
char * container(char * buffer, const Iterator & begin, const Iterator & end, std::size_t max_length,
		 Element & element, Separator & separator);

template<config Config, align Align, char Pad, action Action,
	 class Element, class Separator, class Iterator>
char * container(char * buffer, const Iterator & begin, const Iterator & end, std::size_t max_length,
		 Element & element, Separator & separator, std::size_t & previous_length);
```

* The `Element` contract must be
  ```cpp
  template<config Config, action Action>
  char * handle(char * buffer, const Iterator & it) /* const */;
  ```
* The `Separator` contract must be
  ```cpp
  template<config Config, action Action>
  char * handle(char * buffer) /* const */;
  ```
* The `previous_length` argument is used to avoid padding large buffers which content are slightly modified.

### Functors

###### Nothing
```cpp
//Defined in buffer_handle/nothing.hpp

struct nothing_t
{
  template<action Action>
  char * handle(char * buffer) const;
};
```

###### String
```cpp
//Defined in buffer_handle/string.hpp

template<config Config, align Align, char Pad>
struct string_t
{
  string_t(std::size_t max_length, const char * value = nullptr, std::size_t length = 0);

  const char * value;
  std::size_t length;

  template<action Action>
  char * handle(char * buffer) const;
};

template<config Config, align Align, char Pad>
struct long_string_t : public string_t<Config, Align, Pad>
{
  long_string_t(std::size_t max_length, const char * value = nullptr, std::size_t length = 0);

  template<action Action>
  char * handle(char * buffer) const;
};
```

* `long_string_t::handle` uses the `previous_length` version of `string()`.

###### Number
```cpp
//Defined in buffer_handle/number.hpp

template<config Config, align Align, char Pad, class Itoa, typename I, typename MaxDigits>
struct integral_number_t
{
  integral_number_t(I value = I());

  I value;

  template<action Action>
  char * handle(char * buffer, const Itoa & itoa = Itoa());
};

template<config Config, align Align, char Pad, typename I, typename MaxDigits = uint8_t>
struct long_integral_number_t : public integral_number_t<Config, Align, Pad, I, MaxDigits>
{
  long_integral_number_t(I value = I());

  template<action Action, class Itoa>
  char * handle(char * buffer, const Itoa & itoa = Itoa());
};
```

* The `itoa` functor must conform to the provided [adapter](#itoa) contract.
* `long_integral_number_t::handle` uses the `previous_digits` version of `integral_number()`.

###### Timezone
```cpp
//Defined in buffer_handle/timezone.hpp

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
//Defined in buffer_handle/container.hpp

template<config Config, align Align, char Pad, class Element, class Separator>
struct container_t
{
  container_t(std::size_t max_length,
	      const Element & element = Element(), const Separator & separator = Separator());

  template<action Action, class Iterator>
  char * handle(char * buffer, const Iterator & begin, const Iterator & end);
};

template<config Config, align Align, char Pad, class Element, class Separator>
struct long_container_t : public container_t<Config, Align, Pad, Element, Separator>
{
  long_container_t(std::size_t max_length, const Element & element = Element(), const Separator & separator = Separator());

  template<action Action, class Iterator>
  char * handle(char * buffer, const Iterator & begin, const Iterator & end);
};
```

* `long_container_t::handle` uses the `previous_length` version of `container()`.

### Adapters

##### Itoa

```cpp
namespace adapter
{
  namespace itoa
  {
    struct NAME
    {
      template<typename I>
      char * fwd(char * buffer, I i) const;

      template<typename I>
      char * bwd(char * buffer, I i) const;
    };
  };
};
```

Available implementations are

* `std::to_string`: `struct to_string_t` defined in `buffer_handle/adapter/itoa/to_string.hpp`
* [Itoa](https://github.com/amdn/itoa): `struct itoa_t` defined in `buffer_handle/adapter/itoa/itoa.hpp`

### Helpers

###### Must write

```cpp
//Defined in buffer_handle/helper.hpp

template<config Config, action Action>
constexpr bool must_write()
{
  return (Config == config::static_ && Action == action::prepare)
      || (Config == config::dynamic && Action != action::size);
}

template<bool UsePreviousLength, char Pad, typename Size>
void pad_left(char * begin, char * end, Size max_length, Size & previous_length);

template<bool UsePreviousLength, char Pad, typename Size>
void pad_right(char * begin, char * end, Size max_length, Size & previous_length);
```

* `pad_left` and `pad_right` could be used to fill the left, respectively right, side of a buffer. For `pad_left`, the content to pad is between `begin` and `end` while for `pad_right` it is between `end` and `begin + max_length`. If `UsePreviousLength` is false then the `memset` will happen on these regions. However, if it is true, the `previous_length` argument will be compared to the current length. Then, if the previous length is smaller than the current length, no `memset` is required since the new content entirely overwrites the previous content. On the other hand, if the previous length is strictly bigger than the current length, the differential region between the previous and the actual content is reset by `memset`.

## Tests

Run `make test` to compile and `make run-test` to execute, or simply `make`.

### Dependencies

* [Catch2](https://github.com/catchorg/Catch2) (tested with version [2.3.0](https://github.com/catchorg/Catch2/releases/tag/v2.3.0))

To change the path of these dependencies, create a `config.mk` file and then assign `CATCH`  variables with the appropriate locations (`.` is used by default).
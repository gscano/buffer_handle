# Handle buffer content

When buffers are extensively used to send textual output, their management requires a work to be eased by this **C++** library.

* [Concept](#concept)
* [Reference](#reference)
* [Tests](#tests)

## Concept

Functions and functors of this library can perform [**actions**](#action) on buffers. These actions are determined by function arguments and template parameters. The former will determine what to print in the buffer while the latter are used for behavior and content [**configurations**](#configuration).

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

Since *alignment* may leave some space untouched from the previous **prepare** or **reset** action, it is necessary to fill the buffer with a given value or a default character.

#### Case

When localization does not apply constant strings could be written with either a **lower**, **upper** or **lower with a first upper letter** case.

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

```cpp
#include <buffer_handle/character.hpp>

template<config Config, action Action>
char * character(char * buffer, char c);
```

```cpp
#include <buffer_handle/token.hpp>

template<config Config, action Action>
char * TOKEN(char * buffer); //TOKEN = new_line, carriage_return, space, comma, dot, colon, semicolon, equal
```

```cpp
#include <buffer_handle/boolean.hpp>

template<config Config, case_ Case, align Align, char Pad, action Action>
char * boolean(char * buffer, bool value);
```

```cpp
#include <buffer_handle/string.hpp>

template<config Config, action Action>
char * string(char * buffer, const char * value, std::size_t length);

template<config Config, align Align, char Pad, action Action>
char * string(char * buffer, const char * value, std::size_t length, std::size_t max_length);
```

```cpp
#include <buffer_handle/number.hpp>

template<config Config, char InsteadOfALeadingZero, action Action, typename I>
char * two_digits_number(char * buffer, I i);

template<config Config, action Action, typename I>
char * four_digits_number(char * buffer, I i);

template<config Config, align Align, char Pad, class Itoa, action Action, typename I, typename MaxDigits = uint8_t>
char * integral_number(char * buffer, I i, MaxDigits & max_digits, const Itoa & itoa = Itoa());
```

* The template parameter ```InsteadOfALeadingZero``` can be ```'\0'``` in order to have a leading zero.
* The ```uint8_t & max_digits``` parameter is a reference because the function will assign the value based on the value passed in ```i``` when **prepare** is called. This value should not be modify for later invocations.
* ```uint8_t``` to encode the number of digits should be enough for most applications.

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

### Functors

```cpp
#include <buffer_handle/nothing.hpp>

struct nothing_t
{
  template<action Action>
  char * handle(char * buffer) const;
};
```

```cpp
#include <buffer_handle/string.hpp>

template<config Config, align Align, char Pad>
struct string_t
{
  string_t(std::size_t max_length, const char * value = nullptr, std::size_t length = 0);

  const char * value;
  std::size_t length;

  template<action Action>
  char * handle(char * buffer);
};
```

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

## Tests

Run ```make test``` to compile and ```make run-test``` to execute, or simply ```make```.

### Dependencies

* [Itoa header only version](https://github.com/gscano/itoa)
* [Catch2](https://github.com/catchorg/Catch2) (tested with version [2.3.0](https://github.com/catchorg/Catch2/releases/tag/v2.3.0))

To change the path of these dependencies, create a ```config.mk``` file and then assign ```CATCH``` and ```ITOA``` variables with the appropriate locations (```.``` is used by default).
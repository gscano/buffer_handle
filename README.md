# Handle buffer content

When buffers are extensively used to send textual output, their management requires a work to be eased by this **C++ 11** header-only library under [MIT license](LICENSE).

* [Concept](#concept)
* [Example](#example)
* [Reference](#reference)
* [Tests](#tests)

## Concept

The purpose of this library is to provide a single [function](#function-signature) to perform 4 basic *actions* on a buffer,

* evaluate the **size** it would require for its longest content;
* **prepare** its constant parts;
* **write** its non-constant sections; or
* **reset** it to its **prepare**d state.

In addition, the function can handle both a **static** and a **dynamic** *configuration* which is to be choosen at the calling site via a template parameter.

When a **static** *configuration* is selected, the behavior of the function is similar to that of `snprintf` when either **size** (`snprintf(NULL)`) or **prepare** (`snprintf(buffer)`) *actions* are executed; whereas **write** or **reset** *actions* would have no effect.

When the *configuration* is **dynamic**, the **prepare** *action* must be executed only once, followed by as many **write** or **reset** *actions* one would want. Those need not be intertwined as **reset** only clears the **dynamic** content of the buffer which is not always wanted.


The *action* is also a template parameter so that a specific set of functions would be generated depending on the combination of *actions* and *configurations*.

For some functions, more specific *configurations* can be toggled regarding [alignment](#alignment), [padding](#padding) and [case](#case).
Even more *configurations* can be used to cope with the specificities of a function.

###### Alignment

When values could be shorter than the buffer capacity, the content can be **left** or **right** justified.

###### Padding

When **reset**ting a buffer or when alignment leaves some portions untouched, a default character is used to fill the corresponding memory regions.

###### Case

When localization does not apply, constant strings could be written with either a **lower**, **upper** or **lower with a first upper letter** case.

##### Coding style

The following coding style has been adopted for template parameters and arguments.
As their number can be quite important and also because some functions take references to variables in order to store some of its *configuration*, it is often convenient to define an associated [functor](#structure-layout).

###### Function signature

The *configuration* comes as the first template parameter. It is then followed by other *configuration* template parameters, such as *alignment*, *padding*, *case*, and functors. Then the *action* parameter is supplied, followed by template parameters for which argument deduction would happen frequently. For clarity, the first argument is the buffer.

```cpp
template<config Config, ..., action Action, ...>
char * handle(char * buffer, ...);
```

###### Structure layout

Similarly to functions, a functor is defined by *configuration* and function template parameters. The member function `handle` is templated on the *action* and some argument types.

```cpp
template<config Config, ...>
struct functor_t
{
  template<action Action, ...>
  char * handle(char * buffer, ...) /* const */;
};
```

#####

##### Large buffers

Sometimes, the longest value that could be written to a buffer is much larger than the actual average length. In order to avoid padding a large memory region every time **reset** is called, some functions take `previous_length` as an additional argument passed by reference. This extra argument is used to keep track of the length of the written data after a call. It will then be reused by the next invocation to limit the portion touched by a **reset**. This behavior can also be triggered by two consecutive **write**s depending on the size of each content (see [padding](#pad) functions).
Similarly, some functors have a template parameter `bool IsLong` which can be set to `true` to indicate that the buffer to handle will be large. The `previous_length` will then be a data member of the functor and its management will be transparent for the caller.

## Example

Let's consider as an [example](example.cpp) a buffer which would hold `At 09:23, the weather is sunny with a temperature of 23°C.` and where the `09:23`, `is`, `sunny`, `23` and `C` parts would be subject to modifications.

First, the signature of the function handling such buffers would be as follow:

```cpp
template<config Config, std::size_t MaxDescriptionLength, action Action,
	 typename Hours, typename Minutes, typename Temperature>
char * handle(char * buffer, Hours hours, Minutes minutes, int when,
	      const char * description, Temperature temperature, char scale);
```

In this case, the description `sunny` may be replaced by a string which length must be bounded. So the `MaxDescriptionLength` template parameter is part of the configuration of the object and comes before the action parameter. `Hours`, `Minutes` and `Temperature` are here to be more convenient for a user who may give any integral type holding positive values. The compiler will deduce them from the arguments.
The `when` parameter is positive for future, negative for past and zero for present times.

Moving to the implementation, the first thing is to write constant strings:

```cpp
  buffer = string<config::static_, Action>(buffer, "At ");
  //time
  buffer = string<config::static_, Action>(buffer, ", the weather ");
  //when
  buffer = space<config::static_, Action>(buffer);
  //description
  buffer = string<config::static_, Action>(buffer, " with a temperature of ");
  //temperature
  buffer = string<config::static_, Action>(buffer, "°");
  //scale
  buffer = dot<config::static_, Action>(buffer);
```

Note that `<config::static_, Action>` is used and not `<config::static_, action::prepare>` because in the second case the content would be rewritten every time the function is called may it be with **write** or **reset** (see [`must_write`](#must-write) and [`write_on_reset`](#write-on-reset) to modify this behavior).

Then the varying strings could be introduced:

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
Then the time and the temperature are left:

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

Note that the `two_digits_number` function will always be two characters long so the temperature will start with a space when a single digit number is provided.

For **static** configurations, the required buffer size would be obtained for each one of them with:

```cpp
(std::size_t)handle<config::static_, MaxDescriptionLength, action::size>
	     (nullptr, hours, minutes, when, description, temperature, scale);
```

Note that `nullptr` is passed so that a cast to `std::size_t` of the return value yields the size the buffer would require if really written. For the following calls:

```cpp
handle<config::static_, 10, action::prepare>(buffer, 9, 23, 0, "sunny", 30, 'C');
handle<config::static_, 10, action::prepare>(buffer, 23, 58, -1, "cloudy", 9, 'K');
handle<config::static_, 10, action::prepare>(buffer, 23, 58, 1, "freezing", 9, 'K');
```

the output would be:

```console
At 09:23, the weather is sunny with a temperature of 30°C.
At 02:00, the weather was cloudy with a temperature of 68°F.
At 23:58, the weather will be freezing with a temperature of 99°K.
```

so this configuration mimics `snprintf` but the same code can also by used with a **dynamic** configuration:

```cpp
handle<config::dynamic, 10, action::prepare>(buffer, 0, 0, 0, "", 0, 'C');
handle<config::dynamic, 10, action::write>(buffer, 9, 23, 0, "sunny", 30, 'C');
handle<config::dynamic, 10, action::write>(buffer, 23, 58, -1, "cloudy", 9, 'K');
handle<config::dynamic, 10, action::write>(buffer, 23, 58, 1, "freezing", 9, 'K');
```

for which the buffer would be successively overwritten as such:

```console
At 00:00, the weather                    with a temperature of  0°C.
At 09:23, the weather is           sunny with a temperature of 30°C.
At 02:00, the weather was         cloudy with a temperature of 68°F.
At 23:58, the weather will be   freezing with a temperature of 99°K.
```

after obtaining the maximal size with:

```cpp
(std::size_t)handle<config::dynamic, 10, action::size>(nullptr, 0, 0, 0, nullptr, 0, 'K');
```

where dummy arguments are passed.

## Reference

All code is scoped in `namespace buffer_handle`.

| Documentation             | Example                    |
|---------------------------|----------------------------|
| [Character](#character)   | [test.cpp](test.cpp#L152)  |
| [Boolean](#boolean)       | [test.cpp](test.cpp#L25)   |
| [String](#string)         | [test.cpp](test.cpp#L1975) |

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

### Character

Handle a single character or a given token.
The content is written whenever [`must_write`](#must-write) is `true`.

###### Character

```cpp
//Defined in buffer_handle/character.hpp

template<config Config, action Action>
char * character(char * buffer, char c);
```

###### Token

```cpp
//Defined in buffer_handle/token.hpp

template<config Config, action Action>
char * TOKEN(char * buffer);
//TOKEN = new_line, carriage_return, space, exclamation_mark, double_quote, single_quote,
//        opening_parenthesis, closing_parenthesis, plus, comma, hyphen = minus, dot, slash,
//        colon, semicolon, less_than, equal, greater_than, question_mark, opening_bracket,
//        backslash, closing_bracket, underscore, backquote, opening_brace, pipe, closing_brace
```

### Boolean

Handle a boolean as a string or as a single character.
The content is written whenever [`must_write`](#must-write) is `true`.

```cpp
//Defined in buffer_handle/boolean.hpp

template<config Config, case_ Case, align Align, char Pad, action Action>
char * boolean(char * buffer, bool value);

template<config Config, action Action, char False = '0', char True = '1'>
char * boolean(char * buffer, bool value);
```

### String

```cpp
//Defined in buffer_handle/string.hpp
```

```cpp
template<config Config, action Action>
char * string(char * buffer, const char * value, std::size_t length = std::strlen(value));
```

Handles a string in the most basic way by copying it to the buffer when [must-write](#must-write) is `true`.
The `length` argument conveniently defaults to the length of the string pointed to by `value`. However, a verification against a `nullptr` is only performed, as an `ASSERT`, for a **static** *configuration*. In a **dynamic** *configuration*, `nullptr` can be passed during a **size** or **prepare** (for instance if the actual string is not known yet) as long as the same length is supplied for every *action*. Also note that the **reset** *action* uses the value passed at that time. To **reset** a buffer to a specific value can be achieved with the function below.

```cpp
template<config Config, align Align, char Pad, action Action>
char * string(char * buffer, const char * value, std::size_t length, std::size_t max_length
	      /* , std::size_t & previous_length */);
```

In this overload, the `max_length` argument sets the maximum size of the buffer. It must remain the same for every *action*.
For simplification, passing a `nullptr` for a **write** is equivalent to a **reset**.
When **reset**, the string is filled with `Pad` characters.
For an explanation on the optional `previous_length` argument, refer to the [large buffer](#large-buffers) section.

###### Manual management

In order to avoid copying the `value` to the buffer everytime `must_write` is `true`, the overloads below can be used to assign `value` to the location in the buffer when **prepar**ed. The `value` is assigned either to the left or to the right of the underlying buffer depending on the *case*.
If `nullptr` is passed then the call defaults to the previous functions with `nullptr` as the `value`.

```cpp
template<config Config, action Action>
char * string(char * buffer, char ** value, std::size_t length);

template<config Config, align Align, char Pad, action Action>
char * string(char * buffer, char ** value, std::size_t length, std::size_t max_length);
```

###### Functor

```cpp
template<config Config, align Align, char Pad, bool IsLong = false>
struct string_t
{
  template<action Action>
  char * handle(char * buffer, const char * value, std::size_t length) const;
};
```

###### Number

```cpp
//Defined in buffer_handle/number.hpp

template<config Config, char InsteadOfALeadingZero, action Action, typename I>
char * two_digits_number(char * buffer, I i);

template<config Config, action Action, typename I>
char * four_digits_number(char * buffer, I i);

template<config Action, class Itoa, typename I>
char * integral_number(char * buffer, I i, const Itoa & itoa = Itoa());

template<config Config, align Align, char Pad, action Action, class Itoa,
	 typename I, typename Digits = uint8_t>
char * integral_number(char * buffer, I i, Digits & max_digits, /* Digits & previous_digits, */
		       const Itoa & itoa = Itoa());
```

* The template parameter `InsteadOfALeadingZero` can be `'\0'` in order to have a leading zero.
* Type `I` must be integral and `i`must be positive.
* The `max_digits` argument is a reference because the function will assign it when **prepare** is called based on the value passed in `i`. It must not be modified.
* `uint8_t` should be enough to encode the number of digits for most applications.
* The `Itoa` functor must conform to the provided [adapter](#itoa) contract.
* The function `integral_number` without the `max_digits` argument is equivalent to calling its counterpart as **static**.

###### Time

```cpp
//Defined in buffer_handle/time.hpp

template<config Config, align Align, char Pad, action Action, class Itoa, typename Digits = uint8_t>
char * time_(char * buffer, time_t time, Digits & max_digits, const Itoa & itoa = Itoa());

template<config Config, action Action, typename Hours, typename Minutes>
char * time_(char * buffer, Hours hours, Minutes minutes);

template<config Config, action Action, typename Hours, typename Minutes, typename Seconds>
char * time_(char * buffer, Hours hours, Minutes minutes, Seconds seconds);

template<config Config, action Action>
char * time_(char * buffer, std::tm time);
```

* The `Itoa` functor must conform to the provided [adapter](#itoa) contract.

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
	 action Action, typename Day, typename Month, typename Year>
char * day_month_year(char * buffer, Day day, Month month, Year year);//dd Mon YYYY

template<config Config, action Action, typename Month, typename Day>
char * month_day(char * buffer, Month month, Day day);//Mon dd

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
	   class Handler, class Separator, class Iterator>
char * container(char * buffer, const Iterator & begin, const Iterator & end, std::size_t max_length,
		 Handler & handler, Separator & separator /* , std::size_t & previous_length */);

template<align Align, char Pad, action Action,
	   class Handler, class Separator, typename Iterator>
char * container(char * buffer, const Iterator & begin, Iterator & current, const Iterator & end,
		 std::size_t max_length, Handler & handler, Separator & separator);
```

* The maximum length is determined by the container content when **static**.
* When **dynamic**, the `max_length` is not used unless the function with the `current` argument is used and which stops iterating before the `end`.
* The `Handler` contract is
  ```cpp
  template<typename Element>
  template<config Config, action Action>
  char * handle(char * buffer, Element element) /* const */;
  ```
* The `Separator` contract is
  ```cpp
  template<config Config, action Action>
  char * handle(char * buffer) /* const */;
  ```

###### Bitset

```cpp
//Defined in buffer_handle/bitset.hpp

template<class Bitset, action Action, class Separator>
char * bitset(char * buffer, typename Bitset::value_type value, Separator & separator);

template<config Config, align Align, char Pad, class Bitset, action Action, class Separator>
char * bitset(char * buffer, typename Bitset::value_type value, std::size_t & max_length, Separator & separator);

template<config Config, align Align, char Pad, class Bitset, action Action, class Separator>
char * bitset(char * buffer, typename Bitset::value_type value, std::size_t & max_length, Separator & separator, std::size_t & previous_length);
```

* The `Bitset` contract is
  ```cpp
  static const std::size_t count;//Number of different elements
  static const char * get(value_type value);
  ```

* The `Separator` contract is
  ```cpp
  template<config Config, action Action>
  char * handle(char * buffer) /* const */;
  ```

### Special functors

###### Nothing

```cpp
//Defined in buffer_handle/nothing.hpp

struct nothing_t
{
  template<action Action>
  char * handle(char * buffer) const;
};
```

###### Number

```cpp
//Defined in buffer_handle/number.hpp

template<config Config, align Align, char Pad, class Itoa, typename I, typename Digits,
	 bool IsLong = false>
struct integral_number_t
{
  template<action Action>
  char * handle(char * buffer, I value, const Itoa & itoa = Itoa());
};
```

* The `Itoa` functor must conform to the provided [adapter](#itoa) contract.

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

template<config Config, align Align, char Pad, bool IsLong = false>
struct container_t
{
  void set_max_length(std::size_t length);

  template<action Action, class Iterator, class Handler, class Separator>
  char * handle(char * buffer, Iterator & begin, Iterator & end,
		Handler & handler, Separator & separator);
};
```

###### Bitset

```cpp
//Defined in buffer_handle/bitset.hpp

template<config Config, align Align, char Pad, class Bitset, bool IsLong = false>
struct bitset_t
{
  template<action Action, class Separator>
  char * handle(char * buffer, typename Bitset::value_type value, Separator & separator);
};
```

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

Available implementations are:

* `struct to_string_t` using `std::to_string` and defined in `buffer_handle/adapter/itoa/to_string.hpp`
* `struct itoa_t` using [itoa](https://github.com/amdn/itoa) and defined in `buffer_handle/adapter/itoa/itoa.hpp`

### Helpers

###### Must write

The function `must_write` can be used to test whether the given configuration and action template parameters imply to write something to the buffer. It is true
* for a **dynamic** configuration unless **size** is the action; or
* for a **static** configuration only if the action is to **prepare** the buffer.

```cpp
//Defined in buffer_handle/helper.hpp

constexpr bool must_write(config Config, action Action);
```

###### Write when reset

The function `write_when_reset` can be used to force a **write** to happen when **reset** is given as an argument (see [date.hcp](date.hcp#L254) for an example).

```cpp
//Defined in buffer_handle/helper.hpp

constexpr action write_when_reset(action value);
```

###### Reset

The `reset` function is used to **reset** a buffer depending on alignment and usage of the previous length.

```cpp
template<align Align, bool UsePreviousLength, char Pad, typename Size>
void reset(char * buffer, Size max_length, Size & previous_length);
```

###### Pad

The `pad_left` and `pad_right` functions could be used to fill the left, respectively right, side of a buffer. For `pad_left`, the content to pad is between `begin` and `end` while for `pad_right` it is between `end` and `begin + max_length`. If `UsePreviousLength` is false then the `memset` will happen on these regions. However, if it is true, the `previous_length` argument will be compared to the current length. Then, if the previous length is smaller than the current length, no `memset` is required since the new content entirely overwrites the previous content. On the other hand, if the previous length is strictly bigger than the current length, the differential region between the previous and the actual content is reset by `memset`.

```cpp
//Defined in buffer_handle/helper.hpp

template<bool UsePreviousLength, char Pad, typename Size>
void pad_left(char * begin, char * end, Size max_length, Size & previous_length);

template<bool UsePreviousLength, char Pad, typename Size>
void pad_right(char * begin, char * end, Size max_length, Size & previous_length);
```

###### Container

Functors `character_separator_t` and `character_and_space_separator_t` can be used as the `Separator` template parameter for containers.

```cpp
//Defined in buffer_handle/helper.hpp

template<char Separator>
struct character_separator_t
{
  template<config Config, action Action>
  char * handle(char * buffer) const;
};

template<char Separator>
struct character_and_space_separator_t
{
  template<config Config, action Action>
  char * handle(char * buffer) const;
};
```

## Tests

Run `make test` to compile and `make run-test` to execute.

### Dependencies

* [Catch2](https://github.com/catchorg/Catch2) (tested with version [2.9.1](https://github.com/catchorg/Catch2/releases/tag/v2.9.1))

To change the path of this dependency, create a `config.mk` file and then assign the `CATCH` variable with the appropriate location (`.` is used by default).
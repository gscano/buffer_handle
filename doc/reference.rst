Reference
~~~~~~~~~

All the code is scoped in the ``buffer_handle`` namespace.

----------------

Basic types
===========

.. code:: cpp

   //Declared in buffer_handle/action.hpp
   enum class action { size, prepare, write, reset };

   //Declared in buffer_handle/config.hpp
   enum class config { static_, dynamic };

+----------------------------+------------------------------+--------------------------------+
| Main                       | Misc                         | Helpers                        |
+============================+==============================+================================+
| `Bitset <#bitset>`__       | `Conditions <#conditions>`__ | `Adapters <#adapters>`__       |
+----------------------------+------------------------------+--------------------------------+
| `Boolean <#boolean>`__     | `Format <#format>`__         | ~ `itoa <#itoa>`__             |
+----------------------------+------------------------------+--------------------------------+
| `Character <#character>`__ | `Modifiers <#modifiers>`__   |                                |
+----------------------------+------------------------------+--------------------------------+
| `Container <#container>`__ |                              | `Separators <#separators>`__   |
+----------------------------+------------------------------+--------------------------------+
| `Date <#date>`__           | `Nothing <#nothing>`__       | ~ `container <#container-1>`__ |
+----------------------------+------------------------------+--------------------------------+
| `Number <#number>`__       | `Padding <#padding>`__       |                                |
+----------------------------+------------------------------+--------------------------------+
| `Time <#time>`__           | `Resetting <#resetting>`__   |                                |
+----------------------------+------------------------------+--------------------------------+
| `Timezone <#timezone>`__   |                              |                                |
+----------------------------+------------------------------+--------------------------------+
| `String <#string>`__       |                              |                                |
+----------------------------+------------------------------+--------------------------------+

----------------

Bitset
======

Handle a set of values represented by a value in the power of 2. It
requires a ``set`` and a ``separator`` contract object respectively to
known the number of elements and their conversion to string, and to
eventually separate them with a specific pattern.

.. code:: cpp

   //Defined in buffer_handle/bitset.hpp

   template<class Set, action Action, class Separator>
   char * bitset(char * buffer, typename Set::value_type value, Separator & separator);

   template<config Config, align Align, char Pad, class Set, action Action, class Separator>
   char * bitset(char * buffer, typename Set::value_type value, std::size_t & max_length,
                 Separator & separator);

   template<config Config, align Align, char Pad, class Set, action Action, class Separator>
   char * bitset(char * buffer, typename Set::value_type value, std::size_t & max_length,
                 Separator & separator, std::size_t & previous_length);

   template<config Config, align Align, char Pad, class Set, bool IsLong = false>
   struct bitset_t
   {
     template<action Action, class Separator>
     char * handle(char * buffer, typename Set::value_type value, Separator & separator);
   };

-  The ``Set`` contract is

   .. code:: cpp

      static const std::size_t count;//Number of different elements
      static const char * get(value_type value);

-  The ```Separator`` <#container-separators>`__ contract is

   .. code:: cpp

      template<config Config, action Action>
      char * handle(char * buffer) /* const */;

Boolean
=======

Handle a boolean as a string or as a single character. The content is
written whenever `must_write <#must-write>`__ is ``true``.

.. code:: cpp

   //Defined in buffer_handle/boolean.hpp

   template<config Config, case_ Case, align Align, char Pad, action Action>
   char * boolean(char * buffer, bool value);

   template<config Config, action Action, char False = '0', char True = '1'>
   char * boolean(char * buffer, bool value);

Character
=========

Handle a single character or a given token. The content is written
whenever `must_write <#must-write>`__ is ``true``.

.. code:: cpp

   //Defined in buffer_handle/character.hpp

   template<config Config, action Action>
   char * character(char * buffer, char c);

Token
-----

.. code:: cpp

   //Defined in buffer_handle/token.hpp

   template<config Config, action Action>
   char * TOKEN(char * buffer);
   //TOKEN = new_line, carriage_return, space, exclamation_mark, double_quote, single_quote,
   //        opening_parenthesis, closing_parenthesis, plus, comma, hyphen = minus, dot, slash,
   //        colon, semicolon, less_than, equal, greater_than, question_mark, opening_bracket,
   //        backslash, closing_bracket, underscore, backquote, opening_brace, pipe, closing_brace

Container
=========

Handle a container following the ``std::advance`` order. They require a
``handler`` and a ``separator`` contract object respectively responsible
for writing elements of the container to the buffer and to eventually
separate them with a specific pattern.

Containers can be handled in two ways. The first function writes every
element contained between ``begin`` and ``end`` in a buffer of size
``max_length`` but does not check out of bound data. The size must be
known to not exceed the buffer size prior to calling the function. The
second one writes the most possible elements between ``current`` and
``end`` and updates ``current`` if it is not possible to write every
element. It checks the size of every element before printing it to the
buffer and is thus slower.

.. code:: cpp

   //Defined in buffer_handle/container.hpp

   template<config Config, align Align, char Pad, action Action,
          class Handler, class Separator, class Iterator>
   char * container(char * buffer, const Iterator & begin, const Iterator & end, std::size_t max_length,
            Handler & handler, Separator & separator /* , std::size_t & previous_length */);

   template<align Align, char Pad, action Action,
          class Handler, class Separator, typename Iterator>
   char * container(char * buffer, const Iterator & begin, Iterator & current, const Iterator & end,
            std::size_t max_length, Handler & handler, Separator & separator);

   template<config Config, align Align, char Pad, bool IsLong = false>
   struct container_t
   {
     void set_max_length(std::size_t length);

     template<action Action, class Iterator, class Handler, class Separator>
     char * handle(char * buffer, Iterator & begin, Iterator & end,
           Handler & handler, Separator & separator);
   };

The maximum length is determined by the function, based on the content
of the container, when **static**.

-  The ``Handler`` contract is:

   .. code:: cpp

      template<typename Element>
      template<config Config, action Action>
      char * handle(char * buffer, Element element) /* const */;

-  The ```Separator`` <#container-separators>`__ contract is:

   .. code:: cpp

      template<config Config, action Action>
      char * handle(char * buffer) /* const */;

Date
====

The following functions handle *asc*,
`rfc822 <https://tools.ietf.org/html/rfc822#section-5>`__, `rfc850
(§2.1.4) <https://tools.ietf.org/html/rfc850#section-2>`__ and `rfc1123
(§5.2.14) <https://tools.ietf.org/html/rfc1123#page-55>`__ dates.

Note that for functions accepting directly a month or a year, 1 is for
January and years start at 0 not 1900.

.. code:: cpp

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

The two functions below are mainly used by above functions but can also
can be used independtly. They respectively write the date in format ‘dd
Mon YY’ and ‘Mon dd’.

The template parameter ``InsteadOfALeadingZeroForDay`` can be ``'\0'``
in order to have a leading zero while ``YearOn4Digits`` toggles the year
format to ‘YYYY’ instead of ‘YY’.

.. code:: cpp

   //Defined in buffer_handle/date.hpp

   template<config Config, char InsteadOfALeadingZeroForDay, char Separator, bool YearOn4Digits,
        action Action, typename Day, typename Month, typename Year>
   char * day_month_year(char * buffer, Day day, Month month, Year year);//dd Mon YY

   template<config Config, action Action, typename Month, typename Day>
   char * month_day(char * buffer, Month month, Day day);//Mon dd

Number
======

.. code:: cpp

   //Defined in buffer_handle/number.hpp

   template<config Config, char InsteadOfALeadingZero, action Action, typename I>
   char * two_digits_number(char * buffer, I i);

   template<config Config, action Action, typename I>
   char * four_digits_number(char * buffer, I i);

Those two functions handle a specific and fixed number of digits of a
positive integral number ``i``. There are no verifications that the
decimal representation of ``i`` is bounded to those digits nor that it
is positive (except for an ``assert``). They are mainly used to handle
`times <#time>`__ and `dates <#date>`__. The template parameter
``InsteadOfALeadingZero`` can be used to set the first digit to a
specific character in case of a single digit number. Use ``'\0'`` in
order to have a leading zero.

.. code:: cpp

   template<config Action, class Itoa, typename I>
   char * integral_number(char * buffer, I i, const Itoa & itoa = Itoa());

   template<config Config, align Align, char Pad, action Action, class Itoa,
        typename I, typename Digits = uint8_t>
   char * integral_number(char * buffer, I i, Digits & max_digits, /* Digits & previous_digits, */
                  const Itoa & itoa = Itoa());

   template<config Config, align Align, char Pad, class Itoa, typename I, typename Digits,
        bool IsLong = false>
   struct integral_number_t
   {
     template<action Action>
     char * handle(char * buffer, I value, const Itoa & itoa = Itoa());
   };

The first function above writes a positive integral number to the buffer
when **prepare**\ d. The second one handles a positive integral number
whose decimal representation is limited to ``max_digits``. This argument
would usually be assigned by the function when a buffer is
**prepare**\ d based on the maximum possible value supplied in ``i``, in
which case it must not be modified afterwards. It is also possible to
provide a constant value throughout calls if this value is already
known. A ``uint8_t`` should be enough to encode the number of digits for
most applications but this type could modified for bigger values. The
``Itoa`` functor must conform to the `adapter <#itoa>`__ contract and
the function without the ``max_digits`` argument is equivalent to
calling its counterpart as **static**.

Time
====

These functions handle the three classical time formats: epoch, ‘HH:mm’
and ‘HH:mm:ss’.

.. code:: cpp

   //Defined in buffer_handle/time.hpp

   template<config Config, align Align, char Pad, action Action, class Itoa, typename Digits = uint8_t>
   char * time_(char * buffer, time_t time, Digits & max_digits, const Itoa & itoa = Itoa());

   template<config Config, action Action, typename Hours, typename Minutes>
   char * time_(char * buffer, Hours hours, Minutes minutes);

   template<config Config, action Action, typename Hours, typename Minutes, typename Seconds>
   char * time_(char * buffer, Hours hours, Minutes minutes, Seconds seconds);

   template<config Config, action Action>
   char * time_(char * buffer, std::tm time);

The ``Itoa`` functor must conform to the `adapter <#itoa>`__ contract.

Timezone
========

Those functions and functors handle *universal*, *North American*,
*military* and *differential* timezones.

.. code:: cpp

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

String
======

.. code:: cpp

   //Defined in buffer_handle/string.hpp

   template<config Config, action Action>
   char * string(char * buffer, const char * value, std::size_t length = std::strlen(value));

Handles a string in the most basic way by copying it to the buffer when
`must_write <#must-write>`__ is ``true``. The ``length`` argument
conveniently defaults to the length of the string pointed to by
``value``. However, a verification against a ``nullptr`` is only
performed, as an ``ASSERT``, for a **static** *configuration*. In a
**dynamic** *configuration*, ``nullptr`` can be passed during a **size**
or **prepare** (for instance if the actual string is not known yet) as
long as the same length is supplied for every *action*. Also note that
the **reset** *action* uses the value passed at that time. To **reset**
a buffer to a specific value use the function below.

.. code:: cpp

   template<config Config, align Align, char Pad, action Action>
   char * string(char * buffer, const char * value, std::size_t length, std::size_t max_length
             /* , std::size_t & previous_length */);

In this overload, the ``max_length`` argument sets the maximum size of
the buffer. It must remain the same for every *action*. For
simplification, passing a ``nullptr`` for a **write** is equivalent to a
**reset**. When **reset**\ ing, the string is filled with ``Pad``
characters. For an explanation on the optional ``previous_length``
argument, refer to the `large buffer <#large-buffers>`__ section.

Manual management
-----------------

In order to avoid copying the ``value`` to the buffer everytime
`must_write <#must-write>`__ is ``true``, the overloads below can be used to assign
``value`` to the location in the buffer when **prepar**\ ed. The
``value`` is assigned either to the left or to the right of the
underlying buffer depending on the *case*. If ``nullptr`` is passed then
the call defaults to the previous functions with ``nullptr`` as the
``value``.

.. code:: cpp

   template<config Config, action Action>
   char * string(char * buffer, char ** value, std::size_t length);

   template<config Config, align Align, char Pad, action Action>
   char * string(char * buffer, char ** value, std::size_t length, std::size_t max_length);

   template<config Config, align Align, char Pad, bool IsLong = false>
   struct string_t
   {
     template<action Action>
     char * handle(char * buffer, const char * value, std::size_t length) const;
   };

----------------
   
Conditions
----------

Must write
++++++++++

The function ``must_write`` can be used to test whether the given
*configuration* and *action* template parameters imply to write
something to the buffer. It is ``true``:

- for a **dynamic** configuration unless **size** is the action; or
- for a **static** configuration only if the action is to **prepare** the buffer.

.. code:: cpp

   //Defined in buffer_handle/helper.hpp
   
   constexpr bool must_write(config Config, action Action);

Format
------

.. code:: cpp
   
   //Declared in buffer_handle/align.hpp
   enum class align { left, right };

   //Declared in buffer_handle/case.hpp
   enum class case_ { lower, first_upper, upper };

Modifiers
---------               

It is sometimes necessary to modify the behavior of a given *action* to
adjust a function to a specific external behavior.

The function ``write_when_reset`` can be used to bypass a **reset** with
a **write** (see `date.hcp <date.hcp#L258>`__ for an example).

The function ``always_write`` can be used in a **dynamic** context to
force a **write** into a **reset** actually rewriting the content
everytime as if it were **static**. It can be used when in a **dynamic**
configuration to write a recurring value to a position that may change
depending on the situation.

.. code:: cpp

   //Defined in buffer_handle/helper.hpp

   constexpr action write_when_reset(action value);
   constexpr action always_write(action value);

Nothing
-------

.. code:: cpp

   //Defined in buffer_handle/nothing.hpp

   struct nothing_t
   {
     template<action Action>
     char * handle(char * buffer) const;
   };

Padding
-------

The ``pad_left`` and ``pad_right`` functions could be used to fill the
left, respectively right, side of a buffer. For ``pad_left``, the
content to pad is between ``begin`` and ``end`` while for ``pad_right``
it is between ``end`` and ``begin + max_length``. If
``UsePreviousLength`` is ``false`` then the ``memset`` will happen on
these regions. However, if it is ``true``, the ``previous_length``
argument will be compared to the current length. Then, if the previous
length is smaller than the current length, no ``memset`` is required
since the new content entirely overwrites the previous content. On the
other hand, if the previous length is strictly bigger than the current
length, the differential region between the previous and the actual
content is reset by ``memset``.

.. code:: cpp

   //Defined in buffer_handle/helper.hpp

   template<bool UsePreviousLength, char Pad, typename Size>
   void pad_left(char * begin, char * end, Size max_length, Size & previous_length);

   template<bool UsePreviousLength, char Pad, typename Size>
   void pad_right(char * begin, char * end, Size max_length, Size & previous_length);

Resetting
---------

The ``reset`` function is used to **reset** a buffer depending on
alignment and usage of the previous length.

.. code:: cpp
	  
   template<align Align, bool UsePreviousLength, char Pad, typename Size>
   void reset(char * buffer, Size max_length, Size & previous_length);

----------------

Adapters
--------

Itoa
++++    

The Itoa adapters have to respect the following contract.

.. code:: cpp

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

Available implementations are:

-  ``struct to_string_t`` using ``std::to_string`` and defined in
   ``buffer_handle/adapter/itoa/to_string.hpp``
-  ``struct itoa_t`` using `itoa <https://github.com/amdn/itoa>`__ and
   defined in ``buffer_handle/adapter/itoa/itoa.hpp``
   
Separators
----------

Container
+++++++++

Functors ``character_separator_t`` and
``character_and_space_separator_t`` can be used as the ``Separator``
template parameter for containers.

.. code:: cpp

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

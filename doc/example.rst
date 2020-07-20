Example
~~~~~~~

* `Text example <#text>`__
* `HTTP example <#http>`__

----------------

Text
====

Let’s consider as an example an application ouputing

.. code:: text
	  
   At 09:23, the weather is sunny with a temperature of 23°C.
   
in a buffer and where the ``09:23``, ``is``, ``sunny``, ``23`` and ``C``
parts would be subject to modifications.

First, the function signature would be as follow:

.. code:: cpp

   template<config Config, std::size_t MaxDescriptionLength, action Action,
        typename Hours, typename Minutes, typename Temperature>
   char * handle(char * buffer, Hours hours, Minutes minutes, int when,
             const char * description, Temperature temperature, char scale);

In this case, the description ``sunny`` may be replaced by a string
which length must be bounded. So the ``MaxDescriptionLength`` template
parameter is part of the configuration of the object and comes before
the action parameter. ``Hours``, ``Minutes`` and ``Temperature`` are
here to be more convenient for a user who may give any integral type
holding positive values. The compiler will deduce them from the
arguments. The ``when`` parameter is positive for future, negative for
past and zero for present times.

Moving to the implementation, the first thing is to write constant
strings:

.. code:: cpp

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

Note that ``<config::static_, Action>`` is used and not
``<config::static_, action::prepare>`` because in the second case the
content would be rewritten every time the function is called may it be
with **write** or **reset** (see `must_write <#must-write>`__ and
`action modifiers <#action-modifiers>`__ to change this behavior).

Then the varying strings could be introduced:

.. code:: cpp

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
     buffer = string<Config, align::right, ' ', Action>(buffer, description, length,
                                                        MaxDescriptionLength);
     //description
     ...
     //scale
     buffer = character<Config, Action>(buffer, scale);
     //scale

with the verb left-aligned and the description right-aligned. They are
both padded with spaces . Then the time and the temperature are left:

.. code:: cpp

     ...
     //time
     buffer = time_<Config, Action>(buffer, hours, minutes);
     //time
     ...
     //temperature
     buffer = two_digits_number<Config, ' ', Action>(buffer, temperature);
     //temperature
     ...

Note that the ``two_digits_number`` function will always be two
characters long so the temperature will start with a space when a single
digit number is provided.

For **static** configurations, the required buffer size would be
obtained for each one of them with:

.. code:: cpp

   (std::size_t)handle<config::static_, MaxDescriptionLength, action::size>
            (nullptr, hours, minutes, when, description, temperature, scale);

Note that ``nullptr`` is passed so that a cast to ``std::size_t`` of the
return value yields the size the buffer would require if really written.
For the following calls:

.. code:: cpp

   handle<config::static_, 10, action::prepare>(buffer, 9, 23, 0, "sunny", 30, 'C');
   handle<config::static_, 10, action::prepare>(buffer, 23, 58, -1, "cloudy", 68, 'F');
   handle<config::static_, 10, action::prepare>(buffer, 23, 58, 1, "freezing", 99, 'K');

the output would be:

.. code:: text

   At 09:23, the weather is sunny with a temperature of 30°C.
   At 02:00, the weather was cloudy with a temperature of 68°F.
   At 23:58, the weather will be freezing with a temperature of 99°K.

so this configuration mimics ``snprintf`` but the same code can also by
used with a **dynamic** configuration:

.. code:: cpp

   handle<config::dynamic, 10, action::prepare>(buffer, 0, 0, 0, "", 0, 'C');
   handle<config::dynamic, 10, action::write>(buffer, 9, 23, 0, "sunny", 30, 'C');
   handle<config::dynamic, 10, action::write>(buffer, 23, 58, -1, "cloudy", 68, 'F');
   handle<config::dynamic, 10, action::write>(buffer, 23, 58, 1, "freezing", 99, 'K');

for which the buffer would be successively overwritten as such:

.. code:: text
	  
   At 00:00, the weather                    with a temperature of  0°C.
   At 09:23, the weather is           sunny with a temperature of 30°C.
   At 02:00, the weather was         cloudy with a temperature of 68°F.
   At 23:58, the weather will be   freezing with a temperature of 99°K.

after obtaining the maximal size with:

.. code:: cpp

   (std::size_t)handle<config::dynamic, 10, action::size>(nullptr, 0, 0, 0, nullptr, 0, 'K');

where dummy arguments are passed.

----------------

HTTP
====

Because spaces are allowed in numerous places of the HTTP protocol,
between the `:` of a header field and its content for instance,
let's consider as an illustration how HTTP headers can be handled.

Let's assume the output buffer contains a simple header such as:

.. code:: text
	  
  HTTP/1.1 [code:3-digits-integral] [reason:text(max-length=10)]
  Content-Length: [length:integral(max-digits=4)]
  Content-Type: [type:text(max-length=10)]; charset=UTF-8
  Content-Encoding: [encoding:text(max-length=10)]

where `[]` elements are variable.

First the function signature would go along the lines of:

.. code:: cpp
	  
  template<config Config, action Action>
  char * handle(char * buffer, unsigned short code, const char * reason,
	        std::size_t length, const char * type, const char * encoding);

Then generic variables holding maximum lengths can be declared at the top of the function:

.. code:: cpp

  unsigned char code_max_digits = 3;
  std::size_t reason_max_length = 10;
  std::size_t max_length_digits = 4;
  std::size_t type_max_length = 10;
  std::size_t max_encoding_length = 10;

Because these values are not checked, supplying an argument exceeding them would
cause the buffer to get corrupted or cause a fatal memory access. Although it would
be safer from a caller perspective to have a more complex implementation that would
check arguments against those boundaries, it would incur a cost the caller might
not be willing to pay.
  
The body of the function would then be:
  
.. code:: cpp

  buffer = string<config::static_, Action>(buffer, "HTTP/1.1 ");
  buffer = integral_number<Config, align::right, ' ', Action, adapter::itoa::to_string_t>
    (buffer, code, code_max_digits);
  buffer = space<config::static_, Action>(buffer);
  buffer = string<Config, align::left, ' ', Action>
    (buffer, reason, std::strlen(reason), reason_max_length);

  buffer = carriage_return<config::static_, Action>(buffer);
  buffer = new_line<config::static_, Action>(buffer);

  buffer = string<config::static_, Action>(buffer, "Content-Length: ");
  buffer = integral_number<Config, align::right, ' ', Action, adapter::itoa::to_string_t>
    (buffer, length, max_length_digits);

  buffer = carriage_return<config::static_, Action>(buffer);
  buffer = new_line<config::static_, Action>(buffer);

  buffer = string<config::static_, Action>(buffer, "Content-Type: ");
  buffer = string<Config, align::right, ' ', Action>
    (buffer, type, std::strlen(type), type_max_length);
  buffer = string<config::static_, Action>(buffer, "; charset=UTF-8");

  buffer = carriage_return<config::static_, Action>(buffer);
  buffer = new_line<config::static_, Action>(buffer);

  buffer = string<config::static_, Action>(buffer, "Content-Encoding: ");
  buffer = string<Config, align::right, ' ', Action>
    (buffer, encoding, std::strlen(encoding), max_encoding_length);

After retrieving the maximum size of the corresponding buffer with *maximum values*,

.. code:: cpp

  handle<config::dynamic, action::size>(nullptr, 999, not_found, 9999, plain, identity);

it is possible to first prepare the buffer

.. code:: cpp

  handle<config::dynamic, action::write>(buffer, 200, ok, 2783, html, gzip);

and then write some content to it

.. code:: cpp

  handle<config::dynamic, action::write>(buffer, 0, ok, 0, "", "");
  handle<config::dynamic, action::write>(buffer, 404, not_found, 212, plain, identity);

leading to the following successive outputs:

.. code:: text

  HTTP/1.1               
  Content-Length:     
  Content-Type:           ; charset=UTF-8
  Content-Encoding:           

.. code:: text

  HTTP/1.1 200 OK
  Content-Length: 2783
  Content-Type:  text/html; charset=UTF-8
  Content-Encoding:       gzip

.. code:: text

  HTTP/1.1 404 Not Found
  Content-Length:  212
  Content-Type: text/plain; charset=UTF-8
  Content-Encoding:   identity

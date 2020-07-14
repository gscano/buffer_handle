Example
~~~~~~~

* `Mock example <#mock>`__

----------------

Mock
====

Let’s consider as an example an application ouputing

   ``At 09:23, the weather is sunny with a temperature of 23°C.``
   
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

   ``At 09:23, the weather is sunny with a temperature of 30°C.``
   
   ``At 02:00, the weather was cloudy with a temperature of 68°F.``
   
   ``At 23:58, the weather will be freezing with a temperature of 99°K.``

so this configuration mimics ``snprintf`` but the same code can also by
used with a **dynamic** configuration:

.. code:: cpp

   handle<config::dynamic, 10, action::prepare>(buffer, 0, 0, 0, "", 0, 'C');
   handle<config::dynamic, 10, action::write>(buffer, 9, 23, 0, "sunny", 30, 'C');
   handle<config::dynamic, 10, action::write>(buffer, 23, 58, -1, "cloudy", 68, 'F');
   handle<config::dynamic, 10, action::write>(buffer, 23, 58, 1, "freezing", 99, 'K');

for which the buffer would be successively overwritten as such:

   ``At 00:00, the weather                    with a temperature of  0°C.``
   
   ``At 09:23, the weather is           sunny with a temperature of 30°C.``
   
   ``At 02:00, the weather was         cloudy with a temperature of 68°F.``
   
   ``At 23:58, the weather will be   freezing with a temperature of 99°K.``

after obtaining the maximal size with:

.. code:: cpp

   (std::size_t)handle<config::dynamic, 10, action::size>(nullptr, 0, 0, 0, nullptr, 0, 'K');

where dummy arguments are passed.

Concept
~~~~~~~

The purpose of this library is to provide a utility similar to
``snprintf`` but for which static portions of the output are only
written once. It is primarily intended for high throughput
applications needing intensive output capacities.
Instead of having a central function, types are handled by
specific `functions <reference.rst>`__ which can then be
combined and chained to produce a larger and more complex output.

There are 4 *actions* which can be carried out on a buffer:

-  evaluate the **size** it would require for its longest content;
-  **prepare** its constant parts;
-  **write** its non-constant sections; or
-  **reset** it to its **prepare**\ d state.

In addition, the function can handle both a **static** and a **dynamic**
*configuration* which is to be choosen at the calling site via a
template parameter.

When a **static** *configuration* is selected, the behavior of the
function is similar to that of ``snprintf`` when either **size**
(``snprintf(NULL)``) or **prepare** (``snprintf(buffer)``) *actions* are
executed; whereas **write** or **reset** *actions* would have no effect.

When the *configuration* is **dynamic**, the **prepare** *action* must
be executed only once, followed by as many **write** or **reset**
*actions*. Those need not be intertwined as **reset** only clears the
**dynamic** content of the buffer which is not always wanted. Indeed, a
**reset** is equivalent to a **write** with default values.

The *action* is also a template parameter so that a specific set of
functions would be generated depending on the combination of *actions*
and *configurations*.

The generated code is thus tailored to the use at the calling site.

Additional configuration
------------------------

Alignment
+++++++++

When values could be shorter than the buffer capacity, the content can
be **left** or **right** justified.

Padding
+++++++

When **reset**\ ting a buffer or when alignment leaves some portions
untouched, a default character is used to fill the corresponding memory
regions.

Case
++++

When internationalization does not apply, constant strings could be
written with either a **lower**, **upper** or **lower with a first upper
letter** case.

Coding style
------------

The following coding style has been adopted for template parameters and
arguments of functions and functors.

Function signature
++++++++++++++++++        

The *configuration* comes as the first template parameter. It is then
followed by other *configuration* template parameters, such as
*alignment*, *padding*, *case*, and functors. Then the *action*
parameter is supplied, followed by template parameters for which
argument deduction would happen frequently. For clarity, the first
argument is the buffer.

.. code:: cpp

   template<config Config, ..., action Action, ...>
   char * handle(char * buffer, ...);

Structure layout
++++++++++++++++

As the number of template parameters can be large and also because some
functions take references to variables in order to store some of its
*configuration*, it is often convenient to define an associated functor.
Similarly to functions, a functor is defined by *configuration* and
function template parameters. The member function ``handle`` is
templated on the *action* and some argument types. 

.. code:: cpp

   template<config Config, ...>
   struct functor_t
   {
     template<action Action, ...>
     char * handle(char * buffer, ...) /* const */;
   };

Large buffers
-------------

Sometimes, the longest value that could be written to a buffer is much
larger than the actual average length. In order to avoid padding a large
memory region every time **reset** is called, some functions take
``previous_length`` as an additional argument passed by reference. This
extra argument is used to keep track of the length of the written data
after a call. It will then be reused by the next invocation to limit the
portion touched by a **reset**. This behavior can also be triggered by
two consecutive **write**\ s depending on the size of each content (see
`padding <reference.rst#padding>`__ functions). Similarly, some functors
have a template parameter ``bool IsLong`` which can be set to ``true`` to
indicate that the buffer to handle will be large. The ``previous_length``
will then be a data member of the functor and its management will be
transparent for the caller.

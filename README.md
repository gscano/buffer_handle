When regularly writing textual output to buffers it may be more efficient for
an application to only write non static parts between calls. Buffers can then
be reused as often as necessary, saving the static text initialization cost.

This **C++ 11** header-only library under [MIT license](./LICENSE) attempts to
provide such functionality.

* [Concept](doc/concept.rst)
* [Example](doc/example.rst)
* [Reference](doc/reference.rst)

The code is available on [GitHub](https://github.com/gscano/buffer_handle) and
[code.malloc.fr](https://code.malloc.fr/buffer_handle).

## Notes

### `-Wimplicit-fallthrough` workaround

* If a dependency to `boost/config.hpp` is not a problem, be sure that it is
included before `bitset.hpp`, `container.hpp`, `string.hpp`, `time.hpp`,
`number.hpp`, and `date.hpp`.

* Otherwise, define the macro `BUFFER_HANDLE_FALLTHROUGH` to suit your specific
needs. It defaults to the empty string for old compilers not enforcing the
option but can be set, for instance, to `[[fallthrough]]` for newer compilers.

## Tests

Run `make test` to compile and `make run-test` to execute.

### Dependencies

* [Catch2](https://github.com/catchorg/Catch2) (tested with version
[2.12.4](https://github.com/catchorg/Catch2/releases/tag/v2.12.4))
* [Boost config](https://github.com/boostorg/config) (tested with version
[1.73.0](https://github.com/boostorg/config/archive/boost-1.73.0.zip))

To change the path to those dependencies, create a `config.mk` file and then
assign the `CATCH` and the `BOOST` variables with the appropriate locations
('`.`' is used by default).
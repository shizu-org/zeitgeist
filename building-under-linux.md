# Building under Linux

## Installing the dependencies

*Zeitgeist* requires
- *make* Version 4.3.0 or better.
- *gcc* Version 11.4.0 or better.
- *cmake* Version 3.29.6 or better.
- Optional: *gdb* Version 12.1 or better.

## Building the program
The following instructions will perform an out-of-source build. An out-of-source build does not modify the source directory
by storing any build file, intermediate file, or product file in a dedicated build directory outside of the source directory.
An out of source build is the recommended way of building *Zeitgeist*.

- Checkout `zeitgeist` [https://github.com/shizu-org/zeitgeist.git](https://github.com/shizu-org/zeitgeist.git) into a directory in your file system.
  For the remainder of this manual, we call this directory *source directory* and denote it in command-line prompts by the meta variable `<source-directory>`.
- Create a directory outside of the *source directory* in which the build will be performed.
  That directory *must not* reside in the source directory `<source-directory>`.
  For the remainder of this manual, we call this directory *build directory* and denote it in command-line prompts by the meta variable `<build-directory>`.
- Open a console and enter the build directory `<build-directory>`.
- Enter `cmake <source-directory>`.
- The makefiles should have been generated in `<build-directory>`.

The above builds *Zeitgeist* without debug information. To build *Zeitgeist* with debug information add the parameter `-DCMAKE_BUILD_TYPE="Debug"` to the cmake command.

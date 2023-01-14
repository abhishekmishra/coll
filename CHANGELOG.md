# CHANGELOG
This file documents major changes in every release of the project. The project follows [Semantic Versioning](https://semver.org/). There is a section for each release - which lists major changes made in the release.

**0.1.0-alpha.1**  2023-01-14 Abhishek Mishra  <abhishekmishra3@gmail.com>
- This first release in 2023, incorporates a few minor changes in build and no 
  new changes in functionality.
- The CMakeLists.txt file now has variables for definition of the version.
- The Doxyfile changes the docs build output to `build/docs` folder.
- The toplevel README contains instructions on getting docs either online or in
  a local build.
- The `docs` folder now contains markdown documentation pages (currently just
  the front page of documentation.)

**0.1.0-alpha.0**  2022-11-12 Abhishek Mishra  <abhishekmishra3@gmail.com>

- This is the first early alpha release of __coll__. __coll__ is a collections library written in ANSI C. It has been tested on Windows, Linux and Macos.
- This version is used a few of my personal projects both open source and private.
- The library started as the home for my own ArrayList datastructure.
- There is also a naive implentation of a Map, which uses the ArrayList internally to store the keys and values. There is no hashing in this Map - and it is useful only for small collections.
- The code has some basic API documentation which is deployed at the documentation site at [coll documentation](https://abhishekmishra.github.io/coll/). It can also be generated locally by calling `make docs`.
- There are several improvements possible in the library. This is just a miliestone release - beginning a series of changes and releases that refine this library.
- The focus of future releases will be quality and API design. Expect breaking changes. 
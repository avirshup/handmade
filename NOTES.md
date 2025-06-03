# Why using CMake?

Not because I want to - the HHero alternative of "write a script that does it" is much,
much better. Only using it because I need to get more familiar with it.

Searching for "how to link SDL2 using CMake" gave a completely different answer on every page ...

# C++ class conventions

Seems like there's no one idiom for this, so let's:

- Name public members normally BUT always access with `this->`.
- Use `m_` for anything the public should not use

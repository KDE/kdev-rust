# Rust Language Support plugin

This package is a WIP to provide language support for Rust.

## Compiling

### Build kdev-rust

***A nightly Rust compiler is required for compilation***

```
mkdir build
cmake ..
make
```

### Dependencies

#### Manually building the ast-redux Rust library

kdev-rust automatically downloads and builds ast-redux. If you would like to
build it yourself for debugging purposes or otherwise, do the following:

```
git clone ssh://git@git.kde.org/scratch/egospodinova/ast-redux
cargo build
```

This should create a folder called *target/debug*.

```
export CMAKE_PREFIX_PATH=/path/to/ast-redux/target/debug:$CMAKE_PREFIX_PATH
```

Now you can proceed to build kdev-rust as described above.

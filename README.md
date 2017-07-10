# Rust Language Support plugin

This package is a WIP to provide language support for Rust.

## Compiling

### Get the ast-redux Rust library

```
git clone ssh://git@git.kde.org/scratch/egospodinova/ast-redux
cargo build
```

This should create a folder called *target/debug*.

### Build kdev-rust

```
export CMAKE_PREFIX_PATH=/path/to/ast-redux/target/debug:$CMAKE_PREFIX_PATH
mkdir build
cmake ..
make
```

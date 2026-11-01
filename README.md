# large-file-finder

Find the 10 largest files and directories in a path and its subdirectories. The program prints each item’s path and size.

The program uses `std::filesystem::file_size` to get file sizes. It calculates a directory’s size by finding all files in that directory and its subdirectories, then adding their sizes.

### Build

Built with GCC 16 on Windows using MSYS2.

```sh
g++ -std=c++20 main.cpp -o large-file-finder.exe
```

### Usage

```sh
large-file-finder.exe "path"
```
Replace `path` with the directory to scan.

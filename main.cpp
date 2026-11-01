#include <iostream>
#include <filesystem>
#include <list>
#include <format>

namespace fs = std::filesystem;

using namespace std;

struct fileInfo
{
    uintmax_t size;
    string path;
    uintmax_t numberOfFiles;
};

typedef std::filesystem::__cxx11::directory_entry Path;

bool compare_size (const fileInfo& first, const fileInfo& second)
{
  return ( first.size > second.size );
}

fileInfo getDirectorySize(const Path& path){
    uintmax_t numberOfFiles = 0;
    uintmax_t size = 0;

    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file()){
            size += fs::file_size(entry);
            numberOfFiles++;
        }
    }
    return { size: size, path: path.path().string(), numberOfFiles: numberOfFiles};
}

list<fileInfo> getTopLargestFilesAndDirectories(fs::path path){
    list<fileInfo> files;
    const int limit = 10;

    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file()){
            files.push_back(fileInfo {size: fs::file_size(entry), path: entry.path().string(), numberOfFiles: 1});
        }

        if(entry.is_directory()){
            auto info = getDirectorySize(entry);
            files.push_back(info);
        }

        // sort list
        files.sort(compare_size);

        if(files.size() > 10)
        {
            // keep to limit
            files.resize(limit);
        }
    }

    return files;
}

string getFriendlySize(uintmax_t bytes)
{
  long gb = 1024 * 1024 * 1024;
  long mb = 1024 * 1024;
  long kb = 1024;
  if( bytes >= gb) return format("{0:.2f}", (float)bytes/gb ) +  " GB ";
  if( bytes >= mb) return format("{0:.2f}", (float)bytes/mb ) +  " MB ";
  if( bytes >= kb) return format("{0:.2f}", (float)bytes/kb ) +  " KB ";
  return to_string(bytes) + " B ";
}

int main(int argc, char *argv[]) {

    if (argc == 1){
        cout << "Please provide a path to a directory";
        return 0;
    }

    const fs::path path(argv[1]);

    if(!fs::is_directory(path)){
        cout << "Path was not a directory!";
        return 0;
    }

    auto files = getTopLargestFilesAndDirectories(path);

    for (const auto& entry : files){
        auto size = getFriendlySize(entry.size);
        cout << entry.path << "\n" << size << "\n\n";
    }

    return 0;
}

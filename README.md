# Header only C++11 IES loader.
A simple header only loader/writer for ies files. Parses the file and writes its content to a struct or vice versa. 

## Usage
```c++
// optional: use double precision for loading/writing the file
#define TINYIES_USE_DOUBLE
#include "tiny_ies.hpp"

tiny_ies<float>::light ies;
// optional: use double precision 
// tiny_ies<double>
std::string err;
std::string warn;
if (!tiny_ies<float>::load_ies(filepath, err, warn, ies)) {
	// print loading failed
}
if (!err.empty()) // print error
if (!warn.empty()) // print warning

// write ies to file
if (!tiny_ies<float>::write_ies("out.ies", ies, /*optional precision*/ 10)) {
	// print writing failed
}
```

![asd](image.png)

## Features
* [x] Load IES
* [x] Save IES
* [ ] Filter candela array data (e.g. resize)
* [ ] External tile file aka `TILT=<filename>`

[License (MIT)](https://github.com/fknfilewalker/tinyies/blob/main/LICENSE)
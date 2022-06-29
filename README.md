# Header only C++11 IES loader.
A simple header only loader for ies files. Parses the file and writes its content to a struct. 

## Usage
```c++
#include "tiny_ies.hpp"

tiny_ies::light ies;
std::string err;
std::string warn;
if (!tiny_ies::load_ies(filepath, err, warn, ies)) {
	// print loading failed
}
if (!err.empty()) // print error
if (!warn.empty()) // print warning
```

![asd](image.png)

## Currently not supported
* external tile file aka `TILT=<filename>`

[License (MIT)](https://github.com/fknfilewalker/tinyies/blob/main/LICENSE)
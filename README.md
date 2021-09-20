# Header only C++11 IES loader.
A simple header only loader for ies files. Parses the file and writes its content to a struct. 

## Usage
```c++
tiny_ies::Light ies;
std::string err;
if (!tiny_ies::load_IES(file, err, ies)) {
    // print err
}
```

Upload the candela array to an image of size `#vertical_angles x #horizontal_angles`. Use a sampler and set wrap_s to `CLAMP_TO_BORDER` and wrap_v to `MIRRORED_REPEAT`.
```glsl
float ies(vec3 n, vec3 t, vec3 dir){
    const float v_angle = radians(acos(dot(n, dir)));
    const float h_angle = radians(acos(dot(t, dir)));
    vec2 uv;
    uv.x = (v_angle - v_min_angle) / (v_max_angle - v_min_angle);
    uv.y = (h_angle - h_min_angle) / (h_max_angle - h_min_angle);
    return texture(ies_sampler, uv).r;
}
```

## Currently not supported
* external tile file aka `TILT=<filename>`

[License (MIT)](https://github.com/fknfilewalker/tinyies/blob/main/LICENSE)
-------------------------------------------------------------------------------
	Copyright (C) 2021 fknfilewalker. All rights reserved.

	https://github.com/fknfilewalker/tinyies

	Permission is hereby granted, free of charge, to any person obtaining a
	copy of this software and associated documentation files (the "Software"),
	to deal in the Software without restriction, including without limitation
	the rights to use, copy, modify, merge, publish, distribute, sublicense,
	and/or sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
	OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
	BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
	AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

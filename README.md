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
float ies(vec3 n, vec3 dir){
    const float angle = radians(acos(dot(n, dir)));
    vec2 uv;
    uv.x = (angle - vmin) / (vmax - vmin);
    uv.y = (angle - hmin) / (hmax - hmin);
    return texture(ies, uv).r;
}
```

## Currently not supported
* external tile file aka `TILT=<filename>`

## Licence

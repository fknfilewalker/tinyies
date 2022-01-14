// +----------------------------------------------------------------------
// | Project : tinyies.
// | All rights reserved.
// +----------------------------------------------------------------------
// | Copyright (c) Lukas Lipp 2021.
// +----------------------------------------------------------------------
// | * Redistribution and use of this software in source and binary forms,
// |   with or without modification, are permitted provided that the following
// |   conditions are met:
// |
// | * Redistributions of source code must retain the above
// |   copyright notice, this list of conditions and the
// |   following disclaimer.
// |
// | * Redistributions in binary form must reproduce the above
// |   copyright notice, this list of conditions and the
// |   following disclaimer in the documentation and/or other
// |   materials provided with the distribution.
// |
// | * Neither the name of the ray team, nor the names of its
// |   contributors may be used to endorse or promote products
// |   derived from this software without specific prior
// |   written permission of the ray team.
// |
// | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// | "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// | LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// | A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// | OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// | SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// | LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// | DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// | THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// | (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// | OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// +----------------------------------------------------------------------

#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

class tiny_ies {
public:
    struct light {
        light() : lamp_to_luminaire_geometry(),
    			  number_of_tilt_angles(0),
    			  number_lights(0),
    			  lumens_per_lamp(0),
                  multiplier(0),
                  number_vertical_angles(0),
                  number_horizontal_angles(0),
                  photometric_type(0), units_type(0),
                  width(0), length(0), height(0),
                  ballast_factor(0),
                  future_use(0),
                  input_watts(0),
                  min_vertical_angle(0),
                  max_vertical_angle(0),
                  min_horizontal_angle(0),
                  max_horizontal_angle(0),
                  max_candela(0)
        {
        }

        /*
        ** HEADER
        */
        std::string ies_version;
        std::map<std::string, std::string> properties;
        std::string tilt;

        /*
        ** DATA
        */
        /* tilt data */
        int lamp_to_luminaire_geometry;
        int number_of_tilt_angles;
        std::vector<float> tilt_angles;
        std::vector<float> tilt_multiplying_factors;

        /* data */
        int number_lights;
        int lumens_per_lamp;
        float multiplier;
        int number_vertical_angles;
        int number_horizontal_angles;
        int photometric_type;
        int units_type;
        float width;
        float length;
        float height;

        float ballast_factor;
        float future_use;
        float input_watts;

        std::vector<float> vertical_angles;
        float min_vertical_angle;
        float max_vertical_angle;
        std::vector<float> horizontal_angles;
        float min_horizontal_angle;
        float max_horizontal_angle;

        std::vector<float> candela;
        float max_candela;
    };

    static bool load_ies(const std::string& file, std::string& err_out, std::string& warn_out, light& ies_out) {
        std::ifstream f(file);
        if (!f) {
            err_out = "Failed reading file: " + file;
            return false;
        }

        ies_out = light();
        std::string line;
        /*
        ** HEADER
        */
        // the first line in a valid ies file should be IESNA:
        if (f.good()) {
            std::getline(f, line);
            if (!read_property("IESNA:", line, ies_out.ies_version)) {
                err_out = "First line did not start with IESNA: " + file;
                //return false;
            }
        }

        // read properties
        while (f.good() && std::getline(f, line)) {
            if (read_property("TILT=", line, ies_out.tilt)) break;
            read_property(line, ies_out.properties);
        }
        if (ies_out.tilt.empty()) {
            err_out = "TILT propertie not found: " + file;
            return false;
        }

        /*
        ** DATA
        */
        // replace comma with empty space
        std::ostringstream oss;
        oss << f.rdbuf();
        std::string s = oss.str();
        std::replace(s.begin(), s.end(), ',', ' ');

        // go through the data
    	std::stringstream buffer;
        buffer.str(s);
        float value;

#define NEXT_FLOAT(name) if (!(buffer >> value)) { err_out = "Error reading <" name "> property: " + file; f.close(); return false; }

        // <lamp to luminaire geometry> <#tilt angles> <angles> <multiplying factors>
        if (ies_out.tilt == "INCLUDE") {
            NEXT_FLOAT("lamp to luminaire geometry")    ies_out.lamp_to_luminaire_geometry = static_cast<int>(value);
            NEXT_FLOAT("#tilt angles")                  ies_out.number_of_tilt_angles = static_cast<int>(value);
            ies_out.tilt_angles.reserve(ies_out.number_of_tilt_angles);
            ies_out.tilt_multiplying_factors.reserve(ies_out.number_of_tilt_angles);
            for (int i = 0; i < ies_out.number_of_tilt_angles; i++) {
                NEXT_FLOAT("angles")        ies_out.tilt_angles.push_back(value);
            }
            for (int i = 0; i < ies_out.number_of_tilt_angles; i++) {
                NEXT_FLOAT("angles")        ies_out.tilt_multiplying_factors.push_back(value);
            }
        }

        // <#lamps> <lumen/lamp> <multiplier> <#vertical angles> <#horizontal angles> <photometric type> <units type> <width> <length> <height>
        NEXT_FLOAT("#lamps")                ies_out.number_lights = static_cast<int>(value);
        NEXT_FLOAT("lumens/lamp")           ies_out.lumens_per_lamp = static_cast<int>(value);
        NEXT_FLOAT("multiplier")            ies_out.multiplier = value;
        NEXT_FLOAT("#vertical angles")      ies_out.number_vertical_angles = static_cast<int>(value);
        NEXT_FLOAT("#horizontal angles")    ies_out.number_horizontal_angles = static_cast<int>(value);
        NEXT_FLOAT("photometric type")      ies_out.photometric_type = static_cast<int>(value);
        NEXT_FLOAT("units type")            ies_out.units_type = static_cast<int>(value);
        NEXT_FLOAT("width")                 ies_out.width = value;
        NEXT_FLOAT("length")                ies_out.length = value;
        NEXT_FLOAT("height")                ies_out.height = value;
        // <ballast factor> <future use> <input watts>
        NEXT_FLOAT("ballast factor")        ies_out.ballast_factor = value;
        NEXT_FLOAT("future use")            ies_out.future_use = value;
        NEXT_FLOAT("input watts")           ies_out.input_watts = value;
        // <vertical angles>
        ies_out.min_vertical_angle = 360;
        ies_out.max_vertical_angle = -360;
        ies_out.vertical_angles.reserve(ies_out.number_vertical_angles);
        for (int i = 0; i < ies_out.number_vertical_angles; i++) {
            NEXT_FLOAT("vertical angles")   ies_out.vertical_angles.push_back(value);
            if (value < ies_out.min_vertical_angle) ies_out.min_vertical_angle = value;
            if (value > ies_out.max_vertical_angle) ies_out.max_vertical_angle = value;
        }
        // <horizontal angles>
        ies_out.min_horizontal_angle = 360;
        ies_out.max_horizontal_angle = -360;
        ies_out.horizontal_angles.reserve(ies_out.number_horizontal_angles);
        for (int i = 0; i < ies_out.number_horizontal_angles; i++) {
            NEXT_FLOAT("horizontal angles") ies_out.horizontal_angles.push_back(value);
            if (value < ies_out.min_horizontal_angle) ies_out.min_horizontal_angle = value;
            if (value > ies_out.max_horizontal_angle) ies_out.max_horizontal_angle = value;
        }
        // <candela values for all vertical angles at first horizontal angle>
        //                                              :
        // <candela values for all vertical angles at last horizontal angle>
        ies_out.candela.reserve(static_cast<uint64_t>(ies_out.number_vertical_angles) * static_cast<uint64_t>(ies_out.number_horizontal_angles));
        ies_out.max_candela = 0;
        for (int i = 0; i < ies_out.number_vertical_angles * ies_out.number_horizontal_angles; i++) {
            NEXT_FLOAT("candela values")    ies_out.candela.push_back(value);
            if (ies_out.max_candela < value) ies_out.max_candela = value;
        }
#undef NEXT_FLOAT
        f.close();
        return true;
    }

private:
    /* read property in current line and return value */
    static inline bool read_property(const std::string& name, std::string& line, std::string& out) {
        size_t pos = line.find(name);
        if (pos == std::string::npos) return false;

        const std::string substring = line.substr(pos + name.size(), line.size());
        pos = substring.find_first_not_of(' ');
        if (pos == std::string::npos) return false;
        out = substring.substr(pos, substring.size());
        return true;
    }
    /* read property in current line and add it to the dict */
    static inline bool read_property(std::string& line, std::map<std::string, std::string>& property_map) {
        const std::string start_delim = "[";
        const std::string stop_delim = "]";
        const size_t attribute_start = line.find(start_delim) + start_delim.size();
        const size_t attribute_end = line.find_last_of(stop_delim);
        if (attribute_start == std::string::npos || attribute_end == std::string::npos) return false;

        std::string attribute = line.substr(attribute_start, attribute_end - attribute_start);
        std::string property;
        if (!read_property(attribute + stop_delim, line, property)) return false;
        property_map.insert({ attribute, property });
        return true;
    }
};
#pragma once

#include <unordered_map>

#include "../common/types.h"
#include "../common/Exception.h"
#include "../material/ShaderDirectory.h"

namespace Core {

    class ShaderDirectoryGL final: public ShaderDirectory {
    public:

        ~ShaderDirectoryGL();

        static const char BasicTextured_vertex[];
        static const char BasicTextured_fragment[];

        void init();

    protected:

    };

}
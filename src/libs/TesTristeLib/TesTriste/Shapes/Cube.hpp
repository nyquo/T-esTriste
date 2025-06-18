#include <TesTristeLib/Renderer/Mesh.hpp>

#include <testristelib_export.h>

namespace TesTriste {

class TET_EXPORT Cube : public Mesh {
  public:
    Cube(float size = 1.0F);
    Cube(const Cube& other) = delete;
    Cube(Cube&& other) noexcept = delete;
    Cube& operator=(const Cube& other) = delete;
    Cube& operator=(Cube&& other) noexcept = delete;

    ~Cube() = default;
};

}
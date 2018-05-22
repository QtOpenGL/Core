#pragma once

#include <memory>

namespace Core {

  // forward declaration
  class Object3D;

  class Object3DComponent {
  protected:
    std::shared_ptr<Object3D> owner;
  public:
    Object3DComponent(std::shared_ptr<Object3D> owner);
  };
}

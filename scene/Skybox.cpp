#include "Skybox.h"
#include "../material/StandardAttributes.h"
#include "../material/SkyboxMaterial.h"
#include "../geometry/Mesh.h"
#include "../render/MeshRenderer.h"

namespace Core {

    Skybox::Skybox() {

    }

    void Skybox::build(WeakPointer<CubeTexture> skyboxTexture) {
        WeakPointer<Core::Mesh> skyboxMesh(Engine::instance()->createMesh(36, false));
        skyboxMesh->init();

        Core::Real vertexPositions[] = {
            // back
            -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0,
            -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0,
            // left
            -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0,
            -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0,
            // right
            1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0,
            1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            // top
            -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0,
            -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            // bottom
            -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0,
            -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0,
            // front
            1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0
        };

        skyboxMesh->enableAttribute(Core::StandardAttribute::Position);
        Core::Bool positionInited = skyboxMesh->initVertexPositions();
        ASSERT(positionInited, "Unable to initialize skybox mesh vertex positions.");
        skyboxMesh->getVertexPositions()->store(vertexPositions);

        WeakPointer<SkyboxMaterial> skyboxMaterial = Engine::instance()->createMaterial<SkyboxMaterial>();
        skyboxMaterial->build();
        skyboxMaterial->setTexture(skyboxTexture);

        WeakPointer<Core::RenderableContainer<Mesh>> skyboxObj = Engine::instance()->createObject3D<RenderableContainer<Mesh>>();

        WeakPointer<Core::MeshRenderer> skyboxRenderer = Engine::instance()->createRenderer<Core::MeshRenderer>(skyboxMaterial, skyboxObj);
        skyboxObj->addRenderable(skyboxMesh);
    }

}
#include "simple_logger.h"
#include "gf3d_mesh.h"


typedef struct
{
    Uint32 meshCount;
    Mesh* meshList;
    //vkDevice device;
}MeshManager;

static MeshManager meshManager = {0};

void gf3dMeshClose();

void gf3d_mesh_init(Uint32 mesh_max)
{
    if (meshManager.meshCount != 0)
    {
        slog("Cannot init mesh system, already initialized");
        return;
    }

    if (mesh_max == 0)
    {
        slog("Cannot initalize mesh system for 0!");
        return;
    }

    meshManager.meshList = gfc_allocate_array(sizeof(Mesh),mesh_max);

    //meshManager.device = gf3d  FIX THIS IJANFONASOKGNNAOPSF

    if (!meshManager.meshList)
        return;

    meshManager.meshCount = mesh_max;
    atexit(gf3dMeshClose);
}

void gf3dMeshClose()
{
    int c;
    //go through list of meshes and free them all!

    for (c = 0; c < meshManager.meshCount; c++)
    {
        gf3d_mesh_free(&meshManager.meshList[c]);
    }

    free(meshManager.meshList);

    memset(&meshManager,0,sizeof(MeshManager));

}




Mesh* gf3d_mesh_new()
{
    int c;

    for (c = 0; c < meshManager.meshCount; c++)
    {
        if (meshManager.meshList[c]._refCount == 0)
        {
            
            meshManager.meshList[c].primitives = gfc_list_new();

            if (meshManager.meshList[c].primitives == NULL)
            {
                slog("Cannot allocate memory for a new mesh!");
                return NULL;
            }
            meshManager.meshList[c]._refCount = 1;

            return &meshManager.meshList[c];
        }
    }
    return NULL;
}


void gf3d_mesh_free(Mesh* mesh)
{
    if (!mesh)
       return;

    int c = 0;
    int d = 0;
    MeshPrimitive* prim;
    c = gfc_list_count(mesh->primitives);

    for (d = 0; d < c; d++)
    {
        prim = gfc_list_nth(mesh->primitives, d);
        if (!prim)
            continue;

    }
}

void gf3dMeshPrimitiveFree(MeshPrimitive* prim)
{
    if (!prim)
        return;

    //if (prim->buffer != VK_NULL_HANDLE)
    {
        //FIX THIS KLAFPMPDKLGM
    }
}


Mesh* gf3d_mesh_load_obj(const char* filename)
{ }

/**
 * @brief make an exact, but separate copy of the input mesh
 * @param in the mesh to duplicate
 * @return NULL on error, or a copy of in
 */
Mesh* gf3d_mesh_copy(Mesh* in);

/**
 * @brief move all of the vertices of the mesh by offset at the buffer level
 * @param in the mesh to move
 * @param offset how much to move it
 * @param rotation apply this rotation to the vertices and normals
 */
void gf3d_mesh_move_vertices(Mesh* in, GFC_Vector3D offset, GFC_Vector3D rotation);

/**
 * @brief allocate a zero initialized mesh primitive
 * @return NULL on error or the primitive
 */
MeshPrimitive* gf3d_mesh_primitive_new();


/**
 * @brief get the input attribute descriptions for mesh based rendering
 * @param count (optional, output) the number of attributes
 * @return a pointer to a vertex input attribute description array
 */
VkVertexInputAttributeDescription* gf3d_mesh_get_attribute_descriptions(Uint32* count);

/**
 * @brief get the binding description for mesh based rendering
 * @return vertex input binding descriptions compatible with mesh data
 */
VkVertexInputBindingDescription* gf3d_mesh_get_bind_description();



/**
 * @brief needs to be called once at the beginning of each render frame
 */
void gf3d_mesh_reset_pipes();

/**
 * @brief called to submit all draw commands to the mesh pipelines
 */
void gf3d_mesh_submit_pipe_commands();

/**
 * @brief get the current command buffer for the mesh system
 */
VkCommandBuffer gf3d_mesh_get_model_command_buffer();


/**
 * @brief queue up a render for the current draw frame
 * @param mesh the mesh to render
 * @param pipe the pipeline to use
 * @param uboData the data to use to draw the mesh
 * @param texture texture data to use
 */
void gf3d_mesh_queue_render(Mesh* mesh, Pipeline* pipe, void* uboData, Texture* texture);


/**
 * @brief adds a mesh to the render pass rendered as an outline highlight
 * @note: must be called within the render pass
 * @param mesh the mesh to render
 * @param com the command pool to use to handle the request we are rendering with
 */
void gf3d_mesh_render(Mesh* mesh, VkCommandBuffer commandBuffer, VkDescriptorSet* descriptorSet);

/**
 * @brief render a mesh through a given pipeline
 */
void gf3d_mesh_render_generic(Mesh* mesh, Pipeline* pipe, VkDescriptorSet* descriptorSet);

/**
 * @brief create a mesh's internal buffers based on vertices
 * @param primitive the mesh primitive to populate
 * @note the primitive must have the objData set and it must have be organizes in buffer order
 */
void gf3d_mesh_create_vertex_buffer_from_vertices(MeshPrimitive* primitive);

/**
 * @brief get the pipeline that is used to render basic 3d meshes
 * @return NULL on error or the pipeline in question
 */
Pipeline* gf3d_mesh_get_pipeline();

/**
 * @brief given a model matrix and basic color, build the meshUBO needed to render a model
 * @param modelMat the model Matrix
 * @param colorMod the color for the UBO
 */
MeshUBO gf3d_mesh_get_ubo(
    GFC_Matrix4 modelMat,
    GFC_Color colorMod);
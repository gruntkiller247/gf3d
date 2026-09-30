#include "simple_logger.h"
#include "gf3d_mesh.h"
#include "model.h"
#include "gf3d_obj_load.h"
#include "gf3d_buffers.h"
#include "gf3d_swapchain.h"
#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_commands.h"
#include "gf2d_sprite.h"

typedef struct
{
    Uint16  verts[3];
}Face;

typedef struct
{
    Uint32 meshCount;
    Mesh* meshList;
    VkDevice device;
    //Pipeline* pipe;             /**<the pipeline associated with sprite rendering*/
    //VkBuffer faceBuffer;
    //VkDeviceMemory faceBuffMem;
    //VkVertexInputAttributeDescription attributeDescriptions[3];
    //VkVertexInputBindingDescription bindingDescription;
    Uint32 chainLength;
}MeshManager;

static MeshManager meshManager = {0};

void gf3d_mesh_close();

//I think this is correct!
Mesh* gf3d_mesh_get_by_filename(const char* fileName)
{
    if (!fileName)
        return NULL;

    for (int c = 0; c < meshManager.meshCount; c++)
    {
        if (strlen(meshManager.meshList[c].filename) == 0)
            continue;

        if (gfc_strlcmp(meshManager.meshList[c].filename,fileName))
        {
            return &meshManager.meshList[c];
        }
    }
}

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

    meshManager.meshList = (Mesh*)gfc_allocate_array(sizeof(Mesh), mesh_max);
    
    
    if (!meshManager.meshList)
        return;

    meshManager.chainLength = gf3d_swapchain_get_chain_length();
    meshManager.device = gf3d_vgraphics_get_default_logical_device;
    meshManager.meshCount = mesh_max;

    /*
    if (!model->pipe)
    {
        slog("Failed to make pipeline for models");
        model_close();
        return;
    }*/

    atexit(gf3d_mesh_close);
}

void gf3d_mesh_close()
{
    int c;
    //go through list of meshes and free them all!

    for (c = 0; c < meshManager.meshCount; c++)
    {
        gf3d_mesh_delete(&meshManager.meshList[c]);
    }

    free(meshManager.meshList);

    memset(&meshManager,0,sizeof(MeshManager));

}

Mesh* gf3d_mesh_new()
{
    int c;

    for (c = 0; c < meshManager.meshCount; c++)
    {
        if (meshManager.meshList[c]._refCount == 0 && (strlen(meshManager.meshList[c].filename) == 0))
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

        if (strlen(meshManager.meshList[c].filename) > 0)
        {
            gf3d_mesh_delete(&meshManager.meshList[c]);
        }
        
    }
    return NULL;
}

void gf3d_mesh_delete(Mesh* mesh)
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
        gf3d_mesh_primitive_free(prim);

    }

    if (mesh->primitives)
    {
        gfc_list_delete(mesh->primitives);
    }
    memset(mesh, 0, sizeof(Mesh*));
}

void gf3d_mesh_free(Mesh* mesh)
{
    if (!mesh)
        return;

    mesh->_refCount--;

    if(mesh->_refCount > 0)
    {
        return;
    }
    gf3d_mesh_delete(mesh);
}

void gf3d_mesh_primitive_free(MeshPrimitive* prim)
{
    if (!prim)
        return;

    if (prim->faceBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(meshManager.device, prim->faceBuffer,NULL);
    }

    if (prim->faceBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(meshManager.device,prim->faceBufferMemory,NULL);
    }

    if (prim->vertexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(meshManager.device,prim->vertexBuffer,NULL);
    }

    if (prim->vertexBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(meshManager.device, prim->vertexBufferMemory,NULL);
    }

    if (prim->objData)
    {
        gf3d_obj_free(prim->objData);
    }

    //free(prim);
    memset(prim, 0, sizeof(MeshPrimitive));
}

/*
int gf3d_mesh_buffer_create(Mesh* mesh)
{
    int i, c;
    c = gfc_list_count();
    MeshPrimitive *prim;
    if (!mesh)
       return NULL;

    for (i = 0; i < c; i++)
    {
        prim = gfc_list_nth(mesh->primitives, i);
        if (!prim)
            continue;
        if(!)//I DO NOT KNOW
            //Not finished
    }
}
*/



int gf3d_prim_buffer_create(MeshPrimitive* prim)
{
    Uint32 bufferSize = 0;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    if (!prim || !prim->objData)
        return 0;

    //FACE BUFFERS
    bufferSize = (sizeof(prim->objData->face_count));

    //Something from Sprite then remade;
    

    //Vertex buffers
    //I DO NOT KNOW WHAT HE COPPIED INTO HERE?

    return 0;
    //return 1;
    //Not finished
}

int gf3d_mesh_primitive_buffer_create(MeshPrimitive* prim)
{
    //I Do not know
    //Not finished
    prim->vertexCount = prim->objData->face_vert_count;
    prim->faceCount = prim;//IDK;
}


int mesh_obj_buffer()
{
    //I dont know
    //Not finished
}

int gf3d_mesg_obj_buffer_create(Mesh* mesh)
{
    //Not finished
    //I don't know
}




Mesh* gf3d_mesh_load_obj(const char* filename)
{ 
    if (!filename)
        return NULL;
    int c;

    Mesh* mesh = gf3d_mesh_get_by_filename(filename);
    
    if (mesh)
    {
        mesh->_refCount++;
        return mesh;
    }

    mesh = gf3d_mesh_new();

    if (!mesh)
    {
        slog("Failed to allocate a new mesh. Return NULL!");
        return NULL;
    }
    
    mesh->objData = gf3d_obj_load_from_file(filename);

    if (!mesh->objData)
    {
        slog("Failed to allocate objData for mesh!");
        gf3d_mesh_delete(mesh);
        return NULL;
    }

    if (!gf3d_mesh_buffer_create(mesh))
    {
        slog("Failed to build memory buffers for meash %s", filename);
        gf3d_mesh_delete(mesh);
        return NULL;
    }

    gfc_line_cpy(mesh->filename, filename);
}


/**
 * @brief allocate a zero initialized mesh primitive
 * @return NULL on error or the primitive
 */
MeshPrimitive* gf3d_mesh_primitive_new()
{
    MeshPrimitive* prim;
    prim = gfc_allocate_array(sizeof(MeshPrimitive), 1);

    if (!prim)
    {
        slog("Failed to allocate primitive memory for a mesh");
        return NULL;
    }
    return prim;

    //Finished, 
}


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
void gf3d_mesh_queue_render(Mesh* mesh, Pipeline* pipe, void* uboData, Texture* texture)
{
    if (!mesh || !pipe || !uboData || !texture)
        return;
    int c, i;
    MeshPrimitive *prim;

    c = gfc_list_count(mesh->primitives);

    for (i = 0; i < c; i++)
    {
        prim = gfc_list_nth(mesh->primitives);
        if (!prim)
            continue;
        gf3d_pipeline_queue_render(pipe,prim->vertexBuffer,prim->vertexCount,prim->faceBuffer,uboData,texture)
            ;
    }
    
}


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
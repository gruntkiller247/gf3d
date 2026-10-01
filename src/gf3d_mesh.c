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
MESH_ATTRIBUTE_COUNT = 3;

typedef struct
{
    Uint32 meshCount;
    Mesh* meshList;
    VkDevice device;
    VkVertexInputAttributeDescription attributeDescriptions[3];
    VkVertexInputBindingDescription bindingDescription;
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

    return NULL;
}

//I think this is correct
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

    //meshManager.chainLength = gf3d_swapchain_get_chain_length();
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

//I think this is correct
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

//I think this is correct
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
    Mesh* mesh;
    MeshPrimitive *prim;
    ObjData *data;

    if (!filename)
        return NULL;


    mesh = gf3d_mesh_get_by_filename(filename);
    
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

    prim = gf3d_mesh_primitive_new();

    if (!prim)
    {
        slog("Failed to load mesh primitive! For filename: %f", filename);
        gf3d_mesh_free(mesh);
        return NULL;
    }
    mesh->primitives = gfc_list_new();
    gfc_list_append(mesh->primitives, prim);

    data = gf3d_obj_load_from_file(filename);
    if (!data)
    {
        gf3d_mesh_delete(mesh);
        gf3d_mesh_primitive_free(prim);
        slog("Failed to allocate objData for mesh!");
        return NULL;
    }

    prim->objData = data;
    
    //Last spot working

    //Need to fill prim?

    prim->vertexCount = 3;
    gf3d_mesh_primitive_create_vertex_buffer(prim);
    gf3d_mesh_primitive_create_face_buffer(prim);

    /*
    Uint32          vertexCount;
    VkBuffer        vertexBuffer;
    VkDeviceMemory  vertexBufferMemory;
    Uint32          faceCount;
    VkBuffer        faceBuffer;
    VkDeviceMemory  faceBufferMemory;
    ObjData* objData;
    */

    
    /*
    if (!gf3d_mesh_buffer_create(mesh))
    {
        slog("Failed to build memory buffers for meash %s", filename);
        gf3d_mesh_delete(mesh);
        return NULL;
    }
    */
    gfc_line_cpy(mesh->filename, filename);
}



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

VkVertexInputAttributeDescription* gf3d_mesh_get_attribute_descriptions(Uint32* count)
{
    //IDK
    //Given to model?
    if (count)
    {
        *count = MESH_ATTRIBUTE_COUNT;//3 IDK?
    }

    meshManager.attributeDescriptions[0].binding = 0;
    meshManager.attributeDescriptions[0].location = 0;
    meshManager.attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    meshManager.attributeDescriptions[0].offset = offsetof(Vertex, vertex);

    meshManager.attributeDescriptions[1].binding = 0;
    meshManager.attributeDescriptions[1].location = 1;
    meshManager.attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    meshManager.attributeDescriptions[1].offset = offsetof(Vertex, normal);

    meshManager.attributeDescriptions[2].binding = 0;
    meshManager.attributeDescriptions[2].location = 2;
    meshManager.attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    meshManager.attributeDescriptions[2].offset = offsetof(Vertex, texel);


    return meshManager.attributeDescriptions;
}

VkVertexInputBindingDescription* gf3d_mesh_get_bind_description()
{
    meshManager.bindingDescription.binding = 0;
    meshManager.bindingDescription.stride = sizeof(Vertex);
    meshManager.bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return &meshManager.bindingDescription;
}

//Finished?
void gf3d_mesh_queue_render(Mesh* mesh, Pipeline* pipe, void* uboData, Texture* texture)
{
    if (!mesh || !pipe || !uboData || !texture)
        return;
    int c, i;
    MeshPrimitive *prim;

    c = gfc_list_count(mesh->primitives);

    for (i = 0; i < c; i++)
    {
        prim = gfc_list_nth(mesh->primitives,i);
        if (!prim)
            continue;
        gf3d_pipeline_queue_render(pipe,prim->vertexBuffer,prim->vertexCount,prim->faceBuffer,uboData,texture);
    }
    
}

/**
 * @brief given a model matrix and basic color, build the meshUBO needed to render a model
 * @param modelMat the model Matrix
 * @param colorMod the color for the UBO
 */
MeshUBO gf3d_mesh_get_ubo(GFC_Matrix4 modelMat, GFC_Color colorMod)
{
    //IDK WHAT THIS IS FOR?

    
}





void gf3d_mesh_primitive_create_vertex_buffer(MeshPrimitive* prim)
{
    void* data = NULL;
    VkDevice device = gf3d_vgraphics_get_default_logical_device();
    Face* faces;
    Uint32 fcount;
    size_t bufferSize;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

    if (!prim)
    {
        slog("No mesh primitize provided");
        return;
    }

    faces = prim->objData->outFace;
    fcount = prim->objData->face_count;
    bufferSize = sizeof(Face) * fcount;
    gf3d_buffer_create(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, faces, (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    gf3d_buffer_create(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &prim->faceBuffer, &prim->faceBufferMemory);

    gf3d_buffer_copy(stagingBuffer, prim->faceBuffer, bufferSize);

    prim->faceCount = fcount;

    vkDestroyBuffer(device, stagingBuffer, NULL);
    vkFreeMemory(device, stagingBufferMemory, NULL);
}

void gf3d_mesh_primitive_create_face_buffer(MeshPrimitive* prim)
{
    void* data = NULL;
    VkDevice device = gf3d_vgraphics_get_default_logical_device();
    Face* faces;
    Uint32 fcount;
    size_t bufferSize;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

    if (!prim)
    {
        slog("No mesh primitize provided");
        return;
    }

    faces = prim->objData->outFace;
    fcount = prim->objData->face_count;
    bufferSize = sizeof(Face) * fcount;
    gf3d_buffer_create(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, faces, (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    gf3d_buffer_create(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &prim->faceBuffer, &prim->faceBufferMemory);

    gf3d_buffer_copy(stagingBuffer, prim->faceBuffer, bufferSize);

    prim->faceCount = fcount;

    vkDestroyBuffer(device, stagingBuffer, NULL);
    vkFreeMemory(device, stagingBufferMemory, NULL);

}
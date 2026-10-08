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


#define MESH_ATTRIBUTE_COUNT 3

typedef struct
{
    Uint32 meshCount;
    Mesh* meshList;
    VkDevice device;
    VkVertexInputAttributeDescription attributeDescriptions[MESH_ATTRIBUTE_COUNT];
    VkVertexInputBindingDescription bindingDescription;
}MeshManager;

static MeshManager meshManager = {0};

void gf3d_mesh_close();
void gf3d_mesh_delete(Mesh* mesh);
void gf3d_mesh_primitive_free(MeshPrimitive* prim);
Mesh* gf3d_mesh_get_by_filename(const char* fileName);

//I think this is correct!
Mesh* gf3d_mesh_get_by_filename(const char* fileName)
{
    if (!fileName)
        return NULL;

    for (int c = 0; c < meshManager.meshCount; c++)
    {
        if (strlen(meshManager.meshList[c].filename) == 0 || meshManager.meshList[c]._refCount == 0)
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
    meshManager.device = gf3d_vgraphics_get_default_logical_device(); 
    meshManager.meshCount = mesh_max;
    slog("Initiate Mesh System");
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
    Uint8 foundIndex = 0;
    Uint32 emptyIndex;
    int c;

    for (c = 0; c < meshManager.meshCount; c++)
    {

        if (meshManager.meshList[c]._refCount == 0)
        {
            if (!foundIndex)
            {
                foundIndex = 1;
                emptyIndex = c;
            }


            if ((strlen(meshManager.meshList[c].filename) == 0))
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

        
    }

    if (strlen(meshManager.meshList[c].filename) > 0)
    {
        gf3d_mesh_delete(&meshManager.meshList[c]);
    }

    if (foundIndex != 0)
    {
        gf3d_mesh_delete(&meshManager.meshList[emptyIndex]);
        meshManager.meshList[emptyIndex].primitives = gfc_list_new();

        if (meshManager.meshList[emptyIndex].primitives == NULL)
        {
            slog("Cannot allocate memory for a new mesh!");
            return NULL;
        }
        meshManager.meshList[emptyIndex]._refCount = 1;

        return &meshManager.meshList[emptyIndex];

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
        prim->faceBuffer = VK_NULL_HANDLE;
    }

    if (prim->faceBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(meshManager.device,prim->faceBufferMemory,NULL);
        prim->faceBufferMemory = VK_NULL_HANDLE;
    }

    if (prim->vertexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(meshManager.device,prim->vertexBuffer,NULL);
        prim->vertexBuffer = VK_NULL_HANDLE;
    }

    if (prim->vertexBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(meshManager.device, prim->vertexBufferMemory,NULL);
        prim->vertexBufferMemory = VK_NULL_HANDLE;
    }

    

    if (prim->objData)
    {
        gf3d_obj_free(prim->objData);
    }

    free(prim);
    //memset(prim, 0, sizeof(MeshPrimitive));
}

int gf3d_prim_buffer_create(MeshPrimitive* prim)
{
    Uint32 bufferSize = 0;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    void* data = NULL;

    if (!prim)
    {
        slog("FAILED TO GIVE PRIM BUFFER CREATE A PRIM YOU MONSTER!");
        return 0;
    }

    if (!prim->objData)
    {
        slog("FAILED TO GIVE ME A PRIM WITH OBJDATA!");
        return 0;
    }

    if (!prim->objData->face_count)
    {
        slog("FAILED TO GIVE ME A PRIM WITH OBJ DATA WITH A FACE COUNT!");
        return 0;
    }
        

    //FACE BUFFERS
    bufferSize = (sizeof(Face) * prim->objData->face_count);
    gf3d_buffer_create(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    vkMapMemory(meshManager.device, stagingBufferMemory, 0, bufferSize, 0, &data) != VK_SUCCESS;
    memcpy(data, prim->objData->outFace, (size_t)bufferSize);
    vkUnmapMemory(meshManager.device, stagingBufferMemory);

    gf3d_buffer_create(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &prim->faceBuffer, &prim->faceBufferMemory);

    gf3d_buffer_copy(stagingBuffer, prim->faceBuffer, bufferSize);

    vkDestroyBuffer(meshManager.device, stagingBuffer, NULL);
    vkFreeMemory(meshManager.device, stagingBufferMemory, NULL);

  


    //Vertex BUFFERS
    bufferSize = (sizeof(Vertex) * prim->objData->face_vert_count);
    gf3d_buffer_create(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    vkMapMemory(meshManager.device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, prim->objData->faceVertices, (size_t)bufferSize);
    vkUnmapMemory(meshManager.device, stagingBufferMemory);

    gf3d_buffer_create(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &prim->vertexBuffer, &prim->vertexBufferMemory);

    gf3d_buffer_copy(stagingBuffer, prim->vertexBuffer, bufferSize);

    vkDestroyBuffer(meshManager.device, stagingBuffer, NULL);
    vkFreeMemory(meshManager.device, stagingBufferMemory, NULL);
    
    prim->vertexCount = prim->objData->face_vert_count;
      prim->faceCount = prim->objData->face_count;
    return 1;
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
        slog("Found a mesh by filename!");
        mesh->_refCount++;
        return mesh;
    }

    mesh = gf3d_mesh_new();

    if (!mesh)
    {
        slog("Failed to allocate a new mesh. Return NULL!");
        return NULL;
    }

    slog("Mesh was created!");

    prim = gf3d_mesh_primitive_new();

    if (!prim)
    {
        slog("Failed to load mesh primitive! For filename: %f", filename);
        gf3d_mesh_delete(mesh);
        return NULL;
    }
    //mesh->primitives = gfc_list_new();

    slog("Mesh Prim created!");
   

    data = gf3d_obj_load_from_file(filename);
    if (!data)
    {
        gf3d_mesh_delete(mesh);
        gf3d_mesh_primitive_free(prim);
        slog("Failed to allocate objData for mesh!");
        return NULL;
    }

    slog("Data prim created!");

    prim->objData = data;

    //Need to fill prim?

    if (!gf3d_prim_buffer_create(prim))
    {
        gf3d_mesh_delete(mesh);
        gf3d_mesh_primitive_free(prim);
        slog("Failed to make prim buffer data %s", filename);
        return NULL;
    }

    slog("Prim buffers created!");
 
    gfc_list_append(mesh->primitives,prim);
    //If you want a have multiple primitives do it here
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

    //Finished?
}

VkVertexInputAttributeDescription* gf3d_mesh_get_attribute_descriptions(Uint32* count)
{


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

    if (count)
    {
        *count = MESH_ATTRIBUTE_COUNT;
    }

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
    int c, i;
    MeshPrimitive* prim;

    if (!mesh || !pipe || !uboData || !texture)
    {
        slog("Something wrong in mesh queue renderer");
        return;
    } 

    c = gfc_list_count(mesh->primitives);

    for (i = 0; i < c; i++)
    {
        prim = gfc_list_nth(mesh->primitives,i);
        if (!prim)
            continue;
        gf3d_pipeline_queue_render(pipe,prim->vertexBuffer,prim->vertexCount,prim->faceBuffer,uboData,texture);
    }
    
}
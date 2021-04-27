﻿#pragma once
#include <unordered_map>
#include <filesystem>
#include <iostream>
#include "../Core/utilities.h"
#include "../Renderer/shader.h"


using namespace glm;

enum AssetType{
    texture,
    shaderFile,
    meshFile,
};

class VertexArray;

// TODO: make utility functions toTexture() etc to quickly cast
class Asset{
    friend class AssetManager;
public:
    virtual bool doReload() = 0;

protected:
    AssetType assetType;
    bool reloadScheduled = false;
    std::filesystem::path path;
};



class Texture : public Asset
{
public:
    Texture(uint width, uint height, GLenum formatInternal = GL_RGBA8, uint samples = 1, bool loadDebug = false);
    Texture(const std::filesystem::path& path, uint samples = 1);
    ~Texture();

public:
    virtual bool doReload() override;
    void setTextureData(void* data, size_t size);
    size_t getSize();
    void bind(uint slot);
    uint id() {return m_id;}

private:
    uint m_width   = 0;
    uint m_height  = 0;
    uint m_id      = 0;
    uint m_samples = 1;
    GLenum m_formatInternal = 0;

    void* data  = nullptr;
    void initTexture();
    bool loadFromFile(const std::filesystem::path& path);
    void loadDebugTexture(GLenum formatInternal, uint width, uint height);
};



class ShaderFile : public Asset
{
    friend class Shader;
    enum ShaderType{
        vertex      = GL_VERTEX_SHADER,
        tessControl = GL_TESS_CONTROL_SHADER,
        tessEval    = GL_TESS_EVALUATION_SHADER,
        geometry    = GL_GEOMETRY_SHADER,
        fragment    = GL_FRAGMENT_SHADER,
        compute     = GL_COMPUTE_SHADER
    };

public:
    ShaderFile(const std::filesystem::path& path, const std::string shaderName);
    ShaderFile(const std::filesystem::path& path, ShaderType type, const std::string shaderName);

    virtual bool doReload() override;
    const std::string& shaderName(){return m_shaderName;}

private:
    const std::string m_shaderName;
    ShaderType type;
    char* loadFile();
    bool getTypeFromFile();
    char* data;
};

class MeshFile : public Asset
{
    struct MeshVertex{
        vec3 position;
        vec3 normal;
        vec2 texCoords;
    };

public:
    MeshFile(const std::filesystem::path& path);

    virtual bool doReload() override;
    void createVAO();
    float* vertices(){return m_vertexData;}
    uint vertexCount(){return m_vertexCount;}
    uint16* indices(){return m_indexData;}
    uint indexCount(){return m_indexCount;}
    uint stride(){return m_stride;}

private:
    std::shared_ptr<VertexArray> m_vertexArray;
    uint m_stride = 0;

    float* m_vertexData = nullptr;
    uint m_vertexCount = 0;

    uint16* m_indexData = nullptr;;
    uint m_indexCount = 0;

    bool loadOBJ();

};



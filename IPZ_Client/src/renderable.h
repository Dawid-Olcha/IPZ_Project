﻿#pragma once
#include "buffer.h"
#include <array>
#define MAX_TEXTURE_SLOTS 32

enum RenderableType{
    texturedQuad,
    mesh,
};

class Renderable
{
public:
    //In future might want more then one buffer per renderable?
    //Maybe if u set vertex buffer data here we draw from there?
    Renderable(const std::string& name, RenderableType type);

    RenderableType type(){return m_type;}
    const std::string& name() {return m_name;}
    std::shared_ptr<VertexBuffer> buffer(){return m_buffer;}
    std::shared_ptr<Shader> shader(){return m_shader;}

    void addVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
    void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);
    void setShader(std::shared_ptr<Shader> shader);

    uint maxIndices(){return m_vertexArray->indexBuffer()->count();}

    virtual void onBegin() = 0;
    virtual void onFlush() = 0;

protected:
    const RenderableType m_type;
    const std::string m_name;
    std::shared_ptr<VertexArray> m_vertexArray;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
    std::shared_ptr<VertexBuffer> m_buffer;
    std::shared_ptr<Shader> m_shader;
};

class TexturedQuad : public Renderable
{
    friend class Renderer;

    struct QuadVertex{
        vec3 position;
        vec4 color;
        vec2 texCoord;
        float texIndex;
        float tilingFactor; //???
    };
public:
    TexturedQuad(const std::string& name, std::shared_ptr<Shader> shader, uint maxVBufferSize);

    void onBegin() override;
    void onFlush() override;
    int addTexture(std::shared_ptr<Texture> texture); //returns 0 if buffer is full otherwise returns texIndex

private:
    int texSamplers[MAX_TEXTURE_SLOTS];
    std::array<std::shared_ptr<Texture>, MAX_TEXTURE_SLOTS> textureSlots;
    std::shared_ptr<Texture> whiteTex;
    uint textureCount = 1;

};

class Mesh : public Renderable
{
    friend class Renderer;
    struct MeshVertex{
        vec3 position;
        vec3 normals;
    };
public:
    Mesh(const std::string& name, std::shared_ptr<Shader> shader, uint maxVBufferSize);
    virtual void onBegin() override;
    virtual void onFlush() override;

private:


};

#include "glad/glad.h"

#include "textures.h"
#include "stb_image.h"

char* GetTexturePath(const char* path)
{
    char* buf;

    size_t pathlen = 0;
    while (1)
    {
        if (path[pathlen] == '\0')
            break;
        pathlen++;
    }

    buf = calloc(TEXTURE_DIR_LEN + pathlen + 1, sizeof(char));
    if (!buf)
    {
        printf("Memory allocation failed...");
        return 0;
    }

    for (size_t i = 0; i < TEXTURE_DIR_LEN; i++)
        *(buf + i) = TEXTURE_DIR[i];

    for (size_t i = 0; i < pathlen; i++)
        *(buf + i + TEXTURE_DIR_LEN) = path[i];

    return buf;
}

unsigned int CreateTexture(unsigned int type, const char* path, unsigned int format, unsigned int repeat)
{
    // create texture
    unsigned int texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(type, texture);

    // set options
    glTexParameteri(type, GL_TEXTURE_WRAP_S, repeat);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, repeat);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    char* fullpath = GetTexturePath(path);

    // load image
    int tex_w, tex_h, tex_channels;
    unsigned char* tex_data = stbi_load(fullpath, &tex_w, &tex_h, &tex_channels, 0);
    free(fullpath);

    if (tex_data)
    {
        // apply image to texture
        glTexImage2D(type, 0, GL_RGB, tex_w, tex_h, 0, format, GL_UNSIGNED_BYTE, tex_data);
        glGenerateMipmap(type);
    }
    else
        printf("Failed to load texture\n");

    stbi_image_free(tex_data);
    glBindTexture(type, 0);  // unbind texture

	return texture;
}

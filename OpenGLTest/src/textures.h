#pragma once
#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#define TEXTURE_DIR "res/textures"
#define TEXTURE_DIR_LEN 12

char* GetTexturePath(const char* path);

unsigned int CreateTexture(unsigned int type, const char* path, unsigned int format, unsigned int repeat);

#endif

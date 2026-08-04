#pragma once

#include <Alegengine/helpers/Logger.hpp>
#include <stb/stb_truetype.h>
#include <glad/glad.h>
#include <vector>

namespace Aleg {
  class Font {
  public:
    Font(std::string path, float pixelHeight);
    Font(const unsigned char* data, unsigned int len, float pixelHeight);

    ~Font();

    unsigned int texture;
    int ascent, descent, lineGap;
    int BITMAP_W;
    int BITMAP_H;
    float scale, height;
    stbtt_fontinfo fontInfo;

    stbtt_bakedchar cdata[96];
  private:
    static Logger* logger;

    void bakeFont(float pixelHeight);

    std::vector<unsigned char> ttfBuffer;
  };
}

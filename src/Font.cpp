#include <Alegengine/alegengine.hpp>
#include <sstream>

namespace Aleg {
  Logger* Font::logger = new Logger("Font");

  Font::Font(std::string path, float pixelHeight) {
    ttfBuffer = FileLoader::loadFontFile(path); 
    bakeFont(pixelHeight);
  }

  Font::Font(const unsigned char* data, unsigned int len, float pixelHeight) {
    ttfBuffer.assign(data, data + len);
    bakeFont(pixelHeight);
  }

  void Font::bakeFont(float pixelHeight) {
    BITMAP_W = std::max(512, (int)pixelHeight * 16);
    BITMAP_H = std::max(512, (int)pixelHeight * 16);

    unsigned char* bitmap = new unsigned char[BITMAP_W * BITMAP_H];

    int result = stbtt_BakeFontBitmap(
      ttfBuffer.data(), 0, pixelHeight,
      bitmap, BITMAP_W, BITMAP_H,
      32, 96, cdata
    );

    if (result <= 0) {
      std::ostringstream ss;
      ss << "Font bitmap too small, " << -result << " glyphs didn't fit\n";
      logger->warn(ss.str());
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_ALPHA,
      BITMAP_W, BITMAP_H, 0,
      GL_ALPHA, GL_UNSIGNED_BYTE, bitmap
    );

    delete[] bitmap;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    if (!stbtt_InitFont(
      &fontInfo,
      ttfBuffer.data(),
      stbtt_GetFontOffsetForIndex(
          ttfBuffer.data(),
          0
      )))
    {
      logger->error("Failed to init font\n");
    }

    stbtt_GetFontVMetrics(
      &fontInfo,
      &ascent,
      &descent,
      &lineGap
    );

    scale = stbtt_ScaleForPixelHeight(&fontInfo, pixelHeight); 
    height = (ascent - descent) * scale;

    glBindTexture(GL_TEXTURE_2D, 0);
  }

  Font::~Font() {
    glDeleteTextures(1, &texture);
  }
}

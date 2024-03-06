#include "ImageManager.h"

#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"
#include "stb_image/stb_image.h"

void ImageManager::SaveImage(const char* filename, int width, int height, const unsigned char* image)
{
    int channels = 1; // Liczba kana³ów (skala szarosci)

    if (stbi_write_png(filename, width, height, channels, image, width) == 0) {
        std::cout << "Blad zapisu obrazu.\n";
    }
    else {
        std::cout << "Obraz zapisany pomyslnie.\n";
    }
}

bool ImageManager::LoadDataFromImageToArray(const char* filename, unsigned char* data)
{
    bool okFlag = true;

    unsigned char* m_LocalBuffer = nullptr;
    int m_Width, m_Height, m_BPP;

	m_LocalBuffer = stbi_load(filename, &m_Width, &m_Height, &m_BPP, 1);

    if (m_Width != 256 || m_Height != 256)
        okFlag = false;
    else {

        int arrayIndex = 0;
        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < 256; j++) {
                data[arrayIndex] = m_LocalBuffer[arrayIndex];
                arrayIndex++;
            }
        }
    }

    if (m_LocalBuffer)
        stbi_image_free(m_LocalBuffer);
    else
        okFlag = false;

    return okFlag;
}

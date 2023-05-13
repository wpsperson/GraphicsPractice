#include <algorithm>
#include <iostream>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "cmrDepthRenderer.h"

int main(int argc, char *argv[])
{
    std::string modelURL = "D:/WorkSpace/cat.dae";
    cmrDepthRenderer renderer(modelURL);
    double pose[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    int imageWidth = 400;
    int imageHeight = 400;
    double intrinsics[4] = { 1, 1, 0,0 };
    double zNear = 1;
    double zFar = 100;
    bool useReverseZ = false;

    // float* depthBuffer = renderer.getDepthData(pose, imageWidth, imageWidth, intrinsics, zNear, zFar, useReverseZ);
    auto [depthBuffer, colorBuffer] = renderer.getDepthColorData(pose, imageWidth, imageWidth, intrinsics, zNear, zFar, useReverseZ);
    if (depthBuffer)
    {
        stbi_flip_vertically_on_write(1);
        std::string depthImageFile = "depth.png";
        // 1=Y, 2=YA, 3=RGB, 4=RGBA
        constexpr int depthComp = 1;
        std::vector<unsigned char> depthImageData(imageWidth * imageHeight);
        std::transform(depthBuffer, depthBuffer + imageWidth * imageHeight, depthImageData.begin(), [](float depth) {return static_cast<unsigned char>(depth * 255);});
        stbi_write_png(depthImageFile.c_str(), imageWidth, imageWidth, depthComp, depthImageData.data(), depthComp * imageWidth);
        delete[] depthBuffer;
    }

    if (colorBuffer)
    {
        std::string colorImageFile = "color.png";
        constexpr int colorComp = 4;
        stbi_write_png(colorImageFile.c_str(), imageWidth, imageWidth, colorComp, colorBuffer, colorComp * imageWidth);

        delete[] colorBuffer;
    }
    return 0;
}

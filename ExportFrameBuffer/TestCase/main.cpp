#include <algorithm>
#include <iostream>
#include <vector>


#pragma warning( push )
#pragma warning( disable : 4996)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#pragma warning( pop ) 
#include "cmrDepthRenderer.h"

int main(int argc, char *argv[])
{
    // minx - 0.691483021
    // maxx	 0.860108972
    // miny - 0.014840999
    // maxy	1.52201998
    // minz - 0.698237002
    // maxz	0.505699992
    std::string modelURL = "C:/WorkSpace/bunny.obj";
    cmrDepthRenderer renderer(modelURL);
    double offset_x = -0.17;
    double offset_y = -0.7;
    double offset_z = -1.31;
    double pose[16] = {
        1, 0, 0, offset_x,
        0, 1, 0, offset_y,
        0, 0, 1, offset_z,
        0, 0, 0, 1
    };
    //int imageWidth = 4000;
    //int imageHeight = 4000;
    //double intrinsics[4] = { static_cast<double>(imageWidth / 2), static_cast<double>(imageHeight / 2), static_cast<double>(imageWidth / 2), static_cast<double>(imageHeight / 2) };
    //double zNear = 0.8;
    //double zFar = 2.0;

    int imageWidth = 1024;
    int imageHeight = 1024;
    //double intrinsics[4] = { 4202.4, 4202.4, 2665.4, 1628.4 };
    double intrinsics[4] = { static_cast<double>(imageWidth / 2), static_cast<double>(imageHeight / 2), static_cast<double>(imageWidth / 2), static_cast<double>(imageHeight / 2) };
    double zNear = 1;
    double zFar = 10.0;

    bool useReverseZ = false;

    // float* depthBuffer = renderer.getDepthData(pose, imageWidth, imageWidth, intrinsics, zNear, zFar, useReverseZ);
    auto [depthBuffer, normalBuffer, colorBuffer] = renderer.getDepthColorData(pose, imageWidth, imageHeight, intrinsics, zNear, zFar, useReverseZ);
    if (depthBuffer)
    {
        stbi_flip_vertically_on_write(1);
        std::string depthImageFile = "depth.png";
        // 1=Y, 2=YA, 3=RGB, 4=RGBA
        constexpr int depthComp = 1;
        std::vector<unsigned char> depthImageData(imageWidth * imageHeight);
        std::transform(depthBuffer, depthBuffer + imageWidth * imageHeight, depthImageData.begin(), [](float depth) {return static_cast<unsigned char>(depth * 255);});
        stbi_write_png(depthImageFile.c_str(), imageWidth, imageHeight, depthComp, depthImageData.data(), depthComp * imageWidth);
        delete[] depthBuffer;
    }

    if (normalBuffer)
    {
        std::string normImageFile = "normal.png";
        constexpr int normComp = 3;
        stbi_write_png(normImageFile.c_str(), imageWidth, imageHeight, normComp, normalBuffer, normComp * imageWidth);
        delete[] normalBuffer;
    }

    if (colorBuffer)
    {
        std::string colorImageFile = "color.png";
        constexpr int colorComp = 4;
        stbi_write_png(colorImageFile.c_str(), imageWidth, imageHeight, colorComp, colorBuffer, colorComp * imageWidth);

        delete[] colorBuffer;
    }
    return 0;
}

/*
teapot.obj argments:
    //minx - 3.00000000
    //maxx	3.42963004
    //miny	0.00000000
    //maxy	3.15000010
    //minz - 2.00000000
    //maxz	2.00000000
    std::string modelURL = "D:/WorkSpace/teapot.obj";
    cmrDepthRenderer renderer(modelURL);
    double offset_x = 0.0;
    double offset_y = -1.5;
    double offset_z = -5;
    double pose[16] = {
        1, 0, 0, offset_x,
        0, 1, 0, offset_y,
        0, 0, 1, offset_z,
        0, 0, 0, 1
    };
    int imageWidth = 400;
    int imageHeight = 400;
    double intrinsics[4] = { static_cast<double>(imageWidth/2), static_cast<double>(imageHeight/2), static_cast<double>(imageWidth/2), static_cast<double>(imageHeight/2) };
    double zNear = 3;
    double zFar = 7;
    bool useReverseZ = true;


bunny.obj arguments: 
    // minx - 0.691483021
    // maxx	 0.860108972
    // miny - 0.014840999
    // maxy	1.52201998
    // minz - 0.698237002
    // maxz	0.505699992
    std::string modelURL = "D:/WorkSpace/bunny.obj";
    cmrDepthRenderer renderer(modelURL);
    double offset_x = -0.17;
    double offset_y = -0.7;
    double offset_z = -1.31;
    double pose[16] = {
        1, 0, 0, offset_x,
        0, 1, 0, offset_y,
        0, 0, 1, offset_z,
        0, 0, 0, 1
    };
    int imageWidth = 400;
    int imageHeight = 400;
    double intrinsics[4] = { static_cast<double>(imageWidth/2), static_cast<double>(imageHeight/2), static_cast<double>(imageWidth/2), static_cast<double>(imageHeight/2) };
    double zNear = 0.8;
    double zFar = 2.0;
    bool useReverseZ = true;


cat.dae arguments:
    //minx - 72.2210007
    //maxx	72.2230988
    //miny - 484.040009
    //maxy	277.959015
    //minz	1.52829003
    //maxz	332.752991
    std::string modelURL = "D:/WorkSpace/cat.dae";
    cmrDepthRenderer renderer(modelURL);
    double offset_x = 0;
    double offset_y = 103; //-172;
    double offset_z = -714;
    double pose[16] = {
        1, 0, 0, offset_x,
        0, 1, 0, offset_y,
        0, 0, 1, offset_z,
        0, 0, 0, 1
    };
    int imageWidth = 400;
    int imageHeight = 400;
    double intrinsics[4] = { static_cast<double>(imageWidth/2), static_cast<double>(imageHeight/2), static_cast<double>(imageWidth/2), static_cast<double>(imageHeight/2) };
    double zNear = 381;
    double zFar = 714;
    bool useReverseZ = true;

*/
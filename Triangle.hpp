//
// Created by LEI XU on 4/11/19.
//

#ifndef RASTERIZER_TRIANGLE_H
#define RASTERIZER_TRIANGLE_H

#include <eigen3/Eigen/Eigen>
#include "Texture.hpp"

using namespace Eigen;
class Triangle{

public:
    Vector4f v[3]; /*三角形的原始坐标，按逆时针顺序为v0，v1，v2*/
    /*每个顶点的值*/
    Vector3f color[3]; //每个顶点的颜色；
    Vector2f tex_coords[3]; //纹理 u,v
    Vector3f normal[3]; //每个顶点的法向量

    Texture *tex= nullptr;
    Triangle();

    Eigen::Vector4f a() const { return v[0]; }
    Eigen::Vector4f b() const { return v[1]; }
    Eigen::Vector4f c() const { return v[2]; }

    void setVertex(int ind, Vector4f ver); /*设置第i个顶点坐标*/
    void setNormal(int ind, Vector3f n); /*设置第i个顶点的法向量*/
    void setColor(int ind, float r, float g, float b); /*设置第i个顶点颜色*/

    void setNormals(const std::array<Vector3f, 3>& normals);
    void setColors(const std::array<Vector3f, 3>& colors);
    Vector3f getColor() const { return color[0] * 255; } // 每个三角形只有一种颜色，
    void setTexCoord(int ind,Vector2f uv ); /*设置第i个顶点纹理坐标*/
    std::array<Vector4f, 3> toVector4() const;
};






#endif //RASTERIZER_TRIANGLE_H

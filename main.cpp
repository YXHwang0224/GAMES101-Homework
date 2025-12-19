#include "Scene.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "Light.hpp"
#include "Renderer.hpp"

#pragma comment(lib,"opencv_world4100d.lib")

// In the main function of the program, we create the scene (create objects and lights)
// as well as set the options for the render (image width and height, maximum recursion
// depth, field-of-view, etc.). We then call the render function().
int main()
{
    Scene scene(1280, 960);

    //下面的sph为两个球状物体
    auto sph1 = std::make_unique<Sphere>(Vector3f(-1, 0, -12), 2);
    sph1->materialType = DIFFUSE_AND_GLOSSY;//此处为漫反射材质
    sph1->diffuseColor = Vector3f(0.6, 0.7, 0.8);//漫反射颜色

    auto sph2 = std::make_unique<Sphere>(Vector3f(0.5, -0.5, -8), 1.5);
    sph2->ior = 1.5;//折射率
    sph2->materialType = REFLECTION_AND_REFRACTION;//此处为镜面材质

    //添加球状物体进入场景
    scene.Add(std::move(sph1));
    scene.Add(std::move(sph2));

    // 定义了两个三角形，由于有公用顶点的存在
    // vertIndex来解释哪些顶点组成一个三角形，此处为 [0, 1, 3] 和 [1, 2, 3]
    Vector3f verts[4] = {{-5,-3,-6}, {5,-3,-6}, {5,-3,-16}, {-5,-3,-16}};
    uint32_t vertIndex[6] = {0, 1, 3, 1, 2, 3};
    // st纹理坐标
    Vector2f st[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    // 创建一堆三角形
    auto mesh = std::make_unique<MeshTriangle>(verts, vertIndex, 2, st);
    mesh->materialType = DIFFUSE_AND_GLOSSY;

    scene.Add(std::move(mesh));
    scene.Add(std::make_unique<Light>(Vector3f(-20, 70, 20), 0.5));
    scene.Add(std::make_unique<Light>(Vector3f(30, 50, -12), 0.5));    

    Renderer r;
    r.Render(scene);//渲染

    return 0;
}
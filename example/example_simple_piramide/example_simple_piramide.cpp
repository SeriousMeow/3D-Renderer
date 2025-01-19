#include <renderer/include_all.hpp>

#include "../BMP_Writer/BMP_Writer.hpp"

int main() {
    // Создаем точки
    renderer::Point a = {0, 0, 0};
    renderer::Point b = {1, 0, 0};
    renderer::Point c = {0, 1, 0};
    renderer::Point d = {0, 0, 1};

    // Создаем грани-треугольники из точек. Наружней считается та сторона, относительно которой
    // точки перечисленны против часовой стрелки
    std::vector<renderer::Triangle> faces = {
        {.vertices = {a, c, b}},
        {.vertices = {b, c, d}},
        {.vertices = {a, b, d}},
        {.vertices = {a, d, c}},
    };

    // Создаем объект из граней
    renderer::Object piramide{faces};

    // Создаем сцену и выставляем объект в начало координат
    renderer::Scene scene;
    scene.Push(piramide, renderer::transfroms::kNoTransforms);

    // Создаем камеру в точке (-1, -1, 1), смотрящую на точку (0.5, 0.5, 0.5)
    renderer::TransformMatrix camera_matrix =
        renderer::transfroms::CameraLookAtPoint({-1, -1, 1}, {0.5, 0.5, 0.5});
    renderer::Camera camera{&scene, camera_matrix};

    // Создаем рендерер и инициализируем параметры
    renderer::Renderer renderer{&camera};

    const size_t width = 1280;
    const size_t height = 720;
    const float fov = 90.0;
    const float near_plane_distance = 1.0;

    renderer.Init(width, height, near_plane_distance, fov);

    // Создаем изображение и рендерим сцену
    renderer::Image image{width, height};
    renderer.Render(image);

    // Записываем результат в файл
    bmp::Image result_image{width, height};
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            bmp::Pixel* current_ouput_pixel = result_image.GetPixel(x, y);
            current_ouput_pixel->r = image.GetImageReference()[x + width * y].r;
            current_ouput_pixel->g = image.GetImageReference()[x + width * y].g;
            current_ouput_pixel->b = image.GetImageReference()[x + width * y].b;
        }
    }
    result_image.Save("example_piramide.bmp");
    return 0;
}

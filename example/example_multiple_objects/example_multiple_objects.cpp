#include <renderer/include_all.hpp>

#include "../BMP_Writer/BMP_Writer.hpp"

int main() {
    // Загружаем объект из OBJ файла
    renderer::Object cube = renderer::utils::LoadObjFile("cube.obj");

    // Создаем сцену
    renderer::Scene scene;

    // Добавляем несколько объектов
    //
    // Куб с центром (0, 0, 0)
    scene.Push(cube, renderer::transfroms::kNoTransforms);

    // Куб с центром (2, 2, 0)
    scene.Push(cube, renderer::transfroms::Translate(
                         {-2, -2, 0, 0}));  // принимает матрицу преобразования из объекта в сцену,
                                            // поэтому противоположный вектор

    // Куб с центром (-2, -2, 0), повернутый на 45 градусов относительно вертикали
    scene.Push(cube, renderer::transfroms::Translate({2, 2, 0, 0}) *
                         renderer::transfroms::RotateAboutAxisZ(45));

    // Создаем камеру в точке (3, -4, 2), смотрящую на точку (0, 0, 0)
    renderer::TransformMatrix camera_matrix =
        renderer::transfroms::CameraLookAtPoint({3, -4, 2, 1}, {0, 0, 0, 1});
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
    result_image.Save("example_multiple_objects.bmp");
    return 0;
}

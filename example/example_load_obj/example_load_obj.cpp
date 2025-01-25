#include <renderer/include_all.hpp>

#include "../BMP_Writer/BMP_Writer.hpp"

int main() {
    // Загружаем объект из OBJ файла
    renderer::Object cube = renderer::utils::LoadObjFile("cube.obj");

    // Создаем сцену и выставляем объект в начало координат
    renderer::Scene scene;
    scene.PushObject(cube, renderer::transforms::kNoTransforms);

    // Создаем камеру в точке (3, -4, 2), смотрящую на точку (0, 0, 0)
    renderer::Matrix camera_matrix = renderer::transforms::CameraLookAtPoint({3, -4, 2}, {0, 0, 0});
    renderer::Scene::CameraId camera_id = scene.PushCamera(camera_matrix);

    // Создаем рендерер
    renderer::Renderer renderer;

    // Параметры
    const size_t width = 1280;
    const size_t height = 720;

    // Создаем изображение и рендерим сцену
    renderer::Image image{width, height};
    image = renderer.Render(scene, camera_id, std::move(image));

    // Записываем результат в файл
    bmp::Image result_image{width, height};
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            bmp::Pixel* current_ouput_pixel = result_image.GetPixel(x, y);
            current_ouput_pixel->r = image.GetPixel(x, y).r;
            current_ouput_pixel->g = image.GetPixel(x, y).g;
            current_ouput_pixel->b = image.GetPixel(x, y).b;
        }
    }
    result_image.Save("example_load_obj.bmp");
    return 0;
}

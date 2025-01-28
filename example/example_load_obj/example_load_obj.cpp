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
    renderer::Image image{renderer::Width{width}, renderer::Height{height}};
    image = renderer.Render(scene, camera_id, std::move(image));

    // Записываем результат в файл
    SaveToBmp(image, "example_load_obj.bmp");
    return 0;
}

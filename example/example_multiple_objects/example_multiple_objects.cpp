#include <renderer/include_all.hpp>

#include "../BMP_Writer/BMP_Writer.hpp"

int main() {
    // Загружаем объект из OBJ файла
    renderer::Object cube = renderer::utils::LoadObjFile("cube.obj");

    // Создаем сцену
    renderer::Scene scene;

    // Добавляем несколько объектов

    // Куб с центром (0, 0, 0)
    scene.PushObject(cube, renderer::transforms::kNoTransforms);

    // Куб с центром (2, 2, 0)
    scene.PushObject(cube, renderer::transforms::Translate(
                               {-2, -2, 0}));  // принимает матрицу преобразования из объекта в
                                               // сцену, поэтому противоположный вектор

    // Куб с центром (-2, -2, 0), повернутый на 45 градусов относительно вертикали
    scene.PushObject(cube, renderer::transforms::Translate({2, 2, 0}) *
                               renderer::transforms::RotateAboutAxisZ(45));

    // Создаем камеру в точке (3, -4, 2), смотрящую на точку (0, 0, 0)
    renderer::Matrix camera_matrix = renderer::transforms::CameraLookAtPoint({3, -4, 2}, {0, 0, 0});
    renderer::Scene::CameraId camera_id = scene.PushCamera(camera_matrix);

    // Создаем рендерер
    renderer::Renderer renderer;

    // Параметры
    constexpr size_t kWidth = 1280;
    constexpr size_t kHeight = 720;

    // Создаем изображение и рендерим сцену
    renderer::Image image{renderer::Width{kWidth}, renderer::Height{kHeight}};
    image = renderer.Render(scene, camera_id, std::move(image));

    // Записываем результат в файл
    SaveToBmp(image, "example_multiple_objects.bmp");
    return 0;
}

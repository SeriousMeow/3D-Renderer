#include <renderer/include_all.hpp>

#include "../BMP_Writer/BMP_Writer.hpp"

int main() {
    // Загружаем объект из OBJ файла
    renderer::Object cube = renderer::utils::LoadFile("cube.obj");

    // Создаем сцену
    renderer::Scene scene;

    // Добавляем несколько объектов

    // Куб с центром (0, 0, 0)
    scene.PushObject(cube);

    // Куб с центром (2, 2, 0)
    renderer::Scene::ObjectId cube_2 = scene.PushObject(cube);
    scene.AccessObject(cube_2).AccessPosition() = renderer::Point{2, 2, 0};

    // Куб с центром (-2, -2, 0), повернутый на 45 градусов относительно вертикали
    renderer::Scene::ObjectId cube_3 = scene.PushObject(cube);
    scene.AccessObject(cube_3).AccessPosition() = renderer::Point{-2, -2, 0};
    scene.AccessObject(cube_3).AccessZAngle() = 45;

    // Создаем камеру в точке (4, -4, 3) и направляем в сторону начала координат
    renderer::Camera camera{renderer::Point{4, -4, 3}, 135, -25};
    renderer::Scene::CameraId camera_id = scene.PushCamera(camera);

    // Создаем рендерер
    renderer::Renderer renderer;

    // Параметры
    constexpr size_t kWidth = 1280;
    constexpr size_t kHeight = 720;

    // Создаем изображение и рендерим сцену
    renderer::Image image{renderer::Width{kWidth}, renderer::Height{kHeight}};
    // Дополнительно дорисовываем ребра
    image = renderer.Render(scene, camera_id, std::move(image),
                            renderer::Renderer::DRAW_EDGES | renderer::Renderer::DRAW_FACETS);

    // Записываем результат в файл
    SaveToBmp(image, "example_multiple_objects.bmp");
    return 0;
}

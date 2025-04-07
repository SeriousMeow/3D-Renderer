#include <renderer/include_all.hpp>

#include "../BMP_Writer/BMP_Writer.hpp"

int main() {
    // Загружаем объект из OBJ файла
    renderer::Object cube = renderer::utils::LoadObjFile("cube.obj");

    // Создаем сцену и добавляем объект, который по-умолчанию устанавливается в начало координат
    renderer::Scene scene;
    scene.PushObject(cube);

    // Создаем камеру в точке (3, 3, 3) и направляем в сторону начала координат
    renderer::Camera camera{renderer::Point{3, 3, 3}, 225, -35};
    renderer::Scene::CameraId camera_id = scene.PushCamera(camera);

    // Создаем рендерер
    renderer::Renderer renderer;

    // Параметры
    constexpr size_t kWidth = 1280;
    constexpr size_t kHeight = 720;

    // Создаем изображение и рендерим сцену
    renderer::Image image{renderer::Width{kWidth}, renderer::Height{kHeight}};
    // По-умолчанию рисуются только грани
    image = renderer.Render(scene, camera_id, std::move(image));

    // Записываем результат в файл
    SaveToBmp(image, "example_load_obj.bmp");
    return 0;
}

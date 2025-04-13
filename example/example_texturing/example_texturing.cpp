#include <renderer/include_all.hpp>

#include "../BMP_Writer/BMP_Writer.hpp"

int main() {
    // Загружаем объект из файла
    renderer::Object cube = renderer::utils::LoadFile("textured_cube.fbx");

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

    // Создаем источники света
    scene.PushLight(renderer::AmbientLight{});  // фоновый свет
    scene.PushLight(
        renderer::DirectionalLight{.strength = 0.3,
                                   .color = renderer::colors::kRed,
                                   .direction = {-1, -1, -1}});  // красный направленный свет

    scene.PushLight(renderer::PointLight{.strength = 10.0,
                                         .color = renderer::colors::kBlue,
                                         .position = {5, -2, 3}});  // синий точечный источник

    scene.PushLight(
        renderer::SpotLight{.strength = 100.0,
                            .position = {4, -4, 3},
                            .exponent = 10,
                            .direction = {-4, 4, -3}});  // точечный направленный источник

    // Создаем рендерер
    renderer::Renderer renderer;

    // Параметры
    constexpr size_t kWidth = 1280;
    constexpr size_t kHeight = 720;

    // Создаем изображение и рендерим сцену
    renderer::Image image{renderer::Width{kWidth}, renderer::Height{kHeight}};

    image = renderer.Render(scene, camera_id, std::move(image),
                            renderer::Renderer::DRAW_FACETS | renderer::Renderer::ENABLE_LIGHT);

    // Записываем результат в файл
    SaveToBmp(image, "example_texturing.bmp");
    return 0;
}

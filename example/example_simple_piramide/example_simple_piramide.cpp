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
    scene.PushObject(piramide, renderer::transforms::kNoTransforms);

    // Создаем камеру в точке (-1, -1, 1), смотрящую на точку (0.5, 0.5, 0.5)
    renderer::Matrix camera_matrix =
        renderer::transforms::CameraLookAtPoint({-1, -1, 1}, {0.5, 0.5, 0.5});
    renderer::Camera camera{camera_matrix};
    renderer::Scene::CameraId camera_id = scene.PushCamera(camera);

    // Создаем рендерер
    renderer::Renderer renderer;

    // Параметры
    constexpr size_t kWidth = 1280;
    constexpr size_t kHeight = 720;

    // Создаем изображение и рендерим сцену
    renderer::Image image{renderer::Width{kWidth}, renderer::Height{kHeight}};
    image = renderer.Render(scene, camera_id, std::move(image));

    // Записываем результат в файл
    SaveToBmp(image, "example_simple_piramide.bmp");
    return 0;
}

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
    SaveToBmp(image, "example_simple_piramide.bmp");
    return 0;
}

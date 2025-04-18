#include <renderer/include_all.hpp>

#include "../BMP_Writer/BMP_Writer.hpp"

int main() {
    // Создаем вершины
    renderer::Vertex a = {.point = {0, 0, 0}};
    renderer::Vertex b = {.point = {1, 0, 0}};
    renderer::Vertex c = {.point = {0, 1.5, 0}};
    renderer::Vertex d = {.point = {0, 0, 2}};

    // Создаем грани-треугольники из точек. Наружней считается та сторона, относительно которой
    // точки перечисленны против часовой стрелки
    std::vector<renderer::Triangle> fasets = {
        {.vertices = {a, c, b}},
        {.vertices = {b, c, d}},
        {.vertices = {a, b, d}},
        {.vertices = {a, d, c}},
    };

    // Создаем объект из граней
    renderer::Object piramide{fasets};

    // Создаем сцену и добавляем объект, который по-умолчанию устанавливается в начало координат
    renderer::Scene scene;
    scene.PushObject(piramide);

    // Создаем камеру в точке (2, 2, 2) и направляем в сторону начала координат
    renderer::Camera camera{renderer::Point{2, 2, 2}, 225, -25};
    renderer::Scene::CameraId camera_id = scene.PushCamera(camera);

    // Создаем рендерер
    renderer::Renderer renderer;

    // Параметры
    constexpr size_t kWidth = 1280;
    constexpr size_t kHeight = 720;

    // Создаем изображение и рендерим сцену
    renderer::Image image{renderer::Width{kWidth}, renderer::Height{kHeight}};
    // Рисуется каркас пирамиды
    image = renderer.Render(
        scene, camera_id, std::move(image),
        renderer::Renderer::DRAW_EDGES | renderer::Renderer::DISABLE_BACKFACE_CULLING);

    // Записываем результат в файл
    SaveToBmp(image, "example_simple_piramide.bmp");
    return 0;
}

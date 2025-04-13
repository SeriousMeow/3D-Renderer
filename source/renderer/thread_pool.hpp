/**
 * @file
 * @brief Thread pool для ускорения работы библиотеки
 */
#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace renderer {

/**
 * @brief Singleton класс ThreadPool
 */
class ThreadPool {
public:
    /**
     * @brief Получение объекта ThreadPool
     *
     * Возвращает ссылку на ThreadPool. Объект создается при первом запросе. Количество потоков
     * по-умолчанию std::thread::hardware_concurrency, это значение можно менять с помощью
     * ThreadPool::SetThreadsCount. Учитывается только последний вызов перед первым Get
     *
     * @return Ссылка на объект
     */
    static ThreadPool& Get();

    /**
     * @brief Задание количества потоков
     *
     * Задает количество потоков в ThreadPool. Если num_threads = 0, то устанавливает значение по
     * умолчанию
     *
     * @param[in] num_threads Число потоков
     */
    static void SetThreadsCount(const size_t num_threads);

    /**
     * @brief Получение количества потоков
     *
     * Возвращает количество потоков, заданное после последнего вызова ThreadPool::SetThreadsCount
     *
     * @return Число потоков
     */
    static size_t GetThreadsCount();

    /**
     * @brief Добавление задачи
     *
     * Добавляет задачу в очередь для исполнения
     *
     * @param[in] task Задача
     */
    void Enqueue(std::function<void()>&& task);

    /**
     * @brief Ожидание выполнения всех задач
     *
     * Останавливает вызвавший поток до тех пор, пока не опустеет очередь задач
     */
    void WaitAll();

    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool(ThreadPool&& other) = delete;

    ThreadPool& operator=(const ThreadPool& other) = delete;
    ThreadPool& operator=(ThreadPool&& other) = delete;

    ~ThreadPool();

private:
    /**
     * @brief Создание ThreadPool
     *
     * Создает ThreadPool с k_threads потоками
     */
    ThreadPool();

    /**
     * @brief Цикл, исполняемый в каждом потоке
     */
    void WorkerLoop();

    /**
     * @brief Количество потоков
     */
    static size_t k_threads;

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex mutex_;
    std::condition_variable cv_task_;  // ожидание задач
    std::condition_variable cv_done_;  // ожидание опустошения очереди задач

    size_t active_tasks_ = 0;  // число задач в работе

    bool stop_ = false;  // остановка при разрушении
};

}  // namespace renderer

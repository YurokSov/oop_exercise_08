// Шараковский Юрий Дмитриевич. М8О-206Б-19, МАИ. 09.2020. 
// Лабораторная работа №8. Условие:
//
// Создать приложение, которое будет считывать из стандартного ввода данные фигур, согласно варианту задания, выводить их характеристики на экран и записывать в файл. Фигуры могут задаваться как своими вершинами, так и другими характеристиками (например, координата центра, количество точек и радиус).
// Программа должна:
// 1.	Осуществлять ввод из стандартного ввода данных фигур, согласно варианту задания;
// 2.	Программа должна создавать классы, соответствующие введенным данных фигур;
// 3.	Программа должна содержать внутренний буфер, в который помещаются фигуры. Для создания буфера допускается использовать стандартные контейнеры STL. Размер буфера задается параметром командной строки. Например, для буфера размером 10 фигур: oop_exercise_08 10
// 4.	При накоплении буфера они должны запускаться на асинхронную обработку, после чего буфер должен очищаться;
// 5.	Обработка должна производиться в отдельном потоке;
// 6.	Реализовать два обработчика, которые должны обрабатывать данные буфера:
// a.	Вывод информации о фигурах в буфере на экран;
// b.	Вывод информации о фигурах в буфере в файл. Для каждого буфера должен создаваться файл с уникальным именем.
// 7.	Оба обработчика должны обрабатывать каждый введенный буфер. Т.е. после каждого заполнения буфера его содержимое должно выводиться как на экран, так и в файл.
// 8.	Обработчики должны быть реализованы в виде лямбда-функций и должны хранится в специальном массиве обработчиков. Откуда и должны последовательно вызываться в потоке – обработчике.
// 9.	В программе должно быть ровно два потока (thread). Один основной (main) и второй для обработчиков;
// 10.	В программе должен явно прослеживаться шаблон Publish-Subscribe. Каждый обработчик должен быть реализован как отдельный подписчик.
// 11.	Реализовать в основном потоке (main) ожидание обработки буфера в потоке-обработчике. Т.е. после отправки буфера на обработку основной поток должен ждать, пока поток обработчик выведет данные на экран и запишет в файл.
//
// Вариант 25: треугольник, квадрат, прямоугольник

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>

#include <string>
#include <vector>
#include <list>
#include <array>

#include <mutex>
#include <condition_variable>
#include <thread>

#include <functional>
#include <algorithm>

#include "figure.hpp"

using FigureBuffer = std::list<std::shared_ptr<Figure>>;

class Subscriber {
public:
    bool isActive = true;
    bool isAwaken = false;

    void operator()(FigureBuffer figures) {
        this->Handler(figures);
    }

    std::function<void(FigureBuffer)> Handler;
};

class Publisher {
public:

    Publisher(uint32_t max_size) {
        MAX_SIZE = max_size;
    }
    ~Publisher() {
        Release();
    }

    Publisher(const Publisher& other) = delete;
    Publisher& operator=(const Publisher& other) = delete;
    Publisher(Publisher&& other) = delete;
    Publisher& operator=(Publisher&& other) = delete;

    void Subscribe(Subscriber& sub) {
        this->subscribers.emplace_back(&sub);
    }

    void Release() {
        std::unique_lock<std::mutex> lock(mx_release);
        for (auto& sub : subscribers) {
            sub->isActive = false;
        }
        //this->subscribers.clear();
        this->End = true;
        cv_process.notify_one();
    }

    void Append(Figure* f) {
        std::unique_lock<std::mutex> lock(mx_buffer);
        cv_append.wait(lock, [&]() { return buffer.size() < MAX_SIZE; });
        buffer.emplace_back(f);
        if (buffer.size() == MAX_SIZE) {
            for (auto& sub : subscribers) {
                sub->isAwaken = true;
            }
            cv_process.notify_one();
        }
    }

    void ClearBuffer() {
        std::lock_guard<std::mutex> lock(mx_buffer);
        buffer.clear();
        cv_append.notify_all();
    }

    void WaitForEvent() {
        std::unique_lock<std::mutex> lock(mx_release);
        cv_process.wait(lock, [&]() { return buffer.size() == MAX_SIZE || this->End; });
    }

    FigureBuffer CopyBuffer() {
        std::lock_guard<std::mutex> lock(mx_buffer);
        return buffer;
    }

private:
    uint32_t MAX_SIZE;
    FigureBuffer buffer;

    std::condition_variable cv_append;
    std::condition_variable cv_process;
    std::mutex mx_buffer;
    std::mutex mx_release;

    bool End = false;

    std::vector<Subscriber*> subscribers;
};

void doProcessing(Publisher& pub, std::array<Subscriber, 2>& sub) {
    bool alive;
    bool doneWork;
    do {
        alive = false;
        doneWork = false;
        //std::cout << "\tWAITING" << std::endl;
        pub.WaitForEvent();
        for (auto& subscr : sub) {
            if (subscr.isAwaken) {
                doneWork = true;
                subscr.isAwaken = false;
                subscr(pub.CopyBuffer());
            }
        }
        if (doneWork) {
            pub.ClearBuffer();
        }
        alive = std::any_of(sub.begin(), sub.end(), [](Subscriber& elem) { return elem.isActive; });
    } while (alive);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::terminate();
    }
    Publisher pub(std::atoi(argv[1]));

    std::array<Subscriber, 2> sub;
    uint32_t counter = 0;

    sub.at(0).Handler = [](FigureBuffer buffer) {
        for (auto& elem : buffer) {
            std::cout << *(elem.get()) << std::endl;
        }
    };
    pub.Subscribe(sub.at(0));

    sub.at(1).Handler = [&counter](FigureBuffer buffer) {
        std::fstream file;
        file.open(std::to_string(counter).c_str(), std::fstream::out);
        for (auto& elem : buffer) {
            file << *(elem.get()) << std::endl;
        }
        file.close();
        ++counter;
    };
    pub.Subscribe(sub.at(1));

    std::cout << "1. triangle\n2. square\n3. rectangle" << std::endl;

    std::thread consumers_thread(doProcessing, std::ref(pub), std::ref(sub));

    uint32_t option;
    while (std::cin >> option) {
        Figure* fig;
        if (option == 1) {
            fig = new Triangle();
        }
        else if (option == 2) {
            fig = new Square();
        }
        else if (option == 3) {
            fig = new Rectangle();
        }
        else {
            throw std::runtime_error("Wrong figure type!");
        }
        std::cin >> *fig;
        pub.Append(fig);
    }
    pub.Release();
    consumers_thread.join();

    std::cout << "Goodbye!" << std::endl;
    return 0;
}
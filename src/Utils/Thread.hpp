#ifndef TP_TALLER_DE_PROGRAMACION_FIUBA_THREAD_HPP
#define TP_TALLER_DE_PROGRAMACION_FIUBA_THREAD_HPP

#include <pthread.h>
#include <string>

class Thread {
    public:
        Thread() = default;
        virtual ~Thread() = default;
        void empezarHilo(const std::string& nombreHilo);
        void join(const std::string& nombreHilo) const;
        void dormirHilo();
        void despertarHilo();
        virtual void ejecutar() = 0;
        static void* ejecutar_helper(void* ptr);
        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;
        Thread(Thread&& otroThread) noexcept ;
        Thread& operator=(Thread&& otroThread) noexcept ;
    private:
        pthread_t hilo{};
        pthread_cond_t variableCondicional=PTHREAD_COND_INITIALIZER;
        pthread_mutex_t mutexHilo = PTHREAD_MUTEX_INITIALIZER;
    protected:
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
};


#endif //TP_TALLER_DE_PROGRAMACION_FIUBA_THREAD_HPP

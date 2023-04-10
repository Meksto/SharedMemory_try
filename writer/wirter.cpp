#include <iostream>
#include <sys/shm.h>
#include <sys/sem.h>
#include <cstring>

// Уникальный ключ для разделяемой памяти и семафоров
#define SHM_KEY 1234
#define SEM_KEY 5678

// Структура которая будет помещена в разделяемую память
struct Note {
    char name[256];
    int age;
};

int main() {
    // Создаем идентификаторы для разделяемой памяти и семафоров
    int shm_id = shmget(SHM_KEY, 1024, IPC_CREAT | 0666);
    int sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);

    // Проверяем успешность создания разделяемой памяти и семафоров
    if (shm_id == -1 || sem_id == -1) {
        perror("Failed to create shared-memory or semaphore");
        exit(1);
    }

    // Получаем указатель на разделяемую память
    Note* sharedNote = (Note*)shmat(shm_id, nullptr, 0);
    if (sharedNote == (Note*)-1) {
        perror("Failed to get pointer to shared memory");
        exit(1);
    }

    // Запрашиваем у пользователя ввод строки и заполняем структуру Note
    Note note;
    std::cout << "Input name: ";
    std::cin.getline(note.name, 50);
    std::cout << "Input age: ";
    std::cin >> note.age;

    // Копируем строку в разделяемую память
     memcpy(sharedNote, &note, sizeof(Note));

    // Отсоединяемся от разделяемой памяти
    shmdt(sharedNote);

    int c = 999;
    while (c != 0) {
        std::cout << "Print 0 to quit: ";
        std::cin >> c;
    }

    // Увеличиваем значение семафора на 1
    sembuf sem_op;
    sem_op.sem_num = 0;
    sem_op.sem_op = 1;
    sem_op.sem_flg = 0;
    semop(sem_id, &sem_op, 1);

    return 0;
}

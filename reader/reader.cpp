#include <iostream>
#include <sys/shm.h>
#include <sys/sem.h>
#include <cstring>
#include <unistd.h>

// Уникальный ключ для разделяемой памяти и семафоров
#define SHM_KEY 1234
#define SEM_KEY 5678

struct Note {
    char name[256];
    int age;
};

int main() {
    // Создаем идентификаторы для разделяемой памяти и семафоров
    int shm_id = shmget(SHM_KEY, 1024, 0666);
    int sem_id = semget(SEM_KEY, 1, 0666);

    // Проверяем успешность получения разделяемой памяти и семафоров
    if (shm_id == -1 || sem_id == -1) {
        perror("Failed to get shared-memory or semaphore");
        exit(1);
    }

    
    bool quitnum = 1;
    while (quitnum != 0) {
        sleep(1);

        // Получаем указатель на разделяемую память
        Note* sharedNote = (Note*)shmat(shm_id, nullptr, 0);
        if (sharedNote == (Note*)-1) {
            perror("Failed to get pointer to shared memory");
            exit(1);
        }
        // Ожидаем, пока значение семафора станет равным 1
        std::cout << "Waiting for access to shared memory" << std::endl;
        sembuf sem_op;
        sem_op.sem_num = 0;
        sem_op.sem_op = -1;
        sem_op.sem_flg = 0;
        semop(sem_id, &sem_op, 1);


        // Считываем строку из разделяемой памяти и выводим ее в консоль
        std::cout << "Reading from shared memory:" << std::endl;
        std::cout << "Name: " << sharedNote->name << std::endl;
        std::cout << "Age: " << sharedNote->age << std::endl;

        // Получение команды на выход
        std::cout << "Enter '0' to quit: ";
        std::cin >> quitnum;

        // Отсоединяемся от разделяемой памяти
        shmdt(sharedNote);

        // Увеличиваем значение семафора на 1
        sem_op.sem_num = 0;
        sem_op.sem_op = 1;
        sem_op.sem_flg = 0;
        semop(sem_id, &sem_op, 1);

    }

    // Удаляем разделяемую память
    shmctl(shm_id, IPC_RMID, nullptr);

    // Удаляем семафор
    semctl(sem_id, 0, IPC_RMID);

    return 0;
}

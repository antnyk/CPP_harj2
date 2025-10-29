// PASIN ESIMERKKI KOODI

/*
Jaettu muistialue (shmget + shmat): Vanhempi kirjoittaa numeron, lapsi kasvattaa sitä ja
vanhempi lukee takaisin.
*/

//Yksinkertaisena Esimerkkinä:
//Jaettu muisti prosessien välillä, kriittisen koodin suojaus käyttäen semaforia

#include <iostream>
#include <sys/shm.h>
#include <fcntl.h> //S_IRUSR | S_IWUSR määrittelyt
#include <unistd.h> //fork() määrittely
#include <sys/wait.h>
#include <semaphore.h>

//kaikkialla tarvittavat tunnisteet globaalilla alueella
const char* SEM_NAME = "/mySem";
int segment_id;
const int size = 1024; // varataan kilotavu jaettua muistia

int main() {
    //merkitään jaettu muisti yleisellä alueella (parentissa)
    //  jotta muistin tunnus on kaikkien prosessien tiedossa
    segment_id = shmget(IPC_PRIVATE, size, S_IRUSR | S_IWUSR);

    //luodaaan jaettu muisti parentissa:
    int* memoryPointer {nullptr}; // pointer jolla osoitetaan muistiin parentissa
    memoryPointer = (int*) shmat (segment_id, NULL, 0); // annetaan jaettu muisti parentin käyttöön kiinnittämällä pointteri
    int lukuParent = 50;
    if (memoryPointer) *memoryPointer = lukuParent;
    std::cout << "Vanhempi: kirjoitettu arvo = " << *memoryPointer << std::endl;
    // luodaan semafori käyttöä varten yleisellä alueella (parentissa)
    sem_unlink(SEM_NAME);
    sem_t* sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1);
    if (sem == SEM_FAILED) { perror("sem_open"); return 1; }
    // luodaan lapsiprosessi
    pid_t pid = fork();
    if (pid == -1) { perror("fork"); return 1; }

    if (pid == 0) {
        int* childPtr {nullptr};
        // Lapsi: lue ensin vanhemman asettama arvo ja kasvata sitten
        sleep(5); //pieni viive jotta huomataan lapsen syntyneen
        // uudelle prosessille pitää kertoa jaetusta muistista että se voi käyttää sitä
        childPtr = (int*) shmat (segment_id, NULL, 0); //pointer jolla osoitetaan muistiin lapsessa
        std::cout << "Lapsi: luettu arvo = " << *childPtr << std::endl;
        // esimerkkinä semaforin käyttö
        if (childPtr) {
            sem_wait(sem); //jos semafori vapaa - voit edetä, muuten odota vapautumista
            *childPtr += 50; //turavalista päivittää
            sem_post(sem); //siganloi että semafori vapaa
            std::cout << "Lapsi: kirjoitettu arvo = " << *childPtr << std::endl;
        }
        return 0; //lapsi poistuu
    } else {
        // Vanhempi odottaa että lapsi on valmis, alla odotetaan mitä tahansa valmistuvaa lasta
        wait(nullptr);
        std::cout << "Vanhempi: luettu arvo = " << *memoryPointer << std::endl;
    }
    // Siivotaan sameforit, vain vanhempi elossa täällä
    sem_close(sem); // ei voi käyttää tämän jälkeen
    sem_unlink(SEM_NAME); // nimi ei yhdisty enää semaforeihin
    // Otetaan jaettu muisti pois jaosta
    std::cout << "making waves" << std::endl;
    shmdt(memoryPointer);
    // Poistetaan jaettu muisti
    shmctl(segment_id, IPC_RMID, nullptr);
    return 0;
}

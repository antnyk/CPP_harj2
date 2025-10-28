/* Kääntäminen g++ -w lipulla tällä hetkellä */

// HUOM! Kirjoitan kaiken englanniksi kun on paljon helpompaa henkilokohtaisesti tyoskennella nain

/* Harjoituksen 2 ohjelmarunko */
/* Tee runkoon tarvittavat muutokset kommentoiden */
/* Pistesuorituksista kooste tähän alkuun */
/* 2p = täysin tehty suoritus, 1p = osittain tehty suoritus */
/* Tarkemmat ohjeet Moodlessa */
/* Lisäohjeita, vinkkejä ja apuja löytyy koodin joukosta */
/* OPISKELIJA: merkityt kohdat eritoten kannattaa katsoa huolella */

//peruskirjastot mitä tarvii aika lailla aina kehitystyössä
//OPISKELIJA: lisää tarvitsemasi peruskirjastot
#include <iostream>
#include <vector>
#include <string>

//rinnakaisuuden peruskirjastot
//OPISKLEIJA: lisää tarvittaessa lisää kirjastoja, muista käyttää -pthread lippua käännöksessä ja tarvittaessa -lrt lippua myös
//Huom! Rinnakkaisuusasioista on eri versioita (c++, POSIX/pthread, SystemV)
//Kaikkien käyttö on sallittua
#include <sys/shm.h> //jaetun muistin kirjasto: shmget(), shmmat()
#include <fcntl.h> //S_IRUSR | S_IWUSR määrittelyt esim jaettua muistia varten
#include <unistd.h> //esim fork() määrittely ja muut prosessimäärittelyt
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h> //pthread perussäiemäärittelyt
using namespace std;

//kaikkialla tarvittavat tunnisteet  määritellään globaalilla alueella
//OPISKELIJA: muistele harjoituksista miten eri asiat määriteltiin
//OPISKELIJA: tehtäväsi on sijoittaa itse labyrinttikin jaettuun muistialueeseen ja käyttää sitä sieltä
//->eli:labyrintin käyttö globaalilta alueelta jaetun muistin käyttöön
//vinkki: kannattaa määritellä pointteri nimeltä labyrintti kaksiuloitteiseen taulukkoon jolloin nykytoteutus toimii sellaisenaan
//parent/main alustaa jaettun muistin (eli kirjoittaa sinne) nuo labyrintin alkiot
//poista labyrintti kokonaan globaalilta alueelta, ohjelman pitäisi toimia
//mieti harjoituksista opitun perusteella paljonko labyrintti vähintään tarvitsee jaettua muistia
//tee kaikki ratkaisut niin että ohjelma toimii millä tahansa labyrintilla

//definet voi jättää globaalille alueelle, ne on sitten tiedossa koko tiedostossa
//#define KORKEUS 100 //rivien määrä alla
//#define LEVEYS 100 //sarakkaiden määrä alla
//int labyrintti[KORKEUS][LEVEYS] = {
//    {1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
//    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,2,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,2,0,0,1,0,0,0,1,0,0,2,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,1},
//    {1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,1},
//    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,1,2,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,2,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,2,0,0,0,0,0,0,1,0,0,0,0,2,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,1},
//    {1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1},
//    {1,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,1,2,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,2,0,0,1,1},
//    {1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,1},
//    {1,0,0,0,1,0,1,2,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,2,0,0,1,0,0,2,0,0,1,0,0,2,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,0,2,0,2,0,0,0,0,1,0,1,0,0,2,0,0,0,0,1,0,0,0,1,1},
//    {1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1},
//    {1,0,1,0,0,0,1,0,1,0,0,2,0,0,0,0,1,0,0,0,0,0,0,2,0,2,0,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,2,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,2,0,0,1,0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,2,0,0,1,0,1,0,0,0,1,1},
//    {1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1},
//    {1,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,2,0,0,0,0,0,0,0,2,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,1,0,0,2,1,0,0,0,1,0,0,2,0,0,1,0,0,0,1,0,0,0,1,1},
//    {1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1},
//    {1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,2,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0,1,0,0,2,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1},
//    {1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1},
//    {1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,2,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,2,1,0,0,0,1,0,0,0,1,0,0,2,1,0,0,2,0,2,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,1,1},
//    {1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1},
//    {1,0,1,0,0,2,0,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,2,0,0,1,2,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,1,0,0,2,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,1,0,1,1},
//    {1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1},
//    {1,2,0,0,1,0,0,0,0,0,1,2,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,2,0,0,1,0,0,0,1,0,0,2,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,2,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,1,2,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1},
//    {1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1},
//    {1,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,2,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,2,1,2,0,0,0,0,1,0,1,0,1,1},
//    {1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,1},
//    {1,0,1,0,1,0,1,0,0,0,0,0,0,2,1,0,1,0,0,2,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,2,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,1,0,0,2,0,0,1,1},
//    {1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1},
//    {1,2,0,0,1,0,0,0,0,2,0,0,0,0,1,0,0,2,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,2,0,0,1,0,0,2,0,0,1,0,0,0,1,0,0,2,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,2,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,2,1,2,0,0,0,0,0,0,1,1},
//    {1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1},
//    {1,0,1,0,0,2,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,2,1,0,0,0,1,0,0,0,1,0,0,2,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,1,1},
//    {1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,1},
//    {1,0,1,0,0,0,1,0,1,0,0,2,0,0,1,0,0,0,1,2,0,0,1,0,1,0,0,0,0,0,1,0,1,0,1,0,0,2,0,0,1,0,1,0,1,0,0,0,0,2,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,2,0,0,1,0,0,0,0,2,1,0,1,0,0,0,1,2,0,0,1,1},
//    {1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1},
//    {1,0,0,0,1,0,1,0,1,0,0,2,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,2,0,0,1,0,0,2,1,0,1,0,1,0,1,0,0,0,0,0,1,0,1,0,1,0,0,2,1,0,1,0,0,2,1,0,0,0,1,1},
//    {1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1},
//    {1,0,0,0,1,2,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,0,2,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,1},
//    {1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1},
//    {1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,0,2,0,0,0,2,0,0,1,0,0,0,1,0,0,0,0,2,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,2,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1},
//    {1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1},
//    {1,0,1,0,0,0,1,0,0,0,0,2,0,0,1,0,0,0,1,0,0,2,1,0,1,0,0,0,0,2,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,2,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,1,0,1,2,0,0,1,0,0,2,0,0,1,0,0,0,1,1},
//    {1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1},
//    {1,2,0,0,1,0,0,0,0,0,1,0,0,0,0,2,0,0,1,0,0,0,1,2,0,0,0,0,1,0,0,0,0,0,0,0,0,2,0,0,1,0,1,0,1,2,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,1,2,0,0,1,0,0,0,1,1},
//    {1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1},
//    {1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,1,2,0,0,1,0,0,2,0,0,1,0,0,2,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1},
//    {1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,1},
//    {1,0,1,0,0,2,1,2,0,0,1,0,1,0,0,0,1,0,0,2,0,0,1,0,0,0,1,0,1,0,0,2,0,0,0,0,1,0,1,2,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,2,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,1,0,1,0,1,1},
//    {1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1},
//    {1,0,0,0,1,0,1,0,1,0,0,2,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,2,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,2,1,1},
//    {1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1},
//    {1,0,0,0,0,2,1,0,0,0,1,0,0,0,1,0,0,2,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,2,1,0,1,0,0,0,1,0,0,0,1,0,1,2,0,0,1,0,0,0,1,0,0,0,1,2,0,0,1,0,0,0,1,0,1,0,1,0,0,2,0,0,0,2,1,0,1,2,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1},
//    {1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1},
//    {1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,2,0,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,2,1,0,0,0,0,0,1,0,1,2,0,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,2,0,0,1,0,0,0,1,0,0,0,1,0,1,1},
//    {1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1},
//    {1,0,1,0,0,2,1,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,2,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,2,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,1,0,0,0,1,0,0,0,1,1},
//    {1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,1},
//    {1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,2,0,2,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,2,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,1},
//    {1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1},
//    {1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,2,1,0,1,0,1,2,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,2,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1},
//    {1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1},
//    {1,0,0,0,1,0,0,2,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,2,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,2,1,0,1,0,1,1},
//    {1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1},
//    {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,2,1,0,0,2,0,2,0,2,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,2,0,0,0,0,1,0,1,2,0,0,0,0,1,0,1,0,0,2,0,0,1,0,0,0,1,2,0,0,1,0,0,2,0,0,1,0,1,1},
//    {1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1},
//    {1,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,2,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,2,0,0,1,0,1,0,1,0,0,0,1,2,0,0,1,0,0,0,1,0,0,0,1,0,1,2,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,2,1,1},
//    {1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1},
//    {1,2,0,0,1,0,1,2,0,2,0,0,1,0,1,0,0,0,0,2,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,2,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,2,1,0,1,0,1,0,1,1},
//    {1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1},
//    {1,0,0,0,1,0,0,2,0,0,1,0,1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,0,0,2,0,0,1,0,1,2,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,2,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,1},
//    {1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1},
//    {1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,2,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,2,0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,2,0,0,1,0,0,0,1,0,0,0,1,0,1,1},
//    {1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1},
//    {1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,2,0,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,0,2,0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,1},
//    {1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1},
//    {1,0,1,0,0,0,1,2,0,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,2,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,2,0,0,1,0,0,0,0,2,1,0,0,0,0,2,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,1},
//    {1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,1},
//    {1,0,1,0,0,2,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,2,1,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,0,2,0,0,1,0,1,1},
//    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1},
//    {1,0,1,0,1,0,1,0,0,2,0,0,0,0,1,0,1,2,0,0,1,2,0,0,1,0,0,2,0,0,0,0,1,0,0,0,1,0,0,2,1,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,2,0,0,1,0,1,0,0,0,1,0,0,0,1,1},
//    {1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,1},
//    {1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,2,0,2,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,1,0,1,0,1,0,0,2,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,2,1,0,1,0,0,2,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,1},
//    {1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1},
//    {1,2,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,2,1,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,2,1,0,0,2,1,0,0,0,0,0,1,0,1,2,0,0,1,1},
//    {1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,1},
//    {1,0,1,2,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,0,2,1,0,1,0,1,0,1,0,0,2,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,2,0,0,0,0,0,0,1,0,0,2,0,2,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1},
//    {1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,1},
//    {1,0,0,0,1,0,1,0,1,0,0,2,1,0,0,2,0,0,0,0,1,0,1,0,1,0,1,2,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,2,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1},
//    {1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1},
//    {1,2,0,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,2,1,2,0,0,0,2,0,0,0,0,1,0,0,0,0,0,1,2,0,2,0,0,0,0,1,0,0,2,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,2,0,0,0,0,0,2,0,0,0,0,0,0,1,2,0,2,1,1},
//    {1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1},
//    {1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,1,1},
//    {1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1},
//    {1,0,1,0,1,0,0,0,1,0,1,0,0,0,0,2,0,2,0,0,1,0,1,0,1,0,0,0,0,2,1,0,0,0,0,0,1,0,1,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,2,1,0,0,0,1,0,0,0,1,2,0,0,1,0,1,0,1,0,0,0,1,0,1,1},
//    {1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1},
//    {1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,1,2,0,0,1,0,0,0,1,0,0,0,0,0,0,2,0,2,0,0,1,0,0,0,1,0,0,0,0,2,0,0,1,0,0,2,0,0,1,0,0,0,0,2,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,1,1},
//    {1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,1},
//    {1,0,1,0,1,0,1,0,1,0,0,2,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,2,0,0,0,0,1,2,0,0,1,0,0,0,1,0,0,0,0,2,1,0,0,2,0,0,1,0,0,2,0,0,1,0,1,1},
//    {1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1},
//    {1,0,1,0,1,0,1,0,0,0,0,0,1,2,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,2,0,2,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,1},
//    {1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1},
//    {1,0,0,2,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,2,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,2,1,1},
//    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1},
//    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,1,1},
//};
//apuja: voit testata ratkaisujasi myös alla olevalla yksinkertaisemmalla labyrintilla

// this is how you read the labyrinth x and y
//      0   1   2   3   ...    99
//  99
//  98
//  97
//  96
//  ...
//  0

#define KORKEUS 7
#define LEVEYS 7
int labyrintti[KORKEUS][LEVEYS] = {
    {1,3,1,1,1,1,1},
    {1,0,1,0,1,0,4},
    {1,0,1,0,1,0,1},
    {1,2,0,2,0,2,1},
    {1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1},
    {1,1,1,1,1,1,1},
};

//karttasijainnin tallettamiseen käytettävä rakenne, luotaessa alustuu vasempaan alakulmaan
//HUOM! ykoordinaatti on peilikuva taulukon rivi-indeksiin
//PasiM: TODO, voisi yksinkertaistaa että ykoord olisi sama kuin rivi-indeksi
struct Sijainti {
    int ykoord {0};
    int xkoord {0};
};

//rotan liikkeen suunnan määrittelyyn käytettävä rakenne
//huom! suunnat ovat absoluuttisia kuin kompassissa
//UP -> ykoord++ (yindex--)
//DOWN -> ykoord-- (yindex++)
//LEFT -> xkoord--
//RIGHT -> xkoord++
//DEFAULT -> suunta tuntematon
enum LiikkumisSuunta {
    UP, DOWN, LEFT, RIGHT, DEFAULT
};

//tämän hetken toteutuksessa tämä on tarpeeton rakenne, voisi käyttää rotan omassa kartan ymmärryksen kasvatuksessa
enum Ristausve {
    WALL = 1,
    OPENING = 0
};

//tämä rakenne on jokaisesta risteyksestä jokaiseen suuntaan omansa
//VINKKI: tutkituksi merkittyyn/merkittävään suuntaan ei rotta koskaan lähde enää tutkimaan ;)
struct Suunta {
    Ristausve jatkom; //tutkituille suunnille tämä on määritelty OPENING arvolle
    bool tutkittu {false}; //alkuarvona tutkimaton
};

//TÄRKEIN kaikista rakenteista, kertoo miten risteys on opittu juuri kyseisen rotan taholta
//tutkittavana - arvo kertoo minne suuntaan juuri tämä rotta viimeksi lähtenyt ko risteyksestä tutkimaan
struct Ristaus {
    Sijainti kartalla;
    Suunta up;
    Suunta down;
    Suunta left;
    Suunta right;
    LiikkumisSuunta tutkittavana = DEFAULT; //alkuarvo, kertoo while-silmukan alussa olevalle risteyskoodille että rotta tuli ensimmäistä kertaa ko risteykseen
};

//poikkeuksenheittämistä varten, ei käytössä tällä hetkellä
//PasiM, TODO: poikkeukset
struct Karttavirhe {
    int koodi {0};
    string msg;
};


//tämä on esittely aloitusalgoritmifunktiolle mitä siis kutsutaan oli kyseessä prosessi eli säietoteutus
//tällä hetkellä palauttaa kyseisen rotan liikkujen määrän labyrintin selvittämiseksi
//OPISKELIJA: yhtenä mielenkiintoisena haasteena voisi olla liikkujen määrän optimointi rottien yhteistyötä kehittämällä
//int aloitaRotta();

//OPISKELIJA: lisää tarvittavat muut funktioesittelyt tähän niin koodin järjestykellä tiedostossa ei ole merkitystä


//Rotta-ohjelman algoritmimäärittelyt
//näitä kutsutaan-oli rinnakkaisuus mitä vain
//OPISKELIJA: kun siirrät labyrintin jaettuun muistiin, siihen osoittavaa pointteria varmaankin pitää kuljetella niihin funktioihin missä labyrinttia tutkitaan
//VINKKI: määrittele pointteri niin että olemassa olevissa algoritmeissa oleva koodi toimii sellaisenaan

//etsii kartasta jotain spesifistä, palauttaa sen koordinaatit
Sijainti etsiKartasta(int kohde, int (*labyrintti)[LEVEYS]){
    Sijainti kartalla;
    for (int y = 0; y<KORKEUS ; y++) {
        for (int x = 0; x<LEVEYS ; x++){
            if (labyrintti[y][x] == kohde) {
                kartalla.xkoord = x;
                kartalla.ykoord = KORKEUS-1-y;
                return kartalla;
            }
        }
    }
    return kartalla;
}

//etsitään labyrintin aloituskohta, merkitty 3:lla
Sijainti findBegin(int (*labyrintti)[LEVEYS]){
    Sijainti alkusijainti;
    alkusijainti = etsiKartasta(3, labyrintti);
    return alkusijainti;
}

//TÄRKEÄÄ: reitti on risteyspino - sen muutokset täytyy liikkua tiedonvälityksen mukana, siksi liikkuu viittaus siihen
//OPISKELIJA: rotan liikkumislogiikkaan ei (välttämättä) tarvitse kajota, ainoastaan päätöksentekoon risteyksiin liittyen
//OPISKELIJA: väistämislogiikan tekeminen voi olla poikkeus ylläolevaan
//OHJE: taulukon indeksit ja koordinaatit ovat y-suunnassa peilikuvana
//eli: ykoordinaatti alkaa alhaalta kasvaa ylöspäin, yindeksi alkaa ylhäältä kasvaa alaspäin
//OPISKLEIJA: käytä siis labyrinttia jaetusta muistista ja tee tarvittaessa siihen liittyvät muutokset

//tutkitaan mitä nykypaikan yläpuolella on, prevDir kertoo minkä suuntainen oli viimeisin kyseisen rotan liikku
bool tutkiUp(Sijainti nykysijainti, auto& reitti, LiikkumisSuunta prevDir, int (*labyrintti)[LEVEYS]){
    int yindex = KORKEUS-1-nykysijainti.ykoord-1;
    if (yindex < 0) return false; //ulos kartalta - ei mahdollista
    if (labyrintti[yindex][nykysijainti.xkoord] == 1) return false; //labyrintin seinä
    //tulossa uuteen ristaukseen, siihen siirrytään aina silmukan lopuksi nykytoteutuksessa
    if (labyrintti[yindex][nykysijainti.xkoord] == 2 && prevDir != DOWN) {
        Ristaus ristaus;
        ristaus.kartalla.ykoord = nykysijainti.ykoord+1;
        ristaus.kartalla.xkoord = nykysijainti.xkoord;
        ristaus.down.tutkittu = true;
        ristaus.down.jatkom = OPENING;
        reitti.push_back(ristaus); //lisätään risteys pinon päällimmäiseksi
// debuggausapuja..:
//        for (auto rist : reitti){
//            cout << "Risteys: " << rist.kartalla.ykoord << "," << rist.kartalla.xkoord << endl;
//        }
        return true;
    }
    return true;
}
//..alapuolella
bool tutkiDown(Sijainti nykysijainti, auto& reitti, LiikkumisSuunta prevDir, int (*labyrintti)[LEVEYS]){
    int yindex = KORKEUS-1-nykysijainti.ykoord+1;
    if (yindex > KORKEUS-1) return false; //ulos kartalta - ei mahdollista
    if (labyrintti[yindex][nykysijainti.xkoord] == 1) return false;
    //tulossa uuteen ristaukseen
    if (labyrintti[yindex][nykysijainti.xkoord] == 2 && prevDir != UP){
        Ristaus ristaus;
        ristaus.kartalla.ykoord = nykysijainti.ykoord-1;
        ristaus.kartalla.xkoord = nykysijainti.xkoord;
        ristaus.up.tutkittu = true;
        ristaus.up.jatkom = OPENING;
        reitti.push_back(ristaus);
//        for (auto rist : reitti){
//            cout << "Risteys: " << rist.kartalla.ykoord << "," << rist.kartalla.xkoord << endl;
//        }
        return true;
    }
    return true;
}

//..vasemmalla
bool tutkiLeft(Sijainti nykysijainti, auto& reitti, LiikkumisSuunta prevDir, int (*labyrintti)[LEVEYS]){
    int yindex = KORKEUS-1-nykysijainti.ykoord;
    int xindex = nykysijainti.xkoord-1;
    if (xindex < 0) return false; //ulos kartalta - ei mahdollista
    if (labyrintti[yindex][xindex] == 1) return false;
    //tulossa uuteen ristaukseen
    if (labyrintti[yindex][xindex] == 2 && prevDir != RIGHT){
        Ristaus ristaus;
        ristaus.kartalla.ykoord = nykysijainti.ykoord;
        ristaus.kartalla.xkoord = nykysijainti.xkoord-1;
        ristaus.right.tutkittu = true;
        ristaus.right.jatkom = OPENING;
        reitti.push_back(ristaus);
//        for (auto rist : reitti){
//            cout << "Risteys: " << rist.kartalla.ykoord << "," << rist.kartalla.xkoord << endl;
//        }
        return true;
    }
    return true;
}

//..oikealla
bool tutkiRight(Sijainti nykysijainti, auto& reitti, LiikkumisSuunta prevDir, int (*labyrintti)[LEVEYS]){
    int yindex = KORKEUS-1-nykysijainti.ykoord;
    int xindex = nykysijainti.xkoord+1;
    if (xindex > LEVEYS) return false; //ulos kartalta - ei mahdollista
    if (labyrintti[yindex][xindex] == 1) return false;
    //tulossa uuteen ristaukseen
    if (labyrintti[yindex][xindex] == 2 && prevDir != LEFT){
        Ristaus ristaus;
        ristaus.kartalla.ykoord = nykysijainti.ykoord;
        ristaus.kartalla.xkoord = nykysijainti.xkoord+1;
        ristaus.left.tutkittu = true;
        ristaus.left.jatkom = OPENING;
        reitti.push_back(ristaus);
//        for (auto rist : reitti){
//            cout << "Risteys: " << rist.kartalla.ykoord << "," << rist.kartalla.xkoord << endl;
//        }
        return true;
    }
    return true;
}

//tämä funktio palauttaa aina seuraavan lähtösuunnan ilman lisäehtoja
//OPISKELIJA: älä muuta tätä funktiota suoraan vaan tee omille mahdollisille lisäehdoille oma(t) funktio(t) loogisesti oikeisiin paikkoihin
LiikkumisSuunta findNext(bool onkoRistaus, Sijainti nykysijainti, LiikkumisSuunta prevDir, auto& reitti, int (*labyrintti)[LEVEYS]){
    if (!onkoRistaus) {
        if (tutkiLeft(nykysijainti, reitti, prevDir, labyrintti) && prevDir != RIGHT){
        return LEFT;
        }
        if (tutkiUp(nykysijainti, reitti, prevDir, labyrintti) && prevDir != DOWN){
        return UP;
        }
        if (tutkiDown(nykysijainti, reitti, prevDir, labyrintti) && prevDir != UP){
        return DOWN;
        }
        if (tutkiRight(nykysijainti, reitti, prevDir, labyrintti) && prevDir != LEFT){
        return RIGHT;
        }
        return DEFAULT; //UMPIKUJA - palaa viimeisimpään risteykseen returnin jälkeen
    }
    else if (onkoRistaus) {
        if (tutkiLeft(nykysijainti, reitti, prevDir, labyrintti) && reitti.back().tutkittavana != LEFT && !reitti.back().left.tutkittu){
        return LEFT;
        }
        if (tutkiUp(nykysijainti, reitti, prevDir, labyrintti) && reitti.back().tutkittavana != UP && !reitti.back().up.tutkittu){
        return UP;
        }
        if (tutkiDown(nykysijainti, reitti, prevDir, labyrintti) && reitti.back().tutkittavana != DOWN && !reitti.back().down.tutkittu){
        return DOWN;
        }
        if (tutkiRight(nykysijainti, reitti, prevDir, labyrintti) && reitti.back().tutkittavana != RIGHT && !reitti.back().right.tutkittu){
        return RIGHT;
        }
        return DEFAULT; //palatun ristauksen kaikki suunnat käyty,return jälkeen risteyksen voi poistaa pinosta ja palata sitä edelliseen risteykseen
    }
}

//näissä rotta liikkuu varsinaisesti eli sijainti päivitetään
Sijainti moveUp(Sijainti nykysijainti){
    nykysijainti.ykoord++;
    return nykysijainti;
}
Sijainti moveDown(Sijainti nykysijainti){
    nykysijainti.ykoord--;
    return nykysijainti;
}
Sijainti moveLeft(Sijainti nykysijainti){
    nykysijainti.xkoord--;
    return nykysijainti;
}
Sijainti moveRight(Sijainti nykysijainti){
    nykysijainti.xkoord++;
    return nykysijainti;
}

//PasiM TODO:jatkokehitys:
//toteuta funktiopointterivektorit
//funktiopointterivektorilla voi tiivistää koodia melkoisesti

//TÄRKEÄÄ
//risteyskoodi alapuolella, eli tämä ajetaan silmukan alussa kun ollaan jo risteyksessä (eli liikuttu siihen edellisen silmukan lopussa)
//OPISKELIJA: tästä kutsut tarvittavaan omaan lisättyyn logiikkaan (jos lisäehtoja/optimointia labyrintissa liikkumiselle)
//back() viittaa vektorin viimeisimpään lisättyyn alkioon (=pinon päälle)
//pop_back() poistaa viimeisimmän lisätyn alkion (=pinon päältä)
//tutkittavana - attribuutti kertoo (jää muistiin risteyksestä) minne suuntaan risteyksestä nyt lähdettiinkään
//palauttaa suunnan mihin risteyksestä lähdetään
LiikkumisSuunta doRistaus(Sijainti risteyssijainti, LiikkumisSuunta prevDir, auto& reitti, int (*labyrintti)[LEVEYS]){
    LiikkumisSuunta nextDir;
    nextDir = findNext(true, risteyssijainti, prevDir, reitti, labyrintti);
    //HUOM! päätös risteyksessä toimimisesta tehdään alla
    //OPISKELIJA: voit vaikuttaa päätöksentekoon, lisää oma toiminnallisuus omaan funktioonsa
    if (nextDir == LEFT) reitti.back().tutkittavana = LEFT;
    else if (nextDir == UP) reitti.back().tutkittavana = UP;
    else if (nextDir == RIGHT) reitti.back().tutkittavana = RIGHT;
    else if (nextDir == DOWN) reitti.back().tutkittavana = DOWN;
    else if (nextDir == DEFAULT) reitti.pop_back(); //kaikki suunnat oli jo tutkittu
    return nextDir; //kertoo eteenpäin miten päätettiin toimia risteyksessä
}
/*
 * Tästä alkaa rotan matka labyrintissa - tätä kutsutaan niin prosessista kuin threadistä samalla tavalla
 * Pääsilmukka on yksi liikkumisvuoro: vuoro alkaa analysoinnista nykyisen sijainnin suhteen ja vuoro loppuu siirtymiseen uuteen sijaintiin
 */

//OPISKELIJA: erottele tarvittaessa koodi mitä ajetaan prosessitoteutuksessa ja mitä thread-toteutuksessa
//OPISKELIJA: tarvitset siihen komentoriviltä annettavan parametrin mitä kuljetat sinne minne sitä tarvitaan

//HUOM! olennaista on että kurssilla opeteltuja asioita on sovellettu (mahdollisimman yksinkertainen koodi toimii)
//Siis kutsu alla olevaa niin forkatusta lapsesta kuin luodusta threadistä!
//palauttaa yksittäisen rotan liikkujen määrän
//parametrina tässä siis voi/pitää antaa esimerkiksi että ollaanko prosessina vai threadinä liikkeellä
//kuljeta parametria/parametreja tarvittavissa paikoissa ohjelmassa
//ohjelman lopussa reitti vektorissa (käsitellään pinona) on oikean reitin risteykset ainoastaan
int aloitaRotta(__pid_t id, int (*labyrintti)[LEVEYS]){
    int liikkuCount=0;
    vector<Ristaus> reitti; //pinona käytettävä rotan kulkema reitti (pinossa kuljetut risteykset)
    Sijainti rotanSijainti = findBegin(labyrintti); //hae labyrintin alku
    LiikkumisSuunta prevDir {DEFAULT}; //edellinen suunta:jottei kuljeta edestakaisin vahingossa
    LiikkumisSuunta nextDir {DEFAULT}; //seuraava suunta
    //pyöri labyrintissa kunnes ulostulo on löytynyt
    while (labyrintti[KORKEUS-1-rotanSijainti.ykoord][rotanSijainti.xkoord] != 4) { //ulospääsyn löytymisehto
//DEBUGAUSAPUJA..getpid() palauttaa ajajan
//        pid_t prosessi = getpid();
//        cout << "Olen prosessi: " << prosessi << endl;
        //alla vaihtoehtoisesti n-kertainen for-loop testauksia varten
        //    for (int i = 0 ; i < 50 ; i++){
        //risteykset on labyrinttiin merkitty 2:lla ohjelmoinnin helpottamiseksi
        //risteyksen tutkimiselle on oma koodi alla
        if (labyrintti[KORKEUS-1-rotanSijainti.ykoord][rotanSijainti.xkoord] == 2){
            nextDir = doRistaus(rotanSijainti, prevDir, reitti, labyrintti);
        }
        //muuten tämä, eli "muu kuin risteys" koodi
        else nextDir = findNext(false /* ei risteys */, rotanSijainti, prevDir, reitti, labyrintti);
        //huom! nykyimplementaatiossa jos vuoron alussa (eli yläpuolisissa kutsuissa) liikkuminen on todettu mahdolliseksi se myös tullaan tekemään, muuta tätä tarvittaessa
        //huom! jos on valittu risteykseenmeno niin ristaus on jo lisätty ristauspinoon tässä vaiheessa
        //huom! päätökset on jo tehty, tässä on vain päätösten toimeenpano
        switch (nextDir) {
        case UP:
        rotanSijainti = moveUp(rotanSijainti);
        prevDir = UP;
        break;
        case DOWN:
        rotanSijainti = moveDown(rotanSijainti);
        prevDir = DOWN;
        break;
        case LEFT:
        rotanSijainti = moveLeft(rotanSijainti);
        prevDir = LEFT;
        break;
        case RIGHT:
        rotanSijainti = moveRight(rotanSijainti);
        prevDir = RIGHT;
        break;
        //jos on palautettu DEFAULT niin on joko kyse
        //a)UMPIKUJASTA: (DEFAULT PALAUTETTU findNext() -> paluu edelliseen risteykseen
        //b)AIEMMIN PALATTIIN JO RISTEYKSEEN JONKA KAIKKI SUUNNAT tutkittu-attribuutti true: (DEFAULT PALAUTETTU doRistaus() -> poista risteys pinosta ja palaa edelliseen risteykseen
        case DEFAULT: //=paluu edelliseen risteykseen jossa käymättömiä reittejä
        cout << id << " Dead-end at x: " << rotanSijainti.xkoord << ", y:" << rotanSijainti.ykoord << endl;
        rotanSijainti.ykoord = reitti.back().kartalla.ykoord;
        rotanSijainti.xkoord = reitti.back().kartalla.xkoord;
        cout << id << " Returned to x: " << rotanSijainti.xkoord << ", y:" << rotanSijainti.ykoord << endl;
            switch (reitti.back().tutkittavana){
            case UP:
                reitti.back().up.tutkittu = true;
                reitti.back().up.jatkom = OPENING;
                break;
            case DOWN:
                reitti.back().down.tutkittu = true;
                reitti.back().down.jatkom = OPENING;
                break;
            case LEFT:
                reitti.back().left.tutkittu = true;
                reitti.back().left.jatkom = OPENING;
                break;

            case RIGHT:
                reitti.back().right.tutkittu = true;
                reitti.back().right.jatkom = OPENING;
                break;
            default:
            cout << "CRITICAL ERROR! THIS SHOULD NOT BE PRINTED!" << endl;
            break;
        }
        break;
    }
//DEBUGGAUSAPUJA
    //    cout << "Rotan sijainti nyt: " << rotanSijainti.ykoord << "," << rotanSijainti.xkoord << endl;

    //päivitetään liikkujen laskuri, tällä hetkellä yksi risteykseen paluu on yksi liikku
    liikkuCount++;
    //OPISKELIJA: päivitä rotan hengähtämistaukoa haluamallasi tavalla (se voi vaihdella eri rotilla)
    usleep(10);
} // for //while
    //ohjelman lopuksi palautetaan liikkujen määrä rottakohtaisesti
    //OPISKELIJA: voisit muuttaa paluuarvon rakenteeksi jossa olisi liikkujen määrän lisäksi myös oikea reitti eli jäljelle jäänyt risteyspino, pystyt sitten käyttämään sitä prosesseissa tai säikeissä
    return liikkuCount;
}

//OPISKELIJA: muistele harjoituksista miten eri asiat määriteltiin
//OPISKELIJA: tehtäväsi on sijoittaa itse labyrinttikin jaettuun muistialueeseen ja käyttää sitä sieltä
//->eli:labyrintin käyttö globaalilta alueelta jaetun muistin käyttöön
//vinkki: kannattaa määritellä pointteri nimeltä labyrintti kaksiuloitteiseen taulukkoon jolloin nykytoteutus toimii sellaisenaan
//parent/main alustaa jaettun muistin (eli kirjoittaa sinne) nuo labyrintin alkiot
//poista labyrintti kokonaan globaalilta alueelta, ohjelman pitäisi toimia
//mieti harjoituksista opitun perusteella paljonko labyrintti vähintään tarvitsee jaettua muistia
//tee kaikki ratkaisut niin että ohjelma toimii millä tahansa labyrintilla

// args of shared memory segment
const char* SEM_NAME = "/dummySem";
int segmentId;
const int segmentSize = 1024;

int childHandler(int (*memoryPointer)[LEVEYS], sem_t* sem, int segmentId){
    pid_t childpid = fork();
    if (childpid == -1) { perror("fork"); return 1; }

    if (childpid == 0) {
        // child pointer that points to the address in the mainprocess meaning the labyrinth
        std::cout << "child process id: " << getpid() << endl;
        int (*childPointer)[LEVEYS] = (int (*)[LEVEYS]) shmat(segmentId, NULL, 0);
        // esimerkkinä semaforin käyttö
        if (childPointer) {
            sem_wait(sem); //jos semafori vapaa - voit edetä, muuten odota vapautumista
            aloitaRotta(getpid(), childPointer);
            sleep(1);
            sem_post(sem); //siganloi että semafori vapaa
            std::cout << "freed this id of tasks: " << getpid() << endl;
            std::cout << "Lapsi: kirjoitettu arvo = " << childPointer << std::endl;
        }
        _exit(0); //lapsi poistuu
    } else {
        // Vanhempi odottaa että lapsi on valmis, alla odotetaan mitä tahansa valmistuvaa lasta
        //wait(nullptr);
        std::cout << "Vanhempi: luettu arvo = " << *memoryPointer << std::endl;
    }

    return 0;
}

//OPISKELIJA: nykyinen main on näin yksinkertainen, tästä pitää muokata se rinnakkaisuuden pohja
int main(){
    // creation of shared memory space (ADDRESS). Think this as something like 0x000abc in the memory. We can only save ONE THING at time here. If we want to save more things we create new shared memory addresses
    segmentId = shmget(IPC_PRIVATE, segmentSize, S_IRUSR | S_IWUSR);
    std::cout << "Created memory space. Segment id is: " << segmentId << endl;
    // memory pointer
    int (*memoryPointer)[LEVEYS] {nullptr}; // pointer to array of ints
    memoryPointer = (int(*)[LEVEYS]) shmat(segmentId, NULL, 0); // Pointer attaches to shared memory space. We define that this pointer uses this (NULL meaning whatever available address) address. ex 0x00abc
    int lukuParent = 50; // testing value

    // putting labyrintti into the shared memory space. We are putting a 2d array into there
    for (int i = 0; i < KORKEUS; i++) {
        for (int j = 0; j < LEVEYS; j++) {
            memoryPointer[i][j] = labyrintti[i][j];
        }
    }

    // semaphore
    sem_unlink(SEM_NAME); // removes semaphove with the name if it already exists
    sem_t* sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1); // creates a semaphore into the space we created before
    if (sem == SEM_FAILED) { perror("sem_open"); return 1; }

    // asking if user wants processes or threads
    int answer;
    cout << "Use process [0] or threads [1]: ";
    cin >> answer;
    // asking about how many processes/threads to create
    cout << "How many processes/threads do you want: ";
    cin >> answer;
    // creating child process
    // fork is a function that creates a child process from the parent process (the parent process is this executable that we are running here)
    for (int i = 0; i < answer; i++){
        childHandler(memoryPointer, sem, segmentId);
    }

    // Vanhempi odottaa että lapsi on valmis, alla odotetaan mitä tahansa valmistuvaa lasta
    for (int i = 0; i < answer; ++i){
        wait(nullptr);
    }

    //pid_t childpid = fork();
    //if (childpid == -1) { perror("fork"); return 1; }

    //if (childpid == 0) {
    //    //int (*myPointer)[LEVEYS] = labyrintti;
    //    //aloitaRotta();
    //    int* childPointer {nullptr};
    //    childPointer = (int*) shmat(segmentId, NULL, 0); // child pointer that points to the address in the mainprocess
    //    // esimerkkinä semaforin käyttö
    //    if (childPointer) {
    //        sem_wait(sem); //jos semafori vapaa - voit edetä, muuten odota vapautumista
    //        //*childPointer += 50; //turavalista päivittää
    //        aloitaRotta();
    //        sem_post(sem); //siganloi että semafori vapaa
    //        std::cout << "Lapsi: kirjoitettu arvo = " << *childPointer << std::endl;
    //    }
    //    return 0; //lapsi poistuu
    //} else {
    //    // Vanhempi odottaa että lapsi on valmis, alla odotetaan mitä tahansa valmistuvaa lasta
    //    wait(nullptr);
    //    std::cout << "Vanhempi: luettu arvo = " << *memoryPointer << std::endl;
    //}



    //aloitaRotta();
    //tämän tulee kertoa että kaikki rotat ovat päässeet ulos labyrintista
    //viimeinen jäädytetty kuva sijaintikartasta olisi hyvä olla todistamassa sitä
    cout << "All rats out!" << endl;

    // Siivotaan sameforit, vain vanhempi elossa täällä
    sem_close(sem); // ei voi käyttää tämän jälkeen
    sem_unlink(SEM_NAME); // nimi ei yhdisty enää semaforeihin

    // removing shared memory pointer
    shmdt(memoryPointer);

    // removal of shared memory
    shmctl(segmentId, IPC_RMID, nullptr);
    cout << "Removal of the memory space." << endl;

    // to check that shared memory segment that we created is removed run './a.out && ipcs -m'
    // this shows the ipcs that are currently working AFTER our script has run

    return 0;
}

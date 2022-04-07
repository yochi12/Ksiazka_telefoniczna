#include <iostream>
#include <fstream>      //rejestracja, logowanie oraz zmiana hasla dziala, ale nie na pliku .txt
#include <cstdlib>      //wnetrze funkjci "dokonanyWybor" dodano do "menuGlowne", a funkcje usunieto
#include <windows.h>
#include <conio.h>      //teraz czas stworzyc ten plik .txt oraz zastanowic sie nad przesylaniem ID osoby zalogowanej ORAZ czy potrzeba przesylac CALA strukture
#include <vector>
#include <sstream>

using namespace std;

struct KsiazkaAdresowa{
    int id=0;
    string imie, nazwisko, numerTelefonu, email, adres;
};

struct KontaUzytkownikow{
    int idUzytkownika=0;
    string login, haslo;
};

void zapiszWszystkichUzytkownikowDoStrukturyZTXT(vector<KontaUzytkownikow> &uzytkownicy);
void rozdzielNaOpowiednieZmienneDoStrukturyKontaUzytkownikow(vector<KontaUzytkownikow> &uzytkownicy, string calaLiniaZTXT);
void zapiszJednegoAdresataDoTXT(vector<KontaUzytkownikow> &uzytkownicy);
void zapiszWszystkichAdresatowDoTXT(vector<KontaUzytkownikow> &uzytkownicy);

void menuLogowania(vector<KontaUzytkownikow> &uzytkownicy);
void logowanie(vector<KontaUzytkownikow> &uzytkownicy);
void rejestracja(vector<KontaUzytkownikow> &uzytkownicy);
void menuZalogowanegoUzytkownika(vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika);
void zmianaHasla(vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika);

void wypiszWszystkich(vector<KontaUzytkownikow> &uzytkownicy);

int konwersjaStringNaInt(string liczbaString);
int menuGlowne(vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika);
int zapiszWszystkichAdresatowDoStrukturyZPlikuTXT(vector<KsiazkaAdresowa> &daneOsob);
void zapiszWszystkieOsobyDoTXT(vector<KsiazkaAdresowa> &daneOsob);
void zapiszJednaOsobeDoTXT(vector<KsiazkaAdresowa> &daneOsob);
void wypisywanieDanychPoLinijce (vector<KsiazkaAdresowa> daneOsob, int i);
int znajdzPozycjeOsobyWKsiazce(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID);
void rozdzielNaOpowiednieZmienneDoStruktury(vector<KsiazkaAdresowa> &daneOsob, string calaLiniaZTXT);

void dodajNowaOsobe (vector<KsiazkaAdresowa> &daneOsob);
void szukajPoImieniu(vector<KsiazkaAdresowa> daneOsob);
void szukajPoNazwisku(vector<KsiazkaAdresowa> daneOsob);
void wypiszWszystkieOsoby(vector<KsiazkaAdresowa> daneOsob);
void usunOsobe (vector<KsiazkaAdresowa> &daneOsob);
void menuEdycja(vector<KsiazkaAdresowa> &daneOsob);
char dokananyWyborEdycja(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID);
void zmienImie(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID);
void zmienNazwisko(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID);
void zmienNumerTelefonu(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID);
void zmienAdres(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID);
void zmienEmail(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID);


int main(){
    vector<KontaUzytkownikow> uzytkownicy;

    zapiszWszystkichUzytkownikowDoStrukturyZTXT(uzytkownicy);
    menuLogowania(uzytkownicy);
    return 0;
}

void zapiszWszystkichUzytkownikowDoStrukturyZTXT(vector<KontaUzytkownikow> &uzytkownicy){
    fstream plik2;
    plik2.open("Adresaci.txt",ios::in);

    if(plik2.good()==false)
        return;

    string liniaCase, calaLiniaZTXT;
    int numerLinii=1;

    while(getline(plik2,liniaCase)){
            switch(numerLinii){ //zamiast "numerLinii moglibysmy wpisac po prostu "1"
                case 1: calaLiniaZTXT= liniaCase;            break;
            }
        rozdzielNaOpowiednieZmienneDoStrukturyKontaUzytkownikow(uzytkownicy, calaLiniaZTXT);
    }
    plik2.close();
}

void rozdzielNaOpowiednieZmienneDoStrukturyKontaUzytkownikow(vector<KontaUzytkownikow> &uzytkownicy, string calaLiniaZTXT){
    KontaUzytkownikow uzytkownik_DoPobrania;
    int IDDoZapisaniaWStrukturze, licznik=0;
    string  slowoDoZapisaniaWStrukturze;

    for(int i=0; i<calaLiniaZTXT.length();i++){
        if(calaLiniaZTXT[i]!='|')
                slowoDoZapisaniaWStrukturze+=calaLiniaZTXT[i];
        else{
            switch(licznik){
            case 0: IDDoZapisaniaWStrukturze=konwersjaStringNaInt(slowoDoZapisaniaWStrukturze);
                    uzytkownik_DoPobrania.idUzytkownika=IDDoZapisaniaWStrukturze;                       break;
            case 1: uzytkownik_DoPobrania.login=slowoDoZapisaniaWStrukturze;                            break;
            case 2: uzytkownik_DoPobrania.haslo=slowoDoZapisaniaWStrukturze;                            break;
            }
                slowoDoZapisaniaWStrukturze="";
                licznik++;
        }
    }
    uzytkownicy.push_back(uzytkownik_DoPobrania);
}

void zapiszJednegoAdresataDoTXT(vector<KontaUzytkownikow> &uzytkownicy){
    string znak="|";
    int miejsceGdzieZapisacNowegoUzytkownika=uzytkownicy.size()-1;

    fstream plik2;
    plik2.open("Adresaci.txt", ios::out | ios::app);

        plik2<<uzytkownicy[miejsceGdzieZapisacNowegoUzytkownika].idUzytkownika;         plik2<<znak;
        plik2<<uzytkownicy[miejsceGdzieZapisacNowegoUzytkownika].login;                 plik2<<znak;
        plik2<<uzytkownicy[miejsceGdzieZapisacNowegoUzytkownika].haslo;                 plik2<<znak<<endl;

    plik2.close();
}

void zapiszWszystkichAdresatowDoTXT(vector<KontaUzytkownikow> &uzytkownicy){
    string znak="|";

    fstream plik2;
    plik2.open("Adresaci.txt", ios::out);
    for (int i=0; i<uzytkownicy.size(); i++)
    {
        plik2<<uzytkownicy[i].idUzytkownika;         plik2<<znak;
        plik2<<uzytkownicy[i].login;                 plik2<<znak;
        plik2<<uzytkownicy[i].haslo;                 plik2<<znak<<endl;
    }
    plik2.close();
}




void menuLogowania(vector<KontaUzytkownikow> &uzytkownicy){
    char wybor;
    while(1){
            system("cls");
            cout<<"..::Ksiazka adresowa::.."                <<endl;
            cout<<"1. Logowanie"                            <<endl;
            cout<<"2. Rejestracja"                          <<endl;
            cout<<"0. Zakoncz program"                      <<endl;
            wybor=getch();

        switch(wybor){
            case '1':   logowanie(uzytkownicy);             break;
            case '2':   rejestracja(uzytkownicy);           break;
            case '0':   exit(0);                            break;
            case '4':   wypiszWszystkich(uzytkownicy);      break;
            default: cout<<"Nie ma takiej opcji w menu.";
            Sleep(1000);
            }
        }
}

void rejestracja(vector<KontaUzytkownikow> &uzytkownicy){
    KontaUzytkownikow uzytkownik_DoPobrania;

    string login, haslo;
    int pomocDoWprowadzaniaID, iloscUzytkownikow;

    cout<<endl<<"Podaj nazwe uzytkownika: ";  cin>>login;

    for (int i=0; i<uzytkownicy.size(); i++){
        if(uzytkownicy[i].login == login){
            cout<<"Ten login jest juz zajety"<<endl;
            Sleep(1500);
            system("cls");
            cout<<"..::Ksiazka adresowa::.."                <<endl;
            cout<<"1. Logowanie"                            <<endl;
            cout<<"2. Rejestracja"                          <<endl;
            cout<<"0. Zakoncz program"                      <<endl;
            cout<<endl<<"Podaj inna nazwe uzytkownika: ";  cin>>login;
            i=-1;
        }
    }
    cout<<"Podaj haslo: ";  cin>>haslo;

    uzytkownik_DoPobrania.login=login;
    uzytkownik_DoPobrania.haslo=haslo;
    uzytkownik_DoPobrania.idUzytkownika=uzytkownicy.size()+1;

    uzytkownicy.push_back(uzytkownik_DoPobrania);
    cout<<endl<<"Konto zalozone"<<endl;
    zapiszJednegoAdresataDoTXT(uzytkownicy);
    Sleep(1000);
}

void logowanie(vector<KontaUzytkownikow> &uzytkownicy){
    string login, haslo;
    cout<<endl<<"Podaj login: ";  cin>>login;

    for(int i=0; i<uzytkownicy.size(); i++){
        if(login==uzytkownicy[i].login){
            for(int proby=0; proby<3; proby++){
                cout<<"Podaj haslo. Pozosalo prob ("<<3-proby<<"): ";
                cin>>haslo;

                if(uzytkownicy[i].haslo == haslo){
                    cout<<"Jestes zalogowany jako "<<uzytkownicy[i].login<<endl;
                    Sleep(1000);
                    menuGlowne(uzytkownicy, i);
                }
            }
            cout<<"Podano 3 razy bledne haslo. Poczekaj 3 sek przed kolejna proba"<<endl;
            Sleep(3000);
            return;
        }
    }
    cout<<"Bledny login";
    Sleep(1000);
}

void zmianaHasla(vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika){
    string haslo;
    cout<<"Podaj nowe haslo: "; cin>>haslo;

    uzytkownicy[pozycjaUzytkownika].haslo=haslo;
    zapiszWszystkichAdresatowDoTXT(uzytkownicy);

    cout<<"Haslo zostalo zmienione";
    Sleep(1000);
}


void wypiszWszystkich(vector<KontaUzytkownikow> &uzytkownicy){ //dla sprawdzania, pozniej do usuniecia
        for (int i=0; i<uzytkownicy.size(); i++){
        cout<<endl<<"idUzytkownika: "<<uzytkownicy[i].idUzytkownika     <<endl;
        cout<<"login: "<<uzytkownicy[i].login                           <<endl;
        cout<<"haslo: "<<uzytkownicy[i].haslo                           <<endl;
    }
    cout<<endl<<"rozmiar struktury: "<<uzytkownicy.size()               <<endl<<endl;
    system("pause");
}

////////////////////////////////////////////////////////////////////////////////////////////////

int menuGlowne(vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika){
    vector<KsiazkaAdresowa> daneOsob;
    KsiazkaAdresowa pobrane;
    zapiszWszystkichAdresatowDoStrukturyZPlikuTXT(daneOsob);

    char wybor;
    while(1){
            system("cls");
            cout<<"..::Ksiazka adresowa::.. ("<<uzytkownicy[pozycjaUzytkownika].login<<")"            <<endl;
            cout<<"1. Dodaj adresata"                   <<endl;
            cout<<"2. Wyszukaj po imieniu"              <<endl;
            cout<<"3. Wyszukaj po nazwisku"             <<endl;
            cout<<"4. Wyswietl wszystkich adresatow"    <<endl;
            cout<<"5. Usun adresata"                    <<endl;
            cout<<"6. Edytuj adresata"                  <<endl;
            cout<<"7. Zmien haslo"                      <<endl;
            cout<<"8. Wyloguj sie"                      <<endl;
            cout<<"0. Zakoncz program"                  <<endl;
            wybor=getch();

        switch(wybor){
            case '1':   dodajNowaOsobe(daneOsob);               break;
            case '2':   szukajPoImieniu(daneOsob);              break;
            case '3':   szukajPoNazwisku(daneOsob);             break;
            case '4':   wypiszWszystkieOsoby(daneOsob);         break;
            case '5':   usunOsobe(daneOsob);                    break;
            case '6':   menuEdycja(daneOsob);                   break;
            case '7':   zmianaHasla(uzytkownicy, pozycjaUzytkownika);               break;
            case '8':   menuLogowania(uzytkownicy);             break;
            case '0':   exit(0);                                break;

            default: cout<<"Nie ma takiej opcji w menu.";
            Sleep(1000);
        }
    }
    return 0;
}

int zapiszWszystkichAdresatowDoStrukturyZPlikuTXT(vector<KsiazkaAdresowa> &daneOsob){
    fstream plik1;
    plik1.open("Ksiazka adresowa.txt",ios::in);

    if(plik1.good()==false)
        return 0;

    string liniaCase, calaLiniaZTXT;
    int numerLinii=1;

    while(getline(plik1,liniaCase)){
            switch(numerLinii){ //zamiast "numerLinii moglibysmy wpisac po prostu "1"
                case 1: calaLiniaZTXT= liniaCase;            break;
            }
        rozdzielNaOpowiednieZmienneDoStruktury(daneOsob, calaLiniaZTXT);
    }
    plik1.close();
    return 0;
}

void zapiszJednaOsobeDoTXT(vector<KsiazkaAdresowa> &daneOsob){
    string znak="|";
    int miejsceGdzieZapisacNowaOsobe=daneOsob.size()-1;

    fstream plik1;
    plik1.open("Ksiazka adresowa.txt", ios::out | ios::app);

        plik1<<daneOsob[miejsceGdzieZapisacNowaOsobe].id;                  plik1<<znak;
        plik1<<daneOsob[miejsceGdzieZapisacNowaOsobe].imie;                plik1<<znak;
        plik1<<daneOsob[miejsceGdzieZapisacNowaOsobe].nazwisko;            plik1<<znak;
        plik1<<daneOsob[miejsceGdzieZapisacNowaOsobe].numerTelefonu;       plik1<<znak;
        plik1<<daneOsob[miejsceGdzieZapisacNowaOsobe].email;               plik1<<znak;
        plik1<<daneOsob[miejsceGdzieZapisacNowaOsobe].adres;               plik1<<znak<<endl;

    plik1.close();
}

void zapiszWszystkieOsobyDoTXT(vector<KsiazkaAdresowa> &daneOsob){
    string znak="|";

    fstream plik1;
    plik1.open("Ksiazka adresowa.txt", ios::out);
    for (int i=0; i<daneOsob.size(); i++)
    {
        plik1<<daneOsob[i].id;                  plik1<<znak;
        plik1<<daneOsob[i].imie;                plik1<<znak;
        plik1<<daneOsob[i].nazwisko;            plik1<<znak;
        plik1<<daneOsob[i].numerTelefonu;       plik1<<znak;
        plik1<<daneOsob[i].email;               plik1<<znak;
        plik1<<daneOsob[i].adres;               plik1<<znak<<endl;
    }
    plik1.close();
}

void rozdzielNaOpowiednieZmienneDoStruktury(vector<KsiazkaAdresowa> &daneOsob, string calaLiniaZTXT){
    KsiazkaAdresowa pobrane;
    int IDDoZapisaniaWStrukturze, licznik=0;
    string  slowoDoZapisaniaWStrukturze;

    for(int i=0; i<calaLiniaZTXT.length();i++){
        if(calaLiniaZTXT[i]!='|')
                slowoDoZapisaniaWStrukturze+=calaLiniaZTXT[i];
        else{
            switch(licznik){
            case 0: IDDoZapisaniaWStrukturze=konwersjaStringNaInt(slowoDoZapisaniaWStrukturze);
                    pobrane.id=IDDoZapisaniaWStrukturze;                                        break;
            case 1: pobrane.imie=slowoDoZapisaniaWStrukturze;                                   break;
            case 2: pobrane.nazwisko=slowoDoZapisaniaWStrukturze;                               break;
            case 3: pobrane.numerTelefonu=slowoDoZapisaniaWStrukturze;                          break;
            case 4: pobrane.email=slowoDoZapisaniaWStrukturze;                                  break;
            case 5: pobrane.adres=slowoDoZapisaniaWStrukturze;                                  break;
            }
                slowoDoZapisaniaWStrukturze="";
                licznik++;
        }
    }
    daneOsob.push_back(pobrane);
}

void dodajNowaOsobe (vector<KsiazkaAdresowa> &daneOsob){
    KsiazkaAdresowa pobrane;
    vector <KsiazkaAdresowa>::iterator itr;
    int pomocDoWprowadzaniaID, iloscOsob;  //ta liczbe tymczasowo wprowadzamy do struktury, razem z wyrazami (wiersz wyzej)

    cin.sync();
    cout<<endl<< "Podaj imie: ";    getline(cin,pobrane.imie);
    cout<<"Podaj nazwisko: ";       getline(cin,pobrane.nazwisko);
    cout<<"Podaj numer telefou: ";  getline(cin,pobrane.numerTelefonu);
    cout<<"Podaj email: ";          getline(cin,pobrane.email);
    cout<<"Podaj adres: ";          getline(cin,pobrane.adres);
    cout<<endl;

    if(daneOsob.size()==0)
        pobrane.id=1;   //ilosc osob to wielkosc tablicy przed wprowadzeniem nowej osoby + 1 (np. 0 (pusta ksiazka) +1)
                        //dla ksiazki w ktorej usuniemy wszystkie osoby, pierwsza nowo wpisana osoba znow bedzie miec ID=1
    else{
        iloscOsob=daneOsob.size()-1; //-1, poniewaz tablice etc sa numerowane od 0, wtedy rozmiar wynosi 1
        pomocDoWprowadzaniaID=daneOsob[iloscOsob].id+1; //do ostatniego istniejacego ID dodajemy 1
        pobrane.id=pomocDoWprowadzaniaID;
    }
    daneOsob.push_back(pobrane);

    zapiszJednaOsobeDoTXT(daneOsob);

    cout<<"Nowy adresat zapisany!"<<endl;
    Sleep(1200);
}

void szukajPoImieniu(vector<KsiazkaAdresowa> daneOsob){
    int iloscZnalezionychOsob=0;
    string szukaneImie;
    cout<<endl<<"Szukane imie: "; cin>>szukaneImie;

    for(int i=0; i<daneOsob.size(); i++){
        if(szukaneImie==daneOsob[i].imie){
            wypisywanieDanychPoLinijce(daneOsob, i);
            iloscZnalezionychOsob++;
        }
    }
        if(iloscZnalezionychOsob==0){
            cout<<"Nie ma takiego adresata";
            Sleep(1200);
            return;
        }
    cout<<"Nacisnij dowolny klawisz, aby kontynuowac";
    getch();
}

void szukajPoNazwisku(vector<KsiazkaAdresowa> daneOsob){
    int iloscZnalezionychOsob=0;
    string szukaneNazwisko;
    cout<<endl<<"Szukane nazwisko: "; cin>>szukaneNazwisko;

    for(int i=0; i<daneOsob.size(); i++){
        if(szukaneNazwisko==daneOsob[i].nazwisko){
            wypisywanieDanychPoLinijce(daneOsob, i);
            iloscZnalezionychOsob++;
        }
    }
        if(iloscZnalezionychOsob==0){
            cout<<"Nie ma takiego adresata";
            Sleep(1200);
            return;
        }
    cout<<"Nacisnij dowolny klawisz, aby kontynuowac";
    getch();
}

void wypiszWszystkieOsoby(vector<KsiazkaAdresowa> daneOsob){
    cout<<endl;
    for(int i=0; i<daneOsob.size(); i++)
             wypisywanieDanychPoLinijce(daneOsob, i);

    cout<<"Nacisnij dowolny klawisz, aby kontynuowac";
    getch();
}

void wypisywanieDanychPoLinijce (vector<KsiazkaAdresowa> daneOsob, int i){
    cout<<"ID: ";              cout<<daneOsob[i].id<<endl;
    cout<<"Imie: ";            cout<<daneOsob[i].imie<<endl;
    cout<<"Nazwisko: ";        cout<<daneOsob[i].nazwisko<<endl;
    cout<<"Numer telefonu: ";  cout<<daneOsob[i].numerTelefonu<<endl;
    cout<<"Email: ";           cout<<daneOsob[i].email<<endl;
    cout<<"Adres: ";           cout<<daneOsob[i].adres<<endl<<endl;
}

void usunOsobe (vector<KsiazkaAdresowa> &daneOsob){
    vector <KsiazkaAdresowa>::iterator itr;
    int numerIDDoUsuniecia, pozycjaWKsiazce;
    string potwierdzenieUsuniecia;

    cout<<endl<<"Podaj numer ID osoby ktora chesz usunac: ";      cin>>numerIDDoUsuniecia;       cout<<endl;
    pozycjaWKsiazce=znajdzPozycjeOsobyWKsiazce(daneOsob, numerIDDoUsuniecia);

    if (pozycjaWKsiazce==-1) //zabezpieczenie przed wprowadzeniem ID ktorego nie ma w ksiazce
        return;

    wypisywanieDanychPoLinijce(daneOsob, pozycjaWKsiazce);
    cout<<"Chcesz usunac tego adresata? t/n: ";             cin>>potwierdzenieUsuniecia;

    if (potwierdzenieUsuniecia!="t")
        return;

    itr = daneOsob.begin()+pozycjaWKsiazce;
    daneOsob.erase(itr);

    zapiszWszystkieOsobyDoTXT(daneOsob);

    cout<<"Adresat usuniety!";
    Sleep(1000);
}

int znajdzPozycjeOsobyWKsiazce(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID){
    int i;
    for(i=0; i<daneOsob.size(); i++){
        if(szukanyNumerID==daneOsob[i].id)
            return i;
    }
    cout<<"Nie ma takiego adresata";
    Sleep(1000);
    return i=-1; //zabezpieczenie przed wprowadzeniem ID, ktorego nie ma w ksiazce
}

void menuEdycja(vector<KsiazkaAdresowa> &daneOsob){
    int szukanyNumerID, pozycjaWKsiazce;
    char wybor;
    cout<<endl<<"Podaj ID osoby: ";   cin>>szukanyNumerID;

    while(1){
            pozycjaWKsiazce=znajdzPozycjeOsobyWKsiazce(daneOsob, szukanyNumerID);

            if (pozycjaWKsiazce==-1) //zabezpieczenie przed wprowadzeniem ID ktorego nie ma w ksiazce
            return;

            system("cls");
            cout<<"..::Ksiazka adresowa::.."            <<endl;
            cout<<"Edytuj:"                             <<endl;
            cout<<"1. Imie"                             <<endl;
            cout<<"2. Nazwisko"                         <<endl;
            cout<<"3. Numer telefonu"                   <<endl;
            cout<<"4. Email"                            <<endl;
            cout<<"5. Adres"                            <<endl;
            cout<<"0. Powrot do menu"                   <<endl<<endl;

            wypisywanieDanychPoLinijce(daneOsob, pozycjaWKsiazce);
            wybor=dokananyWyborEdycja(daneOsob, pozycjaWKsiazce);

            if (wybor=='0')
                break;
        }
}

char dokananyWyborEdycja(vector<KsiazkaAdresowa> &daneOsob, int pozycjaWKsiazce){
    char wybor;
    wybor=getch();
    switch(wybor){
        case '1':   zmienImie(daneOsob, pozycjaWKsiazce);                break;
        case '2':   zmienNazwisko(daneOsob, pozycjaWKsiazce);            break;
        case '3':   zmienNumerTelefonu(daneOsob, pozycjaWKsiazce);       break;
        case '4':   zmienEmail(daneOsob, pozycjaWKsiazce);               break;
        case '5':   zmienAdres(daneOsob, pozycjaWKsiazce);               break;
        case '0':   wybor='0';                                          break;

        default: cout<<"Nie ma takiej opcji w menu.";
        Sleep(1000);
    }
    return wybor;
}

void zmienImie(vector<KsiazkaAdresowa> &daneOsob, int pozycjaWKsiazce){
    string informacjaDoZmiany;
    cout<<"Podaj nowe imie: ";       cin>>informacjaDoZmiany;

    daneOsob[pozycjaWKsiazce].imie=informacjaDoZmiany;
    zapiszWszystkieOsobyDoTXT(daneOsob); //Zapisuje WSZYSTKIE informacje na temat tej konkretnej osoby, poniewaz mam taka funkcje,
}                                       //w innym przypadku musialbym tworzyc osobne funkcje do zapisywania kazdej rzeczy osobno

void zmienNazwisko(vector<KsiazkaAdresowa> &daneOsob, int pozycjaWKsiazce){
    string informacjaDoZmiany;
    cout<<"Podaj nowe nazwisko: ";       cin>>informacjaDoZmiany;

    daneOsob[pozycjaWKsiazce].nazwisko=informacjaDoZmiany;
    zapiszWszystkieOsobyDoTXT(daneOsob);
}

void zmienNumerTelefonu(vector<KsiazkaAdresowa> &daneOsob, int pozycjaWKsiazce){
    string informacjaDoZmiany;
    cout<<"Podaj nowy numer telefonu: ";       cin>>informacjaDoZmiany;

    daneOsob[pozycjaWKsiazce].numerTelefonu=informacjaDoZmiany;
    zapiszWszystkieOsobyDoTXT(daneOsob);
}

void zmienEmail(vector<KsiazkaAdresowa> &daneOsob, int pozycjaWKsiazce){
    string informacjaDoZmiany;
    cout<<"Podaj nowy email: ";       cin>>informacjaDoZmiany;

    daneOsob[pozycjaWKsiazce].email=informacjaDoZmiany;
    zapiszWszystkieOsobyDoTXT(daneOsob);
}

void zmienAdres(vector<KsiazkaAdresowa> &daneOsob, int pozycjaWKsiazce){
    string informacjaDoZmiany;
    cout<<"Podaj nowy adres: ";       cin>>informacjaDoZmiany;

    daneOsob[pozycjaWKsiazce].adres=informacjaDoZmiany;
    zapiszWszystkieOsobyDoTXT(daneOsob);
}


int konwersjaStringNaInt(string liczbaString){
    int liczbaInt;
    istringstream iss(liczbaString);
    iss >> liczbaInt;
    return liczbaInt;
}


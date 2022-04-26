#include <iostream>     ///Ksiazka adresowa z mozliwoscia rejestracji i logowania sie (jedno .txt)
#include <fstream>      ///oraz dodawaniem, wyszukiwaniem, edycja i usuwaniem kontaktow (drugie .txt)
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <sstream>

using namespace std;

struct KsiazkaAdresowa{
    int id=0, idUzytkownikaKA=0;
    string imie, nazwisko, numerTelefonu, email, adres;
};

struct KontaUzytkownikow{
    int idUzytkownika=0;
    string login, haslo;
};

//////////////////////////////////////Funkcje wczytujace/zapisujace uzytkownikow z/do .txt
void zapiszWszystkichUzytkownikowDoStrukturyZTXT(vector<KontaUzytkownikow> &uzytkownicy);
void rozdzielNaOpowiednieZmienneDoStrukturyUzytkownicy(vector<KontaUzytkownikow> &uzytkownicy, string calaLiniaZTXT);
void zapiszJednegoUzytkownikaDoTXT(vector<KontaUzytkownikow> &uzytkownicy);
void zapiszWszystkichUzytkownikowDoTXT(vector<KontaUzytkownikow> &uzytkownicy);

//////////////////////////////////////Funkcje dotyczace logowania (uzytkownik)
void menuLogowania(vector<KontaUzytkownikow> &uzytkownicy);
void logowanie(vector<KontaUzytkownikow> &uzytkownicy);
void rejestracja(vector<KontaUzytkownikow> &uzytkownicy);
void zmianaHasla(vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika);

//////////////////////////////////////Menu glowne, funkcje wczytujace/zapisujace uzytkownikow z/do .txt
int menuGlowne(vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika);
int zapiszWszystkichAdresatowDoStrukturyZPlikuTXT(vector<KsiazkaAdresowa> &daneOsob, vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika);
int rozdzielNaOpowiednieZmienneDoStrukturyAdresaci(vector<KsiazkaAdresowa> &daneOsob, string calaLiniaZTXT, vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika);
bool porownajIDOsobyZIDUzytkownika(string calaLiniaZTXT, vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika);
void zapiszJednegoAdresataDoTXT(vector<KsiazkaAdresowa> &daneOsob);

//////////////////////////////////////Funkcje pomocnicze
void wypisywanieAdresatowPoLinijce(vector<KsiazkaAdresowa> daneOsob, int i);
int znajdzPozycjeOsobyWKsiazce(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID);

//////////////////////////////////////Opcje z menu glownego
int dodajNowegoAdresata(vector<KsiazkaAdresowa> &daneOsob, vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika, int ostatnieID);
void szukajPoImieniu(vector<KsiazkaAdresowa> daneOsob);
void szukajPoNazwisku(vector<KsiazkaAdresowa> daneOsob);
void wypiszWszystkichAdresatow(vector<KsiazkaAdresowa> daneOsob);
int usunAdresata(vector<KsiazkaAdresowa> &daneOsob, int ostatnieID);
void menuEdycja(vector<KsiazkaAdresowa> &daneOsob);
char dokananyWyborEdycja(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID);
void zmienImie(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID);
void zmienNazwisko(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID);
void zmienNumerTelefonu(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID);
void zmienAdres(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID);
void zmienEmail(vector<KsiazkaAdresowa> &daneOsob, int szukanyNumerID);

//////////////////////////////////////Przepisywanie danych miedzy dwoma plikami .txt przy edycji i usuwaniu adresatow
int funkcjaZPlikamiDoEdycjiAdresatow(vector<KsiazkaAdresowa> daneOsob, int idUsunietegoAdresata);
string znajdzIDAdresataIUzytkownika(vector<KsiazkaAdresowa> daneOsob, int &IDAdresataPlik1, int &IDUzytkownikaPlik1, int &IDDomniemanegoOstatniegoAdresataPlik1, string liniaCase);

//////////////////////////////////////Konwersje etc
int konwersjaStringNaInt(string liczbaString);



int main(){
    vector<KontaUzytkownikow> uzytkownicy;

    zapiszWszystkichUzytkownikowDoStrukturyZTXT(uzytkownicy);
    menuLogowania(uzytkownicy);
    return 0;
}

//////////////////////////////////////Funkcje wczytujace/zapisujace uzytkownikow z/do .txt
void zapiszWszystkichUzytkownikowDoStrukturyZTXT(vector<KontaUzytkownikow> &uzytkownicy){
    fstream plik2;
    plik2.open("Uzytkowkicy.txt",ios::in);

    if(plik2.good()==false)
        return;

    string liniaCase, calaLiniaZTXT;
    int numerLinii=1;

    while(getline(plik2,liniaCase)){
            switch(numerLinii){ //zamiast "numerLinii moglibysmy wpisac po prostu "1"
                case 1: calaLiniaZTXT= liniaCase;            break;
            }
        rozdzielNaOpowiednieZmienneDoStrukturyUzytkownicy(uzytkownicy, calaLiniaZTXT);
    }
    plik2.close();
}

void rozdzielNaOpowiednieZmienneDoStrukturyUzytkownicy(vector<KontaUzytkownikow> &uzytkownicy, string calaLiniaZTXT){
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

void zapiszJednegoUzytkownikaDoTXT(vector<KontaUzytkownikow> &uzytkownicy){
    string znak="|";
    int miejsceGdzieZapisacNowegoUzytkownika=uzytkownicy.size()-1;

    fstream plik2;
    plik2.open("Uzytkowkicy.txt", ios::out | ios::app);

        plik2<<uzytkownicy[miejsceGdzieZapisacNowegoUzytkownika].idUzytkownika;         plik2<<znak;
        plik2<<uzytkownicy[miejsceGdzieZapisacNowegoUzytkownika].login;                 plik2<<znak;
        plik2<<uzytkownicy[miejsceGdzieZapisacNowegoUzytkownika].haslo;                 plik2<<znak<<endl;

    plik2.close();
}

void zapiszWszystkichUzytkownikowDoTXT(vector<KontaUzytkownikow> &uzytkownicy){
    string znak="|";

    fstream plik2;
    plik2.open("Uzytkowkicy.txt", ios::out);
    for (int i=0; i<uzytkownicy.size(); i++)
    {
        plik2<<uzytkownicy[i].idUzytkownika;         plik2<<znak;
        plik2<<uzytkownicy[i].login;                 plik2<<znak;
        plik2<<uzytkownicy[i].haslo;                 plik2<<znak<<endl;
    }
    plik2.close();
}


//////////////////////////////////////Funkcje dotyczace logowania (uzytkownik)
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
            default: cout<<endl<<"Nie ma takiej opcji w menu.";
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
    zapiszJednegoUzytkownikaDoTXT(uzytkownicy);
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
    cout<<endl<<"Podaj nowe haslo: "; cin>>haslo;

    uzytkownicy[pozycjaUzytkownika].haslo=haslo;
    zapiszWszystkichUzytkownikowDoTXT(uzytkownicy);

    cout<<"Haslo zostalo zmienione";
    Sleep(1000);
}


//////////////////////////////////////Menu glowne, funkcje wczytujace/zapisujace uzytkownikow z/do .txt
int menuGlowne(vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika){
    vector<KsiazkaAdresowa> daneOsob;
    KsiazkaAdresowa pobrane;
    int ostatnieID = zapiszWszystkichAdresatowDoStrukturyZPlikuTXT(daneOsob, uzytkownicy, pozycjaUzytkownika);

    char wybor;
    while(1){
            system("cls");
            cout<<"..::Ksiazka adresowa::.. ("<<uzytkownicy[pozycjaUzytkownika].login<<")"          <<endl;
            cout<<"1. Dodaj adresata"                                                               <<endl;
            cout<<"2. Wyszukaj po imieniu"                                                          <<endl;
            cout<<"3. Wyszukaj po nazwisku"                                                         <<endl;
            cout<<"4. Wyswietl wszystkich adresatow"                                                <<endl;
            cout<<"5. Usun adresata"                                                                <<endl;
            cout<<"6. Edytuj adresata"                                                              <<endl;
            cout<<"7. Zmien haslo"                                                                  <<endl;
            cout<<"8. Wyloguj sie"                                                                  <<endl;
            cout<<"0. Zakoncz program"                                                              <<endl;
            wybor=getch();

        switch(wybor){
            case '1':   ostatnieID = dodajNowegoAdresata(daneOsob, uzytkownicy, pozycjaUzytkownika, ostatnieID);            break;
            case '2':   szukajPoImieniu(daneOsob);                                                                          break;
            case '3':   szukajPoNazwisku(daneOsob);                                                                         break;
            case '4':   wypiszWszystkichAdresatow(daneOsob);                                                                break;
            case '5':   ostatnieID = usunAdresata(daneOsob, ostatnieID);                                                    break;
            case '6':   menuEdycja(daneOsob);                                                                               break;
            case '7':   zmianaHasla(uzytkownicy, pozycjaUzytkownika);                                                       break;
            case '8':   menuLogowania(uzytkownicy);                                                                         break;
            case '0':   exit(0);                                                                                            break;

            default: cout<<endl<<"Nie ma takiej opcji w menu.";
            Sleep(1000);
        }
    }
    return 0;
}

int zapiszWszystkichAdresatowDoStrukturyZPlikuTXT(vector<KsiazkaAdresowa> &daneOsob, vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika){
    int ostatnieID;

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
            ostatnieID = rozdzielNaOpowiednieZmienneDoStrukturyAdresaci(daneOsob, calaLiniaZTXT, uzytkownicy, pozycjaUzytkownika);
    }
    plik1.close();
    return ostatnieID;
}

int rozdzielNaOpowiednieZmienneDoStrukturyAdresaci(vector<KsiazkaAdresowa> &daneOsob, string calaLiniaZTXT, vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika){
    KsiazkaAdresowa pobrane;
    int IDAdresataDoZapisaniaWStrukturze, IDUzytkownikaDoZapisaniaWStrukturze, licznik=0, ostatnieID=0;
    string  slowoDoZapisaniaWStrukturze;

    for(int i=0; i<calaLiniaZTXT.length();i++){
        if(calaLiniaZTXT[i]!='|')
                slowoDoZapisaniaWStrukturze+=calaLiniaZTXT[i];
        else{
            switch(licznik){
            case 0: IDAdresataDoZapisaniaWStrukturze=konwersjaStringNaInt(slowoDoZapisaniaWStrukturze);
                    pobrane.id=IDAdresataDoZapisaniaWStrukturze;                                        break;
            case 1: IDUzytkownikaDoZapisaniaWStrukturze=konwersjaStringNaInt(slowoDoZapisaniaWStrukturze);
                    pobrane.idUzytkownikaKA=IDUzytkownikaDoZapisaniaWStrukturze;                break;
            case 2: pobrane.imie=slowoDoZapisaniaWStrukturze;                                   break;
            case 3: pobrane.nazwisko=slowoDoZapisaniaWStrukturze;                               break;
            case 4: pobrane.numerTelefonu=slowoDoZapisaniaWStrukturze;                          break;
            case 5: pobrane.email=slowoDoZapisaniaWStrukturze;                                  break;
            case 6: pobrane.adres=slowoDoZapisaniaWStrukturze;                                  break;
            }
                slowoDoZapisaniaWStrukturze="";
                licznik++;
        }
    }
    ostatnieID = IDAdresataDoZapisaniaWStrukturze;

    if(porownajIDOsobyZIDUzytkownika(calaLiniaZTXT, uzytkownicy, pozycjaUzytkownika)==true)
        daneOsob.push_back(pobrane);

    return ostatnieID;
}

bool porownajIDOsobyZIDUzytkownika(string calaLiniaZTXT, vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika){
    int IDUzytkownikaDoZapisaniaWStrukturze, licznik=0;
    string  slowoDoZapisaniaWStrukturze;
    for(int i=0; i<calaLiniaZTXT.length();i++){
        if(calaLiniaZTXT[i]!='|')
                slowoDoZapisaniaWStrukturze+=calaLiniaZTXT[i];
        else{
            switch(licznik){
            case 0:                                                                                         break;
            case 1: IDUzytkownikaDoZapisaniaWStrukturze=konwersjaStringNaInt(slowoDoZapisaniaWStrukturze);  break;
            }
                slowoDoZapisaniaWStrukturze="";
                licznik++;
        }
    }
    if (IDUzytkownikaDoZapisaniaWStrukturze==uzytkownicy[pozycjaUzytkownika].idUzytkownika)
        return true;//porownuje ID osoby z ID uzytkownika
    else
        return false;
}

void zapiszJednegoAdresataDoTXT(vector<KsiazkaAdresowa> &daneOsob){
    string znak="|";
    int miejsceGdzieZapisacNowaOsobe=daneOsob.size()-1;

    fstream plik1;
    plik1.open("Ksiazka adresowa.txt", ios::out | ios::app);

        plik1<<daneOsob[miejsceGdzieZapisacNowaOsobe].id;                  plik1<<znak;
        plik1<<daneOsob[miejsceGdzieZapisacNowaOsobe].idUzytkownikaKA;     plik1<<znak;
        plik1<<daneOsob[miejsceGdzieZapisacNowaOsobe].imie;                plik1<<znak;
        plik1<<daneOsob[miejsceGdzieZapisacNowaOsobe].nazwisko;            plik1<<znak;
        plik1<<daneOsob[miejsceGdzieZapisacNowaOsobe].numerTelefonu;       plik1<<znak;
        plik1<<daneOsob[miejsceGdzieZapisacNowaOsobe].email;               plik1<<znak;
        plik1<<daneOsob[miejsceGdzieZapisacNowaOsobe].adres;               plik1<<znak<<endl;

    plik1.close();
}



//////////////////////////////////////Funkcje pomocnicze
void wypisywanieAdresatowPoLinijce (vector<KsiazkaAdresowa> daneOsob, int i){
    //cout<<"ID Uzytkownika (z ksiazki): ";  cout<<daneOsob[i].idUzytkownikaKA<<endl; ///dla sprawdzenia, czy dany kontakt nalezy do tego uzytkownika : ]
    cout<<"ID: ";              cout<<daneOsob[i].id<<endl;
    cout<<"Imie: ";            cout<<daneOsob[i].imie<<endl;
    cout<<"Nazwisko: ";        cout<<daneOsob[i].nazwisko<<endl;
    cout<<"Numer telefonu: ";  cout<<daneOsob[i].numerTelefonu<<endl;
    cout<<"Email: ";           cout<<daneOsob[i].email<<endl;
    cout<<"Adres: ";           cout<<daneOsob[i].adres<<endl<<endl;
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


//////////////////////////////////////Opcje z menu glownego
int dodajNowegoAdresata (vector<KsiazkaAdresowa> &daneOsob, vector<KontaUzytkownikow> &uzytkownicy, int pozycjaUzytkownika, int ostatnieID){
    KsiazkaAdresowa pobrane;
    vector <KsiazkaAdresowa>::iterator itr; //po co to?
    int pomocDoWprowadzaniaID, iloscOsob;  //ta liczbe tymczasowo wprowadzamy do struktury, razem z wyrazami (wiersz wyzej)

    cin.sync();
    cout<<endl<< "Podaj imie: ";    getline(cin,pobrane.imie);
    cout<<"Podaj nazwisko: ";       getline(cin,pobrane.nazwisko);
    cout<<"Podaj numer telefou: ";  getline(cin,pobrane.numerTelefonu);
    cout<<"Podaj email: ";          getline(cin,pobrane.email);
    cout<<"Podaj adres: ";          getline(cin,pobrane.adres);
    cout<<endl;

    pobrane.id=ostatnieID+1;

    pobrane.idUzytkownikaKA=uzytkownicy[pozycjaUzytkownika].idUzytkownika;
    daneOsob.push_back(pobrane);

    zapiszJednegoAdresataDoTXT(daneOsob);

    cout<<"Nowy adresat zapisany!"<<endl;   Sleep(1200);
    return pobrane.id;
}

void szukajPoImieniu(vector<KsiazkaAdresowa> daneOsob){
    int iloscZnalezionychOsob=0;
    string szukaneImie;
    cout<<endl<<"Szukane imie: "; cin>>szukaneImie;

    for(int i=0; i<daneOsob.size(); i++){
        if(szukaneImie==daneOsob[i].imie){
            wypisywanieAdresatowPoLinijce(daneOsob, i);
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
            wypisywanieAdresatowPoLinijce(daneOsob, i);
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

void wypiszWszystkichAdresatow(vector<KsiazkaAdresowa> daneOsob){
    cout<<endl;
    for(int i=0; i<daneOsob.size(); i++)
             wypisywanieAdresatowPoLinijce(daneOsob, i);

    cout<<"Nacisnij dowolny klawisz, aby kontynuowac";
    getch();
}

int usunAdresata (vector<KsiazkaAdresowa> &daneOsob, int ostatnieID){///wymaga "upiekszenia"
    vector <KsiazkaAdresowa>::iterator itr;
    int numerIDDoUsuniecia, pozycjaWKsiazce, idUsuwanegoAdresata;
    string potwierdzenieUsuniecia;

    cout<<endl<<"Podaj numer ID osoby ktora chesz usunac: ";      cin>>numerIDDoUsuniecia;
    pozycjaWKsiazce=znajdzPozycjeOsobyWKsiazce(daneOsob, numerIDDoUsuniecia);

    if (pozycjaWKsiazce==-1) //zabezpieczenie przed wprowadzeniem ID ktorego nie ma w ksiazce
        return ostatnieID;

    wypisywanieAdresatowPoLinijce(daneOsob, pozycjaWKsiazce);
    cout<<"Chcesz usunac tego adresata? t/n: ";             cin>>potwierdzenieUsuniecia;

    if (potwierdzenieUsuniecia!="t"){
        cout<<"Powrot do menu";
        Sleep(1000);
        return ostatnieID;
    }

    idUsuwanegoAdresata=daneOsob[pozycjaWKsiazce].id;

    itr = daneOsob.begin()+pozycjaWKsiazce;
    daneOsob.erase(itr);

    ostatnieID=funkcjaZPlikamiDoEdycjiAdresatow(daneOsob, idUsuwanegoAdresata);

    cout<<"Adresat usuniety!";
    Sleep(1000);

    return ostatnieID;
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

            wypisywanieAdresatowPoLinijce(daneOsob, pozycjaWKsiazce);
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

        default: cout<<endl<<"Nie ma takiej opcji w menu.";
        Sleep(1000);
    }
    return wybor;
}

void zmienImie(vector<KsiazkaAdresowa> &daneOsob, int pozycjaWKsiazce){
    string informacjaDoZmiany;
    cout<<"Podaj nowe imie: ";      cin.sync();       getline(cin, informacjaDoZmiany);

    daneOsob[pozycjaWKsiazce].imie=informacjaDoZmiany;
    funkcjaZPlikamiDoEdycjiAdresatow(daneOsob, pozycjaWKsiazce); //druga zmienna jest potrzebna do usuwania uzytkownika, ale cos trzeba wpisac zeby ruszylo
}

void zmienNazwisko(vector<KsiazkaAdresowa> &daneOsob, int pozycjaWKsiazce){
    string informacjaDoZmiany;
    cout<<"Podaj nowe nazwisko: ";       cin.sync();       getline(cin, informacjaDoZmiany);

    daneOsob[pozycjaWKsiazce].nazwisko=informacjaDoZmiany;
    funkcjaZPlikamiDoEdycjiAdresatow(daneOsob, pozycjaWKsiazce);
}

void zmienNumerTelefonu(vector<KsiazkaAdresowa> &daneOsob, int pozycjaWKsiazce){
    string informacjaDoZmiany;
    cout<<"Podaj nowy numer telefonu: ";       cin.sync();       getline(cin, informacjaDoZmiany);

    daneOsob[pozycjaWKsiazce].numerTelefonu=informacjaDoZmiany;
    funkcjaZPlikamiDoEdycjiAdresatow(daneOsob, pozycjaWKsiazce);
}

void zmienEmail(vector<KsiazkaAdresowa> &daneOsob, int pozycjaWKsiazce){
    string informacjaDoZmiany;
    cout<<"Podaj nowy email: ";       cin.sync();       getline(cin, informacjaDoZmiany);

    daneOsob[pozycjaWKsiazce].email=informacjaDoZmiany;
    funkcjaZPlikamiDoEdycjiAdresatow(daneOsob, pozycjaWKsiazce);
}

void zmienAdres(vector<KsiazkaAdresowa> &daneOsob, int pozycjaWKsiazce){
    string informacjaDoZmiany;
    cout<<"Podaj nowy adres: ";       cin.sync();       getline(cin, informacjaDoZmiany);

    daneOsob[pozycjaWKsiazce].adres=informacjaDoZmiany;
    funkcjaZPlikamiDoEdycjiAdresatow(daneOsob, pozycjaWKsiazce);
}


//////////////////////////////////////Przepisywanie danych miedzy dwoma plikami .txt przy edycji i usuwaniu adresatow
int funkcjaZPlikamiDoEdycjiAdresatow(vector<KsiazkaAdresowa> daneOsob, int idUsunietegoAdresata){
    fstream plik1;
    plik1.open("Ksiazka adresowa.txt",ios::in);
    fstream plik3;
    plik3.open("Plik tymczasowy.txt",ios::out);
    string liniaCase, calaLiniaZTXT, znak="|";
    int IDAdresataPlik1, IDUzytkownikaStruktura, IDUzytkownikaPlik1=0, IDDomniemanegoOstatniegoAdresataPlik1=0;

    while(getline(plik1,liniaCase)){
        calaLiniaZTXT=znajdzIDAdresataIUzytkownika(daneOsob, IDAdresataPlik1, IDUzytkownikaPlik1, IDDomniemanegoOstatniegoAdresataPlik1, liniaCase);

        for (int i=0; i<daneOsob.size(); i++){
            IDUzytkownikaStruktura=daneOsob[i].idUzytkownikaKA;//linijka potrzebna do usunietego adresata

            if(daneOsob[i].id == IDAdresataPlik1){
                plik3<<daneOsob[i].id;                  plik3<<znak;
                plik3<<daneOsob[i].idUzytkownikaKA;     plik3<<znak;
                plik3<<daneOsob[i].imie;                plik3<<znak;
                plik3<<daneOsob[i].nazwisko;            plik3<<znak;
                plik3<<daneOsob[i].numerTelefonu;       plik3<<znak;
                plik3<<daneOsob[i].email;               plik3<<znak;
                plik3<<daneOsob[i].adres;               plik3<<znak<<endl;                  break;
            }

            else if(i==daneOsob.size()-1 && daneOsob[i].id != IDAdresataPlik1 && IDUzytkownikaPlik1!=IDUzytkownikaStruktura)
                plik3<<calaLiniaZTXT<<endl;//zapisuje z oryginalnego pliku txt, jesli ID sie roznia (w strukturze sa tylko niektore kontakty)
        }
        if(daneOsob.size()==0 && IDAdresataPlik1!=idUsunietegoAdresata)
            plik3<<calaLiniaZTXT<<endl;
    }

    if(IDAdresataPlik1==idUsunietegoAdresata)
        IDAdresataPlik1=IDDomniemanegoOstatniegoAdresataPlik1;

    plik1.close();
    plik3.close();

    remove("Ksiazka adresowa.txt");
    rename("Plik tymczasowy.txt", "Ksiazka adresowa.txt");

    return IDAdresataPlik1;//zwraca nam ID OSTATNIEGO adresata
}

string znajdzIDAdresataIUzytkownika(vector<KsiazkaAdresowa> daneOsob, int &IDAdresataPlik1, int &IDUzytkownikaPlik1, int &IDDomniemanegoOstatniegoAdresataPlik1, string liniaCase){
    string calaLiniaZTXT, slowoDoZapisaniaWStrukturze;
    int licznik=0;
    switch(1){
        case 1: calaLiniaZTXT= liniaCase;   break;
    }
    IDDomniemanegoOstatniegoAdresataPlik1=IDAdresataPlik1;//jesli OSTATNI adresat zostanie usuniety, w tej zmiennej bedzie ID ostatniego adresata (przedostatni stanie sie ostatnim)

    for(int i=0; i<calaLiniaZTXT.length();i++){
        if(calaLiniaZTXT[i]!='|')
            slowoDoZapisaniaWStrukturze+=calaLiniaZTXT[i];
        else{
            switch(licznik){
            case 0:   IDAdresataPlik1=konwersjaStringNaInt(slowoDoZapisaniaWStrukturze);        break;
            case 1:   IDUzytkownikaPlik1=konwersjaStringNaInt(slowoDoZapisaniaWStrukturze);     break;
            }
                slowoDoZapisaniaWStrukturze="";
                licznik++;
        }
    }
    return calaLiniaZTXT;
}


//////////////////////////////////////Konwersje etc
int konwersjaStringNaInt(string liczbaString){
    int liczbaInt;
    istringstream iss(liczbaString);
    iss >> liczbaInt;
    return liczbaInt;
}


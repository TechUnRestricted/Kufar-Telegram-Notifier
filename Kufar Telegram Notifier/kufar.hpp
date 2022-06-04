//
//  kufar.hpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 02.06.2022.
//

#ifndef kufar_hpp
#define kufar_hpp

#include <vector>
#include <optional>

namespace Kufar {
    enum class Region {
        Brest = 1,
        Gomel = 2,
        Grodno = 3,
        Mogilev = 4,
        Minsk_Region = 5,
        Vitebsk = 6,
        Minsk = 7
    };

    struct Areas {
        enum class Minsk {
            Centralnyj = 22,
            Sovetskij = 23,
            Pervomajskij = 24,
            Partizanskij = 25,
            Zavodskoj = 26,
            Leninskij = 27,
            Oktyabrskij = 28,
            Moskovskij = 29,
            Frunzenskij = 30
        };
        
        enum class Brest {
            Brest = 1,
            Baranovichi = 37,
            Bereza = 38,
            Beloozyorsk = 123,
            Gancevichi = 48,
            Drogichin = 49,
            Zhabinka = 50,
            Ivanovo = 51,
            Ivacevichi = 52,
            Kamenec = 53,
            Kobrin = 2,
            Luninec = 3,
            Lyahovichi = 54,
            Malorita = 55,
            Pinsk = 4,
            Pruzhany = 56,
            Stolin = 57,
            Others = 31
        };
        
        enum class Gomel {
            Gomel = 5,
            Bragin = 128,
            BudaKoshelevo = 58,
            Vetka = 59,
            Dobrush = 60,
            Elsk = 61,
            Zhitkovichi = 62,
            Zhlobin = 6,
            Kalinkovichi = 63,
            Korma = 129,
            Lelchicy = 130,
            Loev = 131,
            Mozyr = 7,
            Oktyabrskij = 132,
            Narovlya = 64,
            Petrikov = 65,
            Rechica = 8,
            Rogachev = 66,
            Svetlogorsk = 39,
            Hojniki = 67,
            Chechersk = 68,
            Others = 32
        };
        
        enum class Grodno {
            Grodno = 9,
            Berezovka = 69,
            Berestovica = 133,
            Volkovysk = 40,
            Voronovo = 134,
            Dyatlovo = 70,
            Zelva = 135,
            Ive = 71,
            Korelichi = 136,
            Lida = 10,
            Mosty = 72,
            Novogrudok = 73,
            Ostrovec = 74,
            Oshmyany = 75,
            Svisloch = 76,
            Skidel = 77,
            Slonim = 11,
            Smorgon = 41,
            Shchuchin = 78,
            Others = 33
        };
        
        enum class Mogilev {
            Mogilev = 13,
            Belynichi = 137,
            Bobrujsk = 12,
            Byhov = 79,
            Glusk = 80,
            Gorki = 42,
            Dribin = 138,
            Kirovsk = 81,
            Klimovichi = 82,
            Klichev = 83,
            Krasnopole = 139,
            Krugloe = 140,
            Kostyukovichi = 84,
            Krichev = 43,
            Mstislavl = 85,
            Osipovichi = 14,
            Slavgorod = 86,
            Chausy = 87,
            Cherikov = 88,
            Shklov = 89,
            Hotimsk = 141,
            Others = 34
        };
        
        enum class MinskRegion {
            MinskRegion = 142,
            Berezino = 91,
            Borisov = 15,
            Vilejka = 92,
            Volozhin = 93,
            Dzerzhinsk = 94,
            Zhodino = 44,
            Zaslavl = 143,
            Kleck = 95,
            Kopyl = 96,
            Krupki = 97,
            Logojsk = 98,
            Lyuban = 99,
            MarinaGorka = 122,
            Molodechno = 16,
            Myadel = 100,
            Nesvizh = 101,
            Rudensk = 145,
            Sluck = 17,
            Smolevichi = 102,
            Soligorsk = 45,
            StaryeDorogi = 103,
            Stolbcy = 104,
            Uzda = 105,
            Fanipol = 144,
            Cherven = 106,
            Others = 35
        };
        
        enum class Vitebsk {
            Vitebsk = 18,
            Beshenkovichi = 125,
            Baran = 107,
            Braslav = 108,
            Verhnedvinsk = 109,
            Glubokoe = 110,
            Gorodok = 111,
            Dokshicy = 112,
            Dubrovno = 113,
            Lepel = 114,
            Liozno = 115,
            Miory = 116,
            Novolukoml = 117,
            Novopolock = 46,
            Orsha = 19,
            Polock = 20,
            Postavy = 47,
            Rossony = 118,
            Senno = 119,
            Tolochin = 120,
            Ushachi = 126,
            Chashniki = 121,
            Sharkovshchina = 127,
            Shumilino = 124,
            Others = 36
        };
    };

    struct Ad {
        std::string tag;
        std::string title;
        int id;
        time_t date;
        int price;
        std::string sellerName;
        bool phoneNumberIsVisible;
        std::string link;
        std::vector<std::string> images;
    };

    struct KufarConfiguration {
        std::string tag;
        bool onlyTitleSearch = true;
        std::optional<int> priceMin;
        std::optional<int> priceMax;
        
        std::string language;
        int limit;
        Region region;
        std::vector<int> areas;
    };
    
    std::vector<Ad> getAds(const KufarConfiguration &);

};

#endif /* kufar_hpp */

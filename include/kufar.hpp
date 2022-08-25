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

// TODO: В некоторых местах поменять оформление с enum на struct

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

    struct Categories {
        
        // Недвижимость
        enum class RealEstate {
            NewBuildings = 1120,                       // Новостройки
            Apartments = 1010,                         // Квартиры
            Rooms = 1040,                              // Комнаты
            HousesAndCottages = 1020,                  // Дома и коттеджи
            GaragesAndParkingLots = 1030,              // Гаражи и стоянки
            LandPlots = 1080,                          // Участки
            Commercial = 1050                          // Коммерческая
        };
                    
        // Автомобили и транспорт
        enum class CarsAndTransport {
            passengerCars = 2010,                      // Легковые авто
            trucksAndBuses = 2060,                     // Грузовики и автобусы
            motorVehicles = 2030,                      // Мототехника
            partsConsumables = 2040,                   // Запчасти, расходники
            tiresWheels = 2075,                        // Шины, диски
            accessories = 2045,                        // Аксессуары
            agriculturalMachinery = 2080,              // Сельхозтехника
            specialMachinery = 2090,                   // Спецтехника
            trailers = 2020,                           // Прицепы
            waterTransport = 2050,                     // Водный транспорт
            toolsAndEquipment = 2070                   // Инструмент, оборудование
        };
        
        // Бытовая техника
        enum class HouseholdAppliances {
            kitchenAppliances = 15010,                 // Техника для кухни
            largeKitchenAppliances = 15020,            // Крупная техника для кухни
            cleaningEquipment = 15030,                 // Техника для уборки
            clothingCareAndTailoring = 15040,          // Уход за одеждой, пошив
            airConditioningEquipment = 15050,          // Климатическая техника
            beautyAndHealthEquipment = 15060           // Техника для красоты и здоровья
        };
                 
        // Компьютерная техника
        enum class ComputerEquipment {
            laptops = 16040,                           // Ноутбуки
            computers = 16020,                         // Компьютеры / системные блоки
            monitors = 16030,                          // Мониторы
            parts = 16010,                             // Комплектующие
            officeEquipment = 16050,                   // Оргтехника
            peripheryAndAccessories = 16060,           // Периферия и аксессуары
            networkEquipment = 16070,                  // Сетевое оборудование
            otherComputerProducts = 16080              // Прочие компьютерные товары
        };
                 
        // Телефоны и планшеты
        enum class PhonesAndTablets {
            mobilePhones = 17010,                      // Мобильные телефоны
            partsForPhones = 17020,                    // Комплектующие для телефонов
            phoneAccessories = 17030,                  // Аксессуары для телефонов
            telephonyAndCommunication = 17040,         // Телефония и связь
            tablests = 17050,                          // Планшеты
            graphicTablets = 17060,                    // Графические планшеты
            electronicBooks = 17070,                   // Электронные книги
            smartWatchesAndFitnessBracelets = 17090,   // Умные часы и фитнес браслеты
            accessoriesForTabletsBooksWatches = 17080, // Аксессуары для планшетов, книг, часов
            headphones = 5100                          // Наушники
        };
        
        // Электроника
        enum class Electronics {
            audioEquipment = 5020,                     // Аудиотехника
            headphones = 5100,                         // Наушники
            TVAndVideoEquipment = 5060,                // ТВ и видеотехника
            photoEquipmentAndOptics = 5070,            // Фототехника и оптика
            gamesAndConsoles = 5040                    // Игры и приставки
        };
        
        // Женский гардероб
        enum class WomensWardrobe {
            premiumClothing = 8110,                    // Премиум одежда 💎
            womensClothing = 8080,                     // Женская одежда
            womensShoes = 8100,                        // Женская обувь
            womensAccessories = 8020,                  // Женские аксессуары
            repairAndSewingClothes = 13160,            // Ремонт и пошив одежды
            clothesForPregnantWomen = 12120            // Одежда для беременных
        };
        
        // Мужской гардероб
        enum class MensWardrobe {
            mensClothing = 19010,                      // Мужская одежда
            mensShoes = 19020,                         // Мужская обувь
            mensAccessories = 19030                    // Мужские аксуссуары
        };
        
        // Красота и здоровье
        enum class BeautyAndHealth {
            decorativeCosmetics = 18010,               // Декоративная косметика
            careCosmetics = 18020,                     // Уходовая косметика
            perfumery = 18030,                         // Парфюмерия
            manicurePedicure = 18040,                  // Маникюр, педикюр
            hairProducts = 18050,                      // Средства для волос
            hygieneProductsDepilation = 18060,         // Средства гигиены, депиляция
            eyelashesAndEyebrowsTattoo = 18070,        // Ресницы и брови, татуаж
            cosmeticAccessories = 18090,               // Косметические аксессуары
            medicalProducts = 18080,                   // Медицинские товары
            beautyAndHealthEquipment = 15060,          // Техника для красоты и здоровья
            ServicesBeautyAndHealth = 13040            // Услуги: красота и здоровье
        };
        
        // Всё для детей и мам
        enum class AllForChildrenAndMothers {
            clothingUpTo1Year = 12010,                 // Одежда до 1 года
            clothesForGirls = 12140,                   // Одежда для девочек
            clothesForBoys = 12150,                    // Одежда для мальчиков
            accessoriesForChildren = 12170,            // Аксессуары для детей
            childrensShoes = 12020,                    // Детская обувь
            walkersDeckChairsSwings = 12040,           // Ходунки, шезлонги, качели
            strollers = 12050,                         // Коляски
            carSeatsAndBoosters = 12060,               // Автокресла и бустеры
            feedingAndCare = 12070,                    // Кормление и уход
            textileForChildren = 12160,                // Текстиль для детей
            kangarooBagsAndSlings = 12080,             // Сумки-кенгуру и слинги
            toysAndBooks = 12090,                      // Игрушки и книги
            childrensTransport = 12100,                // Детский транспорт
            productsForMothers = 12110,                // Товары для мам
            clothesForPregnant = 12120,                // Одежда для беременных
            otherProductsForChildren = 12130,          // Прочие товары для детей
            furnitureForChildren = 21180               // Детская мебель
        };
        
        // Мебель
        enum class Furniture {
            banquetAndOttomans = 21010,                // Банкетки, пуфики
            hangersAndHallways = 21020,                // Вешалки, прихожие
            furnitureForChildren = 21180,              // Детская мебель
            dressers = 21030,                          // Комоды
            bedsAndMattresses = 21040,                 // Кровати, матрасы
            kitchens = 21050,                          // Кухни
            KitchenCorners = 21060,                    // Кухонные уголки
            cushionedFurniture = 21070,                // Мягкая мебель
            shelvesRacksLockers = 21090,               // Полки, стеллажи, шкафчики
            sleepingHeadsets = 21100,                  // Спальные гарнитуры
            wallsSectionsModules = 21110,              // Стенки, секции, модули
            tablesAndDiningGroups = 21120,             // Столы и обеденные группы
            chairs = 21130,                            // Стулья
            cabinetsCupboards = 21140,                 // Тумбы, буфеты
            wardrobes = 21150,                         // Шкафы
            furnitureAccessoriesAndComponents = 21160, // Мебельная фурнитура и составляющие
            otherFurniture = 21170                     // Прочая мебель
        };
        
        // Всё для дома
        enum class EverythingForHome {
            interiorItemsMirrors = 3060,               // Предметы интерьера, зеркала
            curtainsBlindsCornices = 3150,             // Шторы, жалюзи, карнизы
            textilesAndCarpets = 3160,                 // Текстиль и ковры
            lighting = 3170,                           // Освещение
            householdGoods = 3110,                     // Хозяйственные товары
            tablewareAndKitchenAccessories = 3100,     // Посуда и кухонные аксессуары
            indoorPlants = 3120,                       // Комнатные растения
            householdServices = 13020,                 // Бытовые услуги
            furnitureRepair = 13180                    // Ремонт мебели
        };
        
        // Ремонт и стройка
        enum class RepairAndBuilding {                 // Строительный инструмент
            constructionTools = 14010,                 // Строительное оборудование
            constructionEquipment = 14180,             // Сантехника и отопление
            plumbingAndHeating = 14020,                // Стройматериалы
            buildingMaterials = 14030,                 // Отделочные материалы
            finishingMaterials = 14040,                // Окна и двери
            windowsAndDoors = 14050,                   // Дома, срубы и сооружения
            housesLogCabinsAndStructures = 14080,      // Ворота, заборы
            gatesFences = 14090,                       // Электроснабжение
            powerSupply = 14100,                       // Средства индивидуальной защиты
            personalProtectiveEquipment = 14190,       // Прочее для ремонта и стройки
            otherForRepairAndConstruction = 14060      // Строительный инструмент
        };
        
        // Сад и огород
        enum Garden {
            gardenFurnitureAndSwimmingPools = 10010,   // Садовая мебель и бассейны
            barbecuesAccessoriesFuel = 10100,          // Мангалы, аксессуары, топливо
            tillersAndCultivators = 10110,             // Мотоблоки и культиваторы
            gardenEquipment = 10030,                   // Садовая техника
            gardenTools = 10050,                       // Садовый инвентарь
            greenhouses = 10040,                       // Теплицы и парники
            plantsSeedlingsAndSeeds = 10020,           // Растения, рассада и семена
            fertilizersAndAgrochemicals = 10060,       // Удобрения и агрохимия
            everythingForTheBeekeeper = 10070,         // Все для пчеловода
            bathsHouseholdUnitsBathrooms = 10090,      // Бани, хозблоки, санузлы
            otherForTheGarden = 10080                  // Прочее для сада и огорода
        };
        
        // Хобби, спорт и туризм
        enum HobbiesSportsAndTourism {
            CDDVDRecords = 4010,                       // CD, DVD, пластинки
            antiquesAndCollections = 4030,             // Антиквариат и коллекции
            tickets = 4120,                            // Билеты
            booksAndMagazines = 4040,                  // Книги и журналы
            metalDetectors = 4130,                     // Металлоискатели
            musicalInstruments = 4070,                 // Музыкальные инструменты
            boardGamesAndPuzzles = 4140,               // Настольные игры и пазлы
            huntingAndFishing = 4060,                  // Охота и рыбалка
            touristGoods = 4090,                       // Туристические товары
            radioControlledModels = 4150,              // Радиоуправляемые модели
            handiwork = 4080,                          // Рукоделие
            sportGoods = 4020,                         // Спорттовары
            bicycles = 4050,                           // Велосипеды
            electricTransport = 4160,                  // Электротранспорт
            touristServices = 13140,                   // Туристические услуги
            otherHobbiesSportsAndTourism = 4100        // Прочее в Хобби, спорт и туризм
        };
        
        // Свадьба и праздники
        enum WeddingAndHolidays {
            weddingDresses = 9020,                     // Свадебные платья
            weddingCostumes = 9030,                    // Свадебные костюмы
            weddingShoes = 9040,                       // Свадебная обувь
            weddingAccessories = 9050,                 // Свадебные аксессуары
            giftsAndHolidayGoods = 9010,               // Подарки и праздничные товары
            carnivalCostumes = 9070,                   // Карнавальные костюмы
            servicesForCelebrations = 13170,           // Услуги для торжеств
        };
        
        // Животные
        enum class Animals {
            pets = 11010,                              // Домашние питомцы
            farmAnimals = 11020,                       // Сельхоз животные
            petProducts = 11030,                       // Товары для животных
            animalMating = 11050,                      // Вязка животных
            servicesForAnimals = 13110                 // Услуги для животных
        };
        
        // Готовый бизнес и оборудование
        enum class ReadyBusinessAndEquipment {
            readyBusiness = 20010,                     // Готовый бизнес
            businessEquipment = 20020,                 // Оборудование для бизнеса
        };
        
        // Работа
        enum class Job {
            vacancies = 6010,                          // Вакансии
            lookingForAJob = 6020,                     // Ищу работу
        };
        
        // Услуги
        enum class Services {
           servicesForCars = 13010,                   // Услуги для авто
           householdServices = 13020,                 // Бытовые услуги
           computerServicesInternet = 13030,          // Компьютерные услуги, интернет
           beautyAndHealth = 13040,                   // Красота и здоровье
           nanniesAndNurses = 13050,                  // Няни и сиделки
           educationalServices = 13200,               // Образовательные услуги
           translatorSecretaryServices = 13060,       // Услуги переводчика, секретаря
           transportationOfPassengersAndCargo = 13070,// Перевозки пассажиров и грузов
           advertisingPrinting = 13080,               // Реклама, полиграфия
           constructionWorks = 13090,                 // Строительные работы
           apartmentHouseRenovation = 13190,          // Ремонт квартиры, дома
           furnitureRepair = 13180,                   // Ремонт мебели
           repairAndTailoring = 13160,                // Ремонт и пошив одежды
           gardenLandscaping = 13100,                 // Сад, благоустройство
           servicesForAnimals = 13110,                // Услуги для животных
           servicesForCelebrations = 13170,           // Услуги для торжеств
           photoAndVideoShooting = 13120,             // Фото и видеосъемка
           legalServices = 13130,                     // Юридические услуги
           touristServices = 13140,                   // Туристические услуги
           otherServices = 13150,                     // Прочие услуги
        };
        
        // Прочее
        enum class Other {
            lostAndFound = 7030,                      // Бюро находок
            hookahs = 7070,                           // Кальяны
            officeSupplies = 7060,                    // Канцелярские товары
            foodProducts = 7020,                      // Продукты питания
            electronicSteamGenerators = 7080,         // Электронные парогенераторы
            demand = 7040,                            // Спрос
            everythingElse = 7010                     // Все остальное
        };
        
    };

    struct Ad {
        std::optional<std::string> tag;
        std::string title;
        int id;
        time_t date;
        int price;
        std::string sellerName;
        bool phoneNumberIsVisible;
        std::string link;
        std::vector<std::string> images;
    };

    struct PriceRange {
        std::optional<int> priceMin;
        std::optional<int> priceMax;

        std::optional<std::string> joinPrice() const;
    };
    
	enum class ItemCondition {
        used = 1,
        _new = 2
    };

    enum class SellerType {
        individualPerson = 0,
        company = 1
    };

    enum class SortType {
        descending = 1,
        ascending = 2
    };

    struct KufarConfiguration {
        std::optional<std::string> tag;                 // Default: [undefined]
        std::optional<bool> onlyTitleSearch;            // Default: [undefined]
        PriceRange priceRange = PriceRange();           // Default: [undefined]
        std::optional<std::string> language;            // Default: [undefined]
        std::optional<int> limit;                       // Default: [undefined]
        std::optional<std::string> currency;            // Default: [undefined]
        std::optional<ItemCondition> condition;         // Default: [undefined]
        std::optional<SellerType> sellerType;           // Default: [undefined]
        std::optional<bool> kufarDeliveryRequired;      // Default: [undefined]
        std::optional<bool> kufarPaymentRequired;       // Default: [undefined]
        std::optional<bool> kufarHalvaRequired;         // Default: [undefined]
        std::optional<bool> onlyWithPhotos;             // Default: [undefined]
        std::optional<bool> onlyWithVideos;             // Default: [undefined]
        std::optional<bool> onlyWithExchangeAvailable;  // Default: [undefined]
        std::optional<SortType> sortType;               // Default: [undefined]
        std::optional<int> category;                    // Default: [undefined]
        std::optional<Region> region;                   // Default: [undefined]
        std::optional<std::vector<int>> areas;          // Default: [undefined]
    };
    
    std::vector<Ad> getAds(const KufarConfiguration &);

    namespace EnumString {
        std::string sortType(SortType);
        std::string itemCondition(ItemCondition);
        std::string sellerType(SellerType);
        std::string region(Region);
        std::string area(int);
    }
};

#endif /* kufar_hpp */

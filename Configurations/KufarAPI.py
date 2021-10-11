from service import priceFormatted, areasFormatted
from urllib.parse import quote


class Kufar:
    # Список ключевых слов для парсинга Kufar API
    query_queries = ["iPhone",
                     "iPad",
                     "iPod",
                     "MacBook",
                     "Android",
                     "PlayStation",
                     "Xbox",
                     "NVIDIA",
                     "Ryzen",
                     "AMD",
                     ]

    lang_language = "ru"
    prc_priceRange = priceFormatted(min=0, max=80)
    ar_areas = areasFormatted(areas=[9])
    rgn_region = 3
    ot_searchOnlyByTitle = 0
    limit = 10

    params = (f"ar={quote(ar_areas)}"
              f"&lang={lang_language}"
              f"&rgn={rgn_region}"
              f"&size={limit}"
              f"&ot={ot_searchOnlyByTitle}"
              f"&prc={quote(prc_priceRange)}"
              )

    baseurl = "https://searchapi.kufar.by/v1/search/rendered-paginated?" + params + "&query="

import settings
from regions import *
from service import *


@dataclass
class Advertisement:
    ad_name: str
    phone_hidden: bool
    price_byn: int
    price_usd: int
    seller_name: str
    image_url: str
    date_timestamp: int


def get_ads(query, regions=None, price_range=None, limit=10, lang="ru", title_only_search=False,
            min_timestamp=0):
    parameters = {"lang": lang,
                  "query": query,
                  "size": limit,
                  "ot": (1 if title_only_search else 0)
                  }

    if price_range is not None:
        parameters.update({"prc": f"r:{price_range[0] * 100},{price_range[1] * 100}"})

    if regions is not None:
        if regions.areas is not None:
            parameters.update({"ar": f"v.or:{','.join(str(x) for x in regions.areas)}"})
        if regions.city is not None:
            parameters.update({"rgn": regions.city})

    debug = kufar_base_url + urllib.parse.urlencode(parameters)
    print(debug)
    json = get_json_from_url(kufar_base_url + urllib.parse.urlencode(parameters))

    ads_list = []

    for ad in json.get("ads", []):
        date_timestamp = zulu_date_to_timestamp(ad.get("list_time", 0)) + time_shift

        if date_timestamp < script_start_timestamp - 3600:
            print("skipping")
            continue

        seller_name = ""
        image_url = placeholder_url
        for accountParameter in ad["account_parameters"]:
            if accountParameter["p"] == "name":
                seller_name = accountParameter.get("v", "Unknown")

            for image in ad.get("images", []):
                if image.get("yams_storage", []):
                    image_url = f"https://yams.kufar.by/api/v1/kufar-ads/images/{image['id'][:2]}/{image['id']}.jpg?rule=pictures"
                    break

        ads_list.insert(0, Advertisement(
            ad_name=ad.get("subject", "Unknown"),
            phone_hidden=ad.get("phone_hidden", False),
            price_byn=int(ad.get("price_byn", 0)),
            price_usd=int(ad.get("price_usd", 0)),
            seller_name=seller_name,
            image_url=image_url,
            date_timestamp=date_timestamp
        ))

    # for ad__ in ads_list:
    #    print(ad__)
    return ads_list


def loop_updates_handler():
    for query in settings.TelegramAPI.list_parse:
        ads = get_ads(
            query=query,
            regions=Belarus(city=Cities.Grodno, areas=[Areas.Grodno.Grodno]),
            price_range=[0, 900]
        )
        for ad in ads:
            print(ad)
        sleep(1)
    print("Sleeping for 2mins")
    sleep(2 * 60)
    loop_updates_handler()

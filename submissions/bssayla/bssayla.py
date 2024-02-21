from pandas import read_csv
# this language is trash i need to learn rust - NotAsheraf congrats
df = read_csv("input.txt", sep=",", header=None, names=["city", "product", "price"])
df_grouped_cities = df.groupby("city")["price"].sum()
cheapest_city = df_grouped_cities.idxmin()

with open("output.txt", "w") as file:
    file.write(f"{cheapest_city} {df_grouped_cities.loc[cheapest_city]}\n")
    df_cheapest_city = df[df["city"] == cheapest_city]
    df_cheapest_city = df_cheapest_city.sort_values("price").head(5)
    for index, row in df_cheapest_city.iterrows():
        file.write(f"{row['product']},{row['price']}\n")

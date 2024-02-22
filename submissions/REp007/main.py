import pandas as pd

def remove_duplicates(file_name):
    input_data = pd.read_csv(file_name, header=None, names=["City", "Product", "Price"])
    cleaned_data = []

    for city in input_data["City"].unique():
        products = input_data[input_data["City"] == city]["Product"].unique()
        for product in products:
            df_filtered = input_data[(input_data["City"] == city) & (input_data["Product"] == product)]
            min_price_row = df_filtered.loc[df_filtered["Price"].idxmin()]
            cleaned_data.append(min_price_row.to_dict())

    cleaned_data = pd.DataFrame(cleaned_data)
    return cleaned_data

def get_cheapest_city(data):
    city_totals = data.groupby('City')['Price'].sum()
    return city_totals.idxmin(), city_totals.loc[city_totals.idxmin()]

def get_top_5_cheapest_products(data, cheapest_city):
    cheapest_city_data = data[data["City"] == cheapest_city]
    top_5_cheapest_products = cheapest_city_data.sort_values(by=['Price', 'Product']).head(5)
    return top_5_cheapest_products

if __name__ == '__main__':
    file_name = "input.txt"
    input_data = remove_duplicates(file_name)
    cheapest_city, cheapest_city_sum_product = get_cheapest_city(input_data)
    top_5_cheapest_products = get_top_5_cheapest_products(input_data, cheapest_city)

    # Write the results to the output file
    with open("output.txt", "w") as f:
        f.write(f"{cheapest_city} {cheapest_city_sum_product:.2f}\n")
        for index, row in top_5_cheapest_products.iterrows():
            f.write(f"{row['Product']} {row['Price']:.2f}\n")
